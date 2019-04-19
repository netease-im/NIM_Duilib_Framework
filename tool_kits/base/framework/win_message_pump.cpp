// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: wrt(guangguang)
// Date: 2011/6/8
//
// The base class of a cross flatform message pump implemention

#include "base/framework/win_message_pump.h"

#if defined(OS_WIN)
#include <assert.h>
#include <math.h>

namespace nbase
{

void WinMessagePump::RunWithDispatcher(Delegate* delegate, Dispatcher* dispatcher)
{
	assert(delegate);

	RunState rs;
	rs.delegate = delegate;
	rs.dispatcher = dispatcher;
	rs.should_quit = false;
	rs.run_depth = state_ ? state_->run_depth + 1 : 1;

	RunState* previous_state = state_;
	state_ = &rs;

	DoRunLoop();

	state_ = previous_state;
}

void WinMessagePump::Quit()
{
	assert(state_);
	if (state_)
		state_->should_quit = true;
}

int64_t WinMessagePump::GetCurrentDelay() const
{
	if (delayed_work_time_.is_null())
		return -1;

	// 将微妙的精度转换为毫秒
	double timeout = ceil((delayed_work_time_ - TimeTicks::Now()).ToInternalValue()/1000.0);

	// 如果delay的值是负的，那么表示任务需要被越快运行越好
	int64_t delay = static_cast<int64_t>(timeout);
	if (delay < 0)
		delay = 0;

	return delay;
}

}

#endif  // OS_WIN

