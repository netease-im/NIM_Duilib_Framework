/*
 *
 *	Author		wrt(guangguang)
 *	Date		2010-09-01
 *	Copyright	Hangzhou, Netease Inc.
 *	Brief		a Windows command line parser
 *
 */

#include "base/util/cmd_line_args.h"

#include "base/base_config.h"

#if defined(COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#if defined(OS_WIN)
#include <windows.h>
#endif // OS_WIN

namespace nbase
{

CmdLineArgs::CmdLineArgs(const wchar_t *command_line):
	buffer_(NULL)
{
	size_t alloc;
	size_t length;

#if defined(OS_WIN)
	if (command_line == NULL)
		command_line = ::GetCommandLineW();
#endif // OS_WIN

	if (command_line != NULL)
	{
		length = wcslen(command_line);
		if (length > 0)
		{
			alloc = (length + 1) * sizeof(wchar_t);
			buffer_ = (wchar_t*)malloc(alloc);
			if (buffer_ != NULL)
			{
				wcscpy(buffer_, command_line);
				buffer_[length] = 0;
				ParseCommandLine();
			}
		}
	}
}


CmdLineArgs::~CmdLineArgs()
{
	if (buffer_ != NULL)
		free(buffer_);
}

bool CmdLineArgs::ParseCommandLine()
{
	enum
	{
		Term	= L'\0',
		Quote	= L'"',
	};

	bool isQuoted = false;
	wchar_t *arg, *ch = buffer_;

	if (buffer_ == NULL)
		return false;

	while (*ch != Term)
	{
		/* remove leading space(s) */
		while (iswspace(*ch))
			ch++;
		/* may reach tail */
		if (*ch == Term)
			break;
		isQuoted = (*ch == Quote);
		arg = isQuoted ? ++ch : ch++;

		/* seeking a new argument */
		while (*ch != Term)
		{
			if (isQuoted)
			{
				if (*ch == Quote && (ch[1] == Term || iswspace(ch[1])))
					break; /* a quoted argument found */
			}
			else
			{
				if (iswspace(*ch))
					break; /* a common argument found */
			}
			ch++;
		}

		if (*ch != Term)
			*ch++ = Term;

		/* a valid argument found ? */
		if (*arg != Term)
			push_back(arg);
	}

	return true;
}

} // namespace

#if defined(COMPILER_MSVC)
#pragma warning(pop)
#endif
