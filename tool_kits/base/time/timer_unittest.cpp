// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/10/8
//
// Timer Unittest

#if defined(WITH_UNITTEST)

#include "base/time/timer.h"
#include "base/thread/framework_thread.h"
#include "base/memory/scoped_ptr.h"
#include "gtest/gtest.h"

class OneShotTimerTester
{
public:
	OneShotTimerTester(bool* did_run, unsigned milliseconds = 10)
		: did_run_(did_run),
		  delay_ms_(milliseconds)
	{
	}
	void Start()
	{
		timer_.Start(
			nbase::TimeDelta::FromMilliseconds(delay_ms_), this,
			&OneShotTimerTester::Run);
	}
private:
	OneShotTimerTester& operator=(const OneShotTimerTester &) {};

	void Run()
	{
		*did_run_ = true;
	}
	bool* did_run_;
	nbase::OneShotTimer<OneShotTimerTester> timer_;
	const unsigned delay_ms_;
};

class OneShotSelfDeletingTimerTester
{
public:
	explicit OneShotSelfDeletingTimerTester(bool* did_run)
		: did_run_(did_run),
		  timer_(new nbase::OneShotTimer<OneShotSelfDeletingTimerTester>())
	{
	}
	void Start()
	{
		timer_->Start(
			nbase::TimeDelta::FromMilliseconds(10), this,
			&OneShotSelfDeletingTimerTester::Run);
	}
private:
	void Run()
	{
		*did_run_ = true;
		timer_.reset();
	}
	bool* did_run_;
	nbase::scoped_ptr<nbase::OneShotTimer<OneShotSelfDeletingTimerTester> > timer_;
};

class RepeatingTimerTester
{
public:
	explicit RepeatingTimerTester(bool* did_run)
		: did_run_(did_run), 
		  counter_(10)
	{
	}

	void Start()
	{
		timer_.Start(
			nbase::TimeDelta::FromMilliseconds(10), this,
			&RepeatingTimerTester::Run);
	}
private:
	void Run()
	{
		if (--counter_ == 0)
		{
			*did_run_ = true;
		}
	}
	bool* did_run_;
	int counter_;
	nbase::RepeatingTimer<RepeatingTimerTester> timer_;
};

void RunTest_OneShotTimer(OneShotTimerTester *f)
{
	f->Start();
}

TEST(TimerTest, OneShotTimer)
{
	nbase::FrameworkThread a("OneShotTimerThread");
	a.Start();

	bool did_run = false;
	OneShotTimerTester f(&did_run);

	a.message_loop()->PostNonNestableTask(
		nbase::NewRunnableFunction(&RunTest_OneShotTimer, &f));
	for (int i = 100; i >= 0 && !did_run; --i)
	{
		nbase::Thread::Sleep(10);
	}

	EXPECT_TRUE(did_run);
}

void RunTest_OneShotSelfDeletingTimer(OneShotSelfDeletingTimerTester *f)
{
	f->Start();
}

TEST(TimerTest, OneShotSelfDeletingTimer)
{
	nbase::FrameworkThread a("OneShotSelfDeletingTimerThread");
	a.Start();

	bool did_run = false;
	OneShotSelfDeletingTimerTester f(&did_run);
	a.message_loop()->PostNonNestableTask(
		nbase::NewRunnableFunction(&RunTest_OneShotSelfDeletingTimer, &f));
	for (int i = 100; i >= 0 && !did_run; --i)
	{
		nbase::Thread::Sleep(10);
	}

	EXPECT_TRUE(did_run);
}

void RunTest_RepeatingTimer(RepeatingTimerTester *f)
{
	f->Start();
}

TEST(TimerTest, RepeatingTimer)
{
	nbase::FrameworkThread a("RepeatingTimerThread");
	a.StartWithLoop(nbase::MessageLoop::kIOMessageLoop);

	bool did_run = false;
	RepeatingTimerTester f(&did_run);
	a.message_loop()->PostNonNestableTask(
		nbase::NewRunnableFunction(&RunTest_RepeatingTimer, &f));
	for (int i = 100; i >= 0 && !did_run; --i)
	{
		nbase::Thread::Sleep(10);
	}

	EXPECT_TRUE(did_run);
}

#endif