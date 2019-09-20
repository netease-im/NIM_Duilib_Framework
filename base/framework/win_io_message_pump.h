// a implemention of a Windows specific message pump for I/O,
// the mechanism of which is from the Google Chrome project

#ifndef BASE_FRAMEWORK_WIN_IO_MESSAGE_PUMP_H_
#define BASE_FRAMEWORK_WIN_IO_MESSAGE_PUMP_H_

#include "base/framework/win_message_pump.h"

#if defined(OS_WIN)
#include <list>
#include "base/framework/observer_list.h"
#include "base/win32/scoped_win_handle.h"

namespace nbase
{

class BASE_EXPORT WinIOMessagePump: public WinMessagePump
{
public:
	struct IOContext;

	class IOHandler
	{
	public:
		virtual ~IOHandler() {}
		virtual void OnIOCompleted(IOContext *context, DWORD bytes_transfered, DWORD error) = 0;
	};

	class IOObserver
	{
	public:
		IOObserver();
		virtual void PreProcessIOEvent() = 0;
		virtual void PostProcessIOEvent() = 0;

	protected:
		virtual ~IOObserver();
	};

	struct IOContext
	{
		OVERLAPPED overlapped;
		IOHandler *handler;
	};

	WinIOMessagePump();
	virtual ~WinIOMessagePump() {}

	virtual void ScheduleWork();
	virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time);

	void RegisterIOHandler(HANDLE file_handle, IOHandler *handler);

	bool WaitForIOCompletion(DWORD timeout, IOHandler *handler);

	void AddObserver(IOObserver *observer);
	void RemoveObserver(IOObserver *observer);

private:
	struct IOItem
	{
		IOHandler* handler;
		IOContext* context;
		DWORD bytes_transfered;
		DWORD error;
	};

	virtual void DoRunLoop();
	void WaitForWork();
	bool MatchCompletedIOItem(IOHandler* filter, IOItem* item);
	bool GetIOItem(DWORD timeout, IOItem* item);
	bool ProcessInternalIOItem(const IOItem& item);
	void PreProcessIOEvent();
	void PostProcessIOEvent();

	// IOCP scoped handle
	win32::ScopedWinHandle port_;
	// IO completions that have not been delivered yet
	std::list<IOItem> completed_io_;
	// IO observers
	ObserverList<IOObserver> observers_;
};

} // namespace nbase

#endif  // OS_WIN

#endif // BASE_FRAMEWORK_WIN_IO_MESSAGE_PUMP_H_
