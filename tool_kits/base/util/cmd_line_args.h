#ifndef BASE_UTIL_CMD_LINE_ARGS_H_
#define BASE_UTIL_CMD_LINE_ARGS_H_

#include "base/base_export.h"
#include <vector>
#include <ctype.h>

namespace nbase
{

class BASE_EXPORT CmdLineArgs: public std::vector<wchar_t*>
{
public:

	CmdLineArgs(const wchar_t *command_line = 0);
	virtual ~CmdLineArgs();

private:

	bool ParseCommandLine();

	wchar_t *buffer_;
};

} // namespace

#endif // BASE_UTIL_CMD_LINE_ARGS_H_
