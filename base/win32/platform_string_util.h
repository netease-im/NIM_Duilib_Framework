// Multi-byte strings and Unicode strings conversion, etc.

#ifndef BASE_PLATFORM_STRING_UTIL_H_
#define BASE_PLATFORM_STRING_UTIL_H_

#include "base/base_config.h"

#if defined(OS_WIN)

#include "base/base_export.h"
#include <string>

namespace nbase
{
namespace win32
{

// the following functions are used to convert encodings between MBCS and Unicode
BASE_EXPORT bool MBCSToUnicode(const char *input, std::wstring& output, int code_page = CP_ACP);
BASE_EXPORT bool MBCSToUnicode(const std::string &input, std::wstring& output, int code_page = CP_ACP);
BASE_EXPORT bool UnicodeToMBCS(const wchar_t *input, std::string &output, int code_page = CP_ACP);
BASE_EXPORT bool UnicodeToMBCS(const std::wstring& input, std::string &output, int code_page = CP_ACP);

} // namespace win32
} // namespace nbase

#endif // OS_WIN
#endif // BASE_PLATFORM_STRING_UTIL_H_
