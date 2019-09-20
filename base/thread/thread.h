// Thread is the low-level platform-specific abstraction
// to the OS's threading interface. 

#ifndef BASE_THREAD_THREAD_H_
#define BASE_THREAD_THREAD_H_

#include "base/base_config.h"
#include "base/base_export.h"

#if defined(OS_WIN)
#include <process.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "base/base_types.h"

namespace nbase
{

#if defined(OS_WIN)
typedef uint32_t   ThreadId;
typedef void*      ThreadHandle;
#elif defined(OS_POSIX)
typedef pid_t      ThreadId;
typedef pthread_t  ThreadHandle;
#endif

const ThreadId kInvalidThreadId = 0;

// Valid values for thread priority
enum ThreadPriority
{
	kThreadPriorityDefault,
	kThreadPriorityLow,
	kThreadPriorityNormal,
	kThreadPriorityHigh,
	kThreadPriorityRealtime
};

class BASE_EXPORT Thread
{
public:
	Thread();
	virtual ~Thread();

    // Create a thread with a option, if the option is null, thread will start with
	// a default option
	bool Create();

	// Close the thread
	void Close();

	// Terminate the thread
	void Terminate();

	// Sleeps for the specified duration (units are milliseconds)
	static void Sleep(int duration_ms);

	// Yield the current thread so another thread can be scheduled
	static void YieldThread();

	// Gets the current thread id
	static ThreadId CurrentId();

	ThreadId thread_id() const { return thread_id_; }

	void set_thread_id(ThreadId thread_id) { thread_id_ = thread_id; }

	// Gets the current thread handle
	ThreadHandle thread_handle() const  { return thread_handle_; }

	// Set the thread's priority
	void set_thread_priority(ThreadPriority priority) { thread_priority_ = priority; }

protected:
	// Thread process function
	virtual void Run() {}

private:
#if defined(OS_WIN)
	static uint32_t __stdcall ThreadProcFunc(void *arg);
#else
	static void* ThreadProcFunc(void *arg);
#endif

    ThreadId       thread_id_;
	ThreadHandle   thread_handle_;
	ThreadPriority thread_priority_;
};

#if defined(OS_WIN) && defined(COMPILER_MSVC)
BASE_EXPORT void SetThreadName(DWORD thread_id, const char* name);
#endif

}

#endif  // BASE_THREAD_THREAD_H_