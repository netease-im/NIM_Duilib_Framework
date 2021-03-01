#pragma once
#ifndef yxDESH
#define yxDESH
#include <string>
//support CBC ECB mode,you can  padding  PKCS5 or NOPKCS by yuzj 2010 5 10
#define NOPKCS 0 //默认
#define PKCS5 1


#define ECB_MODE 0 //默认
#define CBC_MODE 1

class yxDES
{
public:
	static std::string Encrypt(const std::string &src, const char* key, int iMode = ECB_MODE,int iPKCS = PKCS5);
	static std::string Decrypt(const std::string &src, const char* key, int iMode = ECB_MODE,int iPKCS = PKCS5);

private:
	//类构造函数
	yxDES(int length=8192); 

	//类析构函数
	~yxDES(); 

	//功能:设置加解密和填充模式，如果用户不调用默认模式都是0
	//参数:整型
	//结果:函数将结果存于int m_iMode和int m_iPkcs
	void SetModeAndPKCS(int iMode = 0,int iPKCS = 0);

	//功能:设置IV,默认为{0,0,0,0,0,0,0,0}
	//参数:8位字符串
	//结果:函数将结果存于char szvi[8]和char szviRev[8]
	void SetIV(char* srcBytes);

	//功能:产生16个28位的key
	//参数:源8位的字符串(key),存放key的序号0-1
	//结果:函数将调用private CreateSubKey将结果存于char SubKeys[keyN][16][48]
	void InitializeKey(const char* srcBytes,unsigned int keyN);

	//功能:加密8位字符串
	//参数:8位字符串,使用Key的序号0-1
	//结果:函数将加密后结果存放于private szCiphertext[16]
	//      用户通过属性Ciphertext得到
	void EncryptData(char* _srcBytes,unsigned int keyN);

	//功能:解密16位十六进制字符串
	//参数:16位十六进制字符串,使用Key的序号0-1
	//结果:函数将解密候结果存放于private szPlaintext[8]
	//      用户通过属性Plaintext得到
	void DecryptData(char* _srcBytes,unsigned int keyN);

	//功能:加密任意长度字符串
	//参数:任意长度字符串,长度,使用Key的序号0-1
	//结果:函数将加密后结果存放于private szFCiphertextAnyLength[8192]
	//      用户通过属性CiphertextAnyLength得到
	void EncryptAnyLength(char* _srcBytes,unsigned int _bytesLength,unsigned int keyN);

	//功能:解密任意长度十六进制字符串
	//参数:任意长度字符串,长度,使用Key的序号0-1
	//结果:函数将加密后结果存放于private szFPlaintextAnyLength[8192]
	//      用户通过属性PlaintextAnyLength得到
	void DecryptAnyLength(char* _srcBytes,unsigned int _bytesLength, unsigned int keyN);

	//功能:Bytes到Bits的转换,
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Bytes2Bits(const char *srcBytes, char* dstBits, unsigned int sizeBits);

	//功能:Bits到Bytes的转换,
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Bits2Bytes(char *dstBytes, char* srcBits, unsigned int sizeBits);

	//功能:Int到Bits的转换,
	//参数:待变换字符串,处理后结果存放缓冲区指针
	void Int2Bits(unsigned int srcByte, char* dstBits);

	//功能:Bits到Hex的转换
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Bits2Hex(char *dstHex, char* srcBits, unsigned int sizeBits);

	//功能:Bits到Hex的转换
	//参数:待变换字符串,处理后结果存放缓冲区指针,Bits缓冲区大小
	void Hex2Bits(char *srcHex, char* dstBits, unsigned int sizeBits);

	//szCiphertextInBinary的get函数
	char* GetCiphertextInBinary();

	//szCiphertextInHex的get函数
	char* GetCiphertextInHex();

	//Ciphertext的get函数
	char* GetCiphertextInBytes();

	//Plaintext的get函数
	char* GetPlaintext();

	//CiphertextAnyLength的get函数
	char* GetCiphertextAnyLength();

	//PlaintextAnyLength的get函数
	char* GetPlaintextAnyLength();

	//字符串转16进制密文文本
	void ConvertCiphertext2Hex(char *szPlainInBytes);

	//16进制密文转字符串
	int ConvertHex2Ciphertext(const char *szCipherInBytes);

	//CiphertextData函数
	char* GetCiphertextData();

	//hexCiphertextAnyLength函数
	char* GetHexCipherAnyLengthData();



private:
	int  m_iLength;
	char szSubKeys[2][16][48];//储存2个16组48位密钥,第2个用于3DES
	char szCiphertextRaw[64]; //储存二进制密文(64个Bits) int 0,1
	char szPlaintextRaw[64]; //储存二进制密文(64个Bits) int 0,1
	char szCiphertextInBytes[8];//储存8位密文
	char szPlaintextInBytes[8];//储存8位明文字符串

	char szCiphertextInBinary[65]; //储存二进制密文(64个Bits) char '0','1',最后一位存'\0'
	char szCiphertextInHex[17]; //储存十六进制密文,最后一位存'\0'
	char szPlaintext[9];//储存8位明文字符串,最后一位存'\0'

	char* szFCiphertextAnyLength;//任意长度密文
	char* szFPlaintextAnyLength;//任意长度明文字符串

	char* szCiphertextData;
	char* hexCiphertextAnyLength;

	char sziv[8];//加密IV
	char szivRev[8];//解密IV	

	int m_iMode;//加解密模式
	int m_iPkcs;//填充模式

	int data_base_length_;//用于编解码需要的字符缓存长度



	//功能:生成子密钥
	//参数:经过PC1变换的56位二进制字符串,生成的szSubKeys编号0-1
	//结果:将保存于char szSubKeys[16][48]
	void CreateSubKey(char* sz_56key,unsigned int keyN);

	//功能:DES中的F函数,
	//参数:左32位,右32位,key序号(0-15),使用的szSubKeys编号0-1
	//结果:均在变换左右32位
	void FunctionF(char* sz_Li,char* sz_Ri,unsigned int iKey,unsigned int keyN);

	//功能:IP变换
	//参数:待处理字符串,处理后结果存放指针
	//结果:函数改变第二个参数的内容
	void InitialPermuteData(char* _src,char* _dst);

	//功能:将右32位进行扩展位48位,
	//参数:原32位字符串,扩展后结果存放指针
	//结果:函数改变第二个参数的内容
	void ExpansionR(char* _src,char* _dst);

	//功能:异或函数,
	//参数:待异或的操作字符串1,字符串2,操作数长度,处理后结果存放指针
	//结果: 函数改变第四个参数的内容
	void XOR(char* szParam1,char* szParam2, unsigned int uiParamLength, char* szReturnValueBuffer);

	//功能:S-BOX , 数据压缩,
	//参数:48位二进制字符串,
	//结果:返回结果:32位字符串
	void CompressFuncS(char* _src48, char* _dst32);

	//功能:IP逆变换,
	//参数:待变换字符串,处理后结果存放指针
	//结果:函数改变第二个参数的内容
	void PermutationP(char* _src,char* _dst);

};

#endif
 