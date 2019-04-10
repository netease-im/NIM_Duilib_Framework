// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/11
//
// Thread Unittest

#if defined(WITH_UNITTEST)

#include "base/thread/thread.h"
#include "gtest/gtest.h"

class TestThread : public nbase::Thread
{
public:
	TestThread() : did_run_(false) {}
	virtual ~TestThread() {}

	bool did_run() { return did_run_; }

protected:
	virtual void Run() { did_run_ = true; }

private:
	bool did_run_;
};

class TestFunctionThread : public TestThread
{
public:
	TestFunctionThread() {}
	~TestFunctionThread() {}

protected:
	virtual void Run()
	{
		nbase::Thread::YieldThread();
		nbase::Thread::Sleep(50);

		TestThread::Run();
	}
};

TEST(TestThread, Basic)
{
	TestThread thread;
	ASSERT_FALSE(thread.did_run());

	thread.Create();
	thread.Close();
	ASSERT_TRUE(thread.did_run());
}

TEST(TestThread, Batch)
{
	TestThread thread[10];

	for (size_t n = 0; n < 10; n++)
	    ASSERT_FALSE(thread[n].did_run());

	for (size_t n = 0; n < 10; n++)
	    thread[n].Create();
	for (size_t n = 0; n < 10; n++)
	    thread[n].Close();
	for (size_t n = 0; n < 10; n++)
	    ASSERT_TRUE(thread[n].did_run());
}

TEST(TestFunctionThread, Function)
{
	nbase::ThreadId main_thread_id = nbase::Thread::CurrentId();

	TestFunctionThread thread[10];

	for (size_t n = 0; n < 10; n++)
		ASSERT_FALSE(thread[n].did_run());

	for (size_t n = 0; n < 10; n++)
		thread[n].Create();
	for (size_t n = 0; n < 10; n++)
		thread[n].Close();
	for (size_t n = 0; n < 10; n++)
	{
		ASSERT_TRUE(thread[n].did_run());
		EXPECT_NE(thread[n].thread_id(), main_thread_id);
	}
}

#endif  // WITH_UNITTEST