// Implementation of Encrypt/Decrypt Method Interface

#ifndef BASE_ENCRYPT_ENCRYPT_IMPL_H_
#define BASE_ENCRYPT_ENCRYPT_IMPL_H_

#if defined(WITH_ENCRYPT)

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include "encrypt.h"
#include "base/base_config.h"
#include "base/base_export.h"
#include "base/util/string_util.h"
#include <assert.h>
#if defined(OS_WIN)
#include <objbase.h>
#endif

namespace nbase
{

template<nbase::EncryptMethod T>
class Dummy_Encrypt : public nbase::EncryptMethodInterface
{
public:
	bool  SetEncryptKey(const std::string &key) { return true; }
	bool  SetDecryptKey(const std::string &key) { return true; }
	bool SetEncryptIvParameterSpec(const std::string &iv) { return true; };
	bool SetDecryptIvParameterSpec(const std::string &iv) { return true; };
	bool EnableEncryptPadding(bool enable, int value) { return true; };
	bool EnableDecryptPadding(bool enable, int value) { return true; };
	nbase::EncryptMethod Type() const { return T; }
	bool  Encrypt(std::string &data) { return true; }
	bool  Decrypt(std::string &data) { return true; }
	bool  Encrypt(const std::string &sdata, std::string &ddata)
	{
		ddata = sdata;
		return true;
	}
	bool  Decrypt(const std::string &sdata, std::string &ddata)
	{
		ddata = sdata;
		return true;
	}
	bool  Encrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		ddata.assign((const char *)sdata,ssize);
		return true;
	}
	bool  Decrypt(const void *sdata,size_t ssize, std::string &ddata)
	{
		ddata.assign((const char *)sdata,ssize);
		return true;
	}
	bool  CreateKey(std::string &key1, std::string &key2)
	{
		key1 = "aaa";
		key2 = "aaa";
		return true;
	}
    bool  SetPaddingMode(int padding_mode)
    {
        return false;
    }
};

static void Md5(const unsigned char *sdata, size_t ssize, std::string &ddata)
{
	const EVP_MD *md = EVP_md5();
	if (!md)
		return;
	EVP_MD_CTX mdctx;
	EVP_MD_CTX_init(&mdctx);
	if (!EVP_DigestInit_ex(&mdctx, md, NULL))
	{
		EVP_MD_CTX_cleanup(&mdctx);
		return;
	}
	if (!EVP_DigestUpdate(&mdctx, (const unsigned char *)sdata, ssize))
	{
		EVP_MD_CTX_cleanup(&mdctx);
		return;
	}
	unsigned char     obuf[16];
	unsigned int      olen;

	if (!EVP_DigestFinal_ex(&mdctx, obuf, &olen))
	{
		EVP_MD_CTX_cleanup(&mdctx);
		return;
	}
	ddata.assign((const char *)obuf,16);

	EVP_MD_CTX_cleanup(&mdctx);
}

// 1. if key.size()<16 , key = md5(key) first
// 2. if ksize > key.size , expand key with hex
// 3. cut key for key.size=ksize
static void ExpandKey(uint32_t ksize, std::string &key)
{
	if (key.size() < 16)
	{
		std::string k;
		k.assign(key.data(), key.size());
		Md5((const unsigned char *)k.data(), k.size(), key);
	}
	if (ksize > 16)
	{
		while (key.size() < ksize)
			key = nbase::BinaryToHexString(key);
	}
	if (key.size() > ksize)
		key.erase(ksize,key.size() - ksize);
}

#if defined(OS_WIN)     
static bool CreateGUID(std::string &guid)
{
	GUID  id;
	if (::CoCreateGuid(&id) == S_OK)
	{
		char buf[16];
		memcpy(buf, &id, 16);
		guid.assign((const char *)buf, 16);
		nbase::BinaryToHexString(guid);
		//    =>md5
		const EVP_MD * md = EVP_md5();
		if ( !md )  
			return false;
		EVP_MD_CTX mdctx;
		EVP_MD_CTX_init(&mdctx);
		if (!EVP_DigestInit_ex(&mdctx, md, NULL))
		{
			EVP_MD_CTX_cleanup(&mdctx);
			return false;
		}
		if (!EVP_DigestUpdate(&mdctx, (const unsigned char *)guid.data(), guid.size()))
		{
			EVP_MD_CTX_cleanup(&mdctx);
			return false;
		}
		unsigned char obuf[16];
		unsigned int  olen;

		if (!EVP_DigestFinal_ex(&mdctx, obuf, &olen))
		{
			EVP_MD_CTX_cleanup(&mdctx);
			return false;
		}
		guid.assign((const char *)obuf, 16);
		EVP_MD_CTX_cleanup(&mdctx);
		return true;
	}
	return false;
}
#endif

template<nbase::EncryptMethod T>
class Openssl_Encrypt_Symmetry_Key : public nbase::EncryptMethodInterface
{
public:
	Openssl_Encrypt_Symmetry_Key() :enable_padding_(false), padding_value_(0), iv_parameter_spec_(""){ chiper_ = chiper(); }
	~Openssl_Encrypt_Symmetry_Key(){    
	}
	bool  SetEncryptKey(const std::string &key)
	{
		key_ = key; 
		ExpandKey(chiper_->key_len, key_);
		return true;
	}
	bool  SetDecryptKey(const std::string &key)
	{
		key_ = key; 
		ExpandKey(chiper_->key_len, key_);
		return true;
	}
	bool SetEncryptIvParameterSpec(const std::string &iv)
	{
		iv_parameter_spec_ = iv;
		return true;
	}
	bool SetDecryptIvParameterSpec(const std::string &iv)
	{
		iv_parameter_spec_ = iv;
		return true;
	}
	bool EnableEncryptPadding(bool enable, int value)
	{ 
		enable_padding_ = enable;
		padding_value_ = (value == 0 ? 0 : 1);
		return true; 
	};
	bool EnableDecryptPadding(bool enable, int value)
	{ 
		enable_padding_ = enable;
		padding_value_ = (value == 0 ? 0 : 1);
		return true; 
	};
	nbase::EncryptMethod Type() const { return T; }
	bool  Encrypt(std::string &data)
	{
		std::string src = data;
		return Encrypt(src.data(), src.size(), data);
	}
	bool  Decrypt(std::string &data)
	{
		std::string src = data;
		return Decrypt(src.data(), src.size(), data);
	}
	bool  Encrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		if (!chiper_)   
			return false;
		if (!ddata.empty())
			ddata.erase();
		EVP_CIPHER_CTX    ctx;
		if (!EVP_EncryptInit(&ctx, chiper_, (const unsigned char*)key_.c_str(), (iv_parameter_spec_.empty() ? nullptr : (const unsigned char*)iv_parameter_spec_.c_str())))
			return false;
		unsigned char out[256];
		if(enable_padding_)
			EVP_CIPHER_CTX_set_padding(&ctx, padding_value_);
		uint32_t      osize = sizeof(out);
		// must ensure that the buffer size is count of block size
		if (chiper_->block_size > 0)
			osize = (sizeof(out)/chiper_->block_size) * chiper_->block_size;
		uint32_t block = ((uint32_t)ssize+osize-1) / osize;

		uint32_t elen  = 0;
		int      lout;
		for (uint32_t i = 0; i < block; i++)
		{
			int lin = ((uint32_t)ssize - elen)>osize ? osize : (uint32_t)ssize - elen;
			if (!EVP_EncryptUpdate(&ctx, out, &lout, ((const unsigned char*)sdata) + elen, lin))
			{
				EVP_CIPHER_CTX_cleanup(&ctx);
				return false;
			}
			assert(lout <= (int)sizeof(out));
			//elen += lout;   //    lin?
			elen += lin;
			ddata.append((const char *)out, lout);
		}
		//lout = sizeof(out);
		if (!EVP_EncryptFinal(&ctx, out, &lout))
		{
			EVP_CIPHER_CTX_cleanup(&ctx);
			return false;
		}
		//elen += lout;   //    lin?
		ddata.append((const char *)out, lout);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return true;
	}
	bool  Decrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		if (!chiper_)
			return false;
		if (!ddata.empty())
			ddata.erase();

		EVP_CIPHER_CTX ctx;
		if (!EVP_DecryptInit(&ctx, chiper_, (const unsigned char*)key_.c_str(), (iv_parameter_spec_.empty() ? nullptr : (const unsigned char*)iv_parameter_spec_.c_str())))
			return false;
		if(enable_padding_)
			EVP_CIPHER_CTX_set_padding(&ctx, padding_value_);
		unsigned char out[256];
		uint32_t      osize = sizeof(out);    //    must >= inl+cipher_block_size
		//    is there some bug?
		//    must ensure that the buffer size is count of block size
		if (chiper_->block_size > 0)
			osize = (sizeof(out)/chiper_->block_size-1) * chiper_->block_size;
		uint32_t block = ((uint32_t)ssize + osize - 1) / osize;

		uint32_t elen  = 0;
		int      lout;
		for (uint32_t i = 0; i < block; i++)
		{
			int lin = (uint32_t)ssize - elen>osize ? osize : ((uint32_t)ssize - elen);
			if (!EVP_DecryptUpdate(&ctx, out, &lout, ((const unsigned char*)sdata) + elen, lin))
			{
				EVP_CIPHER_CTX_cleanup(&ctx);
				return false;
			}
			elen += lin;
			ddata.append((const char *)out, lout);
		}
		if (!EVP_DecryptFinal(&ctx,out, &lout))
		{
			EVP_CIPHER_CTX_cleanup(&ctx);
			return false;
		}
		ddata.append((const char *)out, lout);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return true;
	}
	bool  Encrypt(const std::string &sdata, std::string &ddata)
	{
		return Encrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  Decrypt(const std::string &sdata, std::string &ddata)
	{
		return Decrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  CreateKey(std::string &key1, std::string &key2)
	{
		// 16 bytes
		for (;;)
		{
			char  buf[20];
#if defined(OS_WIN) && defined(COMPILER_MSVC)
			sprintf_s(buf, sizeof(buf), "%X%X",(unsigned int)time(0), (unsigned int)rand());
#else
			sprintf(buf,"%X%X", (unsigned int)time(0), (unsigned int)rand());
#endif
			int len = (int)strlen(buf);
			RAND_add(buf, len, len >> 1);
			if (!RAND_status())   
				continue;
			unsigned char randstr[32];
			RAND_bytes(randstr,32);
			Md5(randstr,32,key1);
			break;
		}
		key2.assign(key1);
		return true;
	}
    
    bool  SetPaddingMode(int padding_mode)
    {
        return false;
    }

private:
	const EVP_CIPHER *chiper()
	{
		switch(T)
		{
		case nbase::ENC_RC2:
			return EVP_rc2_ecb();
		//case nbase::RC5:
		//    return EVP_rc5_32_12_16_ecb();
		//case nbase::IDEA:
		//    return EVP_idea_ecb();
		case nbase::ENC_CAST:
			return EVP_cast5_ecb();
		case nbase::ENC_AES128:
			return EVP_aes_128_ecb();
		case nbase::ENC_AES192:
			return EVP_aes_192_ecb();
		case nbase::ENC_AES256:
			return EVP_aes_256_ecb();
		case nbase::ENC_AES256_CBC:
			return EVP_aes_256_cbc();
		case nbase::ENC_AES128_CBC:
			return EVP_aes_128_cbc();
		case nbase::ENC_DES64:
			return EVP_des_ecb();
		}
		return 0;
	}

private:
	const EVP_CIPHER       *chiper_;
	std::string             key_;
	bool						enable_padding_;
	int						padding_value_;
	std::string			  iv_parameter_spec_;
};

template<nbase::EncryptMethod T>
class Openssl_Encrypt_Hash : public nbase::EncryptMethodInterface
{
public:
	Openssl_Encrypt_Hash()  { md_ = md(); }
	~Openssl_Encrypt_Hash() {      
	}
	bool  SetEncryptKey(const std::string &key)
	{
		return true;
	}
	bool  SetDecryptKey(const std::string &key)
	{
		return true;
	}
	bool SetEncryptIvParameterSpec(const std::string &iv)
	{
		return true;
	}
	bool SetDecryptIvParameterSpec(const std::string &iv)
	{
		return true;
	}
	bool EnableEncryptPadding(bool enable, int value)
	{
		return true;
	};
	bool EnableDecryptPadding(bool enable, int value)
	{
		return true;
	};
	nbase::EncryptMethod Type() const { return T; }
	bool  Encrypt(std::string &data)
	{
		std::string src = data;
		return Encrypt(src.data(), src.size(), data);
	}
	bool  Decrypt(std::string &data)
	{
		std::string src = data;
		return Decrypt(src.data(), src.size(), data);
	}
	bool  Encrypt(const std::string &sdata, std::string &ddata)
	{
		return Encrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  Decrypt(const std::string &sdata, std::string &ddata)
	{
		return Decrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  Encrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		if (!md_)
			return false;
		if (!ddata.empty())
			ddata.erase();

		EVP_MD_CTX mdctx;
		EVP_MD_CTX_init(&mdctx);
		if (!EVP_DigestInit_ex(&mdctx, md_, NULL))
		{
			EVP_MD_CTX_cleanup(&mdctx);
			return false;
		}
		if (!EVP_DigestUpdate(&mdctx, (const unsigned char *)sdata, ssize))
		{
			EVP_MD_CTX_cleanup(&mdctx);
			return false;
		}
		unsigned char  obuf[1024];
		unsigned char *pbuf = obuf;
		unsigned int   olen;

		olen = EVP_MD_size(md_);
		if (olen > sizeof(obuf))
			pbuf = new unsigned char [olen];

		if (!EVP_DigestFinal_ex(&mdctx, pbuf, &olen))
		{
			if ( pbuf != obuf )
				delete [] pbuf;
			EVP_MD_CTX_cleanup(&mdctx);
			return false;
		}
		ddata.append((const char *)pbuf, olen);
		if (pbuf != obuf)
			delete [] pbuf;
		EVP_MD_CTX_cleanup(&mdctx);
		return true;
	}
	bool  Decrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		return false;
	}
	bool  CreateKey(std::string &key1, std::string &key2)
	{
		return false;
	}
    bool SetPaddingMode(int padding_mode)
    {
        return false;
    }

private:
	const EVP_MD* md()
	{
		switch(T)
		{
		case nbase::ENC_MD5:
			return EVP_md5();
		case nbase::ENC_MD2:
		{
			assert(0);
			return 0;
			//return EVP_md2();
		}
		case nbase::ENC_MD4:
			return EVP_md4();
		case nbase::ENC_SHA:
			return EVP_sha();
		case nbase::ENC_SHA1:
			return EVP_sha1();
		case nbase::ENC_DSS:
			return EVP_dss();
		case nbase::ENC_DSS1:
			return EVP_dss1();
		default:
			return 0;
		}
	}

private:
	const EVP_MD      *md_;
};

// only allow public key encrypt/private key decrypt
class IEncryptRSAImpl : public nbase::EncryptMethodInterface
{
public:
	IEncryptRSAImpl() : n_(0),e_(0),d_(0),padding_mode_(RSA_PKCS1_PADDING){}
	virtual ~IEncryptRSAImpl()
	{
		if (n_)
			BN_free(n_);
		if (e_)
			BN_free(e_);
		if (d_)
			BN_free(d_);
	}

	//    export methods
	nbase::EncryptMethod Type() const { return nbase::ENC_RSA; }
	bool  Encrypt(std::string &data)
	{
		std::string tmp;
		tmp.assign(data.data(),data.size());
		return Encrypt(tmp, data);
	}
	bool  Decrypt(std::string &data)
	{
		std::string tmp;
		tmp.assign(data.data(),data.size());
		return Decrypt(tmp, data);
	}
	bool  Encrypt(const std::string &sdata, std::string &ddata)
	{
		return Encrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  Decrypt(const std::string &sdata,std::string &ddata)
	{
		return Decrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  Encrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		if (!n_ || !e_ || !sdata || ssize==0)
			return false;
		if (!ddata.empty())
			ddata.erase();
		unsigned char buf[256];
		RSA          *rsa = RSA_new();
		int           ret = -1;
		try
		{
			rsa->n = n_;
			rsa->e = e_;
            uint32_t block_size = RSA_size(rsa);
            switch (padding_mode_) 
            {
                case RSA_PKCS1_PADDING:
                    block_size = RSA_size(rsa) - RSA_PKCS1_PADDING_SIZE;
                    break;
                case RSA_SSLV23_PADDING:
                    block_size = RSA_size(rsa);
                    break;
                case RSA_NO_PADDING:
                    block_size = RSA_size(rsa);
                    break;
                case RSA_PKCS1_OAEP_PADDING:
                    block_size = RSA_size(rsa) - 41;
                    break;
                case RSA_X931_PADDING:
                    block_size = RSA_size(rsa);
                    break;
                default:
                    break;
            }
			uint32_t nBlock    = ( (uint32_t)ssize + block_size -1 ) / block_size;
			const unsigned char * psrc = (const unsigned char *)sdata;
			for (uint32_t i = 0; i < nBlock; i++)
			{
				uint32_t sz = block_size;
				if ((i+1) == nBlock && (ssize%block_size!=0) 
                    && (RSA_PKCS1_PADDING == padding_mode_ || RSA_PKCS1_OAEP_PADDING == padding_mode_))
					sz = ssize % block_size;
				ret = RSA_public_encrypt(sz, psrc ,buf, rsa, padding_mode_);
				if (ret > 0)
				{
					ddata.append((const char *)buf, ret);
					psrc += sz;
				}
				else
					break;
			}
		}
		catch(...)
		{
		}
		rsa->n = 0;       
		rsa->e = 0;
		RSA_free(rsa);
		return ret>=0 ? true : false;
	}

	bool  Decrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		if (!n_ || !e_ || !d_ || !sdata || ssize==0)
			return false;
		if (!ddata.empty())
			ddata.erase();
        
		unsigned char buf[256];
		RSA          *rsa = RSA_new();
		int           ret = -1;
		try
		{
			rsa->n = n_;
			rsa->d = d_;      
			rsa->e = e_;
            uint32_t block_size = RSA_size(rsa);
			uint32_t nBlock    = ( (uint32_t)ssize + block_size -1 ) / block_size;
			const unsigned char * psrc = (const unsigned char *)sdata;
			for (uint32_t i = 0; i < nBlock; i++)
			{
				ret = RSA_private_decrypt(block_size, psrc, buf, rsa, padding_mode_);
				if (ret>0)
				{
					ddata.append((const char *)buf, ret);
					psrc += block_size;
				}
				else  
					break;
			}
		}
		catch(...)
		{
		}
		rsa->n = 0;       
		rsa->d = 0; 
		rsa->e = 0;
		RSA_free(rsa);
		return ret>=0 ? true : false;
	}
    bool  SetPaddingMode(int padding_mode)
    {
        padding_mode_ = padding_mode;
        return true;
    }

private:
	BIGNUM *    n_;              // public modulus
	BIGNUM *    e_;              // public exponent
	BIGNUM *    d_;              // private exponent
    int         padding_mode_;   // padding mode
};


// stream encode
// stream encode have state. When call setEncryptKey()/setDecryptKey() the encrypt and decrypt pipe will be reseted.
// So that we allow user call the function setEncryptKey()/setDecryptKey() only once time
template<nbase::EncryptMethod T>
class Openssl_Encrypt_Stream : public nbase::EncryptMethodInterface
{
public:
	Openssl_Encrypt_Stream() : ectx_valid_(false), dctx_valid_(false), iv_parameter_spec_(""),enable_padding_(false), padding_value_(0)
	{     
		chiper_ = chiper();
	}
	~Openssl_Encrypt_Stream()
	{    
		if (ectx_valid_)      
			EVP_CIPHER_CTX_cleanup(&ectx_);
		if (dctx_valid_)      
			EVP_CIPHER_CTX_cleanup(&dctx_);
	}
	bool  SetEncryptKey(const std::string &key)
	{
		key_ = key; 
		ExpandKey(chiper_->key_len,key_);

		if (ectx_valid_)
		{
			EVP_CIPHER_CTX_cleanup(&ectx_);
			ectx_valid_ = true;
		}
		if (!EVP_EncryptInit(&ectx_, chiper_, (const unsigned char*)key_.c_str(), (iv_parameter_spec_.empty() ? nullptr : (const unsigned char*)iv_parameter_spec_.c_str())))
			return false;
		ectx_valid_ = true;

		return true;
	}
	bool  SetDecryptKey(const std::string &key)
	{
		key_ = key; 
		ExpandKey(chiper_->key_len, key_);

		if (dctx_valid_)
		{
			EVP_CIPHER_CTX_cleanup(&dctx_);
			dctx_valid_ = false;
		}
		if (!EVP_EncryptInit(&dctx_, chiper_, (const unsigned char*)key_.c_str(), (iv_parameter_spec_.empty() ? nullptr : (const unsigned char*)iv_parameter_spec_.c_str())))
			return false;
		dctx_valid_ = true;

		return true;
	}
	bool SetEncryptIvParameterSpec(const std::string &iv)
	{
		iv_parameter_spec_ = iv;
		return true;
	}
	bool SetDecryptIvParameterSpec(const std::string &iv)
	{
		iv_parameter_spec_ = iv;
		return true;
	}
	bool EnableEncryptPadding(bool enable, int value)
	{
		enable_padding_ = enable;
		padding_value_ = (value == 0 ? 0 : 1);
		return true;
	};
	bool EnableDecryptPadding(bool enable, int value)
	{
		enable_padding_ = enable;
		padding_value_ = (value == 0 ? 0 : 1);
		return true;
	};
	nbase::EncryptMethod Type() const { return T; }

	size_t      EncodeSize(size_t size) { return size; }
	size_t      DecodeSize(size_t size) { return size; }
	size_t      Encrypt(const char *ibuf, size_t isize, char *obuf, size_t obsize)
	{
		assert(ibuf);
		assert(obuf);
		if (isize==0)
			return 0;
		assert(obsize >= EncodeSize(isize));
		if (!chiper_)
			return 0;
		if (!ectx_valid_)
			return 0;

		int lout;
		if (!EVP_EncryptUpdate(&ectx_, (unsigned char *)obuf, &lout, (const unsigned char *)ibuf, (int)isize))
		{
			assert(false);
			return 0;
		}
		assert(isize>=(size_t)lout);
		size_t osize = lout;
		if (!EVP_EncryptFinal(&ectx_, (unsigned char *)&obuf[lout], &lout))
		{
			assert(false);
			return 0;
		}
		osize += (size_t)lout;
		assert(osize == EncodeSize(isize));
		return osize;
	}
	size_t      Decrypt(const char *ibuf, size_t isize, char *obuf, size_t obsize)
	{
		assert(ibuf);
		assert(obuf);
		if (isize == 0)
			return 0;
		assert(obsize>=DecodeSize(isize));
		if (!chiper_)
			return 0;
		if (!dctx_valid_)
			return 0;

		int lout;
		if (!EVP_DecryptUpdate(&dctx_, (unsigned char *)obuf, &lout, (const unsigned char *)ibuf, (int)isize))
		{
			assert(false);
			return 0;
		}
		assert(isize>=(size_t)lout);
		size_t osize = (size_t)lout;
		if (!EVP_DecryptFinal(&dctx_, (unsigned char *)&obuf[lout], &lout))
		{
			assert(false);
			return 0;
		}
		osize += (size_t)lout;
		return osize;
	}
	bool  Encrypt(std::string &data)
	{
		return Encrypt(data.data(), data.size(), data);
	}
	bool  Decrypt(std::string &data)
	{
		return Decrypt(data.data(), data.size(), data);
	}
	bool  Encrypt(const void *sdata, size_t isize, std::string &ddata)
	{
		size_t osize = EncodeSize(isize);
		if (osize > ddata.size())
			ddata.resize(osize);
		char  *ptr = (char *)&ddata[0];
		size_t sz  = Encrypt((const char *)sdata, isize, ptr, osize);
		assert(sz == osize);
		if (ddata.size() > osize)
			ddata.resize(osize);
		return true;
	}
	bool  Decrypt(const void *sdata, size_t isize, std::string &ddata)
	{
		size_t osize = DecodeSize(isize);
		if ( osize > ddata.size() )
			ddata.resize(osize); 
		char  *ptr = (char *)&ddata[0];
		size_t sz  = Decrypt((const char *)sdata, isize, ptr, osize);
		assert(sz == osize);
		if (ddata.size() > osize)
			ddata.resize(osize);
		return true;
	}
	bool  Encrypt(const std::string &sdata, std::string &ddata)
	{
		return Encrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  Decrypt(const std::string &sdata, std::string &ddata)
	{
		return Decrypt(sdata.data(), sdata.size(), ddata);
	}
	bool  CreateKey(std::string &key1, std::string &key2)
	{
		//    16 bytes
		for (;;)
		{
			char  buf[20];
#if defined(OS_WIN) && defined(COMPILER_MSVC)
			sprintf_s(buf, sizeof(buf), "%X%X", (unsigned int)time(0), (unsigned int)rand());
#else
			sprintf(buf, "%X%X", (unsigned int)time(0), (unsigned int)rand());
#endif
			int len = (int)strlen(buf);
			RAND_add(buf,len,len >> 1);
			if (!RAND_status())
				continue;
			unsigned char randstr[32];
			RAND_bytes(randstr, 32);
			Md5(randstr, 32, key1);
			break;
		}
		key2.assign(key1);
		return true;
	}
    bool  SetPaddingMode(int padding_mode)
    {
        return false;
    }

private:
	const EVP_CIPHER* chiper()
	{
		if (T==nbase::ENC_ARC4)
			return EVP_rc4(); 
		return 0;
	}

private:
	const EVP_CIPHER           *chiper_;
	std::string                 key_;
	bool						enable_padding_;
	int						padding_value_;
	std::string			  iv_parameter_spec_;
	EVP_CIPHER_CTX              ectx_;            // encode (have state)
	bool                        ectx_valid_;      // if true , will EVP_CIPHER_CTX_cleanup
	EVP_CIPHER_CTX              dctx_;            // decode (have state)
	bool                        dctx_valid_;      // if true , will EVP_CIPHER_CTX_cleanup
};


///////////////////////////////////////////////////////////////////////////////
class BASE_EXPORT Encrypt_Impl : public nbase::EncryptInterface
{
public:
	Encrypt_Impl()
	{
		support_methods_[nbase::ENC_NO]     = 1;
		support_methods_[nbase::ENC_RSA]    = 1;
		support_methods_[nbase::ENC_ARC4]   = 1;
		//support_methods_[nbase::ENC_RC5]  = 1;
		support_methods_[nbase::ENC_RC2]    = 1;
		//support_methods_[nbase::ENC_IDEA] = 1;        
		support_methods_[nbase::ENC_CAST]   = 1;
		support_methods_[nbase::ENC_AES128] = 1;
		support_methods_[nbase::ENC_AES128_CBC] = 1;
		support_methods_[nbase::ENC_AES192] = 1;
		support_methods_[nbase::ENC_AES256] = 1;
		support_methods_[nbase::ENC_AES256_CBC] = 1;
		support_methods_[nbase::ENC_DES64]  = 1;
		support_methods_[nbase::ENC_MD2]    = 1;
		support_methods_[nbase::ENC_MD4]    = 1;
		support_methods_[nbase::ENC_MD5]    = 1;
		support_methods_[nbase::ENC_SHA]    = 1;
		support_methods_[nbase::ENC_SHA1]   = 1;
		support_methods_[nbase::ENC_DSS]    = 1;
		support_methods_[nbase::ENC_DSS1]   = 1;
	}
	virtual ~Encrypt_Impl(){}
	//    from iencryptMothod
	bool  SetEncryptKey(const std::string &key)
	{
		if (!method_)
			return false;
		return method_->SetEncryptKey(key);
	}
	bool  SetDecryptKey(const std::string &key)
	{
		if (!method_)
			return false;
		return method_->SetDecryptKey(key);
	}
	bool SetEncryptIvParameterSpec(const std::string &iv)
	{
		if (!method_)
			return false;
		return method_->SetEncryptIvParameterSpec(iv);
	}
	bool SetDecryptIvParameterSpec(const std::string &iv)
	{
		if (!method_)
			return false;
		return method_->SetDecryptIvParameterSpec(iv);
	}
	bool EnableEncryptPadding(bool enable, int value)
	{
		if (!method_)
			return false;
		return method_->EnableEncryptPadding(enable, value);
	};
	bool EnableDecryptPadding(bool enable, int value)
	{
		if (!method_)
			return false;
		return method_->EnableDecryptPadding(enable,value);;
	};
	nbase::EncryptMethod Type() const
	{
		if (!method_)
			return nbase::ENC_NO;
		return method_->Type();
	}
	bool  Encrypt(std::string &data)
	{
		if (!method_)
			return false;
		return method_->Encrypt(data);
	}
	bool  Decrypt(std::string &data)
	{
		if (!method_)
			return false;
		return method_->Decrypt(data);
	}
	bool  Encrypt(const std::string &sdata, std::string &ddata)
	{
		if (!method_)
			return false;
		return method_->Encrypt(sdata, ddata);
	}
	bool  Decrypt(const std::string &sdata, std::string &ddata)
	{
		if (!method_)
			return false;
		return method_->Decrypt(sdata, ddata);
	}
	bool  Encrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		if (!method_)
			return false;
		return method_->Encrypt(sdata, ssize, ddata);
	}
	bool  Decrypt(const void *sdata, size_t ssize, std::string &ddata)
	{
		if (!method_)
			return false;
		return method_->Decrypt(sdata, ssize, ddata);
	}
	bool  CreateKey(std::string &key1, std::string &key2)
	{
		if (!method_)
			return false;
		return method_->CreateKey(key1, key2);
	}
    bool  SetPaddingMode(int padding_mode)
    {
        if (!method_)
            return false;
        return method_->SetPaddingMode(padding_mode);
    }
	//    from iencrypt
	bool  SetMethod(nbase::EncryptMethod type)
	{
		MethodMap::iterator it = methods_.find(type);
		if (it == methods_.end())
		{
			nbase::EncryptMethodInterface_var e(CreateMethod(type));
			if (!e)
				return false;
			method_ = e;
			AddMethod(e);
		}
		else
			method_ = it->second;
		return true;
	}
	void  ListSupportMethods(std::list<nbase::EncryptMethod> &types)
	{
		MethodMap::iterator it = methods_.begin();
		for (; it != methods_.end(); it++)
			types.push_back(it->first);
	}
	bool  AddMethod(nbase::EncryptMethodInterface_var method) override
	{
		if (!method)
			return false;
		nbase::EncryptMethodInterface_var e = method;
		MethodMap::iterator it = methods_.find(e->Type());
		if (it!=methods_.end())
		{
			//    update ?
			methods_[e->Type()] = e;
		}
		else
		{
			methods_[e->Type()] = e;
			//support_methods_[e->type()] = 1;
		}
		return true;
	}
	bool  RemoveMethod(nbase::EncryptMethod type)
	{
		MethodMap::iterator it = methods_.find(type);
		if (it != methods_.end())
			methods_.erase(it);
		return true;
	}
	nbase::EncryptMethodInterface* CreateMethod(nbase::EncryptMethod type)
	{
		if (support_methods_.find(type) == support_methods_.end())
			return 0;
		switch(type)
		{
		case nbase::ENC_NO:
			return new Dummy_Encrypt<nbase::ENC_NO>;

		case nbase::ENC_RSA:
		{
			assert(false);	//暂时不支持
			return nullptr;
		}
		case nbase::ENC_ARC4:
			return new Openssl_Encrypt_Stream<nbase::ENC_ARC4>;
			//case nbase::RC5:
			//    return new Openssl_Encrypt_Symmetry_Key<nbase::RC5>;
		case nbase::ENC_RC2:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_RC2>;
			//case nbase::IDEA:
			//    return new Openssl_Encrypt_Symmetry_Key<nbase::IDEA>;
		case nbase::ENC_CAST:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_CAST>;
		case nbase::ENC_AES128:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_AES128>;
		case nbase::ENC_AES128_CBC:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_AES128_CBC>;
		case nbase::ENC_AES192:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_AES192>;
		case nbase::ENC_AES256:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_AES256>;
		case nbase::ENC_AES256_CBC:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_AES256_CBC>;
		case nbase::ENC_DES64:
			return new Openssl_Encrypt_Symmetry_Key<nbase::ENC_DES64>;
		case nbase::ENC_MD2:
		{
			assert(0);
			return 0;
			//return new Openssl_Encrypt_Hash<nbase::ENC_MD2>;
		}
		case nbase::ENC_MD4:
			return new Openssl_Encrypt_Hash<nbase::ENC_MD4>;
		case nbase::ENC_MD5:
			return new Openssl_Encrypt_Hash<nbase::ENC_MD5>;
		case nbase::ENC_SHA:
			return new Openssl_Encrypt_Hash<nbase::ENC_SHA>;
		case nbase::ENC_SHA1:
			return new Openssl_Encrypt_Hash<nbase::ENC_SHA1>;
		case nbase::ENC_DSS:
			return new Openssl_Encrypt_Hash<nbase::ENC_DSS>;
		case nbase::ENC_DSS1:
			return new Openssl_Encrypt_Hash<nbase::ENC_DSS1>;
		default :
			return 0;
		}
	}

private:
	typedef std::map<nbase::EncryptMethod, nbase::EncryptMethodInterface_var>  MethodMap;
	MethodMap                                 methods_;
	std::map<nbase::EncryptMethod, uint32_t>  support_methods_;
	nbase::EncryptMethodInterface_var         method_;
};

} // namespace nbase

#endif // WITH_ENCRYPT
#endif // BASE_ENCRYPT_ENCRYPT_IMPL_H_
