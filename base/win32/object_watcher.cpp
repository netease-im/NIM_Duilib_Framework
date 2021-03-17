// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Ported & Modified by wrt(guangguang)
// 2013/9/3

#include "base/win32/object_watcher.h"

#include <assert.h>

namespace nbase {
namespace win32 {

//-----------------------------------------------------------------------------

ObjectWatcher::ObjectWatcher()
    : object_(NULL),
      wait_object_(NULL),
      origin_loop_(NULL) {
}

ObjectWatcher::~ObjectWatcher() {
  StopWatching();
}

bool ObjectWatcher::StartWatching(HANDLE object, Delegate* delegate) {
  if (wait_object_) {
    return false;
  }

  // Since our job is to just notice when an object is signaled and report the
  // result back to this thread, we can just run on a Windows wait thread.
  DWORD wait_flags = WT_EXECUTEINWAITTHREAD | WT_EXECUTEONLYONCE;

  // DoneWaiting can be synchronously called from RegisterWaitForSingleObject,
  // so set up all state now.
  callback_ = nbase::Bind(&ObjectWatcher::Signal, this, delegate);
  callback_ = signal_weakflag_.ToWeakCallback(callback_);
  object_ = object;
  origin_loop_ = MessageLoop::current();

  if (!RegisterWaitForSingleObject(&wait_object_, object, DoneWaiting,
                                   this, INFINITE, wait_flags)) {
    object_ = NULL;
    wait_object_ = NULL;
    return false;
  }

  // We need to know if the current message loop is going away so we can
  // prevent the wait thread from trying to access a dead message loop.
  MessageLoop::current()->AddDestructionObserver(this);
  return true;
}

bool ObjectWatcher::StopWatching() {
  if (!wait_object_)
    return false;

  // Make sure ObjectWatcher is used in a single-threaded fashion.
  assert(origin_loop_ == MessageLoop::current());

  // Blocking call to cancel the wait. Any callbacks already in progress will
  // finish before we return from this call.
  if (!UnregisterWaitEx(wait_object_, INVALID_HANDLE_VALUE)) {
    return false;
  }

  signal_weakflag_.Cancel();
  object_ = NULL;
  wait_object_ = NULL;

  MessageLoop::current()->RemoveDestructionObserver(this);
  return true;
}

HANDLE ObjectWatcher::GetWatchedObject() {
  return object_;
}

// static
void CALLBACK ObjectWatcher::DoneWaiting(void* param, BOOLEAN timed_out) {
  assert(!timed_out);

  // The destructor blocks on any callbacks that are in flight, so we know that
  // that is always a pointer to a valid ObjectWater.
  ObjectWatcher* that = static_cast<ObjectWatcher*>(param);
  that->origin_loop_->PostTask(that->callback_);
  that->callback_ = nullptr;
}

void ObjectWatcher::Signal(Delegate* delegate) {
  // Signaling the delegate may result in our destruction or a nested call to
  // StartWatching(). As a result, we save any state we need and clear previous
  // watcher state before signaling the delegate.
  HANDLE object = object_;
  StopWatching();
  delegate->OnObjectSignaled(object);
}

void ObjectWatcher::PreDestroyCurrentMessageLoop() {
  // Need to shutdown the watch so that we don't try to access the MessageLoop
  // after this point.
  StopWatching();
}

}  // namespace win32
}  // namespace nbase
