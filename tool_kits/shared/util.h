#pragma once
#include <string>
#include <map>

class QString
{
public:
};

class QPath
{
public:
	/**
	 * @brief 获取 exe 所在目录
	 * @return 返回 exe 所在目录，尾部有斜杠
	 */
	static std::wstring GetAppPath();

	/**
	 * @brief 添加一个目录到执行程序的环境变量中
	 * @param[in] directory 要添加的目录
	 * @return 无
	 */
	static void AddNewEnvironment(const std::wstring& directory);
};

class QCommand
{
public:
	static void ParseCommand(const std::wstring &cmd);
	static bool AppStartWidthCommand(const std::wstring &app, const std::wstring &cmd);
	static bool RestartApp(const std::wstring &cmd);
	static std::wstring Get(const std::wstring &key);
	static void Set(const std::wstring &key, const std::wstring &value);
	static void Erase(const std::wstring &key);
private:
	static std::map<std::wstring,std::wstring> key_value_;
};