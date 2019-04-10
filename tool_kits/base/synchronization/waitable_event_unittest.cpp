// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/30
//
// Waitable event unittest.

#if defined(WITH_UNITTEST)

#include "waitable_event.h"
#include "base/thread/thread.h"
#include "base/time/time.h"
#include "gtest/gtest.h"

namespace nbase
{

TEST(WaitableEvent, ManualBasics)
{
	WaitableEvent event(true, false);

	EXPECT_FALSE(event.IsSignaled());

	event.Signal();
	EXPECT_TRUE(event.IsSignaled());
	EXPECT_TRUE(event.IsSignaled());

	event.Reset();
	EXPECT_FALSE(event.IsSignaled());
	EXPECT_FALSE(event.WaitTimeout(TimeDelta::FromMilliseconds(10)));

	event.Signal();
	EXPECT_TRUE(event.Wait());
	EXPECT_TRUE(event.WaitTimeout(TimeDelta::FromMilliseconds(10)));
}

TEST(WaitableEvent, AutoBasics) {
	WaitableEvent event(false, false);

	EXPECT_FALSE(event.IsSignaled());

	event.Signal();
	EXPECT_TRUE(event.IsSignaled());
	EXPECT_FALSE(event.IsSignaled());

	event.Reset();
	EXPECT_FALSE(event.IsSignaled());
	EXPECT_FALSE(event.WaitTimeout(TimeDelta::FromMilliseconds(10)));

	event.Signal();
	EXPECT_TRUE(event.Wait());
	EXPECT_FALSE(event.WaitTimeout(TimeDelta::FromMilliseconds(10)));

	event.Signal();
	EXPECT_TRUE(event.WaitTimeout(TimeDelta::FromMilliseconds(10)));
}

TEST(WaitableEvent, WaitManyShortcut)
{
	WaitableEvent *ev[5];
	for (unsigned i = 0; i < 5; ++i)
		ev[i] = new WaitableEvent(false, false);

	ev[3]->Signal();
	EXPECT_EQ(WaitableEvent::WaitMultiple(ev, 5), 3u);

	ev[3]->Signal();
	EXPECT_EQ(WaitableEvent::WaitMultiple(ev, 5), 3u);

	ev[4]->Signal();
	EXPECT_EQ(WaitableEvent::WaitMultiple(ev, 5), 4u);

	ev[0]->Signal();
	EXPECT_EQ(WaitableEvent::WaitMultiple(ev, 5), 0u);

	for (unsigned i = 0; i < 5; ++i)
		delete ev[i];
}

class WaitableEventSignaler : public nbase::Thread
{
public:
	WaitableEventSignaler(double seconds, WaitableEvent* ev)
		: seconds_(seconds),
		  ev_(ev) {
	}

protected:
	virtual void Run()
	{
		Thread::Sleep(static_cast<int>(seconds_ * 1000));
		ev_->Signal();
	}

private:
	WaitableEventSignaler& operator=(const WaitableEventSignaler &) {};
	const double seconds_;
	WaitableEvent *const ev_;
};

TEST(WaitableEvent, WaitMultiple)
{
	WaitableEvent* ev[5];
	for (unsigned i = 0; i < 5; ++i)
		ev[i] = new WaitableEvent(false, false);

	WaitableEventSignaler signaler(0.1, ev[2]);
	signaler.Create();

	EXPECT_EQ(WaitableEvent::WaitMultiple(ev, 5), 2u);

    signaler.Close();

	for (unsigned i = 0; i < 5; ++i)
		delete ev[i];
}

}  // namespace nbase

#endif  // WITH_UNITTEST