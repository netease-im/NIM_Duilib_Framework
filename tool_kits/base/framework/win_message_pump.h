// The base class of a cross flatform message pump implemention

#ifndef BASE_FRAMEWORK_WIN_MESSAGE_PUMP_H_
#define BASE_FRAMEWORK_WIN_MESSAGE_PUMP_H_

#include "base/framework/message_pump.h"

#if defined(OS_WIN)
#include <windows.h>
#include "base/time/time.h"

namespace nbase
{

class BASE_EXPORT WinMessagePump: public MessagePump
{
public:

	//	UI消息派发器
	//	UI消息泵如果使用消息派发器，那么将不再使用经典的
	//	TranslateMessage/DispatchMessage模式而改由Dispatcher来完成
	class BASE_EXPORT Dispatcher
	{
	public:

		virtual ~Dispatcher() {}
		virtual bool Dispatch(const MSG &message) = 0;
	};

	WinMessagePump() : have_work_(0), state_(NULL) {}
	virtual ~WinMessagePump() {}

	void RunWithDispatcher(Delegate* delegate, Dispatcher* dispatcher);

	virtual void Run(Delegate* delegate) { return RunWithDispatcher(delegate, NULL); }
	virtual void Quit();

protected:
	struct RunState
	{
		int run_depth;				// 嵌套调用深度
		bool should_quit;			// 是否应该立即退出
		Delegate* delegate;			// 处理任务的委托
		Dispatcher* dispatcher;		// 消息派发器
	};

	// 取当前定时间隔
	int64_t GetCurrentDelay() const;
	virtual void DoRunLoop() = 0;

	// 定时任务下次运行的时间
	TimeTicks delayed_work_time_;

	// 指示消息队列中是否有kMsgDoWork消息
	long have_work_;

	// 指示当前MessagePump的状态
	RunState* state_;
};

}

#endif  // OS_WIN

#endif // BASE_FRAMEWORK_WIN_MESSAGE_PUMP_H_
