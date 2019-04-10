// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/23
//
// Encrypt Unittest

#include "base/encrypt/encrypt_impl.h"
#include "gtest/gtest.h"

#if defined(WITH_UNITTEST)
#if defined(WITH_ENCRYPT)

TEST(EncryptTest, MD5)
{
	std::string string_ori = "I love this product";
	std::string string_md5 = "c075917e5681d9379eb5c9278f83f8d2";
	std::string string_dst;

	nbase::Md5((const unsigned char *)string_ori.data(), string_ori.size(), string_dst);
	EXPECT_STREQ(string_md5.c_str(), nbase::BinaryToHexString(string_dst).c_str());

	nbase::EncryptInterface_var encrypt  = new nbase::Encrypt_Impl();
	encrypt->SetMethod(nbase::ENC_MD5);
	encrypt->Encrypt(string_ori, string_dst);
	EXPECT_STREQ(string_md5.c_str(), nbase::BinaryToHexString(string_dst).c_str());
}

TEST(EncryptTest, MD2)
{
	std::string string_ori = "I love this product";
	std::string string_md2 = "03a4fe1c2321204cbd2f029fbdb451da";
	std::string string_dst;

	nbase::EncryptInterface_var encrypt  = new nbase::Encrypt_Impl();
	encrypt->SetMethod(nbase::ENC_MD2);
	encrypt->Encrypt(string_ori, string_dst);
	EXPECT_STREQ(string_md2.c_str(), nbase::BinaryToHexString(string_dst).c_str());
}

TEST(EncryptTest, MD4)
{
	std::string string_ori = "I love this product";
	std::string string_md4 = "29a35e7a7373deaf252269f679293749";
	std::string string_dst;

	nbase::EncryptInterface_var encrypt  = new nbase::Encrypt_Impl();
	encrypt->SetMethod(nbase::ENC_MD4);
	encrypt->Encrypt(string_ori, string_dst);
	EXPECT_STREQ(string_md4.c_str(), nbase::BinaryToHexString(string_dst).c_str());
}

TEST(EncryptTest, SHA)
{
	std::string string_ori = "I love this product";
	std::string string_sha = "2241bf2032e8218a293412adcbf08aaa4389f69c";
	std::string string_dst;

	nbase::EncryptInterface_var encrypt  = new nbase::Encrypt_Impl();
	encrypt->SetMethod(nbase::ENC_SHA);
	encrypt->Encrypt(string_ori, string_dst);
	EXPECT_STREQ(string_sha.c_str(), nbase::BinaryToHexString(string_dst).c_str());
}

TEST(EncryptTest, SHA1)
{
	std::string string_ori = "I love this product";
	std::string string_sha1 = "8e5458e9dd8b31bbc32cdb4ab4e9de7f86452a8f";
	std::string string_dst;

	nbase::EncryptInterface_var encrypt  = new nbase::Encrypt_Impl();
	encrypt->SetMethod(nbase::ENC_SHA1);
	encrypt->Encrypt(string_ori, string_dst);
	EXPECT_STREQ(string_sha1.c_str(), nbase::BinaryToHexString(string_dst).c_str());
}

TEST(EncryptTest, DSS)
{
	std::string string_ori = "I love this product";
	std::string string_dss = "8e5458e9dd8b31bbc32cdb4ab4e9de7f86452a8f";
	std::string string_dst;

	nbase::EncryptInterface_var encrypt  = new nbase::Encrypt_Impl();
	encrypt->SetMethod(nbase::ENC_DSS);
	encrypt->Encrypt(string_ori, string_dst);
	EXPECT_STREQ(string_dss.c_str(), nbase::BinaryToHexString(string_dst).c_str());
}

TEST(EncryptTest, DSS1)
{
	std::string string_ori = "I love this product";
	std::string string_dss1 = "8e5458e9dd8b31bbc32cdb4ab4e9de7f86452a8f";
	std::string string_dst;

	nbase::EncryptInterface_var encrypt  = new nbase::Encrypt_Impl();
	encrypt->SetMethod(nbase::ENC_DSS1);
	encrypt->Encrypt(string_ori, string_dst);
	EXPECT_STREQ(string_dss1.c_str(), nbase::BinaryToHexString(string_dst).c_str());
}

TEST(EncryptTest, RC2)
{
	std::string string_ori = "I love this product";
	std::string string_key = "skyruan";
	std::string string_rc2 = "c968e2752079b6edbf2d63e9752c25328aa637f8d563ddb2";
	std::string enc_dst;
	std::string dec_dst;

	nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
	encrypt_enc->SetMethod(nbase::ENC_RC2);
	encrypt_enc->SetEncryptKey(string_key);
	encrypt_enc->Encrypt(string_ori, enc_dst);
	EXPECT_STREQ(string_rc2.c_str(), nbase::BinaryToHexString(enc_dst).c_str());

	nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
	encrypt_dec->SetMethod(nbase::ENC_RC2);
	encrypt_dec->SetDecryptKey(string_key);
	encrypt_dec->Decrypt(enc_dst, dec_dst);
	EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
}

TEST(EncryptTest, CAST)
{
	std::string string_ori = "I love this product";
	std::string string_key = "skyruan";
	std::string string_cast = "b55a914a751af57fd99984a10b6f52c90c879755f6bd2040";
	std::string enc_dst;
	std::string dec_dst;

	nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
	encrypt_enc->SetMethod(nbase::ENC_CAST);
	encrypt_enc->SetEncryptKey(string_key);
	encrypt_enc->Encrypt(string_ori, enc_dst);
	EXPECT_STREQ(string_cast.c_str(), nbase::BinaryToHexString(enc_dst).c_str());

	nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
	encrypt_dec->SetMethod(nbase::ENC_CAST);
	encrypt_dec->SetDecryptKey(string_key);
	encrypt_dec->Decrypt(enc_dst, dec_dst);
	EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
}

TEST(EncryptTest, AES128)
{
	std::string string_ori = "I love this product";
	std::string string_key = "skyruan";
	std::string string_cast = "1f3413f82b60a2d2b7e69093787cca8275a3e3b409a6a18f9d4f416a41d32fdb";
	std::string enc_dst;
	std::string dec_dst;

	nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
	encrypt_enc->SetMethod(nbase::ENC_AES128);
	encrypt_enc->SetEncryptKey(string_key);
	encrypt_enc->Encrypt(string_ori, enc_dst);
	EXPECT_STREQ(string_cast.c_str(), nbase::BinaryToHexString(enc_dst).c_str());

	nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
	encrypt_dec->SetMethod(nbase::ENC_AES128);
	encrypt_dec->SetDecryptKey(string_key);
	encrypt_dec->Decrypt(enc_dst, dec_dst);
	EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
}

TEST(EncryptTest, AES192)
{
	std::string string_ori = "I love this product";
	std::string string_key = "skyruan";
	std::string string_aes = "1115f75334d099d93ccc6b7eb18b02792f4924093fd30817229b795f4635530c";
	std::string enc_dst;
	std::string dec_dst;

	nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
	encrypt_enc->SetMethod(nbase::ENC_AES192);
	encrypt_enc->SetEncryptKey(string_key);
	encrypt_enc->Encrypt(string_ori, enc_dst);
	EXPECT_STREQ(string_aes.c_str(), nbase::BinaryToHexString(enc_dst).c_str());

	nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
	encrypt_dec->SetMethod(nbase::ENC_AES192);
	encrypt_dec->SetDecryptKey(string_key);
	encrypt_dec->Decrypt(enc_dst, dec_dst);
	EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
}

TEST(EncryptTest, AES256)
{
	std::string string_ori = "I love this product";
	std::string string_key = "skyruan";
	std::string string_aes = "ef8ae9fff271dd0328e292c069ffd6d7cd7cb4aa9c6cd13d8a369c7c622e15cc";
	std::string enc_dst;
	std::string dec_dst;

	nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
	encrypt_enc->SetMethod(nbase::ENC_AES256);
	encrypt_enc->SetEncryptKey(string_key);
	encrypt_enc->Encrypt(string_ori, enc_dst);
	EXPECT_STREQ(string_aes.c_str(), nbase::BinaryToHexString(enc_dst).c_str());

	nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
	encrypt_dec->SetMethod(nbase::ENC_AES256);
	encrypt_dec->SetDecryptKey(string_key);
	encrypt_dec->Decrypt(enc_dst, dec_dst);
	EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
}
TEST(EncryptTest, DES64)
{
	std::string string_ori = "I love this product";
	std::string string_key = "skyruan";
	std::string string_des = "043f58be9970d4f60a1705bb34646b879710d4fb144cd865";
	std::string enc_dst;
	std::string dec_dst;

	nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
	encrypt_enc->SetMethod(nbase::ENC_DES64);
	encrypt_enc->SetEncryptKey(string_key);
	encrypt_enc->Encrypt(string_ori, enc_dst);
	EXPECT_STREQ(string_des.c_str(), nbase::BinaryToHexString(enc_dst).c_str());

	nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
	encrypt_dec->SetMethod(nbase::ENC_DES64);
	encrypt_dec->SetDecryptKey(string_key);
	encrypt_dec->Decrypt(enc_dst, dec_dst);
	EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
}

TEST(EncryptTest, ARC4)
{
	std::string string_ori = "I love this product";
	std::string string_key = "skyruan";
	std::string string_rc4 = "45e58f9cd88516f8dbcc4358ac86935adc5030";
	std::string enc_dst;
	std::string dec_dst;

	nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
	encrypt_enc->SetMethod(nbase::ENC_ARC4);
	encrypt_enc->SetEncryptKey(string_key);
	encrypt_enc->Encrypt(string_ori, enc_dst);
	EXPECT_STREQ(string_rc4.c_str(), nbase::BinaryToHexString(enc_dst).c_str());

	nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
	encrypt_dec->SetMethod(nbase::ENC_ARC4);
	encrypt_dec->SetDecryptKey(string_key);
	encrypt_dec->Decrypt(enc_dst, dec_dst);
	EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
}

TEST(EncryptTest, RSA)
{	
	std::string string_ori = "I love this product";
	std::string enc_dst;
	std::string dec_dst;

    for (int padding = RSA_PKCS1_PADDING; padding <= RSA_X931_PADDING; ++padding) 
    {
        nbase::EncryptInterface_var encrypt_enc  = new nbase::Encrypt_Impl();
        encrypt_enc->SetMethod(nbase::ENC_RSA);
        std::string key_private;
        std::string key_public;
        encrypt_enc->CreateKey(key_public, key_private);
        
        encrypt_enc->SetPaddingMode(padding);
        encrypt_enc->SetEncryptKey(key_public);
        encrypt_enc->Encrypt(string_ori, enc_dst);
        
        nbase::EncryptInterface_var encrypt_dec  = new nbase::Encrypt_Impl();
        encrypt_dec->SetMethod(nbase::ENC_RSA);
        encrypt_dec->SetPaddingMode(padding);
        encrypt_dec->SetDecryptKey(key_private);
        encrypt_dec->Decrypt(enc_dst, dec_dst);
        EXPECT_STREQ(string_ori.c_str(), dec_dst.c_str());
    }
}

#endif // WITH_ENCRYPT

#endif  // WITH_UNITTEST
