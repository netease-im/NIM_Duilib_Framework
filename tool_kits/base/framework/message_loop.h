// This file trys to implement a cross flatform message loop,
// the mechanism of which is from the Google Chrome project.

#ifndef BASE_FRAMEWORK_MESSAGE_LOOP_H_
#define BASE_FRAMEWORK_MESSAGE_LOOP_H_

#include <queue>									// for std::queue, std::priority_queue
#include <memory>
#include "base/base_types.h"						// for OS_WIN and NULL, etc.
#include "base/framework/default_message_pump.h"
#include "base/framework/message_loop_proxy.h"		// for MessageLoopProxy
#include "base/framework/observer_list.h"			// for ObserverList
#if defined(OS_WIN)
#include "base/framework/win_io_message_pump.h"
#include "base/framework/win_ui_message_pump.h"
#elif defined(OS_POSIX)
#include "base/framework/libevent_message_pump.h"
#endif // OS_WIN
#include "base/synchronization/lock.h"				// for NLock
#include "base/time/time.h"
#include "base/callback/callback.h"

namespace nbase
{

// 一个线程最多只能使用一个MessageLoop。
// MessageLoop用于处理Task，同时，根据类型的不同，MessageLoop还可能处理Windows消息、定时器、IO事件等
// 除非特别说明，MessageLoop类所有的成员函数都只允许在执行MessageLoop的线程上调用
//
// MessageLoop有Task重入保护，也就是说在一个Task执行过程中生成了第二个Task，
// 那么第二个Task将在第一个Task执行完成之后才会被执行，因为第二个Task能也会生成一个MessagePump。
// 要打破这个行为，需要使用SetNestableTasksAllowed方法，详见这个方法的说明

class UIMessageLoop;
class IOMessageLoop;
class UvMessageLoop;
class MessageLoopProxy;

#if defined(OS_WIN)
	typedef WinIOMessagePump IOMessagePump;
	typedef WinUIMessagePump UIMessagePump;
	typedef WinMessagePump::Dispatcher Dispatcher;
#elif defined(OS_POSIX)
	typedef LibeventMessagePump IOMessagePump;
#else
#error Not support currently!
#endif
	
class BASE_EXPORT MessageLoop : public MessagePump::Delegate
{
public:	

	enum Type
	{
		kDefaultMessageLoop,
		kUIMessageLoop,
		kIOMessageLoop,
		kCustomMessageLoop,
	};

	explicit MessageLoop();
	virtual ~MessageLoop();
	static MessageLoop* current();

#if defined(OS_WIN)
	// 转换成对应派生类型的MessageLoop指针
	// 如果原指针非对应类型的指针，将返回NULL
	UIMessageLoop* ToUIMessageLoop();
	IOMessageLoop* ToIOMessageLoop();
#elif defined(OS_POSIX)
	IOMessageLoop* ToIOMessageLoop();	
#endif

	MessagePump * pump() { return pump_.get(); }

	Type type() const { return type_; }

	// 运行MessageLoop
	void Run();
#if defined(OS_WIN)
	void RunWithDispatcher(Dispatcher *dispatcher);
#endif

	// 处理所有待处理的Task、Windows消息、IO事件等等，但是进行Wait/Sleep。
	// 当所有待处理的Task被处理完成后，立即从对这个方法的调用返回
	void RunAllPending();

	// 通知MessageLoop在处理所有待处理任务之后从Run方法中返回。
	// 这个方法只能在调用Run方法的那个线程中被调用
	// 要退出其他线程的MessageLoop，需要使用PostTask族函数投递一个QuitTask（见下面）
	void Quit();

	// Quit方法的变体，用于立即从Run返回而不处理任何待处理的Task
	void QuitNow();

	// PostTask族函数都用于给MessageLoop添加异步执行的任务：
	//
	// - PostTask用于添加普通的嵌套任务
	// - PostDelayedTask用于添加定时任务，适合一些需要被延后指定时间执行的嵌套任务
	// - PostNonNestableTask用于添加非嵌套执行的任务，这类任务只在最顶层的MessageLoop的Run方法中被执行的任务。
	//   在嵌套的MessageLoop中PostNonNestableTask，那么这些被Post的Task将被暂存。
	// - PostNonNestableDelayedTask用于添加非嵌套的定时任务，这类任务只在特定时间在最顶层的MessageLoop的Run方法
	//   中被执行的任务，它不会在嵌套的MessageLoop中，适合需要被延后指定时间执行的非嵌套任务
	//
	// PostTask族函数均线程安全，一个线程可以使用这些方法给其他线程发送任务
	//
	// 注意：一个任务被Post到MessageLoop之后，其生命周期将由这个MessageLoop所在的线程控制
	void PostTask(const StdClosure &task);
	void PostDelayedTask(const StdClosure &task, TimeDelta delay);
	void PostNonNestableTask(const StdClosure &task);
	void PostNonNestableDelayedTask(const StdClosure &task, TimeDelta delay);

	// SetNestableTasksAllowed用于启用或者禁用嵌套任务处理
	// 如果启用嵌套任务，那么Task将被立即执行，否则将先被暂存在一个队列中直到上层任务执行完成再执行
	// 典型的场景：
	// - 线程启动了一个MessageLoop
	// - 线程收到了一个Task，Task #1
	// - Task #1执行过程中弹出了一个MessageBox从而隐式地进入另一个MessageLoop，即MessageBox消息循环。
	// - 在MessageBox的MessageLoop尚未结束前，线程又收到了另一个Task，Task #2
	// - 如果此时嵌套任务处理被启用，那么Task #2将立即被执行（不管Task #1是否已经结束），
	//   否则，Task #2将在Task #1被执行完成后再在线程的MessageLoop中执行。
	void SetNestableTasksAllowed(bool allowed);
	bool IsNestableTasksAllowed() const { return nestable_tasks_allowed_; }
	bool IsNested() const { if (state_) return state_->run_depth > 1; return false; }

	// MessageLoopProxy提供跨线程安全访问MessageLoop的机制，
	// 所有非线程内的PostTask族函数必须通过MessageLoopProxy调用
	std::shared_ptr<MessageLoopProxy> message_loop_proxy() {
		return message_loop_proxy_;
	}

	// MessageLoop销毁观察者，MessageLoop销毁前将会通知这些观察者
	class BASE_EXPORT DestructionObserver
	{
	public:
		virtual void PreDestroyCurrentMessageLoop() = 0;
		virtual ~DestructionObserver() {}
	};

	void AddDestructionObserver(DestructionObserver *observer);
	void RemoveDestructionObserver(DestructionObserver *observer);

	// 任务观察者，每个任务被处理前和处理后均会通知这些观察者
	class BASE_EXPORT TaskObserver
	{
	public:
		virtual void PreProcessTask() = 0;
		virtual void PostProcessTask() = 0;

	protected:
		virtual ~TaskObserver();
	};

	void AddTaskObserver(TaskObserver* observer);
	void RemoveTaskObserver(TaskObserver* observer);

#if defined(OS_WIN)
	bool os_modal_loop() const  { return os_modal_loop_; }
	void set_os_modal_loop(bool os_modal_loop) { os_modal_loop_ = os_modal_loop; }
#endif // OS_WIN

protected:
	struct RunState
	{
		int run_depth;
		bool quit_received;
#if defined(OS_WIN)
		Dispatcher* dispatcher;
#endif
	};

	class BASE_EXPORT AutoRunState : RunState
	{
	public:
		explicit AutoRunState(MessageLoop* loop);
		~AutoRunState();
	private:
		MessageLoop* loop_;
		RunState* previous_state_;
	};

	class ScopedNestableTaskAllower {
	public:
		explicit ScopedNestableTaskAllower(MessageLoop* loop)
			: loop_(loop),
			old_state_(loop_->IsNestableTasksAllowed()) {
				loop_->SetNestableTasksAllowed(true);
		}
		~ScopedNestableTaskAllower() {
			loop_->SetNestableTasksAllowed(old_state_);
		}

	private:
		MessageLoop* loop_;
		bool old_state_;
	};
	
	struct PendingTask
	{
		PendingTask(const StdClosure &task);
		PendingTask(const StdClosure &task,
			nbase::TimeTicks delayed_run_time,
			bool nestable);

		~PendingTask();

		// 用于优先队列的排序，std::heap默认为大顶堆，
		// 而我们要的是小顶堆，所以这个操作符重载实际得返回大于的比较结果
		bool operator<(const PendingTask& other) const;
		// 任务被运行的时刻，这个也用于鉴别定时任务与非定时任务
		nbase::TimeTicks delayed_run_time;
		// 定时任务序号，可作为定时任务的第二排序键，非定时任务此项无效
		int sequence_num;
		// 是否允许在嵌套的MessageLoop中被执行
		bool nestable;

		void Run()
		{
			if (std_task) {
				std_task();
			}
			else {
				assert(false);
			}
		}

	private:
		StdClosure std_task;
	};

	class TaskQueue : public std::queue<PendingTask>
	{
	public:
		void Swap(TaskQueue* queue)
		{
			c.swap(queue->c);  // 常数时间复杂度的内存交换
		}
	};

	typedef std::priority_queue<PendingTask> DelayedTaskQueue;

	virtual bool DoWork();
	virtual bool DoDelayedWork(nbase::TimeTicks* next_delayed_work_time);
	virtual bool DoIdleWork();

	void RunInternal();

	// AddToIncomingQueue函数线程安全，其余均为不线程安全
	virtual void AddToIncomingQueue(const PendingTask &task);
	void AddToDelayedWorkQueue(const PendingTask &task);
	void ReloadWorkQueue();
	bool DeferOrRunPendingTask(const PendingTask &task);
	void RunTask(const PendingTask &task);
	bool ProcessNextDelayedNonNestableTask();
	bool DeletePendingTasks();

	void PreDestruct();
	void PreProcessTask();
	void PostPrecessTask();

	static TimeTicks EvalDelayedRuntime(int64_t delay_ms);

	// MessageLoop类型
	Type type_;
	// 运行状态
	RunState *state_;
	// 消息泵
	std::shared_ptr<MessagePump> pump_;

#if defined(OS_WIN)
	// OS是否处于模态循环中，比如调用了MessageBox之后消息循环被OS接管，这样就处于OS模态循环中
	bool os_modal_loop_;
#endif // OS_WIN

	// 是否允许嵌套任务
	bool nestable_tasks_allowed_;
	// 任务输入队列，任何经过Post族函数加入的任务都首先进入该队列，之后由运行Run的线程分配到各个专职队列
	TaskQueue incoming_queue_;
	// 输入队列锁
	NLock incoming_queue_lock_;

	// 这个工作队列的设计是为了最大限度减少对输入队列的加锁操作。
	// 这种关系类似于双缓存设计，输入队列暂存本线程和其他线程投递过来的任务，
	// 工作队列则仅仅被运行Run方法的线程操作，只有线程检查到工作队列为空才会去输入队列拉任务放到工作队列然后逐个运行。
	// 这里采用了交换内存的方法来提高拉任务的效率，也就是说拉任务的操作其实不是一个一个从输入队列中移动任务来完成的，
	// 而是直接交换双方的内存，这样交换后输入队列瞬时变空，而任务队列则拥有了之前输入队列所有的数据。
	TaskQueue work_queue_;
	// MessageLoop处于嵌套中时，非嵌套任务将被暂时缓存在这个队列，等MessageLoop回到顶层的时候再通过DoIdleWork逐个执行之
	TaskQueue deferred_non_nestable_work_queue_;
	// 定时任务队列（嵌套和非嵌套）
	DelayedTaskQueue delayed_work_queue_;
	// 下一个定时任务的序列号
	int next_delayed_task_sequence_num_;
	// 最近一次调用TimeTicks::Now方法的时间
	TimeTicks recent_tick_;
	// 任务观察者列表
	ObserverList<TaskObserver> task_observers_;
	// MessageLoop销毁观察者列表
	ObserverList<DestructionObserver> destruction_observers_;
	// The message loop proxy associated with this message loop, if one exists.
	std::shared_ptr<MessageLoopProxy> message_loop_proxy_;

	DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

#if defined(OS_WIN)

class BASE_EXPORT UIMessageLoop : public MessageLoop
{
public:

#if defined(OS_WIN)
	typedef WinUIMessagePump::UIObserver UIObserver;
#else
#error Not support currently!
#endif

	UIMessageLoop();

	static UIMessageLoop* current()
	{
		return reinterpret_cast<UIMessageLoop *>(MessageLoop::current());
	}

	void AddUIObserver(UIObserver *observer);
	void RemoveUIObserver(UIObserver *observer);
};

#endif // OS_WIN

class BASE_EXPORT IOMessageLoop : public MessageLoop
{
public:

#if defined(OS_WIN)
	typedef WinIOMessagePump::IOHandler  IOHandler;
	typedef WinIOMessagePump::IOContext  IOContext;
	typedef WinIOMessagePump::IOObserver IOObserver;
#elif defined(OS_POSIX)
	typedef LibeventMessagePump::Watcher Watcher;
	typedef LibeventMessagePump::FileDescriptorWatcher FileDescriptorWatcher;
	typedef LibeventMessagePump::IOObserver IOObserver;

	enum Mode
	{
		WATCH_READ = LibeventMessagePump::WATCH_READ,
		WATCH_WRITE = LibeventMessagePump::WATCH_WRITE,
		WATCH_READ_WRITE = LibeventMessagePump::WATCH_READ_WRITE
	};
#else
#error Not support currently!
#endif

	IOMessageLoop();

	static IOMessageLoop* current()
	{
		return reinterpret_cast<IOMessageLoop *>(MessageLoop::current());
	}

	void AddIOObserver(IOObserver *observer);
	void RemoveIOObserver(IOObserver *observer);

#if defined(OS_WIN)
	void RegisterIOHandler(HANDLE file_handle, IOHandler *handler);
	bool WaitForIOCompletion(DWORD timeout, IOHandler *handler);

#elif defined(OS_POSIX)
	bool WatchFileDescriptor(int fd,
		                     bool persistent,
		                     Mode mode,
		                     FileDescriptorWatcher *controller,
		                     Watcher *delegate);
#endif
};



} // namespace nbase

#endif // BASE_FRAMEWORK_MESSAGE_LOOP_H_
