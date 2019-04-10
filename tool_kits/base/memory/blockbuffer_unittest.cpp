// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/24
//
// blockbuffer Unittest

#if defined(WITH_UNITTEST)

#include "base/memory/blockbuffer.h"
#include "gtest/gtest.h"

typedef nbase::BlockBuffer<nbase::def_block_alloc_4k, 64> Def4BB;

TEST(BlockBuffer, Basic)
{
	Def4BB  bbuffer;
	EXPECT_TRUE(bbuffer.empty());
	EXPECT_EQ(0, bbuffer.size());
	EXPECT_EQ(0, bbuffer.block());
	EXPECT_EQ(4096, bbuffer.blocksize());
	EXPECT_EQ(0, bbuffer.capacity());
	EXPECT_EQ(64*4096, bbuffer.maxsize());
	EXPECT_EQ(64*4096, bbuffer.maxfree());
	EXPECT_EQ(0, bbuffer.freespace());
	EXPECT_EQ(0, bbuffer.current_total_blocks());
	EXPECT_EQ(0, bbuffer.peak_total_blocks());

	std::string append_string = "I love this product.";
	size_t len = append_string.size();
	bbuffer.append(append_string.data(), len);
	EXPECT_TRUE(bbuffer.empty() == false);
	EXPECT_EQ(append_string.size(), bbuffer.size());
	EXPECT_EQ(1, bbuffer.block());
	EXPECT_EQ(4096, bbuffer.capacity());
	EXPECT_EQ(64*4096-20, bbuffer.maxfree());
	EXPECT_EQ(4076, bbuffer.freespace());
	std::string result_string;
	result_string.append(bbuffer.data(), bbuffer.size());
	EXPECT_STREQ("I love this product.", result_string.c_str());

	bbuffer.replace(2, "create", 4);
	result_string.clear();
	result_string.append(bbuffer.data(), bbuffer.size());
	EXPECT_STREQ("I crea this product.", result_string.c_str());

	bbuffer.erase(7, 5);
	result_string.clear();
	result_string.append(bbuffer.data(), bbuffer.size());
	EXPECT_STREQ("I crea product.", result_string.c_str());

	bbuffer.resize(5000);
	EXPECT_EQ(5000, bbuffer.size());
	EXPECT_EQ(2, bbuffer.block());
	EXPECT_EQ(2*4096, bbuffer.capacity());
 	EXPECT_EQ(64*4096-5000, bbuffer.maxfree());
 	EXPECT_EQ(2*4096-5000, bbuffer.freespace());

	EXPECT_TRUE(bbuffer.reserve(30));
	EXPECT_EQ(5000, bbuffer.size());
}

#endif  // WITH_UNITTEST