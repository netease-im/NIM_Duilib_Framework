// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/24
//
// Packet Unittest

#if defined(WITH_UNITTEST)

#include "base/memory/packet.h"
#include "gtest/gtest.h"

TEST(PackBuffer, Basic)
{
	nbase::PackBuffer pbuffer;

	EXPECT_EQ(0, pbuffer.size());

	pbuffer.resize(101);
	EXPECT_EQ(101, pbuffer.size());

	pbuffer.resize(0);
	pbuffer.append("I love this product.");
	EXPECT_EQ(20, pbuffer.size());

	std::string result_string;
	result_string.append(pbuffer.data(), pbuffer.size());
	EXPECT_STREQ("I love this product.", result_string.c_str());

	pbuffer.replace(2, "like", 4);
	result_string.clear();
	result_string.append(pbuffer.data(), pbuffer.size());
	EXPECT_STREQ("I like this product.", result_string.c_str());
}

TEST(PackUnpack, Basic)
{
	nbase::PackBuffer pbuffer;
	nbase::Pack test_pack(pbuffer);

	EXPECT_EQ(0, test_pack.size());

	test_pack.push_uint8(8);
	test_pack.push_uint16(16);
	test_pack.push_uint32(32);
	test_pack.push_uint64(64);

	std::string input1 = "test input string 1";
	std::string input2 = "test input string 2";
	std::string input3 = "test input string 3";
	std::string input4 = "test input string 4";
	std::string input5 = "test input string 5";
	nbase::Varstr var_str(input1.data(), input1.size());
	test_pack.push_varstr(var_str);
	test_pack.push_varstr(input2.c_str());
	test_pack.push_varstr(input3);
	test_pack.push_varstr(input4.data(), input4.size());
	test_pack.push_varstr32(input5.data(), input5.size());

	nbase::Unpack test_unpack(pbuffer.data(), pbuffer.size());
	EXPECT_EQ(8, test_unpack.pop_uint8());
	EXPECT_EQ(16, test_unpack.pop_uint16());
	EXPECT_EQ(32, test_unpack.pop_uint32());
	EXPECT_EQ(64, test_unpack.pop_uint64());
	nbase::Varstr var_output1 = test_unpack.pop_varstr_ptr();
	std::string output1(var_output1.data(), var_output1.size()); 
	EXPECT_EQ(input1, output1);
	nbase::Varstr var_output2 = test_unpack.pop_varstr_ptr();
	std::string output2(var_output2.data(), var_output2.size()); 
	EXPECT_EQ(input2, output2);
	nbase::Varstr var_output3 = test_unpack.pop_varstr_ptr();
	std::string output3(var_output3.data(), var_output3.size()); 
	EXPECT_EQ(input3, output3);
	nbase::Varstr var_output4 = test_unpack.pop_varstr_ptr();
	std::string output4(var_output4.data(), var_output4.size()); 
	EXPECT_EQ(input4, output4);
	nbase::Varstr var_output5 = test_unpack.pop_varstr32_ptr();
	std::string output5(var_output5.data(), var_output5.size()); 
	EXPECT_EQ(input5, output5);	
}

class MarshallableTest : public nbase::Marshallable
{
public:
	virtual void marshal(nbase::Pack &p) const
	{
		p << value_u_;
		p << value_s_;
	}

	virtual void unmarshal(const nbase::Unpack &up)
	{
		up >> value_u_ >> value_s_; 
	}

public:
	uint32_t    value_u_;
	std::string value_s_;
};

TEST(Marshallable, Basic)
{
	MarshallableTest m_test1, m_test2;
	m_test1.value_u_ = 128;
	m_test1.value_s_ = "Hello world";

	nbase::PackBuffer pbuffer;
	nbase::Pack test_pack(pbuffer);
	m_test1.marshal(test_pack);

	nbase::Unpack test_unpack(pbuffer.data(), pbuffer.size());
	m_test2.unmarshal(test_unpack);

	EXPECT_EQ(m_test1.value_u_, m_test2.value_u_);	
	EXPECT_EQ(m_test1.value_s_, m_test2.value_s_);	
}


#endif  // WITH_UNITTEST