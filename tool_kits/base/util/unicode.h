// Unicode define

#ifndef BASE_UTIL_UNICODE_H_
#define BASE_UTIL_UNICODE_H_

#include <string>
#include "base/base_types.h"

typedef char UTF8Char;
#if defined(WCHAR_T_IS_UTF16)
typedef wchar_t UTF16Char;
typedef int32_t UTF32Char;
#else
typedef int16_t UTF16Char;
typedef wchar_t UTF32Char;
#endif

typedef std::basic_string<UTF8Char> UTF8String;
typedef UTF8String U8String;
typedef std::basic_string<UTF16Char> UTF16String;
typedef UTF16String U16String;
typedef std::basic_string<UTF32Char> UTF32String;
typedef UTF32String U32String;

#endif // BASE_UTIL_UNICODE_H_
