// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/9
//
// TimeDelta/Time/TimeTicks implementation

#include "base/time/time.h"

namespace nbase
{

// TimeDelta ------------------------------------------------------------------

int TimeDelta::ToDays() const
{
	return static_cast<int>(delta_ / Time::kMicrosecondsPerDay);
}

int TimeDelta::ToHours() const
{
	return static_cast<int>(delta_ / Time::kMicrosecondsPerHour);
}

int TimeDelta::ToMinutes() const
{
	return static_cast<int>(delta_ / Time::kMicrosecondsPerMinute);
}

double TimeDelta::ToSecondsF() const
{
	return static_cast<double>(delta_) / Time::kMicrosecondsPerSecond;
}

int64_t TimeDelta::ToSeconds() const
{
	return delta_ / Time::kMicrosecondsPerSecond;
}

double TimeDelta::ToMillisecondsF() const
{
	return static_cast<double>(delta_) / Time::kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::ToMilliseconds() const
{
	return delta_ / Time::kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::ToMillisecondsRoundedUp() const
{
	return (delta_ + Time::kMicrosecondsPerMillisecond - 1) /
		Time::kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::ToMicroseconds() const
{
	return delta_;
}

// Time::TimeStruct -------------------------------------------------------------

inline bool is_in_range(int value, int lo, int hi)
{
	return lo <= value && value <= hi;
}

bool Time::TimeStruct::IsValidValues() const
{
	return is_in_range(month_, 1, 12) &&
		   is_in_range(day_of_week_, 0, 6) &&
		   is_in_range(day_of_month_, 1, 31) &&
		   is_in_range(hour_, 0, 23) &&
		   is_in_range(minute_, 0, 59) &&
		   is_in_range(second_, 0, 60) &&
		   is_in_range(millisecond_, 0, 999);
}


// Time -----------------------------------------------------------------------

Time::Time(bool is_local, TimeStruct &ts)
{
	us_ = FromTimeStruct(is_local, ts).us_;
}

Time::Time(bool is_local, int year, int month, int day, 
	       int hour, int minute, int second, int millisecond/* = 0*/)
{
	Time::TimeStruct ts;
	ts.year_ = year;
	ts.month_ = month;
	ts.day_of_month_ = day;
	ts.day_of_week_ = 0;
	ts.hour_ = hour;
	ts.minute_ = minute;
	ts.second_ = second;
	ts.millisecond_ = millisecond;

	us_ = FromTimeStruct(is_local, ts).us_;
}

// static
Time Time::FromTimeT(time_t tt)
{
	if (tt == 0)
		return Time();  // Preserve 0 so we can tell it doesn't exist.
	return Time((tt * kMicrosecondsPerSecond) + kTimeTToMicrosecondsOffset);
}

time_t Time::ToTimeT() const
{
	if (us_ == 0)
		return 0;  // Preserve 0 so we can tell it doesn't exist.
	return (us_ - kTimeTToMicrosecondsOffset) / kMicrosecondsPerSecond;
}

}  // namespace nbase