// Time represents an absolute point in time, internally represented as
// microseconds (s/1,000,000) since a platform-dependent epoch.  Each
// platform's epoch, along with other system-dependent clock interface
// routines, is defined in time_PLATFORM.cc.
// 
// TimeDelta represents a duration of time, internally represented in
// microseconds.
//
// TimeTicks represents an abstract time that is always incrementing for use
// in measuring time durations. It is internally represented in microseconds.
// It can not be converted to a human-readable time, but is guaranteed not to
// decrease (if the user changes the computer clock, Time::Now() may actually
// decrease or jump).

#ifndef BASE_TIME_TIME_H_
#define BASE_TIME_TIME_H_

#include <time.h>

#include "base/base_types.h"
#include "base/base_export.h"

#if defined(OS_WIN)
#  include <windows.h>
#else
#  include <sys/time.h>
#endif // OS_WIN

namespace nbase
{

class Time;
class TimeTicks;

class BASE_EXPORT TimeDelta
{
public:
	TimeDelta() : delta_(0) {}

	// Converts units of time to TimeDeltas.
	static TimeDelta FromDays(int64_t days);
	static TimeDelta FromHours(int64_t hours);
	static TimeDelta FromMinutes(int64_t minutes);
	static TimeDelta FromSeconds(int64_t secs);
	static TimeDelta FromMilliseconds(int64_t ms);
	static TimeDelta FromMicroseconds(int64_t us);

	// Returns the internal numeric value of the TimeDelta object.
	int64_t ToInternalValue() const
	{
		return delta_;
	}

#if defined(OS_POSIX)
	struct timespec ToTimeSpec() const;
#endif

	// Returns the time delta in some unit. The F versions return a floating
	// point value, the "regular" versions return a rounded-down value.
	int ToDays() const;
	int ToHours() const;
	int ToMinutes() const;
	double ToSecondsF() const;
	int64_t ToSeconds() const;
	double ToMillisecondsF() const;
	int64_t ToMilliseconds() const;
	int64_t ToMillisecondsRoundedUp() const;
	int64_t ToMicroseconds() const;

	TimeDelta& operator=(TimeDelta other)
	{
		delta_ = other.delta_;
		return *this;
	}

	// Computations with other deltas.
	TimeDelta operator+(TimeDelta other) const
	{
		return TimeDelta(delta_ + other.delta_);
	}
	TimeDelta operator-(TimeDelta other) const
	{
		return TimeDelta(delta_ - other.delta_);
	}

	TimeDelta& operator+=(TimeDelta other)
	{
		delta_ += other.delta_;
		return *this;
	}
	TimeDelta& operator-=(TimeDelta other)
	{
		delta_ -= other.delta_;
		return *this;
	}
	TimeDelta operator-() const
	{
		return TimeDelta(-delta_);
	}

	// Computations with ints, note that we only allow multiplicative operations
	// with ints, and additive operations with other deltas.
	TimeDelta operator*(int64_t a) const
	{
		return TimeDelta(delta_ * a);
	}
	TimeDelta operator/(int64_t a) const
	{
		return TimeDelta(delta_ / a);
	}
	TimeDelta& operator*=(int64_t a)
	{
		delta_ *= a;
		return *this;
	}
	TimeDelta& operator/=(int64_t a)
	{
		delta_ /= a;
		return *this;
	}
	int64_t operator/(TimeDelta a) const
	{
		return delta_ / a.delta_;
	}

	// Defined below because it depends on the definition of the other classes.
	Time operator+(Time t) const;
	TimeTicks operator+(TimeTicks t) const;

	// Comparison operators.
	bool operator==(TimeDelta other) const
	{
		return delta_ == other.delta_;
	}
	bool operator!=(TimeDelta other) const
	{
		return delta_ != other.delta_;
	}
	bool operator<(TimeDelta other) const
	{
		return delta_ < other.delta_;
	}
	bool operator<=(TimeDelta other) const
	{
		return delta_ <= other.delta_;
	}
	bool operator>(TimeDelta other) const
	{
		return delta_ > other.delta_;
	}
	bool operator>=(TimeDelta other) const
	{
		return delta_ >= other.delta_;
	}

private:
	friend class Time;
	friend class TimeTicks;

	friend TimeDelta operator*(int64_t a, TimeDelta td);

	// Constructs a delta given the duration in microseconds.
	explicit TimeDelta(int64_t delta_us) : delta_(delta_us) {}

	// Delta in microseconds.
	int64_t delta_;
};

inline TimeDelta operator*(int64_t a, TimeDelta td)
{
	return TimeDelta(a * td.delta_);
}


// Time -----------------------------------------------------------------------

// Represents a wall clock time.
class BASE_EXPORT Time
{
public:
	static const int64_t kMillisecondsPerSecond      = 1000;
	static const int64_t kMicrosecondsPerMillisecond = 1000;
	static const int64_t kMicrosecondsPerSecond      = kMicrosecondsPerMillisecond * 
		                                               kMillisecondsPerSecond;
	static const int64_t kMicrosecondsPerMinute      = kMicrosecondsPerSecond * 60;
	static const int64_t kMicrosecondsPerHour        = kMicrosecondsPerMinute * 60;
	static const int64_t kMicrosecondsPerDay         = kMicrosecondsPerHour * 24;
	static const int64_t kMicrosecondsPerWeek        = kMicrosecondsPerDay * 7;
	static const int64_t kNanosecondsPerMicrosecond  = 1000;
	static const int64_t kNanosecondsPerSecond       = kNanosecondsPerMicrosecond *
		                                               kMicrosecondsPerSecond;
#if !defined(OS_WIN)
	// On Mac & Linux, this value is the delta from the Windows epoch of 1601 to
	// the Posix delta of 1970. This is used for migrating between the old
	// 1970-based epochs to the new 1601-based ones. It should be removed from
	// this global header and put in the platform-specific ones when we remove the
	// migration code.
	static const int64_t kWindowsEpochDeltaMicroseconds;
#endif

	// Represents an exploded time that can be formatted nicely. This is kind of
	// like the Win32 SYSTEMTIME structure or the Unix "struct tm" with a few
	// additions and changes to prevent errors.
	struct BASE_EXPORT TimeStruct
	{
		int year_;          // Four digit year "2011"
		int month_;         // 1-based month (values 1 = January, etc.)
		int day_of_week_;   // 0-based day of week (0 = Sunday, etc.)
		int day_of_month_;  // 1-based day of month (1-31)
		int hour_;          // Hour within the current day (0-23)
		int minute_;        // Minute within the current hour (0-59)
		int second_;        // Second within the current minute (0-59).
		int millisecond_;   // Milliseconds within the current second (0-999)

		explicit TimeStruct()
			: year_(0), month_(0), day_of_week_(0), day_of_month_(0), hour_(0), 
			  minute_(0), second_(0), millisecond_(0)
		{}
		bool IsValidValues() const;

		int year()         { return year_; }
		int month()        { return month_; }
		int day_of_week()  { return day_of_week_; }
		int day_of_month() { return day_of_month_; }
		int hour()         { return hour_; }
		int minute()       { return minute_; }
		int second()       { return second_; }
		int millisecond()  { return millisecond_; }
	};

public:
	explicit Time() : us_(0) {}
	explicit Time(int64_t us) : us_(us) {}
	explicit Time(bool is_local, TimeStruct &ts);
	explicit Time(bool is_local, 
		int year, int month, int day, int hour, int minute, int second, int millisecond = 0);

	bool is_null() const { return us_ == 0; }
	int64_t ToInternalValue() const { return us_; }

	// Returns the current time.
	static Time Now();

	// Converts to/from TimeStruct in UTC and a Time class.
	static Time FromTimeStruct(bool is_local, TimeStruct &ts);
	TimeStruct ToTimeStruct(bool is_local);

	// Converts to/from time_t in UTC and a Time class.
	static Time FromTimeT(time_t tt);
	time_t ToTimeT() const;

#if defined(OS_WIN)
	// Converts to/from FILETIME.
	static Time FromFileTime(FILETIME ft);
	FILETIME ToFileTime() const;
#endif // OS_WIN

	Time& operator=(Time other)
	{
		us_ = other.us_;
		return *this;
	}

	// Compute the difference between two times.
	TimeDelta operator-(Time other) const
	{
		return TimeDelta(us_ - other.us_);
	}

	// Modify by some time delta.
	Time& operator+=(TimeDelta delta)
	{
		us_ += delta.delta_;
		return *this;
	}
	Time& operator-=(TimeDelta delta)
	{
		us_ -= delta.delta_;
		return *this;
	}

	// Return a new time modified by some delta.
	Time operator+(TimeDelta delta) const
	{
		return Time(us_ + delta.delta_);
	}
	Time operator-(TimeDelta delta) const
	{
		return Time(us_ - delta.delta_);
	}

	// Comparison operators
	bool operator==(Time other) const
	{
		return us_ == other.us_;
	}
	bool operator!=(Time other) const
	{
		return us_ != other.us_;
	}
	bool operator<(Time other) const
	{
		return us_ < other.us_;
	}
	bool operator<=(Time other) const
	{
		return us_ <= other.us_;
	}
	bool operator>(Time other) const
	{
		return us_ > other.us_;
	}
	bool operator>=(Time other) const
	{
		return us_ >= other.us_;
	}

private:
	friend class TimeDelta;

	// The representation of Jan 1, 1970 UTC in microseconds since the
	// platform-dependent epoch.
	static const int64_t kTimeTToMicrosecondsOffset;

	// NTime in microseconds in UTC.
	int64_t us_;
};

// Inline the TimeDelta factory methods, for fast TimeDelta construction.
// static
inline TimeDelta TimeDelta::FromDays(int64_t days)
{
	return TimeDelta(days * Time::kMicrosecondsPerDay);
}

// static
inline TimeDelta TimeDelta::FromHours(int64_t hours)
{
	return TimeDelta(hours * Time::kMicrosecondsPerHour);
}

// static
inline TimeDelta TimeDelta::FromMinutes(int64_t minutes)
{
	return TimeDelta(minutes * Time::kMicrosecondsPerMinute);
}

// static
inline TimeDelta TimeDelta::FromSeconds(int64_t secs)
{
	return TimeDelta(secs * Time::kMicrosecondsPerSecond);
}

// static
inline TimeDelta TimeDelta::FromMilliseconds(int64_t ms)
{
	return TimeDelta(ms * Time::kMicrosecondsPerMillisecond);
}

// static
inline TimeDelta TimeDelta::FromMicroseconds(int64_t us)
{
	return TimeDelta(us);
}

inline Time TimeDelta::operator+(Time t) const
{
	return Time(t.us_ + delta_);
}


// TimeTicks ------------------------------------------------------------------

class BASE_EXPORT TimeTicks
{
public:
	TimeTicks() : ticks_(0) {}

	explicit TimeTicks(int64_t ticks) : ticks_(ticks) {}

	// Platform-dependent tick count representing "right now."
	// The resolution of this clock is ~1-15ms.  Resolution varies depending
	// on hardware/operating system configuration.
	static TimeTicks Now();

	// Returns a platform-dependent high-resolution tick count. Implementation
	// is hardware dependent and may or may not return sub-millisecond
	// resolution.  THIS CALL IS GENERALLY MUCH MORE EXPENSIVE THAN Now() AND
	// SHOULD ONLY BE USED WHEN IT IS REALLY NEEDED.
	static TimeTicks HighResNow();

	// Returns true if this object has not been initialized.
	bool is_null() const
	{
		return ticks_ == 0;
	}

	// Returns the internal numeric value of the TimeTicks object.
	int64_t ToInternalValue() const
	{
		return ticks_;
	}

	TimeTicks& operator=(TimeTicks other)
	{
		ticks_ = other.ticks_;
		return *this;
	}

	// Compute the difference between two times.
	TimeDelta operator-(TimeTicks other) const
	{
		return TimeDelta(ticks_ - other.ticks_);
	}

	// Modify by some time delta.
	TimeTicks& operator+=(TimeDelta delta)
	{
		ticks_ += delta.delta_;
		return *this;
	}
	TimeTicks& operator-=(TimeDelta delta)
	{
		ticks_ -= delta.delta_;
		return *this;
	}

	// Return a new TimeTicks modified by some delta.
	TimeTicks operator+(TimeDelta delta) const
	{
		return TimeTicks(ticks_ + delta.delta_);
	}
	TimeTicks operator-(TimeDelta delta) const
	{
		return TimeTicks(ticks_ - delta.delta_);
	}

	// Comparison operators
	bool operator==(TimeTicks other) const
	{
		return ticks_ == other.ticks_;
	}
	bool operator!=(TimeTicks other) const
	{
		return ticks_ != other.ticks_;
	}
	bool operator<(TimeTicks other) const
	{
		return ticks_ < other.ticks_;
	}
	bool operator<=(TimeTicks other) const
	{
		return ticks_ <= other.ticks_;
	}
	bool operator>(TimeTicks other) const
	{
		return ticks_ > other.ticks_;
	}
	bool operator>=(TimeTicks other) const
	{
		return ticks_ >= other.ticks_;
	}

protected:
	friend class TimeDelta;

	// Tick count in microseconds.
	int64_t ticks_;
};

inline TimeTicks TimeDelta::operator+(TimeTicks t) const
{
	return TimeTicks(t.ticks_ + delta_);
}

}  // namespace nbase

#endif  // BASE_TIME_TIME_H_