// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/9
//
// TimeDelta/Time/TimeTicks implementation on posix platform

#include "base/time/time.h"

#if defined(OS_POSIX)

#include <sys/time.h>
#include <time.h>
#include <limits>

#if defined(OS_MACOSX) || defined(OS_IOS)
#include <mach/mach_time.h>
#endif

namespace nbase
{

// TimeDelta -----------------------------------------------------------------------
struct timespec TimeDelta::ToTimeSpec() const
{
	int64_t microseconds = ToMicroseconds();
	time_t seconds = 0;
	if (microseconds >= Time::kMicrosecondsPerSecond)
	{
		seconds = ToSeconds();
		microseconds -= seconds * Time::kMicrosecondsPerSecond;
	}
	struct timespec result = {seconds,
							  microseconds * Time::kNanosecondsPerMicrosecond};
	return result;
}

	
// Time -----------------------------------------------------------------------
	
// Windows uses a Gregorian epoch of 1601.  We need to match this internally
// so that our time representations match across all platforms.  See bug 14734.
//   irb(main):010:0> Time.at(0).getutc()
//   => Thu Jan 01 00:00:00 UTC 1970
//   irb(main):011:0> Time.at(-11644473600).getutc()
//   => Mon Jan 01 00:00:00 UTC 1601
static const int64_t kWindowsEpochDeltaSeconds = GG_INT64_C(11644473600);
static const int64_t kWindowsEpochDeltaMilliseconds =
	kWindowsEpochDeltaSeconds * Time::kMillisecondsPerSecond;

// static
const int64_t Time::kWindowsEpochDeltaMicroseconds =
	kWindowsEpochDeltaSeconds * Time::kMicrosecondsPerSecond;

// Some functions in time.cc use time_t directly, so we provide an offset
// to convert from time_t (Unix epoch) and internal (Windows epoch).
// static
const int64_t Time::kTimeTToMicrosecondsOffset = kWindowsEpochDeltaMicroseconds;


// static
Time Time::Now()
{
	struct timeval tv;
	struct timezone tz = { 0, 0 };  // UTC
	if (gettimeofday(&tv, &tz) != 0)
	{
	}
	// Combine seconds and microseconds in a 64-bit field containing microseconds
	// since the epoch.  That's enough for nearly 600 centuries.  Adjust from
	// Unix (1970) to Windows (1601) epoch.
	return Time((tv.tv_sec * kMicrosecondsPerSecond + tv.tv_usec) +
		kWindowsEpochDeltaMicroseconds);
}

Time::TimeStruct Time::ToTimeStruct(bool is_local)
{
	TimeStruct ts;
	// Time stores times with microsecond resolution, but Exploded only carries
	// millisecond resolution, so begin by being lossy.  Adjust from Windows
	// epoch (1601) to Unix epoch (1970);
	int64_t milliseconds = (us_ - kWindowsEpochDeltaMicroseconds) /
		kMicrosecondsPerMillisecond;
	time_t seconds = milliseconds / kMillisecondsPerSecond;

	struct tm timestruct;
	if (is_local)
		localtime_r(&seconds, &timestruct);
	else
		gmtime_r(&seconds, &timestruct);

	ts.year_         = timestruct.tm_year + 1900;
	ts.month_        = timestruct.tm_mon + 1;
	ts.day_of_week_  = timestruct.tm_wday;
	ts.day_of_month_ = timestruct.tm_mday;
	ts.hour_         = timestruct.tm_hour;
	ts.minute_       = timestruct.tm_min;
	ts.second_       = timestruct.tm_sec;
	ts.millisecond_  = milliseconds % kMillisecondsPerSecond;
	return ts;
}

// static
Time Time::FromTimeStruct(bool is_local, TimeStruct &ts)
{
	struct tm timestruct;
	timestruct.tm_sec    = ts.second();
	timestruct.tm_min    = ts.minute();
	timestruct.tm_hour   = ts.hour();
	timestruct.tm_mday   = ts.day_of_month();
	timestruct.tm_mon    = ts.month() - 1;
	timestruct.tm_year   = ts.year() - 1900;
	timestruct.tm_wday   = ts.day_of_week();  // mktime/timegm ignore this
	timestruct.tm_yday   = 0;     // mktime/timegm ignore this
	timestruct.tm_isdst  = -1;    // attempt to figure it out
	timestruct.tm_gmtoff = 0;     // not a POSIX field, so mktime/timegm ignore
	timestruct.tm_zone   = NULL;  // not a POSIX field, so mktime/timegm ignore

	time_t seconds;
	if (is_local)
		seconds = mktime(&timestruct);
	else
		seconds = timegm(&timestruct);

	int64_t milliseconds;
	// Handle overflow.  Clamping the range to what mktime and timegm might
	// return is the best that can be done here.  It's not ideal, but it's better
	// than failing here or ignoring the overflow case and treating each time
	// overflow as one second prior to the epoch.
	if (seconds == -1 &&
		(ts.year() < 1969 || ts.year() > 1970))
	{
		// If exploded.year is 1969 or 1970, take -1 as correct, with the
		// time indicating 1 second prior to the epoch.  (1970 is allowed to handle
		// time zone and DST offsets.)  Otherwise, return the most future or past
		// time representable.  Assumes the time_t epoch is 1970-01-01 00:00:00 UTC.
		//
		// The minimum and maximum representible times that mktime and timegm could
		// return are used here instead of values outside that range to allow for
		// proper round-tripping between exploded and counter-type time
		// representations in the presence of possible truncation to time_t by
		// division and use with other functions that accept time_t.
		//
		// When representing the most distant time in the future, add in an extra
		// 999ms to avoid the time being less than any other possible value that
		// this function can return.
		if (ts.year() < 1969)
		{
			milliseconds = std::numeric_limits<time_t>::min() *
				kMillisecondsPerSecond;
		}
		else
		{
			milliseconds = (std::numeric_limits<time_t>::max() *
				kMillisecondsPerSecond) +
				kMillisecondsPerSecond - 1;
		}
	}
	else
	{
		milliseconds = seconds * kMillisecondsPerSecond + ts.millisecond();
	}

	// Adjust from Unix (1970) to Windows (1601) epoch.
	return Time((milliseconds * kMicrosecondsPerMillisecond) +
		kWindowsEpochDeltaMicroseconds);
}


// TimeTicks ------------------------------------------------------------------
#if defined(OS_MACOSX) || defined(OS_IOS)

// static
TimeTicks TimeTicks::Now()
{
	uint64_t absolute_micro;

	static mach_timebase_info_data_t timebase_info;
	if (timebase_info.denom == 0)
	{
		// Zero-initialization of statics guarantees that denom will be 0 before
		// calling mach_timebase_info.  mach_timebase_info will never set denom to
		// 0 as that would be invalid, so the zero-check can be used to determine
		// whether mach_timebase_info has already been called.  This is
		// recommended by Apple's QA1398.
		mach_timebase_info(&timebase_info);
	}

	// mach_absolute_time is it when it comes to ticks on the Mac.  Other calls
	// with less precision (such as TickCount) just call through to
	// mach_absolute_time.

	// timebase_info converts absolute time tick units into nanoseconds.  Convert
	// to microseconds up front to stave off overflows.
	absolute_micro = mach_absolute_time() / Time::kNanosecondsPerMicrosecond *
		             timebase_info.numer / timebase_info.denom;

	// Don't bother with the rollover handling that the Windows version does.
	// With numer and denom = 1 (the expected case), the 64-bit absolute time
	// reported in nanoseconds is enough to last nearly 585 years.

	return TimeTicks(absolute_micro);
}

#elif (defined(OS_POSIX) &&                                               \
       defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK >= 0) || \
	   defined(OS_FREEBSD)

// static
TimeTicks TimeTicks::Now()
{
	uint64_t absolute_micro;

	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
	{
		return TimeTicks();
	}

	absolute_micro =
		(static_cast<int64>(ts.tv_sec) * Time::kMicrosecondsPerSecond) +
		(static_cast<int64>(ts.tv_nsec) / Time::kNanosecondsPerMicrosecond);

	return TimeTicks(absolute_micro);

}

#else

// static
TimeTicks TimeTicks::Now()
{
	uint64_t absolute_micro;

	struct timeval tv;
	struct timezone tz = { 0, 0 };  // UTC
	if (gettimeofday(&tv, &tz) != 0)
	{
		return TimeTicks();
	}

	absolute_micro =
		(static_cast<int64_t>(tv.tv_sec) * Time::kMicrosecondsPerSecond) +
		(static_cast<int64_t>(tv.tv_usec));

	return TimeTicks(absolute_micro);
}

#endif 

// static
TimeTicks TimeTicks::HighResNow()
{
	return TimeTicks::Now();
}

}  // namespace nbase

#endif  // OS_POSIX