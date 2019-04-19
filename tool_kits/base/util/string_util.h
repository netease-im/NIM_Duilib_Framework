#ifndef BASE_UTIL_STRING_UTIL_H_
#define BASE_UTIL_STRING_UTIL_H_

#include <list>
#include "base/base_export.h"
#include "base/base_types.h"
#include "base/util/unicode.h"

namespace nbase
{

// format a string
BASE_EXPORT std::string StringPrintf(const char *format, ...);
BASE_EXPORT std::wstring StringPrintf(const wchar_t *format, ...);
BASE_EXPORT const std::string& StringPrintf(std::string &output, const char *format, ...);
BASE_EXPORT const std::wstring& StringPrintf(std::wstring &output, const wchar_t *format, ...);
BASE_EXPORT void StringPrintfV(std::string &output, const char *format, va_list ap);
BASE_EXPORT void StringPrintfV(std::wstring &output, const wchar_t *format, va_list ap);

// format a string and append the result to a existing string
BASE_EXPORT void StringAppendF(std::string &output, const char *format, ...);
BASE_EXPORT void StringAppendF(std::wstring &output, const wchar_t *format, ...);
BASE_EXPORT void StringAppendV(std::string &output, const char *format, va_list ap);
BASE_EXPORT void StringAppendV(std::wstring &output, const wchar_t *format, va_list ap);

// find all tokens splitted by one of the characters in 'delimitor'
BASE_EXPORT std::list<std::string> StringTokenize(const char *input, const char *delimitor);
BASE_EXPORT std::list<std::wstring> StringTokenize(const wchar_t *input, const wchar_t *delimitor);
BASE_EXPORT int StringTokenize(const std::string& input, const std::string& delimitor, std::list<std::string>& output);
BASE_EXPORT int StringTokenize(const std::wstring& input, const std::wstring& delimitor, std::list<std::wstring>& output);

// replace all 'find' with 'replace' in the string
BASE_EXPORT size_t StringReplaceAll(const std::string& find, const std::string& replace, std::string& output);
BASE_EXPORT size_t StringReplaceAll(const std::wstring& find, const std::wstring& replace, std::wstring& output);

// print the binary data in a human-readable hexadecimal format
BASE_EXPORT void BinaryToHexString(const void *binary, size_t length, std::string &output);
BASE_EXPORT std::string BinaryToHexString(const void *binary, size_t length);
BASE_EXPORT std::string BinaryToHexString(const std::string &binary);

BASE_EXPORT void LowerString(std::string &str);
BASE_EXPORT void LowerString(std::wstring &str);
BASE_EXPORT void UpperString(std::string &str);
BASE_EXPORT void UpperString(std::wstring &str);
BASE_EXPORT std::string MakeLowerString(const std::string &src);
BASE_EXPORT std::wstring MakeLowerString(const std::wstring &src);
BASE_EXPORT std::string MakeUpperString(const std::string &src);
BASE_EXPORT std::wstring MakeUpperString(const std::wstring &src);

// convert hexadecimal string to binary
// the functions will try to convert the characters in input stream until the end of the stream or an error occurs
BASE_EXPORT int HexStringToBinary(const char *input, size_t length, std::string &output);
BASE_EXPORT std::string HexStringToBinary(const std::string &input);

// convert a hexadecimal character to decimal 8-bit signed integer
// if c is not a hexadecimal character, -1 will be returned
BASE_EXPORT char HexCharToInt8(char c);

// the following functions are used to convert encodings in utf-8、utf-16 and utf-32
BASE_EXPORT std::wstring UTF8ToUTF16(const UTF8Char *utf8, size_t length);
BASE_EXPORT std::string UTF16ToUTF8(const UTF16Char *utf16, size_t length);
BASE_EXPORT std::basic_string<UTF32Char> UTF8ToUTF32(const UTF8Char *utf8, size_t length);
BASE_EXPORT std::string UTF32ToUTF8(const UTF32Char *utf32, size_t length);
BASE_EXPORT std::basic_string<UTF32Char> UTF16ToUTF32(const UTF16Char *utf16, size_t length);
BASE_EXPORT std::wstring UTF32ToUTF16(const UTF32Char *utf32, size_t length);
BASE_EXPORT std::wstring UTF8ToUTF16(const std::string &utf8);
BASE_EXPORT std::string UTF16ToUTF8(const std::wstring &utf16);
BASE_EXPORT std::basic_string<UTF32Char> UTF8ToUTF32(const std::string &utf8);
BASE_EXPORT std::string UTF32ToUTF8(const std::basic_string<UTF32Char> &utf32);
BASE_EXPORT std::basic_string<UTF32Char> UTF16ToUTF32(const std::wstring &utf16);
BASE_EXPORT std::wstring UTF32ToUTF16(const std::basic_string<UTF32Char> &utf32);

// the following functions are used to validate encoded streamings, such as utf-8, gbk, big5, etc.
BASE_EXPORT void UTF8CreateLengthTable(unsigned table[256]);
BASE_EXPORT bool ValidateUTF8Stream(const void* stream, unsigned length);
BASE_EXPORT bool ValidateGB2312Stream(const void* stream, unsigned length);
BASE_EXPORT bool ValidateGBKStream(const void* stream, unsigned length);
BASE_EXPORT bool ValidateBIG5Stream(const void* stream, unsigned length);

// trimming, removing extra spaces
BASE_EXPORT std::string StringTrimLeft(const char *input);
BASE_EXPORT std::string StringTrimRight(const char *input);
BASE_EXPORT std::string StringTrim(const char *input); /* both left and right */
BASE_EXPORT std::string& StringTrimLeft(std::string &input);
BASE_EXPORT std::string& StringTrimRight(std::string &input);
BASE_EXPORT std::string& StringTrim(std::string &input); /* both left and right */
BASE_EXPORT std::wstring StringTrimLeft(const wchar_t *input);
BASE_EXPORT std::wstring StringTrimRight(const wchar_t *input);
BASE_EXPORT std::wstring StringTrim(const wchar_t *input); /* both left and right */
BASE_EXPORT std::wstring& StringTrimLeft(std::wstring &input);
BASE_EXPORT std::wstring& StringTrimRight(std::wstring &input);
BASE_EXPORT std::wstring& StringTrim(std::wstring &input); /* both left and right */

}

#endif // BASE_UTIL_STRING_UTIL_H_
