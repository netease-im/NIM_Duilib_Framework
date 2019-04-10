// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/29
//
// FrameworkThread Unittest

#if defined(WITH_UNITTEST)

#include "framework_thread.h"
#include "base/framework/message_loop.h"
#include "base/memory/scoped_ptr.h"
#include "gtest/gtest.h"
#include <vector>

namespace
{
class ToggleValue : public nbase::Task
{
public:
	explicit ToggleValue(bool *value) : value_(value) {}
	virtual void Run() 
	{
		*value_ = !*value_;
	}
private:
	bool *value_;
};

void ToggleValueFunction(bool *value)
{
	bool value_temp = !*value;
	*value = value_temp;
}

class ToggleValueMethod : public nbase::ThreadSafeRefCount
{
public:
	void ToggleValue(bool *value)
	{
		bool value_temp = !*value;
		*value = value_temp;
	}
};

class SleepSome : public nbase::Task 
{
public:
	explicit SleepSome(int msec) : msec_(msec) {}
	virtual void Run()
	{
		nbase::Thread::Sleep(msec_);
	}
private:
	int msec_;
};

class SleepInsideInitThread : public nbase::FrameworkThread
{
public:
	SleepInsideInitThread() : FrameworkThread("none") { init_called_ = false; }
	virtual ~SleepInsideInitThread() { }

	virtual void Init()
	{
		nbase::Thread::Sleep(500);
		init_called_ = true;
	}
	bool InitCalled() { return init_called_; }
private:
	bool init_called_;
};

enum ThreadEvent
{
	// Thread::Init() was called.
	THREAD_EVENT_INIT = 0,

	// The MessageLoop for the thread was deleted.
	THREAD_EVENT_MESSAGE_LOOP_DESTROYED,

	// Thread::CleanUp() was called.
	THREAD_EVENT_CLEANUP,

	// Keep at end of list.
	THREAD_NUM_EVENTS
};

typedef std::vector<ThreadEvent> EventList;

class CaptureToEventList : public nbase::FrameworkThread
{
public:
	// This Thread pushes events into the vector |event_list| to show
	// the order they occured in. |event_list| must remain valid for the
	// lifetime of this thread.
	explicit CaptureToEventList(EventList *event_list)
		: FrameworkThread("none"), event_list_(event_list)
	{}

	virtual ~CaptureToEventList()
	{
		// Must call Stop() manually to have our CleanUp() function called.
		Stop();
	}

	virtual void Init()
	{
		event_list_->push_back(THREAD_EVENT_INIT);
	}

	virtual void Cleanup()
	{
		event_list_->push_back(THREAD_EVENT_CLEANUP);
	}

private:
	EventList *event_list_;
};

// Observer that writes a value into |event_list| when a message loop has been
// destroyed.
class CapturingDestructionObserver : public nbase::MessageLoop::DestructionObserver
{
public:
	// |event_list| must remain valid throughout the observer's lifetime.
	explicit CapturingDestructionObserver(EventList *event_list)
		: event_list_(event_list)
	{}

	// DestructionObserver implementation:
	virtual void PreDestroyCurrentMessageLoop()
	{
		event_list_->push_back(THREAD_EVENT_MESSAGE_LOOP_DESTROYED);
		event_list_ = NULL;
	}

private:
	EventList *event_list_;
};

// Task that adds a destruction observer to the current message loop.
class RegisterDestructionObserver : public nbase::Task
{
public:
	explicit RegisterDestructionObserver(
		nbase::MessageLoop::DestructionObserver* observer)
		: observer_(observer)
	{}

	virtual void Run() {
		nbase::MessageLoop::current()->AddDestructionObserver(observer_);
		observer_ = NULL;
	}

private:
	nbase::MessageLoop::DestructionObserver *observer_;
};

}  // namespace


TEST(FrameworkThread, Restart)
{
	nbase::FrameworkThread a("Restart");
	a.Stop();
	EXPECT_FALSE(a.message_loop());
	EXPECT_FALSE(a.IsRunning());
	EXPECT_TRUE(a.Start());
	EXPECT_TRUE(a.message_loop() != NULL);
	EXPECT_TRUE(a.IsRunning());
	a.Stop();
	EXPECT_FALSE(a.message_loop());
	EXPECT_FALSE(a.IsRunning());
	EXPECT_TRUE(a.Start());
	EXPECT_TRUE(a.message_loop() != NULL);
	EXPECT_TRUE(a.IsRunning());
	a.Stop();
	EXPECT_FALSE(a.message_loop() != NULL);
	EXPECT_FALSE(a.IsRunning());
	a.Stop();
	EXPECT_FALSE(a.message_loop() != NULL);
	EXPECT_FALSE(a.IsRunning());
}


TEST(FrameworkThread, HasTask)
{
	nbase::FrameworkThread a("HasTask");

	EXPECT_TRUE(a.Start());
	EXPECT_TRUE(a.message_loop() != NULL);
	EXPECT_TRUE(a.IsRunning());

	bool was_invoked = false;
	a.message_loop()->PostTask(new ToggleValue(&was_invoked));

	// wait for the task to run (we could use a kernel event here
	// instead to avoid busy waiting, but this is sufficient for
	// testing purposes).
	for (int i = 100; i >= 0 && !was_invoked; --i)
	{
		nbase::Thread::Sleep(10);
	}
	EXPECT_TRUE(was_invoked);

	a.message_loop()->PostTask(nbase::NewRunnableFunction(&ToggleValueFunction, &was_invoked));
	for (int i = 100; i >= 0 && was_invoked; --i)
	{
		nbase::Thread::Sleep(10);
	}
	EXPECT_FALSE(was_invoked);

	ToggleValueMethod *method = new ToggleValueMethod;
	a.message_loop()->PostTask(nbase::NewRunnableMethod(method, &ToggleValueMethod::ToggleValue, &was_invoked));
	for (int i = 100; i >= 0 && !was_invoked; --i)
	{
		nbase::Thread::Sleep(10);
	}
	EXPECT_TRUE(was_invoked);
}

TEST(FrameworkThread, TwoTasks)
{
	bool was_invoked = false;
	{
		nbase::FrameworkThread a("TwoTasks");
		EXPECT_TRUE(a.Start());
		EXPECT_TRUE(a.message_loop() != NULL);

		// Test that all events are dispatched before the Thread object is
		// destroyed.  We do this by dispatching a sleep event before the
		// event that will toggle our sentinel value.
		a.message_loop()->PostTask(new SleepSome(20));
		a.message_loop()->PostTask(new ToggleValue(&was_invoked));
	}
	EXPECT_TRUE(was_invoked);
}

TEST(FrameworkThread, StopSoon)
{
	nbase::FrameworkThread a("StopSoon");
	EXPECT_TRUE(a.Start());
	EXPECT_TRUE(a.message_loop() != NULL);
	EXPECT_TRUE(a.IsRunning());
	a.StopSoon();
	a.StopSoon();
	a.Stop();
	EXPECT_FALSE(a.message_loop() != NULL);
	EXPECT_FALSE(a.IsRunning());
}

TEST(FrameworkThread, ThreadName)
{
	nbase::FrameworkThread a("ThreadName");
	EXPECT_TRUE(a.Start());
	EXPECT_EQ("ThreadName", a.thread_name());
}

// Make sure we can't use a thread between Start() and Init().
TEST(FrameworkThread, SleepInsideInit)
{
	SleepInsideInitThread t;
	EXPECT_FALSE(t.InitCalled());
	t.Start();
	EXPECT_TRUE(t.InitCalled());
}

// Make sure that the destruction sequence is:
//
//  (1) Thread::CleanUp()
//  (2) MessageLoop::~MessageLoop()
//      MessageLoop::DestructionObservers called.
TEST(FrameworkThread, CleanUp)
{
	EventList captured_events;
	CapturingDestructionObserver loop_destruction_observer(&captured_events);

	{
		// Start a thread which writes its event into |captured_events|.
		CaptureToEventList t(&captured_events);
		EXPECT_TRUE(t.Start());
		EXPECT_TRUE(t.message_loop() != NULL);
		EXPECT_TRUE(t.IsRunning());

		// Register an observer that writes into |captured_events| once the
		// thread's message loop is destroyed.
		t.message_loop()->PostTask(
			new RegisterDestructionObserver(&loop_destruction_observer));

		// Upon leaving this scope, the thread is deleted.
	}

	// Check the order of events during shutdown.
	ASSERT_EQ(static_cast<size_t>(THREAD_NUM_EVENTS), captured_events.size());
	EXPECT_EQ(THREAD_EVENT_INIT, captured_events[0]);
	EXPECT_EQ(THREAD_EVENT_CLEANUP, captured_events[1]);
	EXPECT_EQ(THREAD_EVENT_MESSAGE_LOOP_DESTROYED, captured_events[2]);
}

#endif  // WITH_UNITTEST