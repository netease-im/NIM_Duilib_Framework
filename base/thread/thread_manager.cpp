// Copyright (c) 2012, NetEase Inc. All rights reserved.
//
// wrt(guangguang)
// 2012/2/22
//
// a thread manager for iter-thread communicatios, etc.

#include "thread_manager.h"
#include "base/framework/message_loop.h"
//#include "base/log/log.h"
#include "base/memory/singleton.h"
#include "base/third_party/chrome/atomicops.h"

#define AUTO_MAP_LOCK() NAutoLock __l(&GetInstance()->lock_);
#define AQUIRE_ACCESS() {if (!AquireAccess()) { /*DCHECK(false);*/ return false; }}

namespace nbase
{

bool ThreadMap::AquireAccess()
{
	FrameworkThreadTlsData *tls = FrameworkThread::GetTlsData();
	if (!tls || tls->managed < 1)
		return false;
	return true;
}

bool ThreadMap::RegisterThread(int self_identifier)
{
	//DCHECK(self_identifier >= 0);
	if (self_identifier < 0)
		return false;

	FrameworkThreadTlsData *tls = FrameworkThread::GetTlsData();
	//DCHECK(tls); // should be called by a Framework thread
	if (tls == NULL)
		return false;

	AUTO_MAP_LOCK()
	std::pair<std::map<int, FrameworkThread*>::iterator, bool> pr =
		ThreadMap::GetInstance()->threads_.insert(
			std::make_pair(self_identifier, tls->self));
	if (!pr.second)
	{
		if (pr.first->second != tls->self)
		{
			//DCHECK(false); // another thread has registered with the same id
			return false;
		}
		// yes, it's me, check logic error
		//DCHECK(tls->managed > 0);
		//DCHECK(tls->managed_thread_id == self_identifier);
	}
	// 'self' is registered
	tls->managed++;
	tls->managed_thread_id = self_identifier;
	return true;
}

bool ThreadMap::UnregisterThread()
{
	FrameworkThreadTlsData *tls = FrameworkThread::GetTlsData();
	//DCHECK(tls); // should be called by a Framework thread
	//DCHECK(tls->managed > 0); // should be managed
	if (!tls || tls->managed < 1)
		return false;

	// remove from internal thread map
	// here, since tls->managed is greater than zero,
	// we must have a reference of the glabal ThreadManager instance (see RegisterThread)
	if (--tls->managed == 0)
	{
		AUTO_MAP_LOCK()
		std::map<int, FrameworkThread*>::iterator iter = GetInstance()->threads_.find(tls->managed_thread_id);
		if (iter != GetInstance()->threads_.end())
			GetInstance()->threads_.erase(iter);
		else {}
			//DCHECK(false);	// logic error, we should not come here
		tls->managed_thread_id = -1;
	}

	return true;
}

// no lock
FrameworkThread* ThreadMap::QueryThreadInternal(int identifier) const
{
	std::map<int, FrameworkThread*>::iterator iter
		= GetInstance()->threads_.find(identifier);
	if (iter == GetInstance()->threads_.end())
		return NULL;
	return iter->second;
}

int ThreadMap::QueryThreadId(const FrameworkThread *thread)
{
	AQUIRE_ACCESS()
	AUTO_MAP_LOCK()
	std::map<int, FrameworkThread*>::iterator iter;
	for (iter = GetInstance()->threads_.begin();
		iter != GetInstance()->threads_.end(); iter++) {
		if (iter->second == thread)
			return iter->first;
	}
	return -1;
}

std::shared_ptr<MessageLoopProxy> ThreadMap::GetMessageLoop(int identifier) const
{
	FrameworkThread *thread = QueryThreadInternal(identifier);
	if (thread == NULL)
		return NULL;
	MessageLoop *message_loop = thread->message_loop();
	if (message_loop == NULL)
		return NULL;
	return message_loop->message_loop_proxy();
}

bool ThreadManager::RegisterThread(int self_identifier)
{
	return ThreadMap::GetInstance()->RegisterThread(self_identifier);
}

bool ThreadManager::UnregisterThread()
{
	return ThreadMap::GetInstance()->UnregisterThread();
}

int ThreadManager::QueryThreadId(const FrameworkThread *thread)
{
	return ThreadMap::GetInstance()->QueryThreadId(thread);
}

FrameworkThread* ThreadManager::CurrentThread()
{
	FrameworkThreadTlsData *tls = FrameworkThread::GetTlsData();
	//DCHECK(tls); // should be called by a Framework thread
	//DCHECK(tls->managed > 0); // should be managed
	
	if (!tls || tls->managed < 1)
		return NULL;
	return tls->self;
}

bool ThreadManager::PostTask(const StdClosure &task)
{
	MessageLoop::current()->PostTask(task);
	return true;
}

bool ThreadManager::PostTask(int identifier, const StdClosure &task)
{
	std::shared_ptr<MessageLoopProxy> message_loop =
		ThreadMap::GetInstance()->GetMessageLoop(identifier);
	if (message_loop == NULL)
		return false;
	message_loop->PostTask(task);
	return true;
}

bool ThreadManager::PostDelayedTask(const StdClosure &task, TimeDelta delay)
{
	MessageLoop::current()->PostDelayedTask(task, delay);
	return true;
}

bool ThreadManager::PostDelayedTask(int identifier, const StdClosure &task, TimeDelta delay)
{
	std::shared_ptr<MessageLoopProxy> message_loop =
		ThreadMap::GetInstance()->GetMessageLoop(identifier);
	if (message_loop == NULL)
		return false;
	message_loop->PostDelayedTask(task, delay);
	return true;
}

void ThreadManager::PostRepeatedTask(const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times)
{
	StdClosure closure = nbase::Bind(&ThreadManager::RunRepeatedly, task, delay, times);
	nbase::ThreadManager::PostDelayedTask(closure, delay);
}

void ThreadManager::PostRepeatedTask(int thread_id, const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times)
{
	StdClosure closure = nbase::Bind(&ThreadManager::RunRepeatedly2, thread_id, task, delay, times);
	nbase::ThreadManager::PostDelayedTask(thread_id, closure, delay);
}

bool ThreadManager::PostNonNestableTask(const StdClosure &task)
{
	MessageLoop::current()->PostNonNestableTask(task);
	return true;
}

bool ThreadManager::PostNonNestableTask(int identifier, const StdClosure &task)
{
	std::shared_ptr<MessageLoopProxy> message_loop =
		ThreadMap::GetInstance()->GetMessageLoop(identifier);
	if (message_loop == NULL)
		return false;
	message_loop->PostNonNestableTask(task);
	return true;
}

bool ThreadManager::PostNonNestableDelayedTask(const StdClosure &task, TimeDelta delay)
{
	MessageLoop::current()->PostNonNestableDelayedTask(task, delay);
	return true;
}

bool ThreadManager::PostNonNestableDelayedTask(int identifier, const StdClosure &task, TimeDelta delay)
{
	std::shared_ptr<MessageLoopProxy> message_loop =
		ThreadMap::GetInstance()->GetMessageLoop(identifier);
	if (message_loop == NULL)
		return false;
	message_loop->PostNonNestableDelayedTask(task, delay);
	return true;
}

void ThreadManager::RunRepeatedly(const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times)
{
	if (task.Expired()) {
		return;
	}
	task();
	if (task.Expired()) {
		return;
	}
	if (times != TIMES_FOREVER) {
		times--;
	}
	if (times != 0) {
		PostRepeatedTask(task, delay, times);
	}
}

void ThreadManager::RunRepeatedly2(int thread_id, const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times)
{
	if (task.Expired()) {
		return;
	}
	task();
	if (task.Expired()) {
		return;
	}
	if (times != TIMES_FOREVER) {
		times--;
	}
	if (times != 0) {
		PostRepeatedTask(thread_id, task, delay, times);
	}
}


}
