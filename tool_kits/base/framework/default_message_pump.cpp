// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2011/6/24
//
// A implemetation of a cross flatform waitable event based message loop

#include "base/framework/default_message_pump.h"
#include <cassert>

namespace nbase
{

DefaultMessagePump::DefaultMessagePump() :
	event_(false, false),
	should_quit_(0)
{

}

void DefaultMessagePump::Run(Delegate* delegate)
{
	// Quit must have been called outside of Run!
	assert(should_quit_ == false);

	for (;;)
	{
		bool did_work = delegate->DoWork();
		if (should_quit_)
			break;

		did_work |= delegate->DoDelayedWork(&delayed_work_time_);
		if (should_quit_)
			break;

		if (did_work)
			continue;

		did_work = delegate->DoIdleWork();
		if (should_quit_)
			break;

		if (did_work)
			continue;

		if (delayed_work_time_.is_null())
		{
			Wait();
		}
		else
		{
			TimeDelta delay = delayed_work_time_ - TimeTicks::Now();
			if (delay > TimeDelta())
				WaitTimeout(delay);
			else
			{
				// It looks like delayed_work_time_ indicates a time in the past, so we
				// need to call DoDelayedWork now.
				delayed_work_time_ = TimeTicks();
			}
		}
	}

	should_quit_ = false;
}

void DefaultMessagePump::Quit()
{
	should_quit_ = true;
}

void DefaultMessagePump::ScheduleWork()
{
	// Since this can be called on any thread, we need to ensure that our Run
	// loop wakes up.
	Wakeup();
}

void DefaultMessagePump::ScheduleDelayedWork(const TimeTicks& delayed_work_time)
{
	// We know that we can't be blocked on Wait right now since this method can
	// only be called on the same thread as Run, so we only need to update our
	// record of how long to sleep when we do sleep.
	delayed_work_time_ = delayed_work_time;
}

void DefaultMessagePump::Wait()
{
	event_.Wait();
}

void DefaultMessagePump::WaitTimeout(const TimeDelta &timeout)
{
	event_.WaitTimeout(timeout);
}

void DefaultMessagePump::Wakeup()
{
	event_.Signal();
}

}
