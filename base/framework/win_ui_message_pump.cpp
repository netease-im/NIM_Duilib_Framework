// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2011/6/8
//
// a implemention of a Windows specific message pump for user interface,
// the mechanism of which is from the Google Chrome project

#include "base/framework/win_ui_message_pump.h"

#if defined(OS_WIN)
#include <algorithm>
#include "base/framework/message_loop.h"

namespace nbase
{

static const wchar_t kWndClass[] = L"NeteaseMessagePumpWindow";
static const unsigned int kMsgHaveWork = WM_USER + 1;
static const int kUserTimerMinmum = 10;

WinUIMessagePump::WinUIMessagePump()
{
	InitMessageWnd();
}

WinUIMessagePump::~WinUIMessagePump()
{
	::DestroyWindow(message_hwnd_);
	::UnregisterClassW(kWndClass, ::GetModuleHandle(NULL));
}

void WinUIMessagePump::ScheduleWork()
{
	if (::InterlockedExchange(&have_work_, 1))
		return;	// Pump已经开始

	// 通知MessagePump有新任务到达，如果MessagePump处于睡眠状态，这将唤醒它
	::PostMessageW(message_hwnd_, kMsgHaveWork, reinterpret_cast<WPARAM>(this), 0);
}

void WinUIMessagePump::ScheduleDelayedWork(const TimeTicks& delayed_work_time)
{
	//
	// We would *like* to provide high resolution timers.  Windows timers using
	// SetTimer() have a 10ms granularity.  We have to use WM_TIMER as a wakeup
	// mechanism because the application can enter modal windows loops where it
	// is not running our MessageLoop; the only way to have our timers fire in
	// these cases is to post messages there.
	//
	// To provide sub-10ms timers, we process timers directly from our run loop.
	// For the common case, timers will be processed there as the run loop does
	// its normal work.  However, we *also* set the system timer so that WM_TIMER
	// events fire.  This mops up the case of timers not being able to work in
	// modal message loops.  It is possible for the SetTimer to pop and have no
	// pending timers, because they could have already been processed by the
	// run loop itself.
	//
	// We use a single SetTimer corresponding to the timer that will expire
	// soonest.  As new timers are created and destroyed, we update SetTimer.
	// Getting a spurrious SetTimer event firing is benign, as we'll just be
	// processing an empty timer queue.
	//
	delayed_work_time_ = delayed_work_time;

	DWORD wait_time;
	int64_t delay_msec = GetCurrentDelay();
	if (delay_msec < kUserTimerMinmum)
		wait_time = kUserTimerMinmum;
	else if (delay_msec < 0)
		wait_time = INFINITE;
	else if (delay_msec > 0xfffffffe)
		wait_time = 0xfffffffe;
	else
		wait_time = static_cast<DWORD>(delay_msec);


	// Create a WM_TIMER event that will wake us up to check for any pending
	// timers (in case we are running within a nested, external sub-pump).
	::SetTimer(message_hwnd_, reinterpret_cast<UINT_PTR>(this), wait_time, NULL);
}

void WinUIMessagePump::PumpOutPendingPaintMessages()
{
	// If we are being called outside of the context of Run, then don't try to do
	// any work.
	if (!state_)
		return;

	// Create a mini-message-pump to force immediate processing of only Windows
	// WM_PAINT messages.  Don't provide an infinite loop, but do enough peeking
	// to get the job done.  Actual common max is 4 peeks, but we'll be a little
	// safe here.
	const int kMaxPeekCount = 20;
	int peek_count;
	for (peek_count = 0; peek_count < kMaxPeekCount; ++peek_count)
	{
		MSG msg;
		if (!::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE | PM_QS_PAINT))
			break;
		ProcessMessageHelper(msg);
		if (state_->should_quit)  // Handle WM_QUIT.
			break;
	}
}

void WinUIMessagePump::InitMessageWnd()
{
	HINSTANCE hinst = ::GetModuleHandle(NULL);

	WNDCLASSEXW wc = {0};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = WndProcThunk;
	wc.hInstance = hinst;
	wc.lpszClassName = kWndClass;
	::RegisterClassExW(&wc);

	message_hwnd_ = ::CreateWindowW(kWndClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, hinst, 0);
}

LRESULT CALLBACK WinUIMessagePump::WndProcThunk(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case kMsgHaveWork:
		reinterpret_cast<WinUIMessagePump*>(wparam)->HandleWorkMessage();
		break;
	case WM_TIMER:
		reinterpret_cast<WinUIMessagePump*>(wparam)->HandleTimerMessage();
		break;
	}
	return ::DefWindowProcW(hwnd, message, wparam, lparam);
}

void WinUIMessagePump::DoRunLoop()
{
	// IF this was just a simple ::PeekMessageW() loop (servicing all possible work
	// queues), then Windows would try to achieve the following order according
	// to MSDN documentation about ::PeekMessageW with no filter):
	//    * Sent messages
	//    * Posted messages
	//    * Sent messages (again)
	//    * WM_PAINT messages
	//    * WM_TIMER messages
	//
	// Summary: none of the above classes is starved, and sent messages has twice
	// the chance of being processed (i.e., reduced service time).

	for (;;)
	{
		// If we do any work, we may create more messages etc., and more work may
		// possibly be waiting in another task group.  When we (for example)
		// ProcessNextWindowsMessage(), there is a good chance there are still more
		// messages waiting.  On the other hand, when any of these methods return
		// having done no work, then it is pretty unlikely that calling them again
		// quickly will find any work to do.  Finally, if they all say they had no
		// work, then it is a good time to consider sleeping (waiting) for more
		// work.

		bool more_work_is_plausible = ProcessNextWindowsMessage();
		if (state_->should_quit)
			break;

		more_work_is_plausible |= state_->delegate->DoWork();
		if (state_->should_quit)
			break;

		more_work_is_plausible |=
			state_->delegate->DoDelayedWork(&delayed_work_time_);
		// If we did not process any delayed work, then we can assume that our
		// existing WM_TIMER if any will fire when delayed work should run.  We
		// don't want to disturb that timer if it is already in flight.  However,
		// if we did do all remaining delayed work, then lets kill the WM_TIMER.
		if (more_work_is_plausible && delayed_work_time_.is_null())
			KillTimer(message_hwnd_, reinterpret_cast<UINT_PTR>(this));
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

void WinUIMessagePump::WaitForWork()
{
	// Wait until a message is available, up to the time needed by the timer
	// manager to fire the next set of timers.
	int64_t delay = GetCurrentDelay();
	DWORD wait_time;
	if (delay < 0)  // Negative value means no timers waiting.
		wait_time = INFINITE;
	else if (delay > 0xfffffffe)
		wait_time = 0xfffffffe;
	else
		wait_time = static_cast<DWORD>(delay);

	DWORD result = ::MsgWaitForMultipleObjectsEx(0, NULL, wait_time, QS_ALLINPUT, MWMO_INPUTAVAILABLE);

	if (WAIT_OBJECT_0 == result)
	{
		// A WM_* message is available.
		// If a parent child relationship exists between windows across threads
		// then their thread inputs are implicitly attached.
		// This causes the MsgWaitForMultipleObjectsEx API to return indicating
		// that messages are ready for processing (specifically mouse messages
		// intended for the child window. Occurs if the child window has capture)
		// The subsequent PeekMessages call fails to return any messages thus
		// causing us to enter a tight loop at times.
		// The WaitMessage call below is a workaround to give the child window
		// sometime to process its input messages.
		MSG msg = {0};
		DWORD queue_status = ::GetQueueStatus(QS_MOUSE);
		if (HIWORD(queue_status) & QS_MOUSE && !::PeekMessageW(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		{
			WaitMessage();
		}
		return;
	}
}

void WinUIMessagePump::HandleWorkMessage()
{
	// If we are being called outside of the context of Run, then don't try to do
	// any work.  This could correspond to a MessageBox call or something of that
	// sort.
	if (!state_)
	{
		// Since we handled a kMsgHaveWork message, we must still update this flag.
		::InterlockedExchange(&have_work_, 0);
		return;
	}

	// Let whatever would have run had we not been putting messages in the queue
	// run now.  This is an attempt to make our dummy message not starve other
	// messages that may be in the Windows message queue.
	ProcessPumpReplacementMessage();

	// Now give the delegate a chance to do some work.  He'll let us know if he
	// needs to do more work.
	if (state_->delegate->DoWork())
		ScheduleWork();
}

void WinUIMessagePump::HandleTimerMessage()
{
	::KillTimer(message_hwnd_, reinterpret_cast<UINT_PTR>(this));

	// If we are being called outside of the context of Run, then don't do
	// anything.  This could correspond to a MessageBox call or something of
	// that sort.
	if (!state_)
		return;

	state_->delegate->DoDelayedWork(&delayed_work_time_);
	if (!delayed_work_time_.is_null())
	{
		// A bit gratuitous to set delayed_work_time_ again, but oh well.
		ScheduleDelayedWork(delayed_work_time_);
	}
}

bool WinUIMessagePump::ProcessNextWindowsMessage()
{
	// If there are sent messages in the queue then PeekMessage internally
	// dispatches the message and returns false. We return true in this
	// case to ensure that the message loop peeks again instead of calling
	// MsgWaitForMultipleObjectsEx again.
	bool sent_messages_in_queue = false;
	DWORD queue_status = ::GetQueueStatus(QS_SENDMESSAGE);
	if (HIWORD(queue_status) & QS_SENDMESSAGE)
		sent_messages_in_queue = true;

	MSG msg;
	if (::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		return ProcessMessageHelper(msg);

	return sent_messages_in_queue;
}

bool WinUIMessagePump::ProcessMessageHelper(const MSG& msg)
{
	if (WM_QUIT == msg.message)
	{
		// Repost the QUIT message so that it will be retrieved by the primary
		// GetMessage() loop.
		state_->should_quit = true;
		::PostQuitMessage(static_cast<int>(msg.wParam));
		return false;
	}

	// While running our main message pump, we discard kMsgHaveWork messages.
	if (msg.message == kMsgHaveWork && msg.hwnd == message_hwnd_)
		return ProcessPumpReplacementMessage();

	if (::CallMsgFilter(const_cast<MSG*>(&msg), kMessageFilterCode))
		return true;

	PreProcessMessage(msg);

	if (state_->dispatcher)
	{
		if (!state_->dispatcher->Dispatch(msg))
			state_->should_quit = true;
	}
	else
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	PostProcessMessage(msg);

	return true;
}

bool WinUIMessagePump::ProcessPumpReplacementMessage()
{
	// When we encounter a kMsgHaveWork message, this method is called to peek
	// and process a replacement message, such as a WM_PAINT or WM_TIMER.  The
	// goal is to make the kMsgHaveWork as non-intrusive as possible, even though
	// a continuous stream of such messages are posted.  This method carefully
	// peeks a message while there is no chance for a kMsgHaveWork to be pending,
	// then resets the have_work_ flag (allowing a replacement kMsgHaveWork to
	// possibly be posted), and finally dispatches that peeked replacement.  Note
	// that the re-post of kMsgHaveWork may be asynchronous to this thread!!

	bool have_message = false;
	MSG msg;
	// We should not process all window messages if we are in the context of an
	// OS modal loop, i.e. in the context of a windows API call like MessageBox.
	// This is to ensure that these messages are peeked out by the OS modal loop.
	if (MessageLoop::current()->os_modal_loop())
	{
		// We only peek out WM_PAINT and WM_TIMER here for reasons mentioned above.
		have_message = ::PeekMessageW(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE) ||
			::PeekMessageW(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
	}
	else
	{
		have_message = (0 != ::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE));
	}

	// Since we discarded a kMsgHaveWork message, we must update the flag.
	::InterlockedExchange(&have_work_, 0);

	// We don't need a special time slice if we didn't have_message to process.
	if (!have_message)
		return false;

	// Guarantee we'll get another time slice in the case where we go into native
	// windows code.   This ScheduleWork() may hurt performance a tiny bit when
	// tasks appear very infrequently, but when the event queue is busy, the
	// kMsgHaveWork events get (percentage wise) rarer and rarer.
	ScheduleWork();

	return ProcessMessageHelper(msg);
}

void WinUIMessagePump::AddObserver(UIObserver *observer)
{
	observers_.AddObserver(observer);
}

void WinUIMessagePump::RemoveObserver(UIObserver *observer)
{
	observers_.RemoveObserver(observer);
}

void WinUIMessagePump::PreProcessMessage(const MSG& msg)
{
	AutoLazyEraser lazy_eraser(&observers_);
	size_t index = 0;
	UIObserver* observer;
	while (index < observers_.GetObserverCount())
	{
		observer = observers_.GetObserver(index++);
		if (observer == NULL)
			continue;
		observer->PreProcessMessage(msg);
	}
}

void WinUIMessagePump::PostProcessMessage(const MSG& msg)
{
	AutoLazyEraser lazy_eraser(&observers_);
	size_t index = 0;
	UIObserver* observer;
	while (index < observers_.GetObserverCount())
	{
		observer = observers_.GetObserver(index++);
		if (observer == NULL)
			continue;
		observer->PostProcessMessage(msg);
	}
}

} // namespace nbase

#endif  // OS_WIN

