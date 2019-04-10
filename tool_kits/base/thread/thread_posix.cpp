// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/11
//
// Thread implementation on posix platform.

#include "base/thread/thread.h"

#if defined(OS_POSIX)

#include <assert.h>
#include <errno.h>

#if defined(OS_LINUX)
#include <sys/syscall.h>
#endif

namespace nbase
{

Thread::Thread()
{
	thread_id_       = 0;
	thread_handle_   = 0;
	thread_priority_ = kThreadPriorityDefault;
}

Thread::~Thread()
{
	Close();
}

bool Thread::Create()
{
	// initialize the thread attribute
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

	// if thread need set priority
	if (thread_priority_ > kThreadPriorityDefault)
	{
		pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
		struct sched_param param;
		if (thread_priority_ == kThreadPriorityLow)
			param.sched_priority = 15;
		else if (thread_priority_ == kThreadPriorityNormal)
			param.sched_priority = 0;
		else if (thread_priority_ == kThreadPriorityHigh)
			param.sched_priority = -15;
		else if (thread_priority_ == kThreadPriorityRealtime)
			param.sched_priority = -20;
		pthread_attr_setschedparam(&thread_attr, &param);
	}

	if (pthread_create(&thread_handle_, &thread_attr, ThreadProcFunc, this))
	{
		pthread_attr_destroy(&thread_attr);
		return false;
	}
	pthread_attr_destroy(&thread_attr);

	return true;
}

void Thread::Close()
{
	if (thread_handle_ != 0)
	{
		if (pthread_join(thread_handle_, NULL))
			assert(0);
		thread_handle_ = 0;
	}
}

void Thread::Terminate()
{
	if (thread_handle_ != 0)
	{
        pthread_cancel(thread_handle_);
	}
}

// static
void Thread::Sleep(int duration_ms)
{
	struct timespec sleep_time, remaining;

	// Contains the portion of duration_ms >= 1 sec.
	sleep_time.tv_sec = duration_ms / 1000;
	duration_ms -= sleep_time.tv_sec * 1000;

	// Contains the portion of duration_ms < 1 sec.
	sleep_time.tv_nsec = duration_ms * 1000 * 1000;  // nanoseconds.

	while (nanosleep(&sleep_time, &remaining) == -1 && errno == EINTR)
		sleep_time = remaining;
}

// static
void Thread::YieldThread()
{
	sched_yield();
}

// static
ThreadId Thread::CurrentId()
{
#if defined(OS_LINUX)
	return syscall(_NR_gettid);
#elif defined(OS_FREEBSD)
	return reinterpret_cast<int64_t> (pthread_self());
#else	
	return reinterpret_cast<int64_t> (pthread_self());
#endif
}

void* Thread::ThreadProcFunc(void *arg)
{
	Thread* the_thread = (Thread*)arg;
	assert(the_thread);
	if (the_thread != NULL)
	{
		the_thread->Run();
	}
    return NULL;
}

}  // namespace nbase

#endif  // OS_POSIX


