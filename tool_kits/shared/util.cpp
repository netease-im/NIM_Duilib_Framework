#include "stdafx.h"
#include "util.h"
#include "shared/log.h"
#include "shellapi.h"

std::wstring QPath::GetAppPath()
{
	return nbase::win32::GetCurrentModuleDirectory();
}

void QPath::AddNewEnvironment(const std::wstring& directory)
{
	TCHAR path_environment[4096];
	GetEnvironmentVariable(L"PATH", path_environment, 4096);
	std::wstring new_path = directory + L";";
	std::wstring new_environment = new_path + path_environment;
	SetEnvironmentVariable(L"PATH", new_environment.c_str());
}

//
std::map<std::wstring,std::wstring> QCommand::key_value_;

void QCommand::ParseCommand( const std::wstring &cmd )
{
	std::list<std::wstring> arrays = ui::StringHelper::Split(cmd, L"/");
	for(std::list<std::wstring>::const_iterator i = arrays.begin(); i != arrays.end(); i++)
	{
		std::list<std::wstring> object = ui::StringHelper::Split(*i, L" ");
		assert(object.size() == 2);
		key_value_[ *object.begin() ] = *object.rbegin();
	}
}

std::wstring QCommand::Get( const std::wstring &key )
{
	std::map<std::wstring,std::wstring>::const_iterator i = key_value_.find(key);
	if(i == key_value_.end())
		return L"";
	else
		return i->second;
}

void QCommand::Set( const std::wstring &key, const std::wstring &value )
{
	key_value_[key] = value;
}

void QCommand::Erase(const std::wstring &key)
{
	key_value_.erase(key);
}

bool QCommand::AppStartWidthCommand( const std::wstring &app, const std::wstring &cmd )
{
	HINSTANCE hInst = ::ShellExecuteW(NULL, L"open", app.c_str(), cmd.c_str(), NULL, SW_SHOWNORMAL);
	return (int)hInst > 32;
}

bool QCommand::RestartApp(const std::wstring &cmd)
{
	wchar_t app[1024] = { 0 };
	GetModuleFileName(NULL, app, 1024);
	HINSTANCE hInst = ::ShellExecuteW(NULL, L"open", app, cmd.c_str(), NULL, SW_SHOWNORMAL);
	return (int)hInst > 32;
}