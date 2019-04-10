// Copyright (c) 2011, NetEase Inc. All rights reserved.
// All rights reserved.
//
// Author: Wang Rongtao <rtwang@corp.netease.com>
// Date: 2011/7/1
//
// Unittest of Windows string conversion

#if defined(WITH_UNITTEST)

#include "base/win32/platform_string_util.h"
#if defined(OS_WIN)
#include "gtest/gtest.h"

#define TEST_STRING "asfj我是中国宁"
#define TEST_WSTRING L"asfj我是中国宁"

TEST(Win32, MBCSUnicodeConversion)
{
	std::string mbcs;
	std::wstring unicode;
	nbase::win32::MBCSToUnicode(TEST_STRING, unicode);
	EXPECT_EQ(std::wstring(TEST_WSTRING), unicode);
	nbase::win32::UnicodeToMBCS(unicode, mbcs);
	EXPECT_EQ(std::string(TEST_STRING), mbcs);

	nbase::win32::UnicodeToMBCS(TEST_WSTRING, mbcs);
	EXPECT_EQ(std::string(TEST_STRING), mbcs);
	nbase::win32::MBCSToUnicode(mbcs, unicode);
	EXPECT_EQ(std::wstring(TEST_WSTRING), unicode);
}

#endif // OS_WIN
#endif // WITH_UNITTEST