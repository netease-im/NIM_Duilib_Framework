#ifndef SHARED_CMD_LINE_H_
#define SHARED_CMD_LINE_H_

#include <string>
#include <map>

namespace shared
{
class CommandLine
{
public:
	CommandLine(const std::wstring &cmd);
	~CommandLine();

	bool Get(const std::wstring &key, std::wstring &value);
	bool IsExist(const std::wstring &key);
	std::map<std::wstring, std::wstring> GetMap(void);

protected:
	// 简单将cmd用'/'分割，参数中如果包含'/'会出问题
	static bool Parse(const std::wstring &cmd, std::map<std::wstring, std::wstring> &param_map);

protected:
	std::wstring cmd_;
	std::map<std::wstring, std::wstring> map_;
};
}//namespace shared

#endif //SHARED_CMD_LINE_H_
