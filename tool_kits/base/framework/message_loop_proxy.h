// This file trys to implement a cross flatform message loop proxy,
// the mechanism of which is from the Google Chrome project.

#ifndef BASE_MESSAGE_LOOP_PROXY_H_
#define BASE_MESSAGE_LOOP_PROXY_H_
#pragma once

#include "base/base_export.h"
#include "base/framework/message_loop.h"
#include "base/synchronization/lock.h"
#include "base/callback/callback.h"

namespace nbase
{

class MessageLoopProxy;
struct BASE_EXPORT MessageLoopProxyTraits
{
	static void Destruct(const MessageLoopProxy* proxy);
};

// A stock implementation of MessageLoopProxy that is created and managed by a
// MessageLoop. For now a MessageLoopProxy can only be created as part of a
// MessageLoop.
class BASE_EXPORT MessageLoopProxy : public nbase::SupportWeakCallback
{
public:
	static std::shared_ptr<MessageLoopProxy> current();

	// MessageLoopProxy implementation
	virtual bool PostTask(const StdClosure &task);
	virtual bool PostDelayedTask(const StdClosure &task,
								 TimeDelta delay);
	virtual bool PostNonNestableTask(const StdClosure &task);
	virtual bool PostNonNestableDelayedTask(const StdClosure &task,
											TimeDelta delay);

	template<typename T1, typename T2>
	bool PostTaskAndReply(const std::function<T1>& task, const std::function<T2>& reply)
	{
		PostTaskAndReplyRelay<T1, T2>* relay = new PostTaskAndReplyRelay<T1, T2>(task, reply);
		if (!PostTask(nbase::Bind(&PostTaskAndReplyRelay<T1, T2>::Run, relay))) {
			delete relay;
			return false;
		}

		return true;
	}

	virtual bool BelongsToCurrentThread() const;
	virtual ~MessageLoopProxy();

private:
	// Allow the messageLoop to create a MessageLoopProxy.
	friend class MessageLoop;
	friend struct MessageLoopProxyTraits;
	
	MessageLoopProxy();

	// Called directly by MessageLoop::~MessageLoop.
	virtual void WillDestroyCurrentMessageLoop();
	// Called when the reference decreased to 0
	virtual void OnDestruct() const;

	bool PostTaskHelper(const StdClosure& task,
		TimeDelta delay,
		bool nestable);

	void DeleteSelf() const;

	// The lock that protects access to target_message_loop_.
	mutable NLock message_loop_lock_;
	MessageLoop* target_message_loop_;

private:
	// This relay class remembers the MessageLoop that it was created on, and
	// ensures that both the |task| and |reply| Closures are deleted on this same
	// thread. Also, |task| is guaranteed to be deleted before |reply| is run or
	// deleted.
	//
	// If this is not possible because the originating MessageLoop is no longer
	// available, the the |task| and |reply| Closures are leaked.  Leaking is
	// considered preferable to having a thread-safetey violations caused by
	// invoking the Closure destructor on the wrong thread.
	class MessageLoopProxyNull
	{

	};

	template<typename T1, typename T2>
	class PostTaskAndReplyRelay : public nbase::SupportWeakCallback
	{
	public:
		PostTaskAndReplyRelay(const std::function<T1>& task, const std::function<T2>& reply)
			: origin_loop_(MessageLoopProxy::current()) 
		{
			std_task_ = task;
			std_reply_ = reply;
		}

		void Run()
		{
			auto ret = std_task_();
			origin_loop_->PostTask(
				nbase::Bind(&PostTaskAndReplyRelay::RunReplyAndSelfDestructWithParam<decltype(ret)>, this, ret));
		}

		~PostTaskAndReplyRelay() 
		{
			assert(origin_loop_->BelongsToCurrentThread());
			std_task_ = nullptr;
			std_reply_ = nullptr;
		}

	private:
		void RunReplyAndSelfDestruct() {
			assert(origin_loop_->BelongsToCurrentThread());

			// Force |task_| to be released before |reply_| is to ensure that no one
			// accidentally depends on |task_| keeping one of its arguments alive while
			// |reply_| is executing.
			std_task_ = nullptr;

			std_reply_();

			// Cue mission impossible theme.
			delete this;
		}

		template<typename InernalT>
		void RunReplyAndSelfDestructWithParam(InernalT ret) {
			assert(origin_loop_->BelongsToCurrentThread());

			// Force |task_| to be released before |reply_| is to ensure that no one
			// accidentally depends on |task_| keeping one of its arguments alive while
			// |reply_| is executing.
			std_task_ = nullptr;

			std_reply_(ret);

			// Cue mission impossible theme.
			delete this;
		}

		std::shared_ptr<MessageLoopProxy> origin_loop_;

		std::function<T2> std_reply_;
		std::function<T1> std_task_;
	};
};

template<>
void MessageLoopProxy::PostTaskAndReplyRelay<void(), void()>::Run()
{
	std_task_();
	origin_loop_->PostTask(
		nbase::Bind(&PostTaskAndReplyRelay::RunReplyAndSelfDestruct, this));
}

}  // namespace nbase

#endif  // BASE_MESSAGE_LOOP_PROXY_H_
