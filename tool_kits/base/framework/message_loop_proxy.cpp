// Copyright (c) 2013, NetEase Inc. All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2013/08/26
//
// This file trys to implement a cross flatform message loop proxy,
// the mechanism of which is from the Google Chrome project.

#include "base/framework/message_loop_proxy.h"

#include "base/synchronization/lock.h"

namespace nbase
{

MessageLoopProxy::~MessageLoopProxy()
{

}

bool MessageLoopProxy::PostTask(const StdClosure &task)
{
	return PostTaskHelper(task, TimeDelta(), true);
}

bool MessageLoopProxy::PostDelayedTask(
	const StdClosure& task, TimeDelta delay)
{
	return PostTaskHelper(task, delay, true);
}

bool MessageLoopProxy::PostNonNestableTask(const StdClosure &task)
{
	return PostTaskHelper(task, TimeDelta(), false);
}

bool MessageLoopProxy::PostNonNestableDelayedTask(
	const StdClosure& task, TimeDelta delay)
{
	return PostTaskHelper(task, delay, false);
}

bool MessageLoopProxy::BelongsToCurrentThread() const
{
	NAutoLock lock(&message_loop_lock_);
	return (target_message_loop_ &&
		(MessageLoop::current() == target_message_loop_));
}

// MessageLoop::DestructionObserver implementation
void MessageLoopProxy::WillDestroyCurrentMessageLoop()
{
	NAutoLock lock(&message_loop_lock_);
	target_message_loop_ = nullptr;
}

void MessageLoopProxy::OnDestruct() const
{
	bool delete_later = false;
	{
		NAutoLock lock(&message_loop_lock_);
		if (target_message_loop_ &&
			(MessageLoop::current() != target_message_loop_))
		{
			target_message_loop_->PostNonNestableTask(
				nbase::Bind(&MessageLoopProxy::DeleteSelf, this));
			delete_later = true;
		}
	}
	if (!delete_later)
		delete this;
}

void MessageLoopProxy::DeleteSelf() const
{
	delete this;
}

MessageLoopProxy::MessageLoopProxy()
    : target_message_loop_(MessageLoop::current())
{

}

bool MessageLoopProxy::PostTaskHelper(
	const StdClosure& task, TimeDelta delay, bool nestable)
{
	NAutoLock lock(&message_loop_lock_);
	if (target_message_loop_) {
		if (nestable) {
			if (delay == TimeDelta())
				target_message_loop_->PostTask(task);
			else
				target_message_loop_->PostDelayedTask(task, delay);
		} else {
			if (delay == TimeDelta())
				target_message_loop_->PostNonNestableTask(task);
			else
				target_message_loop_->PostNonNestableDelayedTask(task, delay);
		}
		return true;
	}
	return false;
}

std::shared_ptr<MessageLoopProxy> MessageLoopProxy::current()
{
	MessageLoop* cur_loop = MessageLoop::current();
	if (!cur_loop)
		return nullptr;
	return cur_loop->message_loop_proxy();
}

void MessageLoopProxyTraits::Destruct(const MessageLoopProxy* proxy)
{
	proxy->OnDestruct();
}

}  // namespace nbase
