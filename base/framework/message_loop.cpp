// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2011/06/09
//
// This file trys to implement a cross flatform message loop,
// the mechanism of which is from the Google Chrome project.

#include "base/framework/message_loop.h"

#include <assert.h>
#include "base/memory/lazy_instance.h"
#include "base/thread/thread_local.h"

namespace nbase
{

LazyInstance<ThreadLocalPointer<MessageLoop> > g_lazy_ptr;

MessageLoop::MessageLoop()
	: type_(kDefaultMessageLoop),
	  state_(NULL),
#if defined(OS_WIN)
	  os_modal_loop_(false),
#endif // OS_WIN
	  nestable_tasks_allowed_(true),
	  next_delayed_task_sequence_num_(0)
{
	// 一个线程内不能存在两个或以上MessageLoop
	assert(g_lazy_ptr.Pointer()->Get() == NULL);
	// 默认消息循环
	if (type_ == kDefaultMessageLoop)
		pump_.reset(new DefaultMessagePump);
	g_lazy_ptr.Pointer()->Set(this);

	message_loop_proxy_.reset(new MessageLoopProxy, &MessageLoopProxyTraits::Destruct);
	message_loop_proxy_->target_message_loop_ = this;
}

MessageLoop::~MessageLoop()
{
	bool has_work = false;

	// 清理未处理的任务可能导致生成新的任务，
	// 这里通过有限次的循环尝试清理这些新生成的任务
	for (int i = 0; i < 100; i++)
	{
		DeletePendingTasks();
		ReloadWorkQueue();
		has_work = DeletePendingTasks();
		if (!has_work)
			break;
	}

	assert(!has_work);

	PreDestruct();

	message_loop_proxy_->WillDestroyCurrentMessageLoop();
	message_loop_proxy_ = nullptr;

	g_lazy_ptr.Pointer()->Set(NULL);
}

MessageLoop* MessageLoop::current()
{
	return g_lazy_ptr.Pointer()->Get();
}

#if defined(OS_WIN)

UIMessageLoop* MessageLoop::ToUIMessageLoop()
{
	if (type_ == kUIMessageLoop)
		return reinterpret_cast<UIMessageLoop *>(this);
	return NULL;
}

IOMessageLoop* MessageLoop::ToIOMessageLoop()
{
	if (type_ == kIOMessageLoop)
		return reinterpret_cast<IOMessageLoop *>(this);
	return NULL;
}

void MessageLoop::RunWithDispatcher(Dispatcher *dispatcher)
{
	assert(this == current());
	AutoRunState state(this);
	state_->dispatcher = dispatcher;
	RunInternal();
}

#elif defined(OS_POSIX)
	
IOMessageLoop* MessageLoop::ToIOMessageLoop()
{
	if (type_ == kIOMessageLoop)
		return reinterpret_cast<IOMessageLoop *>(this);
	return NULL;
}
	
#endif // OS_WIN

void MessageLoop::Run()
{
	assert(this == current());
	AutoRunState state(this);
	RunInternal();
}

void MessageLoop::RunAllPending()
{
	assert(this == current());
	AutoRunState state(this);
	state_->quit_received = true;  // Means run until we would otherwise block.
	RunInternal();
}

void MessageLoop::RunInternal()
{
	assert(this == current());

#if defined(OS_WIN)
	if (state_->dispatcher && type() == kUIMessageLoop)
	{
		static_cast<WinUIMessagePump *>(pump_.get())->
			RunWithDispatcher(this, state_->dispatcher);
		return;
	}
#endif

	pump_->Run(this);
}

bool MessageLoop::DeletePendingTasks()
{
	bool has_work = false;
	while (!work_queue_.empty())
	{
		PendingTask task = work_queue_.front();
		work_queue_.pop();
		if (!task.delayed_run_time.is_null())
			AddToDelayedWorkQueue(task);
	}

	while (!deferred_non_nestable_work_queue_.empty())
		deferred_non_nestable_work_queue_.pop();

	has_work = !delayed_work_queue_.empty();
	while (!delayed_work_queue_.empty())
		delayed_work_queue_.pop();

	return has_work;
}

void MessageLoop::Quit()
{
	if (state_)
		state_->quit_received = true;
}

void MessageLoop::QuitNow()
{
	if (pump_)
		pump_->Quit();
}

void MessageLoop::PostTask(const StdClosure &task)
{
	PendingTask pending_task(task);
	AddToIncomingQueue(pending_task);
}

void MessageLoop::PostDelayedTask(const StdClosure &task, TimeDelta delay)
{
	PendingTask pending_task(task,
		TimeTicks::Now() + delay,
		true);
	AddToIncomingQueue(pending_task);
}

void MessageLoop::PostNonNestableTask(const StdClosure &task)
{
	PendingTask pending_task(task,
		TimeTicks(),
		false);
	AddToIncomingQueue(pending_task);
}

void MessageLoop::PostNonNestableDelayedTask(const StdClosure &task, TimeDelta delay)
{
	PendingTask pending_task(task,
		TimeTicks::Now() + delay,
		false);
	AddToIncomingQueue(pending_task);
}

TimeTicks MessageLoop::EvalDelayedRuntime(int64_t delay_ms)
{
	TimeTicks delayed_run_time;
	if (delay_ms > 0)
		delayed_run_time = TimeTicks::Now() + TimeDelta::FromMilliseconds(delay_ms);
	return delayed_run_time;
}

void MessageLoop::AddToIncomingQueue(const PendingTask &task)
{
	// 本方法可能会在另一个线程中被执行，所以必须线程安全
	std::shared_ptr<MessagePump> pump;
	{
		NAutoLock lock(&incoming_queue_lock_);
		bool was_empty = incoming_queue_.empty();
		incoming_queue_.push(task);
		if (!was_empty)
			return;
		// 因为这函数可能是间接地在另一个线程中被调用的，
		// 此时MessageLoop中可能正有任务在运行，
		// 这些任务中可能包含销毁MessageLoop的任务，
		// 为了保证对MessageLoop中的MessagePump引用有效，
		// 这里需要用到引用指针
		pump = pump_;
	}
	pump->ScheduleWork();
}

void MessageLoop::AddToDelayedWorkQueue(const PendingTask &task)
{
	PendingTask new_task(task);
	new_task.sequence_num = next_delayed_task_sequence_num_++;
	delayed_work_queue_.push(new_task);
}

void MessageLoop::ReloadWorkQueue()
{
	if (!work_queue_.empty())
		return;

	{
		NAutoLock lock(&incoming_queue_lock_);
		if (incoming_queue_.empty())
			return;
		// 常数时间交换内存
		work_queue_.Swap(&incoming_queue_);
	}
}

bool MessageLoop::DeferOrRunPendingTask(const PendingTask &task)
{
	// 任务符合立即执行的条件，那么执行之
	if (task.nestable || state_->run_depth == 1)
	{
		RunTask(task);
		return true;
	}
	// 不可嵌套任务，需要缓存之直到在最顶层MessageLoop中执行
	deferred_non_nestable_work_queue_.push(task);
	return false;
}

void MessageLoop::RunTask(const PendingTask &task)
{
	assert(nestable_tasks_allowed_);

	// 考虑到最坏情况下，任务可能是不可重入的，
	// 所以暂时禁用嵌套任务

	nestable_tasks_allowed_ = false;
	PendingTask pending_task = task;
	PreProcessTask();
	pending_task.Run();
	PostPrecessTask();
	nestable_tasks_allowed_ = true;
}

bool MessageLoop::DoWork()
{
	// 任务当前是否允许被执行
	if (!nestable_tasks_allowed_)
		return false;

	for (;;)
	{
		// 先从incoming队列取任务
		ReloadWorkQueue();
		if (work_queue_.empty())
			break;

		// 一次性处理work队列中的所有任务
		do
		{
			PendingTask task = work_queue_.front();
			work_queue_.pop();
			if (!task.delayed_run_time.is_null())
			{
				// 加入到定时任务队列
				AddToDelayedWorkQueue(task);
				// 如果加入的新任务是将被最先执行的，那么需要重新调度
				if (delayed_work_queue_.top().sequence_num == task.sequence_num)
					pump_->ScheduleDelayedWork(task.delayed_run_time);
			}
			else
			{
				if (DeferOrRunPendingTask(task))
					return true;
			}
		} while (!work_queue_.empty());
	}

	return false;
}

bool MessageLoop::DoDelayedWork(nbase::TimeTicks* next_delayed_work_time)
{
	if (!nestable_tasks_allowed_ || delayed_work_queue_.empty())
	{
		*next_delayed_work_time = recent_tick_ = TimeTicks();
		return false;
	}

	// recent_tick_记录最近一次调用TimeTick::Now时的时间，
	// 它不能代替TimeTick::Now，它是只是一个过去的Now的缓存，
	// 用来最大限度减少对TimeTick::Now的调用。
	// recent_tick_用来进行第一轮判断，要进行精确判断需要更新它为真正的TimeTick::Now

	TimeTicks next_run_time = delayed_work_queue_.top().delayed_run_time;
	if (next_run_time > recent_tick_)
	{
		// 可能是recent_tick_的不精确性引起，需要更新之
		recent_tick_ = TimeTicks::Now();
		if (next_run_time > recent_tick_)
		{
			// 真的是一个将来才需要被运行的任务，留到将来运行
			*next_delayed_work_time = next_run_time;
			return false;
		}
	}

	// 这个定时任务运行时刻已到，运行之
	PendingTask task = delayed_work_queue_.top();
	delayed_work_queue_.pop();

	if (!delayed_work_queue_.empty())
		*next_delayed_work_time = delayed_work_queue_.top().delayed_run_time;

	return DeferOrRunPendingTask(task);
}

bool MessageLoop::ProcessNextDelayedNonNestableTask()
{
	// 嵌套任务？
	if (state_->run_depth != 1)
		return false;

	if (deferred_non_nestable_work_queue_.empty())
		return false;

	PendingTask task = deferred_non_nestable_work_queue_.front();
	deferred_non_nestable_work_queue_.pop();
	RunTask(task);
	return true;
}

bool MessageLoop::DoIdleWork()
{
	// 进入Idle状态后，先尝试执行被缓存着的非嵌套任务
	if (ProcessNextDelayedNonNestableTask())
		return true;

	// 检查退出标记
	if (state_->quit_received)
		pump_->Quit();

	return false;
}

void MessageLoop::SetNestableTasksAllowed(bool allowed)
{
	if (nestable_tasks_allowed_ != allowed)
	{
		nestable_tasks_allowed_ = allowed;
		if (!nestable_tasks_allowed_)
			return;
		pump_->ScheduleWork();
	}
}

void MessageLoop::AddDestructionObserver(DestructionObserver *observer)
{
	assert(this == current());
	destruction_observers_.AddObserver(observer);
}

void MessageLoop::RemoveDestructionObserver(DestructionObserver *observer)
{
	assert(this == current());
	destruction_observers_.RemoveObserver(observer);
}

void MessageLoop::AddTaskObserver(TaskObserver *observer)
{
	assert(this == current());
	task_observers_.AddObserver(observer);
}

void MessageLoop::RemoveTaskObserver(TaskObserver *observer)
{
	assert(this == current());
	task_observers_.RemoveObserver(observer);
}

void MessageLoop::PreDestruct()
{
	size_t index = 0;
	DestructionObserver* observer;
	AutoLazyEraser lazy_eraser(&destruction_observers_);
	while (index < destruction_observers_.GetObserverCount())
	{
		observer = destruction_observers_.GetObserver(index++);
		if (observer == NULL)
			continue;
		observer->PreDestroyCurrentMessageLoop();
	}
}

void MessageLoop::PreProcessTask()
{
	size_t index = 0;
	TaskObserver* observer;
	AutoLazyEraser lazy_eraser(&task_observers_);
	while (index < task_observers_.GetObserverCount())
	{
		observer = task_observers_.GetObserver(index++);
		if (observer == NULL)
			continue;
		observer->PreProcessTask();
	}
	// NOT compact the observer list, here
}

void MessageLoop::PostPrecessTask()
{
	size_t index = 0;
	TaskObserver* observer;
	AutoLazyEraser lazy_eraser(&task_observers_);
	while (index < task_observers_.GetObserverCount())
	{
		observer = task_observers_.GetObserver(index++);
		if (observer == NULL)
			continue;
		observer->PostProcessTask();
	}
}

// the AutoRunState class
MessageLoop::AutoRunState::AutoRunState(MessageLoop* loop) : loop_(loop)
{
	// Make the loop reference us.
	previous_state_ = loop_->state_;
	if (previous_state_) {
		run_depth = previous_state_->run_depth + 1;
	} else {
		run_depth = 1;
	}
	loop_->state_ = this;

	// Initialize the other fields:
	quit_received = false;
#if defined(OS_WIN)
	dispatcher = NULL;
#endif
}

MessageLoop::AutoRunState::~AutoRunState()
{
	loop_->state_ = previous_state_;
}

MessageLoop::PendingTask::PendingTask(const StdClosure &task)
	: std_task(task), nestable(true), sequence_num(0)
{

}

MessageLoop::PendingTask::PendingTask(const StdClosure &task, TimeTicks delayed_run_time, bool nestable) :
	std_task(task), delayed_run_time(delayed_run_time),
	nestable(nestable), sequence_num(0)
{

}

MessageLoop::PendingTask::~PendingTask()
{

}

bool MessageLoop::PendingTask::operator<(const MessageLoop::PendingTask& other) const
{
	if (delayed_run_time > other.delayed_run_time)
		return true;
	if (delayed_run_time < other.delayed_run_time)
		return false;
	return sequence_num > other.sequence_num;
}

#if defined(OS_WIN)

// the UIMessageLoop class
UIMessageLoop::UIMessageLoop()
{
	pump_.reset(new UIMessagePump);
	type_ = kUIMessageLoop;
}

void UIMessageLoop::AddUIObserver(UIObserver* observer)
{
	assert(this == current());
	static_cast<UIMessagePump *>(pump())->AddObserver(observer);
}

void UIMessageLoop::RemoveUIObserver(UIObserver *observer)
{
	assert(this == current());
	static_cast<UIMessagePump *>(pump())->RemoveObserver(observer);
}
	
#endif // OS_WIN
	
// the IOMessageLoop class
IOMessageLoop::IOMessageLoop()
{
	pump_.reset(new IOMessagePump);
	type_ = kIOMessageLoop;
}

void IOMessageLoop::AddIOObserver(IOObserver* observer)
{
	assert(this == current());
	static_cast<IOMessagePump *>(pump())->AddObserver(observer);
}

void IOMessageLoop::RemoveIOObserver(IOObserver *observer)
{
	assert(this == current());
	static_cast<IOMessagePump *>(pump())->RemoveObserver(observer);
}

#if defined(OS_WIN)
void IOMessageLoop::RegisterIOHandler(HANDLE file_handle, IOHandler *handler)
{
	assert(this == current());
	static_cast<IOMessagePump *>(pump())->RegisterIOHandler(file_handle, handler);
}

bool IOMessageLoop::WaitForIOCompletion(DWORD timeout, IOHandler *handler)
{
	assert(this == current());
	return static_cast<IOMessagePump *>(pump())->WaitForIOCompletion(timeout, handler);
}

#elif defined(OS_POSIX)
bool IOMessageLoop::WatchFileDescriptor(int fd,
					                    bool persistent,
						                Mode mode,
						                FileDescriptorWatcher *controller,
						                Watcher *delegate)
{
	assert(this == current());
	return static_cast<IOMessagePump *>(pump())()->WatchFileDescriptor(
		fd, persistent, static_cast<LibeventMessagePump::Mode>(mode), controller, delegate);
}
#endif


} // namespace nbase
