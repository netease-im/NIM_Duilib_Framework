// Copyright (c) 2011, NetEase Inc. All rights reserved.
// All rights reserved.
//
// Author: Wang Rongtao <rtwang@corp.netease.com>
// Date: 2011/6/24
//
// string utilities unit test

#if defined(WITH_UNITTEST)

#include "base/util/string_util.h"
#include "gtest/gtest.h"

TEST(String, HexBinaryConversion)
{
	std::string test1 = nbase::HexStringToBinary(nbase::BinaryToHexString(std::string("abcdefg12345")));
	EXPECT_EQ(test1, std::string("abcdefg12345"));
}

TEST(String, CaseConversion)
{
	std::string lower1 = "我1378abcdefghijklmnopqrstuvwxyz的=！&××";
	std::wstring lower2 = L"我1378abcdefghijklmnopqrstuvwxyz的=！&××";
	std::string upper1 = "我1378ABCDEFGHIJKLMNOPQRSTUVWXYZ的=！&××";
	std::wstring upper2 = L"我1378ABCDEFGHIJKLMNOPQRSTUVWXYZ的=！&××";
	EXPECT_EQ(upper1, nbase::MakeUpperString(lower1));
	EXPECT_EQ(upper2, nbase::MakeUpperString(lower2));
	EXPECT_EQ(lower1, nbase::MakeLowerString(upper1));
	EXPECT_EQ(lower2, nbase::MakeLowerString(upper2));
}

TEST(String, Replace)
{
	std::wstring empty_string;
	nbase::StringReplaceAll(L"abcd", L"!@#$", empty_string);
	std::wstring test2 = L"abcdefgabcdefgaafdjfsdajjfdsabcdabceabc";
	nbase::StringReplaceAll(L"abcd", L"!@#$", test2);
	EXPECT_EQ(test2, std::wstring(L"!@#$efg!@#$efgaafdjfsdajjfds!@#$abceabc"));
	nbase::StringReplaceAll(L"!@#$", L"()", test2);
	EXPECT_EQ(test2, std::wstring(L"()efg()efgaafdjfsdajjfds()abceabc"));
}

TEST(String, UTFConversion)
{
#if defined(OS_WIN)	
	std::string s8;
	std::wstring s16 = L"网易fighter!！~!@#$%^&*()_+|极光炫影™";
	std::basic_string<nbase::UTF32Char> s32;
	while (s16.length() < 8096)
		s16 = s16 + s16;
	std::wstring result = s16;
	nbase::UTF16ToUTF32(s16, s32);
	nbase::UTF32ToUTF8(s32, s8);
	nbase::UTF8ToUTF16(s8, s16);
	EXPECT_EQ(result, s16);
	std::string result2 = s8;
	nbase::UTF8ToUTF32(s8, s32);
	nbase::UTF32ToUTF16(s32, s16);
	nbase::UTF16ToUTF8(s16, s8);
	EXPECT_EQ(result2, s8);
#else
	std::string s8;
	std::wstring s32 = L"网易fighter!！~!@#$%^&*()_+|极光炫影™";
	std::basic_string<nbase::UTF16Char> s16;
	while (s32.length() < 8096)
		s32 = s32 + s32;
	std::wstring result = s32;
	nbase::UTF32ToUTF16(s32, s16);
	nbase::UTF16ToUTF8(s16, s8);
	nbase::UTF8ToUTF32(s8, s32);
	EXPECT_EQ(result, s32);
	std::basic_string<nbase::UTF16Char> result2 = s16;
	nbase::UTF16ToUTF32(s16, s32);
	nbase::UTF32ToUTF8(s32, s8);
	nbase::UTF8ToUTF16(s8, s16);
	EXPECT_EQ(result2, s16);
#endif
}

TEST(String, StringTrimming)
{
	std::wstring test6 = L"		 中Trimming String\b		";
	EXPECT_EQ(std::wstring(L"中Trimming String\b		"), nbase::StringTrimLeft(test6.c_str()));
	EXPECT_EQ(std::wstring(L"		 中Trimming String"), nbase::StringTrimRight(test6.c_str()));
	EXPECT_EQ(std::wstring(L"中Trimming String"), nbase::StringTrim(test6.c_str()));
}

#endif // WITH_UNITTEST
