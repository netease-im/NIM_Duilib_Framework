// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/26
//
// Lock Unittest

#if defined(WITH_UNITTEST)

#include "base/synchronization/lock.h"
#include "base/thread/thread.h"
#include "gtest/gtest.h"

class TryLockThread : public nbase::Thread
{
public:
	TryLockThread(nbase::NLock *lock) : lock_(lock), got_lock_(false) {}

	bool got_lock() { return got_lock_; }

protected:
	virtual void Run()
	{
		got_lock_ = lock_->Try();
		if (got_lock_)
			lock_->Unlock();
	}


private:
	nbase::NLock *lock_;
	bool          got_lock_;
};

TEST(TryLockThread, Basic)
{
	nbase::NLock lock;
	ASSERT_TRUE(lock.Try());

	{
		TryLockThread thread(&lock);
		thread.Create();
		thread.Close();
		ASSERT_FALSE(thread.got_lock());
	}

	lock.Unlock();

	{
		TryLockThread thread(&lock);
		thread.Create();
		thread.Close();
		ASSERT_TRUE(thread.got_lock());
		ASSERT_TRUE(lock.Try());
	}

	lock.Unlock();
}

class TestLockThread : public nbase::Thread
{
public:
	TestLockThread(nbase::NLock *lock, int *value) : lock_(lock), value_(value) {}

	static void DoStuff(nbase::NLock *lock, int *value)
	{
		for (int i = 0; i < 40; i++)
		{
			lock->Lock();
			int v = *value;
			nbase::Thread::Sleep(5);
			*value = v + 1;
			lock->Unlock();
		}
	}

protected:
	virtual void Run()
	{
		DoStuff(lock_, value_);
	}
private:
	nbase::NLock *lock_;
	int          *value_;
};

TEST(TestLockThread, FourThreads)
{
	nbase::NLock lock;
	int value = 0;

	TestLockThread thread1(&lock, &value);
	TestLockThread thread2(&lock, &value);
	TestLockThread thread3(&lock, &value);

	thread1.Create();
	thread2.Create();
	thread3.Create();

	TestLockThread::DoStuff(&lock, &value);

	thread1.Close();
	thread2.Close();
	thread3.Close();

	EXPECT_EQ(4 * 40, value);
}

#endif  // WITH_UNITTEST