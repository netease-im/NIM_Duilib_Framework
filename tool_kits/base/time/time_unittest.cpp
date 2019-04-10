// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/9
//
// TimeDelta/Time/TimeTicks Unittest

#if defined(WITH_UNITTEST)

#include "base/time/time.h"
#include "base/thread/thread.h"
#include "gtest/gtest.h"

using nbase::Time;
using nbase::TimeDelta;
using nbase::TimeTicks;

// TimeDelta Unittest
TEST(TimeDelta, FromAndTo)
{
    EXPECT_TRUE(TimeDelta::FromDays(2) == TimeDelta::FromHours(48));
    EXPECT_TRUE(TimeDelta::FromHours(3) == TimeDelta::FromMinutes(180));
    EXPECT_TRUE(TimeDelta::FromMinutes(2) == TimeDelta::FromSeconds(120));
    EXPECT_TRUE(TimeDelta::FromSeconds(2) == TimeDelta::FromMilliseconds(2000));
    EXPECT_TRUE(TimeDelta::FromMilliseconds(2) ==
        TimeDelta::FromMicroseconds(2000));

    EXPECT_EQ(13, TimeDelta::FromDays(13).ToDays());
    EXPECT_EQ(13, TimeDelta::FromHours(13).ToHours());
    EXPECT_EQ(13, TimeDelta::FromMinutes(13).ToMinutes());
    EXPECT_EQ(13, TimeDelta::FromSeconds(13).ToSeconds());
    EXPECT_EQ(13.0, TimeDelta::FromSeconds(13).ToSecondsF());
    EXPECT_EQ(13, TimeDelta::FromMilliseconds(13).ToMilliseconds());
    EXPECT_EQ(13.0, TimeDelta::FromMilliseconds(13).ToMillisecondsF());
    EXPECT_EQ(13, TimeDelta::FromMicroseconds(13).ToMicroseconds());
}

TEST(TimeDelta, Operator)
{
    EXPECT_EQ(1, (TimeDelta::FromDays(13) - TimeDelta::FromDays(12)).ToDays());
    EXPECT_EQ(5, (TimeDelta::FromHours(3) + TimeDelta::FromHours(2)).ToHours());

    EXPECT_TRUE(TimeDelta::FromDays(1) > TimeDelta::FromHours(23));
    EXPECT_TRUE(TimeDelta::FromMinutes(30) < TimeDelta::FromHours(3));
}

#if defined(OS_POSIX)
TEST(TimeDelta, TimeSpecConversion) {
    struct timespec result = TimeDelta::FromSeconds(0).ToTimeSpec();
    EXPECT_EQ(result.tv_sec, 0);
    EXPECT_EQ(result.tv_nsec, 0);

    result = TimeDelta::FromSeconds(1).ToTimeSpec();
    EXPECT_EQ(result.tv_sec, 1);
    EXPECT_EQ(result.tv_nsec, 0);

    result = TimeDelta::FromMicroseconds(1).ToTimeSpec();
    EXPECT_EQ(result.tv_sec, 0);
    EXPECT_EQ(result.tv_nsec, 1000);

    result = TimeDelta::FromMicroseconds(
        Time::kMicrosecondsPerSecond + 1).ToTimeSpec();
    EXPECT_EQ(result.tv_sec, 1);
    EXPECT_EQ(result.tv_nsec, 1000);
}
#endif  // OS_POSIX

// TimeTicks Unittest
TEST(TimeTicks, Delta)
{
    TimeTicks ticks_start = TimeTicks::Now();
    nbase::Thread::Sleep(10);
    TimeTicks ticks_stop = TimeTicks::Now();
    TimeDelta delta = ticks_stop - ticks_start;

    EXPECT_GE(delta.ToMilliseconds(), 9);
    EXPECT_GE(delta.ToMicroseconds(), 9000);
    EXPECT_EQ(delta.ToSeconds(), 0);
}

TEST(TimeTicks, HighResNow)
{
    const int kTargetGranularityUs = 10000;  // 10ms

    bool success = false;
    int retries = 100;  // Arbitrary.
    TimeDelta delta;
    while (!success && retries--)
    {
        TimeTicks ticks_start = TimeTicks::HighResNow();
        // Loop until we can detect that the clock has changed.  Non-HighRes timers
        // will increment in chunks, e.g. 10ms.  By spinning until we see a clock
        // change, we detect the minimum time between measurements.
        do
        {
            delta = TimeTicks::HighResNow() - ticks_start;
        } while (delta.ToMilliseconds() == 0);

        if (delta.ToMicroseconds() <= kTargetGranularityUs)
            success = true;
    }

    // In high resolution mode, we expect to see the clock increment
    // in intervals less than 10ms.
    EXPECT_TRUE(success);
}

// Time Unittest
TEST(Time, Construct)
{
    Time t1(true, 2011, 6, 23, 16, 20, 0, 0);

    Time::TimeStruct ts;
    ts.year_  = 2011;
    ts.month_ = 6;
    ts.day_of_month_ = 23;
    ts.hour_ = 16;
    ts.minute_ = 20;
    ts.second_ = 0;
    ts.millisecond_ = 0;
    Time t2 = Time::FromTimeStruct(true, ts);

    EXPECT_TRUE(t1 == t2);
}

TEST(Time, TimeTAndTimeStruct)
{
    // C library time and exploded time.
    time_t now_t_1 = time(NULL);
    struct tm tms;
#if defined(OS_WIN)
    localtime_s(&tms, &now_t_1);
#elif defined(OS_POSIX)
    localtime_r(&now_t_1, &tms);
#endif

    // Convert to ours.
    Time our_time_1 = Time::FromTimeT(now_t_1);
    Time::TimeStruct ts = our_time_1.ToTimeStruct(true);

    // This will test both our exploding and our time_t -> Time conversion.
    EXPECT_EQ(tms.tm_year + 1900, ts.year());
    EXPECT_EQ(tms.tm_mon + 1, ts.month());
    EXPECT_EQ(tms.tm_mday, ts.day_of_month());
    EXPECT_EQ(tms.tm_hour, ts.hour());
    EXPECT_EQ(tms.tm_min, ts.minute());
    EXPECT_EQ(tms.tm_sec, ts.second());

    // Convert exploded back to the time struct.
    Time our_time_2 = Time::FromTimeStruct(true, ts);
    EXPECT_TRUE(our_time_1 == our_time_2);

    time_t now_t_2 = our_time_2.ToTimeT();
    EXPECT_EQ(now_t_1, now_t_2);

    EXPECT_EQ(10, Time().FromTimeT(10).ToTimeT());

    // Conversions of 0 should stay 0.
    EXPECT_EQ(0, Time().ToTimeT());
    EXPECT_EQ(0, Time::FromTimeT(0).ToInternalValue());

    Time t_a = Time::Now();
    ts = t_a.ToTimeStruct(true);
    Time t_b = Time::FromTimeStruct(true, ts);
    EXPECT_TRUE((t_a - t_b) < TimeDelta::FromSeconds(1));

    ts = t_a.ToTimeStruct(false);
    t_b = Time::FromTimeStruct(false, ts);
    EXPECT_TRUE((t_a - t_b) < TimeDelta::FromSeconds(1));
    t_b = Time::FromTimeStruct(true, ts);
    EXPECT_TRUE(t_a != t_b);
}

TEST(Time, Operator)
{
    Time t_now = Time::Now();
    Time::TimeStruct ts_now = t_now.ToTimeStruct(true);
    Time t_a = Time::FromTimeStruct(true, ts_now);
    EXPECT_TRUE((t_now - t_a) < TimeDelta::FromSeconds(1));

    Time::TimeStruct ts_b = t_now.ToTimeStruct(true);
	if (ts_now.day_of_month() < 30 && ts_now.month() != 2)
	{
		ts_b.day_of_month_ = ts_now.day_of_month() + 1;
		Time t_b = Time::FromTimeStruct(true, ts_b);
		EXPECT_TRUE(t_b > t_a);
		EXPECT_TRUE((t_b - t_a) == TimeDelta::FromDays(1));

		Time t_c = t_a + TimeDelta::FromDays(1);
		EXPECT_TRUE(t_c == t_b);
	}
}

#endif  // WITH_UNITTEST

