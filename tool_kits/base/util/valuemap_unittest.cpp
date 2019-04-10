// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/11/1
//
// valuemap unittest

#if defined(WITH_UNITTEST)

#include "base/util/valuemap.h"
#include "gtest/gtest.h"

TEST(ValueMap, Basic)
{
	char c = 'k';
	float f = 1.3f;
	double d = 33.1;
	int8_t i8 = 3;
	uint16_t u16 = 47;
	int32_t i32 = 3824;
    int64_t i64 = 76653123;
	uint16_t h = 12;

	nbase::ValueMap value_map;

	value_map.bind("char", &c, nbase::ValueMap::t_char);
	value_map.bind("float", &f, nbase::ValueMap::t_float);
	value_map.bind("double", &d, nbase::ValueMap::t_double);
	value_map.bind("int8", &i8, nbase::ValueMap::t_i8);
	value_map.bind("uint16", &u16, nbase::ValueMap::t_u16);
	value_map.bind("int32", &i32, nbase::ValueMap::t_i32);
	value_map.bind("int64", &i64, nbase::ValueMap::t_i64);
	value_map.bind("h16", &h, nbase::ValueMap::t_h16);

	EXPECT_STREQ(value_map.get("char").c_str(), "k");
	EXPECT_STREQ(value_map.get("float").c_str(), "1.300000");
	EXPECT_STREQ(value_map.get("double").c_str(), "33.100000");
	EXPECT_STREQ(value_map.get("int8").c_str(), "3");
	EXPECT_STREQ(value_map.get("uint16").c_str(), "47");
	EXPECT_STREQ(value_map.get("int32").c_str(), "3824");
	EXPECT_STREQ(value_map.get("int64").c_str(), "76653123");
	EXPECT_STREQ(value_map.get("h16").c_str(), "c");
}

#endif  // WITH_UNITTEST