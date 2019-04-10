// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/10/6
//
// template_util unittest

#if defined(WITH_UNITTEST)

#include "base/util/template_util.h"
#include "gtest/gtest.h"

struct AStruct {};
class AClass {};
enum AnEnum {};

class Parent {};
class Child : public Parent {};

TEST(TemplateUtilTest, IsPointer)
{
	EXPECT_FALSE(nbase::is_pointer<int>::value);
	EXPECT_FALSE(nbase::is_pointer<int&>::value);
	EXPECT_TRUE(nbase::is_pointer<int*>::value);
	EXPECT_TRUE(nbase::is_pointer<const int*>::value);
}

TEST(TemplateUtilTest, IsArray)
{
	EXPECT_FALSE(nbase::is_array<int>::value);
	EXPECT_FALSE(nbase::is_array<int*>::value);
	EXPECT_FALSE(nbase::is_array<int(*)[3]>::value);
	EXPECT_TRUE(nbase::is_array<int[]>::value);
	EXPECT_TRUE(nbase::is_array<const int[]>::value);
	EXPECT_TRUE(nbase::is_array<int[3]>::value);
}

TEST(TemplateUtilTest, IsNonConstReference)
{
	EXPECT_FALSE(nbase::is_non_const_reference<int>::value);
	EXPECT_FALSE(nbase::is_non_const_reference<const int&>::value);
	EXPECT_TRUE(nbase::is_non_const_reference<int&>::value);
}

TEST(TemplateUtilTest, IsConvertible)
{
	// Extra parens needed to make EXPECT_*'s parsing happy. Otherwise,
	// it sees the equivalent of
	//
	//  EXPECT_TRUE( (is_convertible < Child), (Parent > ::value));
	//
	// Silly C++.
	EXPECT_TRUE( (nbase::is_convertible<Child, Parent>::value) );
	EXPECT_FALSE( (nbase::is_convertible<Parent, Child>::value) );
	EXPECT_FALSE( (nbase::is_convertible<Parent, AStruct>::value) );

	EXPECT_TRUE( (nbase::is_convertible<int, double>::value) );
	EXPECT_TRUE( (nbase::is_convertible<int*, void*>::value) );
	EXPECT_FALSE( (nbase::is_convertible<void*, int*>::value) );
}

TEST(TemplateUtilTest, IsSame)
{
	EXPECT_FALSE( (nbase::is_same<Child, Parent>::value) );
	EXPECT_FALSE( (nbase::is_same<Parent, Child>::value) );
	EXPECT_TRUE( (nbase::is_same<Parent, Parent>::value) );

	EXPECT_TRUE( (nbase::is_same<int*, int*>::value) );
	EXPECT_TRUE( (nbase::is_same<int, int>::value) );
	EXPECT_TRUE( (nbase::is_same<void, void>::value) );
	EXPECT_FALSE( (nbase::is_same<int, double>::value) );
}

TEST(TemplateUtilTest, IsClass)
{
	EXPECT_TRUE(nbase::is_class<AStruct>::value);
	EXPECT_TRUE(nbase::is_class<AClass>::value);

	EXPECT_FALSE(nbase::is_class<AnEnum>::value);
	EXPECT_FALSE(nbase::is_class<int>::value);
	EXPECT_FALSE(nbase::is_class<char*>::value);
	EXPECT_FALSE(nbase::is_class<int&>::value);
	EXPECT_FALSE(nbase::is_class<char[3]>::value);
}


#endif  // WITH_UNITTEST