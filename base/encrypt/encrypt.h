// Encrypt/Decrypt Method Interface

#ifndef  BASE_ENCRYPT_ENCRYPT_H_
#define  BASE_ENCRYPT_ENCRYPT_H_

#if defined(WITH_ENCRYPT)

#include <string>
#include <list>
#include <map>
#include <memory>

namespace nbase
{
// enum the encrypt algorithm
enum EncryptMethod
{
    ENC_NO            =     0,
    /*    stream */
    ENC_ARC4          =     1,
    //    block
    ENC_RC2           =     10,
    ENC_RC5           =     11,    // TODO
    ENC_IDEA          =     12,    // TODO
    ENC_CAST          =     13,
    ENC_AES128        =     14,
    ENC_AES192        =     15,
    ENC_AES256        =     16,
    ENC_DES64         =     17,
	ENC_DES128        =     18,    // TODO
	ENC_AES128_CBC = 19,
	ENC_AES256_CBC = 20,

    //    hash
    ENC_MD2           =     100,
    ENC_MD4           =     101,
    ENC_MD5           =     102,
    ENC_SHA           =     103,
    ENC_SHA1          =     104,
    ENC_DSS           =     105,
    ENC_DSS1          =     106,

    //    keyless

    //    Asymmetric algorithm
    ENC_RSA           =     300,
    //    custom
    ENC_USR           =     1000
};

// interface of encrypt algorithm
class EncryptMethodInterface
{
public:
	virtual bool SetEncryptKey(const std::string &key) = 0;
	virtual bool SetDecryptKey(const std::string &key) = 0;
	virtual bool SetEncryptIvParameterSpec(const std::string &iv) = 0;
	virtual bool SetDecryptIvParameterSpec(const std::string &iv) = 0;
	virtual bool EnableEncryptPadding(bool enable,int value) = 0;
	virtual bool EnableDecryptPadding(bool enable, int value) = 0;
	virtual nbase::EncryptMethod Type() const = 0;
	virtual bool Encrypt(std::string &data) = 0;
	virtual bool Decrypt(std::string &data) = 0;
	virtual bool Encrypt(const std::string &sdata, std::string &ddata) = 0;
	virtual bool Decrypt(const std::string &sdata, std::string &ddata) = 0;
	virtual bool Encrypt(const void *sdata, size_t ssize, std::string &ddata) = 0;
	virtual bool Decrypt(const void *sdata, size_t ssize, std::string &ddata) = 0;
	virtual bool CreateKey(std::string &key1, std::string &key2) = 0 ;
    virtual bool SetPaddingMode(int padding_mode) = 0;
};
typedef std::shared_ptr<EncryptMethodInterface>  EncryptMethodInterface_var;
typedef EncryptMethodInterface*                       EncryptMethodInterface_ptr;

//    
class EncryptInterface : public nbase::EncryptMethodInterface
{
public:
	virtual bool SetMethod(nbase::EncryptMethod type) = 0;
	virtual void ListSupportMethods(std::list<nbase::EncryptMethod> &types) = 0;
	virtual bool AddMethod(nbase::EncryptMethodInterface_var method) = 0;
	virtual bool RemoveMethod(nbase::EncryptMethod type) = 0;
	virtual nbase::EncryptMethodInterface* CreateMethod(nbase::EncryptMethod type) = 0;
};
typedef std::shared_ptr<EncryptInterface>        EncryptInterface_var;
typedef EncryptInterface*                             EncryptInterface_ptr;

////////////////////////////////////////////////////////////////
//    simple encrypt instead of cleartext
inline nbase::EncryptMethod cm()
{
    return nbase::ENC_NO;  // test
}
inline const std::string& cek()
{
	// 32 bytes block
    static std::string k;
    // static std::string k = "581afc6ecc2e692539ad70893c9cb991";
    return k;
}
inline const std::string & cdk()
{
	// 32 bytes block
    static std::string k;
    //static std::string k = "581afc6ecc2e692539ad70893c9cb991";
    return k;
}
}

#endif // WITH_ENCRYPT
#endif  // BASE_ENCRYPT_ENCRYPT_H_

