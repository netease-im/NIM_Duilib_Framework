// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/9
//
// TimeDelta/Time/TimeTicks implementation on windows platform

#include "base/base_config.h"

#if defined(OS_WIN)

#include "base/base_export.h"
#include "base/hardware/cpu.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"

#if defined(COMPILER_MSVC)
#pragma comment(lib, "winmm.lib")
#endif
#include <stdlib.h>		/* for abs(int64_t) in GCC */
#include <mmsystem.h>

namespace nbase
{

namespace
{

// From MSDN, FILETIME "Contains a 64-bit value representing the number of
// 100-nanosecond intervals since January 1, 1601 (UTC)."
int64_t FileTimeToMicroseconds(const FILETIME& ft)
{
	// Need to bit_cast to fix alignment, then divide by 10 to convert
	// 100-nanoseconds to milliseconds. This only works on little-endian
	// machines.
	return bit_cast<int64_t, FILETIME>(ft) / 10;
}

void MicrosecondsToFileTime(int64_t us, FILETIME* ft)
{
	if (us < 0)
		return;

	// Multiply by 10 to convert milliseconds to 100-nanoseconds. Bit_cast will
	// handle alignment problems. This only works on little-endian machines.
	*ft = bit_cast<FILETIME, int64_t>(us * 10);
}

int64_t CurrentWallclockMicroseconds()
{
	FILETIME ft;
	::GetSystemTimeAsFileTime(&ft);
	return FileTimeToMicroseconds(ft);
}

}  // namespace


// NTime -----------------------------------------------------------------------

// The internal representation of NTime uses FILETIME, whose epoch is 1601-01-01
// 00:00:00 UTC.  ((1970-1601)*365+89)*24*60*60*1000*1000, where 89 is the
// number of leap year days between 1601 and 1970: (1970-1601)/4 excluding
// 1700, 1800, and 1900.
// static
const int64_t Time::kTimeTToMicrosecondsOffset = GG_INT64_C(11644473600000000);

// static
Time Time::Now()
{
	int64_t us = CurrentWallclockMicroseconds();
	return Time(us);
}

// static
Time Time::FromTimeStruct(bool is_local, TimeStruct &ts)
{
	// Create the system struct representing our exploded time. It will either be
	// in local time or UTC.
	SYSTEMTIME st;
	st.wYear      = (WORD)ts.year();
	st.wMonth     = (WORD)ts.month();
	st.wDayOfWeek = (WORD)ts.day_of_week();
	st.wDay       = (WORD)ts.day_of_month();
	st.wHour      = (WORD)ts.hour();
	st.wMinute    = (WORD)ts.minute();
	st.wSecond    = (WORD)ts.second();
	st.wMilliseconds = (WORD)ts.millisecond();

	// Convert to FILETIME.
	FILETIME ft;
	if (!SystemTimeToFileTime(&st, &ft))
	{
		return Time(0);
	}

	// Ensure that it's in UTC.
	if (is_local)
	{
		FILETIME utc_ft;
		LocalFileTimeToFileTime(&ft, &utc_ft);
		return Time(FileTimeToMicroseconds(utc_ft));
	}
	return Time(FileTimeToMicroseconds(ft));
}

Time::TimeStruct Time::ToTimeStruct(bool is_local)
{
	TimeStruct ts;

	// FILETIME in UTC.
	FILETIME utc_ft;
	MicrosecondsToFileTime(us_, &utc_ft);

	// FILETIME in local time if necessary.
	BOOL success = TRUE;
	FILETIME ft;
	if (is_local)
		success = FileTimeToLocalFileTime(&utc_ft, &ft);
	else
		ft = utc_ft;

	// FILETIME in SYSTEMTIME (exploded).
	SYSTEMTIME st;
	if (!success || !FileTimeToSystemTime(&ft, &st))
	{
		return ts;
	}

	ts.year_         = st.wYear;
	ts.month_        = st.wMonth;
	ts.day_of_week_  = st.wDayOfWeek;
	ts.day_of_month_ = st.wDay;
	ts.hour_         = st.wHour;
	ts.minute_       = st.wMinute;
	ts.second_       = st.wSecond;
	ts.millisecond_  = st.wMilliseconds;

	return ts;
}

// static
Time Time::FromFileTime(FILETIME ft)
{
	return Time(FileTimeToMicroseconds(ft));
}

FILETIME Time::ToFileTime() const
{
	FILETIME utc_ft;
	MicrosecondsToFileTime(us_, &utc_ft);
	return utc_ft;
}


// TimeTicks ------------------------------------------------------------------
namespace
{
// Accumulation of time lost due to rollover (in milliseconds).
int64_t rollover_ms = 0;

// The last timeGetTime value we saw, to detect rollover.
DWORD last_seen_now = 0;

// Lock protecting rollover_ms and last_seen_now.
// Note: this is a global object, and we usually avoid these. However, the time
// code is low-level, and we don't want to use Singletons here (it would be too
// easy to use a Singleton without even knowing it, and that may lead to many
// gotchas). Its impact on startup time should be negligible due to low-level
// nature of time code.
nbase::NLock rollover_lock;

// We use timeGetTime() to implement TimeTicks::Now().  This can be problematic
// because it returns the number of milliseconds since Windows has started,
// which will roll over the 32-bit value every ~49 days.  We try to track
// rollover ourselves, which works if TimeTicks::Now() is called at least every
// 49 days.
TimeDelta RolloverProtectedNow()
{
	nbase::NAutoLock locked(&rollover_lock);
	// We should hold the lock while calling tick_function to make sure that
	// we keep last_seen_now stay correctly in sync.
	DWORD now = timeGetTime();
	if (now < last_seen_now)
		rollover_ms += GG_LONGLONG(0x100000000);  // ~49.7 days.
	last_seen_now = now;
	return TimeDelta::FromMilliseconds(now + rollover_ms);
}

// Overview of time counters:
// (1) CPU cycle counter. (Retrieved via RDTSC)
// The CPU counter provides the highest resolution time stamp and is the least
// expensive to retrieve. However, the CPU counter is unreliable and should not
// be used in production. Its biggest issue is that it is per processor and it
// is not synchronized between processors. Also, on some computers, the counters
// will change frequency due to thermal and power changes, and stop in some
// states.
//
// (2) QueryPerformanceCounter (QPC). The QPC counter provides a high-
// resolution (100 nanoseconds) time stamp but is comparatively more expensive
// to retrieve. What QueryPerformanceCounter actually does is up to the HAL.
// (with some help from ACPI).
// According to http://blogs.msdn.com/oldnewthing/archive/2005/09/02/459952.aspx
// in the worst case, it gets the counter from the rollover interrupt on the
// programmable interrupt timer. In best cases, the HAL may conclude that the
// RDTSC counter runs at a constant frequency, then it uses that instead. On
// multiprocessor machines, it will try to verify the values returned from
// RDTSC on each processor are consistent with each other, and apply a handful
// of workarounds for known buggy hardware. In other words, QPC is supposed to
// give consistent result on a multiprocessor computer, but it is unreliable in
// reality due to bugs in BIOS or HAL on some, especially old computers.
// With recent updates on HAL and newer BIOS, QPC is getting more reliable but
// it should be used with caution.
//
// (3) System time. The system time provides a low-resolution (typically 10ms
// to 55 milliseconds) time stamp but is comparatively less expensive to
// retrieve and more reliable.
class HighResNowSingleton
{
public:
	static HighResNowSingleton* GetInstance()
	{
		static HighResNowSingleton instance;
		return &instance;
	}

	bool IsUsingHighResClock()
	{
		return ticks_per_microsecond_ != 0.0;
	}

	void DisableHighResClock()
	{
		ticks_per_microsecond_ = 0.0;
	}

	TimeDelta Now()
	{
		if (IsUsingHighResClock())
			return TimeDelta::FromMicroseconds(UnreliableNow());

		// Just fallback to the slower clock.
		return RolloverProtectedNow();
	}

	int64_t GetQPCDriftMicroseconds()
	{
		if (!IsUsingHighResClock())
			return 0;

		return abs((UnreliableNow() - ReliableNow()) - skew_);
	}

private:
	HighResNowSingleton()
		: ticks_per_microsecond_(0.0), skew_(0)
	{
		InitializeClock();

		// On Athlon X2 CPUs (e.g. model 15) QueryPerformanceCounter is
		// unreliable.  Fallback to low-res clock.
		nbase::CPU cpu;
		if (cpu.vendor_name() == "AuthenticAMD" && cpu.family() == 15)
			DisableHighResClock();
	}

	// Synchronize the QPC clock with GetSystemTimeAsFileTime.
	void InitializeClock()
	{
		LARGE_INTEGER ticks_per_sec = {{0}};
		if (!QueryPerformanceFrequency(&ticks_per_sec))
			return;  // Broken, we don't guarantee this function works.
		ticks_per_microsecond_ = static_cast<float>(ticks_per_sec.QuadPart) /
			static_cast<float>(Time::kMicrosecondsPerSecond);

		skew_ = UnreliableNow() - ReliableNow();
	}

	// Get the number of microseconds since boot in an unreliable fashion.
	int64_t UnreliableNow()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return static_cast<int64_t>(now.QuadPart / ticks_per_microsecond_);
	}

	// Get the number of microseconds since boot in a reliable fashion.
	int64_t ReliableNow()
	{
		return RolloverProtectedNow().ToMicroseconds();
	}

	// Cached clock frequency -> microseconds. This assumes that the clock
	// frequency is faster than one microsecond (which is 1MHz, should be OK).
	float ticks_per_microsecond_;  // 0 indicates QPF failed and we're broken.
	int64_t skew_;  // Skew between lo-res and hi-res clocks (for debugging).
};

}  // namespace

// static
TimeTicks TimeTicks::Now()
{
	return TimeTicks() + RolloverProtectedNow();
}

// static
TimeTicks TimeTicks::HighResNow()
{
	return TimeTicks() + HighResNowSingleton::GetInstance()->Now();
}

}  // namespace nbase

#endif  // OS_WIN
