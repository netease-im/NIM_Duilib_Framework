// A implemetation of a cross flatform waitable event based message loop 

#ifndef BASE_FRAMEWORK_DEFAULT_MESSAGE_PUMP_H_
#define BASE_FRAMEWORK_DEFAULT_MESSAGE_PUMP_H_

#include "base/framework/message_pump.h"
#include "base/time/time.h"
#include "base/synchronization/waitable_event.h"

namespace nbase
{

class DefaultMessagePump : public MessagePump
{
public:

	DefaultMessagePump();
	virtual ~DefaultMessagePump() {}

	virtual void Run(Delegate* delegate);
	virtual void Quit();
	virtual void ScheduleWork();
	virtual void ScheduleDelayedWork(const TimeTicks& delay_message_time);

private:
	void Wait();
	void WaitTimeout(const TimeDelta &timeout);
	void Wakeup();

	WaitableEvent event_;
	bool should_quit_;
	TimeTicks delayed_work_time_;

	DISALLOW_COPY_AND_ASSIGN(DefaultMessagePump);
};

}

#endif // BASE_FRAMEWORK_DEFAULT_MESSAGE_PUMP_H_
