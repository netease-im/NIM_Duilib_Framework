#include "des.h"
#include "base/base_types.h"
#include "base/util/string_util.h"

// permuted choice table (PC1)
const static char PC1_Table[56] = {
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};
// permuted choice key (PC2)
const static char PC2_Table[48] = {
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};
// number left rotations of pc1 
const static char Shift_Table[16] = {
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
// initial permutation (IP)
const static char IP_Table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};
// expansion operation matrix (E)
const static char E_Table[48] = {
	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};
// The (in)famous S-boxes 
const static char S_Box[8][4][16] = {
	// S1
	14,  4,	13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	// S2 
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	// S3 
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	// S4 
	7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
	// S5 
	2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
	// S6 
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
	// S7 
	4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
	// S8 
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};
// 32-bit permutation function P used on the output of the S-boxes 
const static char P_Table[32] = {
	16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
	2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};
// final permutation IP^-1 
const static char IPR_Table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};
std::string yxDES::Decrypt(const std::string &src, const char* key, int iMode,int iPKCS)
{
	std::string des;

	yxDES de_des((int)src.size());
	de_des.SetModeAndPKCS(iMode,iPKCS);

	de_des.InitializeKey(key,0);
	de_des.DecryptAnyLength((char*)src.c_str(), (int)src.size(), 0);

	des = de_des.GetPlaintextAnyLength();
	return des;
}
std::string yxDES::Encrypt(const std::string &src, const char* key, int iMode,int iPKCS)
{
	std::string des;
	yxDES en_des((int)src.size());
	en_des.SetModeAndPKCS(iMode,iPKCS);

	en_des.InitializeKey(key,0);
	char* src_ch = new char[src.size()];
	memcpy(src_ch, src.c_str(), sizeof(char) * src.size());
	en_des.EncryptAnyLength(src_ch,(int)src.size(),0);
	des.append(en_des.GetCiphertextAnyLength(), en_des.m_iLength);
	delete[] src_ch;
	return des;
}

yxDES::yxDES(int length)
{
	data_base_length_ = length*2+16;
	szFCiphertextAnyLength = new char[data_base_length_];
	szFPlaintextAnyLength = new char[data_base_length_];
	szCiphertextData = new char[data_base_length_];
	hexCiphertextAnyLength = new char[data_base_length_*2];
	memset(szCiphertextRaw,0,64);
	memset(szPlaintextRaw,0,64);
	memset(szCiphertextInBytes,0,8);
	memset(szPlaintextInBytes,0,8);
	memset(szCiphertextInBinary,0,65);
	memset(szCiphertextInHex,0,17);
	memset(szPlaintext,0,9);
	memset(szFCiphertextAnyLength,0,data_base_length_);
	memset(szFPlaintextAnyLength,0,data_base_length_);
	memset(szCiphertextData,0,data_base_length_);
	memset(hexCiphertextAnyLength,0,data_base_length_*2);
	memset(sziv,0x0,8);
	for(int i = 0;i < 8;i++)
	{
		sziv[i] = 0;
		szivRev[i] = 0;
	}
	m_iMode = 0;
	m_iPkcs = 0;
}
yxDES::~yxDES()
{
	delete[] szFCiphertextAnyLength;
	delete[] szFPlaintextAnyLength;
	delete[] szCiphertextData;
	delete[] hexCiphertextAnyLength;
}

void yxDES::InitializeKey(const char* srcBytes,unsigned int keyN)
{
	//convert 8 char-bytes key to 64 binary-bits
	char sz_64key[64] = {0};
	Bytes2Bits(srcBytes,sz_64key,64);
	//PC 1
	char sz_56key[56] = {0};
	for(int k=0;k<56;k++)
	{
		sz_56key[k] = sz_64key[PC1_Table[k]-1];
	}
	CreateSubKey(sz_56key,keyN);
}

void yxDES::CreateSubKey(char* sz_56key,unsigned int keyN)
{
	char szTmpL[28] = {0};
	char szTmpR[28] = {0};
	char szCi[28] = {0};
	char szDi[28] = {0};
	memcpy(szTmpL,sz_56key,28);
	memcpy(szTmpR,sz_56key + 28,28);

	for(int i=0;i<16;i++)
	{
		//shift to left
		//Left 28 bits
		memcpy(szCi,szTmpL + Shift_Table[i],28 - Shift_Table[i]);
		memcpy(szCi + 28 - Shift_Table[i],szTmpL,Shift_Table[i]);
		//Right 28 bits
		memcpy(szDi,szTmpR + Shift_Table[i],28 - Shift_Table[i]);
		memcpy(szDi + 28 - Shift_Table[i],szTmpR,Shift_Table[i]);

		//permuted choice 48 bits key
		char szTmp56[56] = {0};
		memcpy(szTmp56,szCi,28);
		memcpy(szTmp56 + 28,szDi,28);
		for(int j=0;j<48;j++)
		{
			szSubKeys[keyN][i][j] = szTmp56[PC2_Table[j]-1];
		}
		//Evaluate new szTmpL and szTmpR
		memcpy(szTmpL,szCi,28);
		memcpy(szTmpR,szDi,28);
	}
}

void yxDES::EncryptData(char* _srcBytes,unsigned int keyN)
{
	char temp[8];
	memset(temp,0x0,8);
	if(1 == m_iMode)//cbc
	{
		XOR(_srcBytes,sziv,8,temp);
		memcpy(_srcBytes,temp,8);
	}
	
	char szSrcBits[64] = {0};
	char sz_IP[64] = {0};
	char sz_Li[32] = {0};
	char sz_Ri[32] = {0};
	char sz_Final64[64] = {0};

	Bytes2Bits(_srcBytes,szSrcBits,64);
	//IP
	InitialPermuteData(szSrcBits,sz_IP);
	memcpy(sz_Li,sz_IP,32);
	memcpy(sz_Ri,sz_IP + 32,32);

	for(int i=0;i<16;i++)
	{
		FunctionF(sz_Li,sz_Ri,i,keyN);
	}
	//so D=LR

	memcpy(sz_Final64,sz_Ri,32);
	memcpy(sz_Final64 + 32,sz_Li,32);

	//~IP
	for(int j=0;j<64;j++)
	{
		szCiphertextRaw[j] = sz_Final64[IPR_Table[j]-1];
	}
	Bits2Bytes(szCiphertextInBytes,szCiphertextRaw,64);
	if(1 == m_iMode)//cbc
	{
		memcpy(sziv,szCiphertextInBytes,8);
	}
}

void yxDES::DecryptData(char* _srcBytes,unsigned int keyN)
{
	char temp[8];
	char tempszivRev[8];
	memset(temp,0x0,8);
	memset(tempszivRev,0x0,8);
	if(1 == m_iMode)//cbc
	{
		memcpy(tempszivRev,_srcBytes,8);
	}
	
	char szSrcBits[64] = {0};
	char sz_IP[64] = {0};
	char sz_Li[32] = {0};
	char sz_Ri[32] = {0};
	char sz_Final64[64] = {0};
	Bytes2Bits(_srcBytes,szSrcBits,64);
	//IP --- return is sz_IP
	InitialPermuteData(szSrcBits,sz_IP);
	//divide the 64 bits data to two parts
	memcpy(sz_Ri,sz_IP,32); //exchange L to R
	memcpy(sz_Li,sz_IP + 32,32);  //exchange R to L
	//16 rounds F and xor and exchange
	for(int i=0;i<16;i++)
	{
		FunctionF(sz_Ri,sz_Li,15-i,keyN);
	}
	memcpy(sz_Final64,sz_Li,32);
	memcpy(sz_Final64 + 32,sz_Ri,32);
	// ~IP
	for(int j=0;j<64;j++)
	{
		szPlaintextRaw[j] = sz_Final64[IPR_Table[j]-1];
	}
	Bits2Bytes(szPlaintextInBytes,szPlaintextRaw,64);
	if(1 == m_iMode)//cbc
	{
		XOR(szPlaintextInBytes,szivRev,8,temp);
		memcpy(szPlaintextInBytes,temp,8);
		memcpy(szivRev,tempszivRev,8);
	}
	
}

void yxDES::FunctionF(char* sz_Li,char* sz_Ri,unsigned int iKey,unsigned int keyN)
{
	char sz_48R[48] = {0};
	char sz_xor48[48] = {0};
	char sz_P32[32] = {0};
	char sz_Rii[32] = {0};
	char sz_Key[48] = {0};
	char s_Compress32[32] = {0};
	memcpy(sz_Key,szSubKeys[keyN][iKey],48);
	ExpansionR(sz_Ri,sz_48R);
	XOR(sz_48R,sz_Key,48,sz_xor48);

	CompressFuncS(sz_xor48,s_Compress32);
	PermutationP(s_Compress32,sz_P32);
	XOR(sz_P32,sz_Li,32,sz_Rii);
	memcpy(sz_Li,sz_Ri,32);
	memcpy(sz_Ri,sz_Rii,32);
}

void yxDES::InitialPermuteData(char* _src,char* _dst)
{
	//IP
	for(int i=0;i<64;i++)
	{
		_dst[i] = _src[IP_Table[i]-1];
	}
}

void yxDES::ExpansionR(char* _src,char* _dst)
{
	for(int i=0;i<48;i++)
	{
		_dst[i] = _src[E_Table[i]-1];
	}
}

void yxDES::XOR(char* szParam1,char* szParam2, unsigned int uiParamLength, char* szReturnValueBuffer)
{
	for(unsigned int i=0; i<uiParamLength; i++)
	{
		szReturnValueBuffer[i] = szParam1[i] ^ szParam2[i];
	}
}

void yxDES::CompressFuncS(char* _src48, char* _dst32)
{
	char bTemp[8][6]={0};
	char dstBits[4]={0};
	for(int i=0;i<8;i++)
	{
		memcpy(bTemp[i],_src48+i*6,6);
		int iX = (bTemp[i][0])*2 + (bTemp[i][5]);
		int iY = 0;
		for(int j=1;j<5;j++)
		{
			iY += bTemp[i][j]<<(4-j);
		}
		Int2Bits(S_Box[i][iX][iY], dstBits);
		memcpy(_dst32 + i * 4, dstBits, 4);
	}

}

void yxDES::PermutationP(char* _src,char* _dst)
{
	for(int i=0;i<32;i++)
	{
		_dst[i] = _src[P_Table[i]-1];
	}
}

void yxDES::Bytes2Bits(const char *srcBytes, char* dstBits, unsigned int sizeBits)
{
	for(unsigned int i=0; i < sizeBits; i++)
		dstBits[i] = ((srcBytes[i>>3]<<(i&7)) & 128)>>7;
}

void yxDES::Bits2Bytes(char *dstBytes, char* srcBits, unsigned int sizeBits)
{
	memset(dstBytes,0,sizeBits>>3);
	for(unsigned int i=0; i < sizeBits; i++)
		dstBytes[i>>3] |= (srcBits[i] << (7 - (i & 7)));
}

void yxDES::Int2Bits(unsigned int _src, char* dstBits)
{
	for(unsigned int i=0; i < 4; i++)
		dstBits[i] = ((_src<<i) & 8)>>3;
}

void yxDES::Bits2Hex(char *dstHex, char* srcBits, unsigned int sizeBits)
{
	memset(dstHex,0,sizeBits>>2);
	for(unsigned int i=0; i < sizeBits; i++) //convert to int 0-15
		dstHex[i>>2] += (srcBits[i] << (3 - (i & 3)));
	for(unsigned int j=0;j < (sizeBits>>2);j++)
		dstHex[j] += dstHex[j] > 9 ? 55 : 48; //convert to char '0'-'F'
}

void yxDES::Hex2Bits(char *srcHex, char* dstBits, unsigned int sizeBits)
{
	memset(dstBits,0,sizeBits);
	for(unsigned int i=0;i < (sizeBits>>2);i++)
		srcHex[i] -= srcHex[i] > 64 ? 55 : 48; //convert to char int 0-15
	for(unsigned int j=0; j < sizeBits; j++) 
		dstBits[j] = ((srcHex[j>>2]<<(j&3)) & 15) >> 3;

}

char* yxDES::GetCiphertextInBinary()
{
	for(unsigned int i=0;i<64;i++)
	{
		szCiphertextInBinary[i] = szCiphertextRaw[i] + 48; // from int(0) to char('0') and int1 to char('1')
	}
	szCiphertextInBinary[64] = '\0';
	return szCiphertextInBinary;
}

char* yxDES::GetCiphertextInHex()
{
	Bits2Hex(szCiphertextInHex,szCiphertextRaw,64);
	szCiphertextInHex[16] = '\0';
	return szCiphertextInHex;
}

char* yxDES::GetCiphertextInBytes()
{
	return szCiphertextInBytes;
}

char* yxDES::GetPlaintext()
{
	memcpy(szPlaintext,szPlaintextInBytes,8);
	szPlaintext[8] = '\0';
	return szPlaintext;
}

char* yxDES::GetCiphertextAnyLength()
{
	return szFCiphertextAnyLength;
}
char* yxDES::GetPlaintextAnyLength()
{
	if(1 == m_iPkcs)//cbcm_iMode
	{
		int iP = 0;
		int i = (int)strlen(szFPlaintextAnyLength);

		while(iP < i)
		{
			char a = szFPlaintextAnyLength[iP];
			int ipos = (int)a;
			if(ipos>=1 && ipos <=8)
			{
				char* temp = NULL;
				temp = (char*)malloc(iP);
				if (temp)
				{
					memset(temp,0x0,iP);
					memcpy(temp,szFPlaintextAnyLength,iP);
					memset(szFPlaintextAnyLength,0x0,data_base_length_);
					memcpy(szFPlaintextAnyLength,temp,iP);
					free(temp);
				}
				break;
			}
			iP++;
		}
	}
	return szFPlaintextAnyLength;
}
//char* yxDES::GetPlaintextAnyLength()
//{
//	if(1 == m_iMode)//cbc
//	{
//		int i = strlen(szFPlaintextAnyLength);
//		char a = szFPlaintextAnyLength[i-1];
//		int ipos = (int)a;
//		if(ipos>=1 && ipos <=8)
//		{
//			char* temp = NULL;
//			temp = (char*)malloc(i-ipos);
//			memset(temp,0x0,i-ipos);
//			memcpy(temp,szFPlaintextAnyLength,i-ipos);
//			memset(szFPlaintextAnyLength,0x0,data_base_length_);
//			memcpy(szFPlaintextAnyLength,temp,i-ipos);
//			free(temp);
//		}
//	}
//	return szFPlaintextAnyLength;
//}

void yxDES::EncryptAnyLength(char* _srcBytes,unsigned int _bytesLength,unsigned int keyN)
{
	m_iLength = _bytesLength;
	if(_bytesLength == 8)
	{
		char _temp8bytes[8] = {0};
		EncryptData(_srcBytes,keyN);
		memcpy(szFCiphertextAnyLength,szCiphertextInBytes,8);

		//PKCS5
		if(1 == m_iPkcs)
		{
			m_iLength = _bytesLength*2;
			memset(_temp8bytes,8,8);
			EncryptData(_temp8bytes,keyN);
			memcpy(szFCiphertextAnyLength+8,szCiphertextInBytes,8);
			szFCiphertextAnyLength[16] = '\0';
		}
		else
		{
			szFCiphertextAnyLength[8] = '\0';
		}
		//end
	}
	else if(_bytesLength < 8)
	{
		int iLeave = 8 - _bytesLength;
		char _temp8bytes[8] = {0};
		memcpy(_temp8bytes,_srcBytes,_bytesLength);
		//PKCS5
		if(1 == m_iPkcs)
		{
			for(unsigned int i = 0;i < 8;i++)
			{
				if(i >= _bytesLength)
				{
					_temp8bytes[i] = iLeave;
				}
			}
		}
		m_iLength += iLeave;
		//end
		EncryptData(_temp8bytes,keyN);
		memcpy(szFCiphertextAnyLength,szCiphertextInBytes,8);
		szFCiphertextAnyLength[8] = '\0';
	}
	else if(_bytesLength > 8)
	{

		int iParts = _bytesLength>>3;
		int iResidue = _bytesLength % 8;
		int iLeave = 8 - iResidue;

		char szLast8Bits[8] = {0};
		for(int i=0;i<iParts;i++)
		{
			memcpy(szLast8Bits,_srcBytes + (i<<3),8);
			EncryptData(szLast8Bits,keyN);
			memcpy(szFCiphertextAnyLength + (i<<3),szCiphertextInBytes,8);
		}
		memset(szLast8Bits,0,8);
		memcpy(szLast8Bits,_srcBytes + (iParts<<3),iResidue);
		//PKCS5
		if(1 == m_iPkcs)
		{
			for(int i = 0;i < 8;i++)
			{
				if(i >= iResidue)
				{
					szLast8Bits[i] = iLeave;
				}
			}
		}
		m_iLength += iLeave;
		//end
		EncryptData(szLast8Bits,keyN);
		memcpy(szFCiphertextAnyLength + (iParts<<3),szCiphertextInBytes,8);
		szFCiphertextAnyLength[((iParts+1)<<3)] = '\0';
	}
}

void yxDES::DecryptAnyLength(char* _srcBytes,unsigned int _bytesLength, unsigned int keyN)
{
	
	if(_bytesLength == 8)
	{
		DecryptData(_srcBytes,keyN);
		memcpy(szFPlaintextAnyLength,szPlaintextInBytes,8);
		szFPlaintextAnyLength[8] = '\0';
	}
	else if(_bytesLength < 8)
	{
		char _temp8bytes[8] = {0};
		memcpy(_temp8bytes,_srcBytes,8);
		DecryptData(_temp8bytes,keyN);
		memcpy(szFPlaintextAnyLength,szPlaintextInBytes,_bytesLength);
		szFPlaintextAnyLength[_bytesLength] = '\0';
	}
	else if(_bytesLength > 8)
	{
		int iParts = _bytesLength>>3;
		int iResidue = _bytesLength % 8;
		char szLast8Bits[8] = {0};
		for(int i=0;i<iParts;i++)
		{
			memcpy(szLast8Bits,_srcBytes + (i<<3),8);
			DecryptData(szLast8Bits,keyN);
			memcpy(szFPlaintextAnyLength + (i<<3),szPlaintextInBytes,8);
		}
		if(iResidue != 0)
		{
			memset(szLast8Bits,0,8);
			memcpy(szLast8Bits,_srcBytes + (iParts<<3),8);
			DecryptData(szLast8Bits,keyN);
			memcpy(szFPlaintextAnyLength + (iParts<<3),szPlaintextInBytes,iResidue);
		}
		szFPlaintextAnyLength[_bytesLength] = '\0';
	}
}

//字符串转16进制密文文本
void yxDES::ConvertCiphertext2Hex(char *szPlainInBytes)
{
	if (!szPlainInBytes)
	{
		return;
	}
	//int iBitsLen = strlen(szPlainInBytes) % 8 == 0 ? strlen(szPlainInBytes) << 3 : ((strlen(szPlainInBytes)>>3) + 1) << 6;
	int iBitsLen = m_iLength % 8 == 0 ? m_iLength << 3 : ((m_iLength>>3) + 1) << 6;
	char* bitsCiphertextAnyLength = new char[data_base_length_*4];
	memset(bitsCiphertextAnyLength,0,data_base_length_*4);
	Bytes2Bits(szFCiphertextAnyLength,bitsCiphertextAnyLength,iBitsLen);
	Bits2Hex(hexCiphertextAnyLength,bitsCiphertextAnyLength,iBitsLen);
	delete[] bitsCiphertextAnyLength;
}

//16进制密文转字符串
int yxDES::ConvertHex2Ciphertext(const char *szCipherInBytes)
{
	char* bitsTmpCiphertextAnyLength = new char[data_base_length_*4];
	char* hexTmpCiphertextAnyLength = new char[data_base_length_*2];

	int iLen = 0;

	memset(szCiphertextData,0,data_base_length_);
	memset(hexTmpCiphertextAnyLength,0,data_base_length_*2);
	

	iLen = (((int)strlen(szCipherInBytes)>>2) + ((int)strlen(szCipherInBytes) % 4 == 0 ? 0 : 1))<<4;
	
	memcpy(hexTmpCiphertextAnyLength,szCipherInBytes,strlen(szCipherInBytes));
	Hex2Bits(hexTmpCiphertextAnyLength,bitsTmpCiphertextAnyLength,iLen);
	Bits2Bytes(szCiphertextData,bitsTmpCiphertextAnyLength,iLen);

	delete[] bitsTmpCiphertextAnyLength;
	delete[] hexTmpCiphertextAnyLength;

	return iLen >>3;
}

//CiphertextData函数
char* yxDES::GetCiphertextData()
{
	return szCiphertextData;
}

//hexCiphertextAnyLength函数
char* yxDES::GetHexCipherAnyLengthData()
{
	return hexCiphertextAnyLength;
}

//功能:设置加解密和填充模式，如果用户不调用默认模式都是0
//参数:整型
//结果:函数将结果存于int m_iMode和int m_iPkcs
void yxDES::SetModeAndPKCS(int iMode/* = 0*/,int iPKCS/* = 0*/)
{
	m_iMode = iMode;
	m_iPkcs = iPKCS;
}

//功能:设置
//参数:8位字符串
//结果:函数将结果存于char szvi[8]和char szviRev[8]
void yxDES::SetIV(char* srcBytes)
{
	memset(sziv,0x0,8);
	memset(szivRev,0x0,8);
	memcpy(sziv,srcBytes,8);
	memcpy(szivRev,srcBytes,8);
}