// a implemention of a Windows specific message pump for user interface,
// the mechanism of which is from the Google Chrome project

#ifndef BASE_FRAMEWORK_WIN_UI_MESSAGE_PUMP_H_
#define BASE_FRAMEWORK_WIN_UI_MESSAGE_PUMP_H_

#include "base/framework/win_message_pump.h"

#if defined(OS_WIN)
#include <list>
#include "base/framework/observer_list.h"

namespace nbase
{

class BASE_EXPORT WinUIMessagePump: public WinMessagePump
{
public:
	static const int kMessageFilterCode = 0x5001;

	class BASE_EXPORT UIObserver
	{
	public:
		virtual void PreProcessMessage(const MSG &msg) = 0;
		virtual void PostProcessMessage(const MSG &msg) = 0;
	};

	void AddObserver(UIObserver *observer);
	void RemoveObserver(UIObserver *observer);

	WinUIMessagePump();
	virtual ~WinUIMessagePump();

	virtual void ScheduleWork();
	virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time);

	// 通知MessagePump提取并处理消息队列中的WM_PAINT消息
	void PumpOutPendingPaintMessages();

private:
	static LRESULT CALLBACK WndProcThunk(
	  HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	virtual void DoRunLoop();
	virtual void PreProcessMessage(const MSG& msg);
	virtual void PostProcessMessage(const MSG& msg);
	void InitMessageWnd();
	void WaitForWork();
	void HandleWorkMessage();
	void HandleTimerMessage();
	bool ProcessNextWindowsMessage();
	bool ProcessMessageHelper(const MSG& msg);
	bool ProcessPumpReplacementMessage();

	// 用来接收自定义消息的隐藏窗口
	HWND message_hwnd_;
	// UIObserver列表
	ObserverList<UIObserver> observers_;
};

} // namespace nbase

#endif // OS_WIN

#endif // BASE_FRAMEWORK_WIN_UI_MESSAGE_PUMP_H_
