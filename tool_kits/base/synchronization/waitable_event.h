// The base class of a cross flatform waitable event
// NOTE: support Windows only currently

#ifndef BASE_SYNCHRONIZATION_WAITABLE_EVENT_H_
#define BASE_SYNCHRONIZATION_WAITABLE_EVENT_H_

#include "base/base_config.h"
#include "base/base_types.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_POSIX)
#include <list>
#include <utility>
#include "base/memory/ref_count.h"
#include "base/synchronization/lock.h"
#endif  // OS_POSIX

namespace nbase
{

class TimeDelta;

// This replaces INFINITE from Win32
static const int kNoTimeout = -1;


class WaitableEvent
{
public:

	WaitableEvent(bool manual_reset, bool initially_signaled);

#if defined(OS_WIN)

	explicit WaitableEvent(HANDLE handle);

	// Releases ownership of the handle from this object.
	HANDLE Release();

	operator HANDLE() const { return handle_; }

#endif

	~WaitableEvent();

	// Put the event in the un-signaled state.
	void Reset();

	// Put the event in the signaled state. Causing any thread blocked on Wait
	// to be woken up.
	void Signal();

	// Returns true if the event is in the signaled state, else false.  If this
	// is not a manual reset event, then this test will cause a reset.
	bool IsSignaled();

	// Wait indefinitely for the event to be signaled.  Returns true if the event
	// was signaled, else false is returned to indicate that waiting failed.
	bool Wait();

	// Wait up until max_time has passed for the event to be signaled.  Returns
	// true if the event was signaled.  If this method returns false, then it
	// does not necessarily mean that max_time was exceeded.
	bool WaitTimeout(const TimeDelta &timeout);

	// Wait, synchronously, on multiple events.
	static size_t WaitMultiple(WaitableEvent **events, size_t count);

	// This is a private helper class. It's here because it's used by friends of
	// this class (such as WaitableEventWatcher) to be able to enqueue elements
	// of the wait-list
	class Waiter
	{
	public:
		// Signal the waiter to wake up.
		//
		// Consider the case of a Waiter which is in multiple WaitableEvent's
		// wait-lists. Each WaitableEvent is automatic-reset and two of them are
		// signaled at the same time. Now, each will wake only the first waiter in
		// the wake-list before resetting. However, if those two waiters happen to
		// be the same object (as can happen if another thread didn't have a chance
		// to dequeue the waiter from the other wait-list in time), two auto-resets
		// will have happened, but only one waiter has been signaled!
		//
		// Because of this, a Waiter may "reject" a wake by returning false. In
		// this case, the auto-reset WaitableEvent shouldn't act as if anything has
		// been notified.
		virtual bool Fire(WaitableEvent *signaling_event) = 0;

		// Waiters may implement this in order to provide an extra condition for
		// two Waiters to be considered equal. In WaitableEvent::Dequeue, if the
		// pointers match then this function is called as a final check. See the
		// comments in ~Handle for why.
		virtual bool Compare(void *tag) = 0;

	protected:
		virtual ~Waiter() {}
	};

private:
#if defined(OS_WIN)
	HANDLE handle_;
#else
	struct WaitableEventKernel : public ThreadSafeRefCount
	{
	public:
		WaitableEventKernel(bool manual_reset, bool initially_signaled);
		virtual ~WaitableEventKernel();

		bool Dequeue(Waiter *waiter, void *tag);

		nbase::NLock lock_;
		const bool manual_reset_;
		bool signaled_;
		std::list<Waiter*> waiters_;
	};

	typedef std::pair<WaitableEvent*, size_t> WaiterAndIndex;

	// When dealing with arrays of WaitableEvent*, we want to sort by the address
	// of the WaitableEvent in order to have a globally consistent locking order.
	// In that case we keep them, in sorted order, in an array of pairs where the
	// second element is the index of the WaitableEvent in the original,
	// unsorted, array.
	static size_t EnqueueMultiple(WaiterAndIndex *waitables,
		size_t count, Waiter *waiter);

	bool SignalAll();
	bool SignalOne();
	void Enqueue(Waiter *waiter);

	scoped_refptr<WaitableEventKernel> kernel_;

#endif

	DISALLOW_COPY_AND_ASSIGN(WaitableEvent);
};

}

#endif // BASE_SYNCHRONIZATION_WAITABLE_EVENT_H_
