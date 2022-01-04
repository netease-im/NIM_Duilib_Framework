#ifndef UI_UTILS_STRINGUTIL_H_
#define UI_UTILS_STRINGUTIL_H_

namespace ui
{

class UILIB_API StringHelper
{
public:
	static std::wstring ReparsePath(const std::wstring& strFilePath);

	// format a string
	static std::wstring Printf(const wchar_t *format, ...);

	// replace all 'find' with 'replace' in the string
	static size_t ReplaceAll(const std::wstring& find, const std::wstring& replace, std::wstring& output);

	static std::wstring MakeLowerString(const std::wstring &str);
	static std::wstring MakeUpperString(const std::wstring &str);

	static bool MBCSToUnicode(const char *input, std::wstring& output, int code_page = CP_ACP);
	static bool MBCSToUnicode(const std::string &input, std::wstring& output, int code_page = CP_ACP);
	static bool UnicodeToMBCS(const wchar_t *input, std::string &output, int code_page = CP_ACP);
	static bool UnicodeToMBCS(const std::wstring& input, std::string &output, int code_page = CP_ACP);

	// trimming, removing extra spaces
	static std::string TrimLeft(const char *input);
	static std::string TrimRight(const char *input);
	static std::string Trim(const char *input); /* both left and right */
	static std::string& TrimLeft(std::string &input);
	static std::string& TrimRight(std::string &input);
	static std::string& Trim(std::string &input); /* both left and right */
	static std::wstring TrimLeft(const wchar_t *input);
	static std::wstring TrimRight(const wchar_t *input);
	static std::wstring Trim(const wchar_t *input); /* both left and right */
	static std::wstring& TrimLeft(std::wstring &input);
	static std::wstring& TrimRight(std::wstring &input);
	static std::wstring& Trim(std::wstring &input); /* both left and right */

	// find all tokens splitted by one of the characters in 'delimitor'
	static std::list<std::string> Split(const std::string& input, const std::string& delimitor);
	static std::list<std::wstring> Split(const std::wstring& input, const std::wstring& delimitor);
};

}

#endif // UI_UTILS_STRINGUTIL_H_
