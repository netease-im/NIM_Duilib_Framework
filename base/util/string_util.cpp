/*
 *
 *	Author		wrt(guangguang)
 *	Date		2011-06-08
 *	Copyright	Hangzhou, Netease Inc.
 *	Brief		Utilities for string operation
 *
 */

#include "string_util.h"
#include <stdio.h>			/* for vsnprintf */
#include <assert.h>			/* for assert */
#include <memory.h>			/* for mem*** */
#include <stdarg.h>			/* va_list, va_start, va_end */
#include <ctype.h>
#include "base/macros.h"
#include "base/third_party/convert_utf/ConvertUTF.h"

namespace nbase
{

namespace
{

template<typename CharType>
int StringTokenizeT(const std::basic_string<CharType> &input,
					const std::basic_string<CharType> &delimitor,
					std::list<std::basic_string<CharType> > &output)
{
	size_t token_begin;
	size_t token_end;

	output.clear();

	for (token_begin = token_end = 0; token_end != std::basic_string<CharType>::npos;)
	{
		token_begin = input.find_first_not_of(delimitor, token_begin);
		if (token_begin == std::basic_string<CharType>::npos)
			break;
		token_end = input.find_first_of(delimitor, token_begin + 1);
		output.push_back(input.substr(token_begin, token_end - token_begin));
		token_begin = token_end + 1;
	}

	return static_cast<int>(output.size());
}

template<typename CharType>
size_t StringReplaceAllT(const std::basic_string<CharType> &find,
						 const std::basic_string<CharType> &replace,
						 std::basic_string<CharType> &output)
{
	size_t find_length = find.size();
	size_t replace_length = replace.size();
	size_t offset = 0, endpos;
	size_t target = 0, found_pos;
	size_t replaced = 0;
	CharType *data_ptr;

	if (find.empty() || output.empty())
		return 0;

	/*
	 * to avoid extra memory reallocating,
	 * we use two passes to finish the task in the case that replace.size() is greater find.size()
	 */

	if (find_length < replace_length)
	{
		/* the first pass, count all available 'find' to be replaced  */
		for (;;)
		{
			offset = output.find(find, offset);
			if (offset == std::basic_string<CharType>::npos)
				break;
			replaced++;
			offset += find_length;
		}

		if (replaced == 0)
			return 0;

		size_t newsize = output.size() + replaced * (replace_length - find_length);

		/* we apply for more memory to hold the content to be replaced */
		endpos = newsize;
		offset = newsize - output.size();
		output.resize(newsize);
		data_ptr = &output[0];

		memmove((void*)(data_ptr + offset),
				(void*)data_ptr,
				(output.size() - offset) * sizeof(CharType));
	}
	else
	{
		endpos = output.size();
		offset = 0;
		data_ptr = const_cast<CharType *>(&output[0]);
	}

	/* the second pass,  the replacement */
	while (offset < endpos)
	{
		found_pos = output.find(find, offset);
		if (found_pos != std::basic_string<CharType>::npos)
		{
			/* move the content between two targets */
			if (target != found_pos)
				memmove((void*)(data_ptr + target),
						(void*)(data_ptr + offset),
						(found_pos - offset) * sizeof(CharType));

			target += found_pos - offset;

			/* replace */
			memcpy(data_ptr + target,
				   replace.data(),
				   replace_length * sizeof(CharType));

			target += replace_length;
			offset = find_length + found_pos;
			replaced++;
		}
		else
		{
			/* ending work  */
			if (target != offset)
				memcpy((void*)(data_ptr + target),
					   (void*)(data_ptr + offset),
					   (endpos - offset) * sizeof(CharType));
			break;
		}
	}

	if (replace_length < find_length)
		output.resize(output.size() - replaced * (find_length - replace_length));

	return replaced;
}

inline int vsnprintfT(char *dst, size_t count, const char *format, va_list ap)
{
#if defined(OS_WIN)	
	return _vsnprintf(dst, count, format, ap);
#else	
	return vsnprintf(dst, count, format, ap);
#endif	
}

inline int vsnprintfT(wchar_t *dst, size_t count, const wchar_t *format, va_list ap)
{
#if defined(OS_WIN)	
	return _vsnwprintf(dst, count, format, ap);
#else	
	return vswprintf(dst, count, format, ap);
#endif	
}

template<typename CharType>
void StringAppendVT(const CharType *format, va_list ap, std::basic_string<CharType> &output)
{
	CharType stack_buffer[1024];

	/* first, we try to finish the task using a fixed-size buffer in the stack */
	va_list ap_copy;
	GG_VA_COPY(ap_copy, ap);

	int result = vsnprintfT(stack_buffer, COUNT_OF(stack_buffer), format, ap_copy);
	va_end(ap_copy);
	if (result >= 0 && result < static_cast<int>(COUNT_OF(stack_buffer)))
	{
		/* It fits */
		output.append(stack_buffer, result);
		return;
	}

	/* then, we have to repeatedly increase buffer size until it fits. */
	int buffer_size = COUNT_OF(stack_buffer);
	std::basic_string<CharType> heap_buffer;
	for (;;)
	{
#if !defined(OS_WIN)
		if (result < 0)
		{
			assert(0);
			return; /* not expected, result should be -1 here */
		}
#endif
		buffer_size <<= 1; /* try doubling the buffer size */
		if (buffer_size > 32 * 1024 * 1024)
		{
			assert(0);
			return;	/* too long */
		}
		/* resize */
		heap_buffer.resize(buffer_size);
		/*
		 * NOTE: You can only use a va_list once.  Since we're in a while loop, we
		 * need to make a new copy each time so we don't use up the original.
		 */
		GG_VA_COPY(ap_copy, ap);
		result = vsnprintfT(&heap_buffer[0], buffer_size, format, ap_copy);
		va_end(ap_copy);

		if ((result >= 0) && (result < buffer_size)) {
			/* It fits */
			output.append(&heap_buffer[0], result);
			return;
		}
	}
}

#define NOT_SPACE(x) ((x) != 0x20 && ((x) < 0 || 0x1d < (x)))

template<typename CharType>
void StringTrimT(std::basic_string<CharType> &output)
{
	if (output.empty())
		return;
	size_t bound1 = 0;
	size_t bound2 = output.length();
	const CharType *src = output.data();

	for (; bound2 > 0; bound2--)
		if (NOT_SPACE(src[bound2-1]))
			break;

	for (; bound1 < bound2; bound1++)
		if (NOT_SPACE(src[bound1]))
			break;

	if (bound1 < bound2) {
		memmove((void *)src,
				src + bound1,
				sizeof(CharType) * (bound2 - bound1));
	}

	output.resize(bound2 - bound1);
}

template<typename CharType>
void StringTrimLeftT(std::basic_string<CharType> &output)
{
	size_t check = 0;
	size_t length = output.length();
	const CharType *src = output.data();

	for (; check < length; check++)
		if (NOT_SPACE(src[check]))
			break;

	output.erase(0, check);
}

template<typename CharType>
void StringTrimRightT(std::basic_string<CharType> &output)
{
	size_t length = output.length();
	const CharType *src = output.data();

	for (; length > 0; length--)
		if (NOT_SPACE(src[length-1]))
			break;

	output.resize(length);
}

} // anonymous namespace

std::string StringPrintf(const char *format, ...)
{
	va_list	args;
	va_start(args, format);
	std::string output;
	StringAppendV(output, format, args);
	va_end(args);

	return output;
}

std::wstring StringPrintf(const wchar_t *format, ...)
{
	va_list	args;
	va_start(args, format);
	std::wstring output;
	StringAppendV(output, format, args);
	va_end(args);

	return output;
}

const std::string& StringPrintf(std::string &output, const char *format, ...)
{
	va_list	args;
	va_start(args, format);
	output.clear();
	StringAppendV(output, format, args);
	va_end(args);
	return output;
}

const std::wstring& StringPrintf(std::wstring &output, const wchar_t *format, ...)
{
	va_list	args;
	va_start(args, format);
	output.clear();
	StringAppendV(output, format, args);
	va_end(args);
	return output;
}

void StringPrintfV(std::string &output, const char *format, va_list ap)
{
	output.clear();
	StringAppendV(output, format, ap);
}

void StringPrintfV(std::wstring &output, const wchar_t *format, va_list ap)
{
	output.clear();
	StringAppendV(output, format, ap);
}

void StringAppendF(std::string &output, const char *format, ...)
{
	va_list	args;
	va_start(args, format);
	StringAppendV(output, format, args);
	va_end(args);
}

void StringAppendF(std::wstring &output, const wchar_t *format, ...)
{
	va_list	args;
	va_start(args, format);
	StringAppendV(output, format, args);
	va_end(args);
}

void StringAppendV(std::string &output, const char *format, va_list ap)
{
	StringAppendVT<char>(format, ap, output);
}

void StringAppendV(std::wstring &output, const wchar_t *format, va_list ap)
{
	StringAppendVT<wchar_t>(format, ap, output);
}

std::list<std::string> StringTokenize(const char *input, const char *delimitor)
{
	std::list<std::string> output;
	std::string input2(input);

	if (input2.empty())
		return output;

	char *token = strtok(&input2[0], delimitor);
	while (token != NULL)
	{
		output.push_back(token);
		token = strtok(NULL, delimitor);
	}

	return output;
}

std::list<std::wstring> StringTokenize(const wchar_t *input, const wchar_t *delimitor)
{
	std::list<std::wstring> output;
	std::wstring input2(input);

	if (input2.empty())
		return output;

#if defined(OS_WIN)	
	wchar_t *token = wcstok(&input2[0], delimitor);
	while (token != NULL)
	{
		output.push_back(token);
		token = wcstok(NULL, delimitor);
	}
#else
	wchar_t *ptr;
	wchar_t *token = wcstok(const_cast<wchar_t *>(&input2[0]), delimitor, &ptr);
    while (token != NULL)
	{
		output.push_back(token);
		token = wcstok(NULL, delimitor, &ptr);
	}	
#endif

	return output;
}

int StringTokenize(const std::string& input, const std::string& delimitor, std::list<std::string>& output)
{
	return StringTokenizeT<char>(input, delimitor, output);
}

int StringTokenize(const std::wstring& input, const std::wstring& delimitor, std::list<std::wstring>& output)
{
	return StringTokenizeT<wchar_t>(input, delimitor, output);
}

size_t StringReplaceAll(const std::string& find, const std::string& replace, std::string& output)
{
	return StringReplaceAllT<char>(find, replace, output);
}

size_t StringReplaceAll(const std::wstring& find, const std::wstring& replace, std::wstring& output)
{
	return StringReplaceAllT<wchar_t>(find, replace, output);
}

void LowerString(std::string &str)
{
	if (str.empty())
		return;
	char *start = &str[0];
	char *end = start + str.length();
	for (; start < end; start++)
	{
		if (*start >= 'A' && *start <= 'Z')
			*start += 'a' - 'A';
	}
}

void LowerString(std::wstring &str)
{
	if (str.empty())
		return;
	wchar_t *start = &str[0];
	wchar_t *end = start + str.length();
	for (; start < end; start++)
	{
		if (*start >= L'A' && *start <= L'Z')
			*start += L'a' - L'A';
	}
}

void UpperString(std::string &str)
{
	if (str.empty())
		return;
	char *start = &str[0];
	char *end = start + str.length();
	for (; start < end; start++)
	{
		if (*start >= 'a' && *start <= 'z')
			*start -= 'a' - 'A';
	}
}

void UpperString(std::wstring &str)
{
	if (str.empty())
		return;
	wchar_t *start = &str[0];
	wchar_t *end = start + str.length();
	for (; start < end; start++)
	{
		if (*start >= L'a' && *start <= L'z')
			*start -= L'a' - L'A';
	}
}

std::string MakeLowerString(const std::string &src)
{
    std::string dest(src);
	LowerString(dest);
	return dest;
}

std::wstring MakeLowerString(const std::wstring &src)
{
    std::wstring dest(src);
	LowerString(dest);
	return dest;
}

std::string MakeUpperString(const std::string &src)
{
    std::string dest(src);
	UpperString(dest);
	return dest;
}

std::wstring MakeUpperString(const std::wstring &src)
{
    std::wstring dest(src);
	UpperString(dest);
	return dest;
}

void BinaryToHexString(const void *binary, size_t length, std::string &output)
{
	static const unsigned char kHexChars[] = "0123456789abcdef";

	output.resize(length << 1);
	if (length == 0)
		return;

	unsigned char *dst = reinterpret_cast<unsigned char *>(&output[0]);
	const unsigned char *src = reinterpret_cast<const unsigned char *>(binary);

	for (size_t i = 0; i < length; i++)
	{
		dst[i<<1]   = kHexChars[src[i]>>4];
		dst[(i<<1)+1] = kHexChars[src[i]&0xF];
	}
}

std::string BinaryToHexString(const void *binary, size_t length)
{
	std::string output;
	BinaryToHexString(binary, length, output);
	return output;
}

std::string BinaryToHexString(const std::string &binary)
{
	std::string output;
	BinaryToHexString(binary.c_str(), binary.size(), output);
	return output;
}

int HexStringToBinary(const char *input, size_t length, std::string &output)
{
	output.resize(length >> 1);
	if (length == 0)
		return 0;

	const char *src = input;
	char *dst = &output[0];
	size_t i, size = output.size();

	for (i = 0; i < size; i++)
	{
		char h = HexCharToInt8(src[i<<1]);
		char l = HexCharToInt8(src[(i<<1)+1]);
		if (l == -1 || h == -1)
			break;
		dst[i] = (h<<4)|l;
	}

	// i may be shorter than size when an error occurs
	output.resize(i);

	return (int)i;
}

std::string HexStringToBinary(const std::string &input)
{
	std::string output;
	HexStringToBinary(input.c_str(), input.size(), output);
	return output;
}

char HexCharToInt8(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	assert(0);	// not a hexadecimal char
	return -1;
}

std::wstring UTF8ToUTF16(const UTF8Char *utf8, size_t length)
{
	UTF16Char output[4096];
	const UTF8 *src_begin = reinterpret_cast<const UTF8 *>(utf8);
	const UTF8 *src_end = src_begin + length;
	UTF16 *dst_begin = reinterpret_cast<UTF16 *>(output);

	std::wstring  utf16;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF8toUTF16(&src_begin,
													 src_end,
													 &dst_begin,
													 dst_begin + COUNT_OF(output),
													 lenientConversion);

		utf16.append(output, dst_begin - reinterpret_cast<UTF16 *>(output));
		dst_begin = reinterpret_cast<UTF16 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf16.clear();
			break;
		}
	}

	return utf16;
}

std::string UTF16ToUTF8(const UTF16Char *utf16, size_t length)
{
	UTF8Char output[8192];
	const UTF16 *src_begin = reinterpret_cast<const UTF16 *>(utf16);
	const UTF16 *src_end = src_begin + length;
	UTF8 *dst_begin = reinterpret_cast<UTF8 *>(output);

	std::string utf8;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF16toUTF8(&src_begin,
													 src_end,
													 &dst_begin,
													 dst_begin + COUNT_OF(output),
													 lenientConversion);

		utf8.append(output, dst_begin - reinterpret_cast<UTF8 *>(output));
		dst_begin = reinterpret_cast<UTF8 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf8.clear();
			break;
		}
	}

	return utf8;
}

std::basic_string<UTF32Char> UTF8ToUTF32(const UTF8Char *utf8, size_t length)
{
	UTF32Char output[4096];
	const UTF8 *src_begin = reinterpret_cast<const UTF8 *>(utf8);
	const UTF8 *src_end = src_begin + length;
	UTF32 *dst_begin = reinterpret_cast<UTF32 *>(output);

	std::basic_string<UTF32Char> utf32;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF8toUTF32(&src_begin,
													 src_end,
													 &dst_begin,
													 dst_begin + COUNT_OF(output),
													 lenientConversion);

		utf32.append(output, dst_begin - reinterpret_cast<UTF32 *>(output));
		dst_begin = reinterpret_cast<UTF32 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf32.clear();
			break;
		}
	}

	return utf32;
}

std::string UTF32ToUTF8(const UTF32Char *utf32, size_t length)
{
	UTF8Char output[8192];
	const UTF32 *src_begin = reinterpret_cast<const UTF32 *>(utf32);
	const UTF32 *src_end = src_begin + length;
	UTF8 *dst_begin = reinterpret_cast<UTF8 *>(output);
		
	std::string utf8;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF32toUTF8(&src_begin,
													 src_end,
													 &dst_begin,
													 dst_begin + COUNT_OF(output),
													 lenientConversion);

		utf8.append(output, dst_begin - reinterpret_cast<UTF8 *>(output));
		dst_begin = reinterpret_cast<UTF8 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf8.clear();
			break;
		}
	}

	return utf8;
}

std::basic_string<UTF32Char> UTF16ToUTF32(const UTF16Char *utf16, size_t length)
{
	UTF32Char output[4096];
	const UTF16 *src_begin = reinterpret_cast<const UTF16 *>(utf16);
	const UTF16 *src_end = src_begin + length;
	UTF32 *dst_begin = reinterpret_cast<UTF32 *>(output);

	std::basic_string<UTF32Char> utf32;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF16toUTF32(&src_begin,
													  src_end,
													  &dst_begin,
													  dst_begin + COUNT_OF(output),
													  lenientConversion);

		utf32.append(output, dst_begin - reinterpret_cast<UTF32 *>(output));
		dst_begin = reinterpret_cast<UTF32 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf32.clear();
			break;
		}
	}

	return utf32;
}

std::wstring UTF32ToUTF16(const UTF32Char *utf32, size_t length)
{
	UTF16Char output[8192];
	const UTF32 *src_begin = reinterpret_cast<const UTF32 *>(utf32);
	const UTF32 *src_end = src_begin + length;
	UTF16 *dst_begin = reinterpret_cast<UTF16 *>(output);

	std::wstring utf16;
	while (src_begin < src_end)
	{
		ConversionResult result = ConvertUTF32toUTF16(&src_begin,
													  src_end,
													  &dst_begin,
													  dst_begin + COUNT_OF(output),
													  lenientConversion);

		utf16.append(output, dst_begin - reinterpret_cast<UTF16 *>(output));
		dst_begin = reinterpret_cast<UTF16 *>(output);
		if (result == sourceIllegal || result == sourceExhausted)
		{
			utf16.clear();
			break;
		}
	}

	return utf16;
}

std::wstring UTF8ToUTF16(const std::string &utf8)
{
	return UTF8ToUTF16(utf8.c_str(), utf8.length());
}

std::string UTF16ToUTF8(const std::wstring &utf16)
{
	return UTF16ToUTF8(utf16.c_str(), utf16.length());
}

std::basic_string<UTF32Char> UTF8ToUTF32(const std::string &utf8)
{
	return UTF8ToUTF32(utf8.c_str(), utf8.length());
}

std::string UTF32ToUTF8(const std::basic_string<UTF32Char> &utf32)
{
	return UTF32ToUTF8(utf32.c_str(), utf32.length());
}

std::basic_string<UTF32Char> UTF16ToUTF32(const std::wstring &utf16)
{
	return UTF16ToUTF32(utf16.c_str(), utf16.length());
}

std::wstring UTF32ToUTF16(const std::basic_string<UTF32Char> &utf32)
{
	return UTF32ToUTF16(utf32.c_str(), utf32.length());
}

void UTF8CreateLengthTable(unsigned table[256])
{
	int c;

	for (c = 0; c < 256; c++)
	{
		if (c < 0x80)
			table[c] = 1;
		else if (0xBF < c && c < 0xE0)
			table[c] = 2;
		else if (0xDF < c && c < 0xF0)
			table[c] = 3;
		else if (0xEF < c && c < 0xF8)
			table[c] = 4;
		else
			table[c] = 0;
	}
}

bool ValidateUTF8Stream(const void* stream, unsigned length)
{
	/*
	 *	根据RFC3629（http://tools.ietf.org/html/rfc3629），
	 *	UTF-8流一个字符的第一个字节由0-4个数值为1的二进制位
	 *	这些位之后的第一位必须是0，当这些位的个数为0的时候表
	 *	示这个字符是ASCII，占用一个字节，除此之外表示这个字符
	 *	所占用的字节数；第二个字节开始，每个字节必须使用10的
	 *	二进制位开头，这样利于快速定位一个字符的起始字节，例如：
	 *
	 *	0XXXXXXX
	 *	110XXXXX 10XXXXXX
	 *	1110XXXX 10XXXXXX 10XXXXXX
	 *	11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
	 *
	 *	另，UTF-8理论上支持6字节长度，但是标准将其限定为4字节
	 */

	unsigned i, j, k;
	unsigned char* s = (unsigned char*)stream;
	static unsigned int table_created = 0;
	static unsigned int table[256];

	/* 保证多线程安全 */
	if (!table_created)
	{
		/* 利用lock-free的思想创建一模一样的表 */
		UTF8CreateLengthTable(table);
		/* 标记，之后的线程将不会重复创建该表 */
		table_created = 1;
	}

	/* 这里使用查表法是因为考虑到这个表会被放入CPU的Cache */

	for (i = 0; i < length;)
	{
		k = table[s[i]];
		if (k == 0)
			break;
		for (j = 1; j < k; j++)
			if (table[s[i + j]])
				return false;
		i += j;
	}

	return i == length;
}

bool ValidateGB2312Stream(const void* stream, unsigned length)
{
	/*
	 *	根据http://zh.wikipedia.org/zh-cn/Gb2312：
	 *	01-09区为特殊符号。
	 *	16-55区为一级汉字，按拼音排序。
	 *	56-87区为二级汉字，按部首／笔画排序。
	 *	10-15区及88-94区则未有编码。
	 *
	 *	每个汉字及符号以两个字节来表示。第一个字节称为“高位字节”，第二个字节称为“低位字节”。
	 *	“高位字节”使用了0xA1-0xF7（把01-87区的区号加上0xA0），“低位字节”使用了0xA1-0xFE（把01-94加上 0xA0）。
	 *	由于一级汉字从16区起始，汉字区的“高位字节”的范围是0xB0-0xF7，“低位字节”的范围是0xA1-0xFE，
	 *	占用的码位是 72*94=6768。其中有5个空位是D7FA-D7FE。
	 */

	unsigned char* s = (unsigned char*)stream;
	unsigned char* e = s + length;

	for (; s < e; s++)
	{
		if (*s < 0x80)
			continue;
		if (*s < 0xA1 || 0xE7 < *s)
			break;
		if (++s == e)
			return false;
		if (*s < 0xA1 || 0xFE < *s)
			break;
	}

	return s == e;
}

bool ValidateGBKStream(const void* stream, unsigned length)
{
	unsigned char* s = (unsigned char*)stream;
	unsigned char* e = s + length;

	for (; s < e; s++)
	{
		if (*s < 0x80)
			continue;
		if (*s < 0x81 || 0xFE < *s)
			break;
		if (++s == e)
			return false;
		if (*s < 0x40 || 0xFE < *s)
			break;
	}

	return s == e;
}

bool ValidateBIG5Stream(const void* stream, unsigned length)
{
	unsigned char* s = (unsigned char*)stream;
	unsigned char* e = s + length;

	for (; s < e; s++)
	{
		if (*s < 0x80)
			continue;
		if (*s < 0xA1 || 0xF9 < *s)
			break;
		if (++s == e)
			return false;
		if (*s < 0x40 || 0xFE < *s || (0x7E < *s && *s < 0xA1))
			break;
	}

	return s == e;
}

std::string StringTrimLeft(const char *input)
{
	std::string output = input;
	StringTrimLeft(output);
	return output;
}

std::string StringTrimRight(const char *input)
{
	std::string output = input;
	StringTrimRight(output);
	return output;
}

std::string StringTrim(const char *input) /* both left and right */
{
	std::string output = input;
	StringTrim(output);
	return output;
}

std::string& StringTrimLeft(std::string &input)
{
	StringTrimLeftT<char>(input);
	return input;
}

std::string& StringTrimRight(std::string &input)
{
	StringTrimRightT<char>(input);
	return input;
}

std::string& StringTrim(std::string &input) /* both left and right */
{
	StringTrimT<char>(input);
	return input;
}

std::wstring StringTrimLeft(const wchar_t *input)
{
	std::wstring output = input;
	StringTrimLeft(output);
	return output;
}

std::wstring StringTrimRight(const wchar_t *input)
{
	std::wstring output = input;
	StringTrimRight(output);
	return output;
}

std::wstring StringTrim(const wchar_t *input) /* both left and right */
{
	std::wstring output = input;
	StringTrim(output);
	return output;
}

std::wstring& StringTrimLeft(std::wstring &input)
{
	StringTrimLeftT<wchar_t>(input);
	return input;
}

std::wstring& StringTrimRight(std::wstring &input)
{
	StringTrimRightT<wchar_t>(input);
	return input;
}

std::wstring& StringTrim(std::wstring &input) /* both left and right */
{
	StringTrimT<wchar_t>(input);
	return input;
}

} // namespace nbase
