// The base class of a cross flatform message pump implemention

#ifndef BASE_FRAMEWORK_MESSAGE_PUMP_H_
#define BASE_FRAMEWORK_MESSAGE_PUMP_H_

#include "base/base_export.h"
#include "base/base_config.h"

namespace nbase
{

class TimeTicks;

// MessagePump用于驱动各种类型的消息循环，比如UI消息、IO消息等，因此可以派生为UIMessagePump、IOMessagePump等等。
//
// MessagePump有三种状态，即任务状态、空闲状态、睡眠状态。MessagePump提取并处理了消息、Delegate处理了即时任务或定时任务,
// 那么MessagePump便处于任务状态。当MessagePump不处于任务状态并且Delegte处理了闲时任务，那么MessagePump处于空闲状态。当
// MessagePump没有任何消息和任务需要处理的时候，便会进入睡眠状态以节省CPU消耗。
//
// 处于任务状态的MessagePump在每一轮处理完毕后会立即尝试开始新一轮处理，如果新一轮没有消息和即时、定时任务需要处理，那么
// 会尝试开始处理闲时任务，如果有闲时任务需要处理，那么便进入空闲状态，如果没有，那么便会进入睡眠状态。进入空闲状态后，
// 如果新的消息到达或者有新的即时、定时任务被处理，那么立即切换成任务状态；如果空闲状态下没有新的空闲任务需要处理，那么
// 便进入睡眠状态。处于睡眠状态的MessagePump可以被ScheduleWork、ScheduleDelayedWork等唤醒进入任务状态。

class BASE_EXPORT MessagePump
{
public:

	// Delegate是一个回调类，让MessagePump给机会利用它处理其他任务。
	//   DoWork用来处理即时任务，这类任务需要尽可能快地被开始处理
	//   DoDelayedWork用来处理定时任务，这类任务在特定时刻被处理，next_delayed_message_time为下个任务执行的时刻
	//   DoIdleWork用来处理闲时任务，这类任务在MessagePump处于空闲状态时被处理
	// 三者返回值意义类似，返回true表示处理了该类任务，返回false表示没有该类任务需要处理
	class BASE_EXPORT Delegate
	{
	public:
		virtual ~Delegate() {}
		virtual bool DoWork() = 0;
		virtual bool DoDelayedWork(TimeTicks *next_delayed_message_time) = 0;
		virtual bool DoIdleWork() = 0;
	};

	MessagePump();
	virtual ~MessagePump();

	// 启动消息循环
	virtual void Run(Delegate* delegate) = 0;

	// 通知退出消息循环
	virtual void Quit() = 0;

	// 通知MessagePump处理即时任务
	// 可以跨线程调用这个接口
	virtual void ScheduleWork() = 0;

	// 通知MessagePump处理定时任务
	// 此接口不允许跨线程调用
	virtual void ScheduleDelayedWork(const TimeTicks& delay_message_time) = 0;
};

} // namespace nbase

#endif // BASE_FRAMEWORK_MESSAGE_PUMP_H_
