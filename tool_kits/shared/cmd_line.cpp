#include "stdafx.h"
#include "cmd_line.h"
#include "base/util/string_util.h"

namespace shared
{
CommandLine::CommandLine(const std::wstring &cmd) : cmd_(cmd)
{
	Parse(cmd_, map_);
}

CommandLine::~CommandLine()
{

}

bool CommandLine::Parse(const std::wstring &cmd, std::map<std::wstring, std::wstring> &param_map)
{
	std::list<std::wstring> param_list;
	if (nbase::StringTokenize(cmd, L"/", param_list) == 0)
		return false;

	std::wstring token, key, value;
	size_t pos;
	for (std::list<std::wstring>::iterator it = param_list.begin(); it != param_list.end(); ++it)
	{
		token = *it;
		key = L"";
		value = L"";
		pos = token.find(L' ', 0);
		if (pos < 0 || pos >= token.size())
			pos = token.size();
		key = token.substr(0, pos);
		if (pos < token.size())
		{
			value = token.substr(pos, token.size() - pos);
			value = nbase::StringTrim(value.c_str());
			if (value.size() >= 2 && value[0] == '"' && value[value.size()-1] == '"')
			{
				std::wstring str = value.substr(1, value.size() - 2);
				value = str;
			}
		}
		param_map[key] = value;
	}

	return true;
}

bool CommandLine::Get(const std::wstring &key, std::wstring &value)
{
	value = L"";
	if (map_.size() == 0)
		return false;

	std::map<std::wstring, std::wstring>::iterator it = map_.find(key);
	if (it == map_.end())
		return false;

	value = it->second;

	return true;
}

std::map<std::wstring, std::wstring> CommandLine::GetMap(void)
{
	return map_;
}

bool CommandLine::IsExist(const std::wstring &key)
{
	return (map_.find(key) != map_.end());
}
}//namespace shared