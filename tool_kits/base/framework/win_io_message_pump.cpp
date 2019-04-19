// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2011/6/8
//
// a implemention of a Windows specific message pump for I/O,
// the mechanism of which is from the Google Chrome project

#include "base/framework/win_io_message_pump.h"

#if defined(OS_WIN)
#include <assert.h>
#include <algorithm>

namespace nbase
{

WinIOMessagePump::WinIOMessagePump()
{
	port_.Reset(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1));
	assert(port_.Valid());
}

void WinIOMessagePump::ScheduleWork()
{
	if (::InterlockedExchange(&have_work_, 1))
		return;  // Someone else continued the pumping.

	// Make sure the MessagePump does some work for us.
	BOOL ret = ::PostQueuedCompletionStatus(port_, 0,
											reinterpret_cast<ULONG_PTR>(this),
											reinterpret_cast<OVERLAPPED*>(this));
	assert(ret);
}

void WinIOMessagePump::ScheduleDelayedWork(const TimeTicks& delayed_work_time)
{
	// We know that we can't be blocked right now since this method can only be
	// called on the same thread as Run, so we only need to update our record of
	// how long to sleep when we do sleep.
	delayed_work_time_ = delayed_work_time;
}

void WinIOMessagePump::RegisterIOHandler(HANDLE file_handle, IOHandler *handler)
{
	ULONG_PTR key = reinterpret_cast<ULONG_PTR>(handler);
	HANDLE port = ::CreateIoCompletionPort(file_handle, port_, key, 1);
	assert(port);
}

void WinIOMessagePump::DoRunLoop()
{
	for (;;)
	{
		// If we do any work, we may create more messages etc., and more work may
		// possibly be waiting in another task group.  When we (for example)
		// WaitForIOCompletion(), there is a good chance there are still more
		// messages waiting.  On the other hand, when any of these methods return
		// having done no work, then it is pretty unlikely that calling them
		// again quickly will find any work to do.  Finally, if they all say they
		// had no work, then it is a good time to consider sleeping (waiting) for
		// more work.

		bool more_work_is_plausible = state_->delegate->DoWork();
		if (state_->should_quit)
			break;

		more_work_is_plausible |= WaitForIOCompletion(0, NULL);
		if (state_->should_quit)
			break;

		more_work_is_plausible |=
				state_->delegate->DoDelayedWork(&delayed_work_time_);
		if (state_->should_quit)
			break;

		if (more_work_is_plausible)
			continue;

		more_work_is_plausible = state_->delegate->DoIdleWork();
		if (state_->should_quit)
			break;

		if (more_work_is_plausible)
			continue;

		WaitForWork();  // Wait (sleep) until we have work to do again.
	}
}

// Wait until IO completes, up to the time needed by the timer manager to fire
// the next set of timers.
void WinIOMessagePump::WaitForWork()
{
	// We do not support nested IO message loops. This is to avoid messy
	// recursion problems.
	assert(state_->run_depth == 1);

	int64_t timeout = GetCurrentDelay();
	DWORD wait_time;
	if (timeout < 0)  // Negative value means no timers waiting.
		wait_time = INFINITE;
	else if (timeout > 0xfffffffe)
		wait_time = 0xfffffffe;
	else
		wait_time = static_cast<DWORD>(timeout);

	WaitForIOCompletion(wait_time, NULL);
}

bool WinIOMessagePump::WaitForIOCompletion(DWORD timeout, IOHandler *filter)
{
	IOItem item;
	if (completed_io_.empty() || !MatchCompletedIOItem(filter, &item))
	{
		// We have to ask the system for another IO completion.
		if (!GetIOItem(timeout, &item))
			return false;

		if (ProcessInternalIOItem(item))
			return true;
	}

	if (item.context->handler)
	{
		if (filter && item.handler != filter)
		{
			// Save this item for later
			completed_io_.push_back(item);
		}
		else
		{
			assert(item.context->handler == item.handler);
			PreProcessIOEvent();
			item.handler->OnIOCompleted(item.context, item.bytes_transfered, item.error);
			PostProcessIOEvent();
		}
	}
	else
	{
		// The handler must be gone by now, just cleanup the mess.
		delete item.context;
	}

	return true;
}

// Asks the OS for another IO completion result.
bool WinIOMessagePump::GetIOItem(DWORD timeout, IOItem* item)
{
	memset(item, 0, sizeof(*item));
	ULONG_PTR key = NULL;
	OVERLAPPED* overlapped = NULL;
	if (!::GetQueuedCompletionStatus(port_.Get(),
									 &item->bytes_transfered,
									 &key,
									 &overlapped,
									 timeout))
	{
		if (!overlapped)
			return false;  // Nothing in the queue.
		item->error = GetLastError();
		item->bytes_transfered = 0;
	}

	item->handler = reinterpret_cast<IOHandler*>(key);
	item->context = reinterpret_cast<IOContext*>(overlapped);
	return true;
}

bool WinIOMessagePump::ProcessInternalIOItem(const IOItem& item)
{
	if (this == reinterpret_cast<WinIOMessagePump*>(item.context) &&
		this == reinterpret_cast<WinIOMessagePump*>(item.handler))
	{
		// This is our internal completion.
		assert(!item.bytes_transfered);
		::InterlockedExchange(&have_work_, 0);
		return true;
	}
	return false;
}

// Returns a completion item that was previously received.
bool WinIOMessagePump::MatchCompletedIOItem(IOHandler* filter, IOItem* item)
{
	assert(!completed_io_.empty());
	for (std::list<IOItem>::iterator it = completed_io_.begin(); it != completed_io_.end(); ++it)
	{
		if (!filter || it->handler == filter)
		{
			*item = *it;
			completed_io_.erase(it);
			return true;
		}
	}
	return false;
}

void WinIOMessagePump::AddObserver(IOObserver *observer)
{
	observers_.AddObserver(observer);
}

void WinIOMessagePump::RemoveObserver(IOObserver *observer)
{
	observers_.RemoveObserver(observer);
}

void WinIOMessagePump::PreProcessIOEvent()
{
	AutoLazyEraser lazy_eraser(&observers_);
	size_t index = 0;
	IOObserver* observer;
	while (index < observers_.GetObserverCount())
	{
		observer = observers_.GetObserver(index++);
		if (observer == NULL)
			continue;
		observer->PreProcessIOEvent();
	}
}

void WinIOMessagePump::PostProcessIOEvent()
{
	AutoLazyEraser lazy_eraser(&observers_);
	size_t index = 0;
	IOObserver* observer;
	while (index < observers_.GetObserverCount())
	{
		observer = observers_.GetObserver(index++);
		if (observer == NULL)
			continue;
		observer->PostProcessIOEvent();
	}
}

} // namespace nbase

#endif  // OS_WIN
