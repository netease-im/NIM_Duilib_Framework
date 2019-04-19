#ifndef SHARED_TOOL_H_
#define SHARED_TOOL_H_

#include <string>
#include <vector>
#include "base/time/time.h"
#include "base/base_types.h"

namespace shared
{
namespace tools
{
// 系统相关
int GetOSVersion();
std::string GetOSVersionString();
bool IsWow64();

// 字符串处理
std::string	FormatTime(nbase::Time::TimeStruct	t);
std::wstring FormatBytes(double bytes);
std::wstring FormatBps(double bytes);
void SplitString(const std::string &str, const std::string &seperator, std::vector<std::string> &array);
void SplitString(const std::wstring &str, const std::wstring &seperator, std::vector<std::wstring> &array);
std::string StringReverse(const std::string &input);

// 过滤无法显示的表情字符
std::string FilterUCS4(const std::string& str, bool replace_spaces = true);

// 格式化SQL语句里的字符串（如转义单引号），避免执行SQL语句抛出异常
UTF8String FormatSQLText(const UTF8String &sql_text);

// 号码能否发短信
bool IsChinaPhoneNumber(const std::string &str);
bool IsAuthCode(const std::string &str);

// 文件处理
std::wstring GetSpecialFolder(int id);
std::wstring GetResourcePath(void);
void MakeExtendPath(std::wstring &path);
void ClearFile(const wchar_t *path, long file_length_limit);
bool FilePathIsExist(const std::string &utf8_filepath_in, bool is_directory);

// 采用递归的方式查找指定目下的文件
bool FindFile(const std::wstring &dir,const std::wstring &file_desc,std::wstring &file_path);
std::wstring EnsureValidFileName(const std::wstring &file_name, const std::wstring &find = L"\\/:*?\"<>|", const std::wstring &replace = L"_");

// 数学相关
int CalcAvg(const std::vector<int> &vec, bool is_revise = true);
int CalcStDev(const std::vector<int> &vec);

// 打开网址
void SafeOpenUrl(const std::wstring &url, int show_cmd);
void SafeOpenUrlEx(const std::string &url, int show_cmd);

bool RestartComputer();
bool RunAppWithRedirection(const std::wstring &app_path, const std::wstring &app_cmd, std::string &output);

// 生成时间戳，精度毫秒
std::string GenerateTimeStamp();

// Register Server
BOOL DllRegisterServer(LPCTSTR lpszFileName);
BOOL DllUnregisterServer(LPCTSTR lpszFileName);

// 进程通信message
void RegisterShowWndForgroudMessage();
UINT GetShowWndForgroudMessageID();

// 获取图片类型对应的CLSID
bool GetImageCLSID(const wchar_t* format, CLSID* pCLSID);

// 遍历文件下的文件（仅限第一层）
struct FileItem
{
	std::wstring file_;
	long size_;
};

typedef std::vector<std::wstring> StringVector;
typedef std::vector<FileItem> FileItemVector;

class File
{
public:
	//dir最后不需要"\\"
	static void GetFolder(const std::wstring &dir, StringVector &out, bool full_path = false); 
	//dir最后不需要"\\"
	static void GetFile(const std::wstring &dir, std::vector<FileItem> &out, bool full_path = false); 
};

}//namespace tools
}//namespace shared

#endif //SHARED_TOOL_H_
