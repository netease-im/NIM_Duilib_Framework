// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/10
//
// Semaphore Unittest

#if defined(WITH_UNITTEST)

#include "base/synchronization/semaphore.h"
#include "base/thread/thread.h"
#include "gtest/gtest.h"

TEST(Semaphore, Basic)
{
	nbase::Semaphore sem;
	EXPECT_TRUE(sem.CreateSemaphore("sem_test", 1, 20));

	EXPECT_TRUE(sem.WaitSemaphore());
	EXPECT_FALSE(sem.WaitSemaphore(10));

	sem.PostSemaphore(1);
	EXPECT_TRUE(sem.WaitSemaphore(10));

	sem.PostSemaphore(2);
	EXPECT_TRUE(sem.WaitSemaphore(10));
	EXPECT_TRUE(sem.WaitSemaphore(10));
	EXPECT_FALSE(sem.WaitSemaphore(10));
}

class ThreadEventTest : public nbase::Thread
{
public:
	ThreadEventTest() : event_count_(0), continue_(true)
	{
		sem_.CreateSemaphore("ThreadEventTest", 0, 256);
	}

	void Invoke() {	sem_.PostSemaphore(1); }

	void Cancel() {	continue_ = false; }

	int event_count() { return event_count_; }

protected:
	virtual void Run()
	{
		while(continue_ &&
			  sem_.WaitSemaphore())
		{
			if (continue_ == false)
				break;
			event_count_++;
		}
	}

private:
	int  event_count_;
	bool continue_;
	nbase::Semaphore sem_;
};

TEST(Semaphore, ThreadEvent)
{
	ThreadEventTest thread;
	
	thread.Create();
	EXPECT_EQ(0, thread.event_count());

	thread.Invoke();
	nbase::Thread::Sleep(5);
	EXPECT_EQ(1, thread.event_count());
	thread.Invoke();
	thread.Invoke();
	nbase::Thread::Sleep(5);
	EXPECT_EQ(3, thread.event_count());
	thread.Cancel();
	thread.Invoke();
	EXPECT_EQ(3, thread.event_count());

	thread.Close();
}

#endif  // WITH_UNITTEST