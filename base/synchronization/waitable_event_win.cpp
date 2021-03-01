// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2011/6/24
//
// The base class of a cross flatform waitable event

#include "base/synchronization/waitable_event.h"
#if defined(OS_WIN)

#include <math.h>
#include <assert.h>
#include "base/time/time.h"

namespace nbase
{

WaitableEvent::WaitableEvent(bool manual_reset, bool initially_signaled)
{
	handle_ = ::CreateEventW(NULL, manual_reset, initially_signaled, NULL);
}

WaitableEvent::WaitableEvent(HANDLE handle)
{
	handle_ = handle;
}

HANDLE WaitableEvent::Release()
{
	HANDLE old_handle = handle_;
	handle_ = NULL;
	return old_handle;
}

WaitableEvent::~WaitableEvent()
{
	if (handle_)
		::CloseHandle(handle_);
}

void WaitableEvent::Reset()
{
	assert(handle_);
	::ResetEvent(handle_);
}

void WaitableEvent::Signal()
{
	assert(handle_);
	::SetEvent(handle_);
}

bool WaitableEvent::IsSignaled()
{
	assert(handle_);
	return ::WaitForSingleObject(handle_, 0) == WAIT_OBJECT_0;
}

bool WaitableEvent::Wait()
{
	assert(handle_);
	return WAIT_OBJECT_0 == ::WaitForSingleObject(handle_, INFINITE);
}

bool WaitableEvent::WaitTimeout(const TimeDelta &timeout)
{
	assert(handle_);
	assert(timeout.ToMillisecondsF() >= 0.0);
	DWORD wait_result = ::WaitForSingleObject(handle_, static_cast<DWORD>(ceil(timeout.ToMillisecondsF())));
	return WAIT_OBJECT_0 == wait_result;
}

size_t WaitableEvent::WaitMultiple(WaitableEvent **events, size_t count)
{
	HANDLE handles[MAXIMUM_WAIT_OBJECTS];
	assert(count <= MAXIMUM_WAIT_OBJECTS);
	for (size_t i = 0; i < count; i++)
		handles[i] = events[i]->handle_;
	DWORD wait_result = ::WaitForMultipleObjects(static_cast<DWORD>(count),
												 handles,
												 FALSE,
												 INFINITE);
	if (wait_result >= WAIT_OBJECT_0 + count)
		return 0;
	return wait_result - WAIT_OBJECT_0;
}

} // namespace nbase

#endif // OS_WIN