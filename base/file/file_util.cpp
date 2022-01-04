// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/12
//
// Modified by wrt(guangguang)
// Date: 2013/9/17
//
// Utilities for file and filepath operation

#include "base/file/file_util.h"
#include <memory>
#include "base/util/string_util.h"
#include "base/memory/deleter.h"

#if defined(OS_POSIX)
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif  // OS_POSIX

namespace nbase
{

#if defined(OS_POSIX)
const PathChar kEndChar = '\0';
const PathChar kFilePathSeparators[] = "/";
const PathChar kFilePathCurrentDirectory[]  = ".";
const PathChar kFilePathParentDirectory[]   = "..";
const PathChar kFilePathExtensionSeparator  = '.';
#elif defined(OS_WIN)
const PathChar kEndChar = L'\0';
const PathChar kFilePathSeparators[] = L"\\/";
const PathChar kFilePathCurrentDirectory[] = L".";
const PathChar kFilePathParentDirectory[]  = L"..";
const PathChar kFilePathExtensionSeparator = L'.';
#endif  // OS_WIN

bool IsFilePathSeparator(const PathChar separator)
{
	if (separator == kEndChar)
		return false;

	size_t len = sizeof(kFilePathSeparators)/sizeof(PathChar);
	for (size_t i = 0; i < len; i++)
	{
		if (separator == kFilePathSeparators[i])
			return true;
	}

	return false;
}

bool IsFilePathSeparator(const PathString &separator)
{
	if (separator.empty())
		return false;
	PathChar c = separator[0];
	return IsFilePathSeparator(c);
}

bool FilePathExtension(const PathString &filepath_in, PathString &extension_out)
{
	if (filepath_in.size() == 0)
		return false;
	bool ret = false;
	PathString file_name;
	if (FilePathApartFileName(filepath_in, file_name))
	{
		size_t pos = file_name.rfind(kFilePathExtensionSeparator);
		if (pos != PathString::npos)
		{
			extension_out = file_name.substr(pos, PathString::npos);
			ret = true;
		}
	}
	return ret;
}

bool FilePathApartDirectory(const PathString &filepath_in,
							PathString &directory_out)
{
	size_t index = filepath_in.size() - 1;
	if (index <= 0 || filepath_in.size() == 0)
		return false;
	for (; index != 0; index--)
	{
		if (IsFilePathSeparator(filepath_in[index]))
		{
			if (index == filepath_in.size() - 1)
				directory_out = filepath_in;
			else
			    directory_out = filepath_in.substr(0, index + 1);
			return true;
		}
	}
	return false;
}

bool FilePathApartFileName(const PathString &filepath_in,
						   PathString &filename_out)
{
	if (filepath_in.size() == 0)
		return false;
	bool ret = true;
	size_t separator_pos = PathString::npos;
	size_t separators_count = sizeof(kFilePathSeparators) / sizeof(PathChar);
	for (size_t index = 0; index < separators_count; index++)
	{
		separator_pos = filepath_in.rfind(kFilePathSeparators[index]);
		if (separator_pos != PathString::npos)
			break;
	}
	if (separator_pos++ != PathString::npos && separator_pos < filepath_in.size())
		filename_out = filepath_in.substr(separator_pos);
	else if (separator_pos >= filepath_in.size())
		ret = false;
	else
		filename_out = filepath_in;
	return ret;
}

template<typename CharType>
bool ParsePathComponentsT(const CharType *path,
						  const CharType *seperators,
						  std::list<std::basic_string<CharType> > &components)
{
	components.clear();
	if (path == NULL)
		return false;
	const CharType *prev = NULL;
	const CharType *next = path;
	const CharType *c;
	while (*next)
	{
		prev = next;
		// find the first seperator
		for (;;)
		{
			for (c = seperators; *c && *next != *c; c++);
			if (*c || !*next)
				break;
			next++;
		}
		components.push_back(std::basic_string<CharType>(prev, next - prev));
		if (*next)
			components.back().push_back(*seperators);
		// skip duplicated seperators
		for (++next;;)
		{
			for (c = seperators; *c && *next != *c; c++);
			if (!*c)
				break;
			next++;
		}
	}
	return true;
}

bool ParsePathComponents(const PathChar *path,
						 std::list<PathString> &components)
{
	return ParsePathComponentsT<PathChar>(path,
										  kFilePathSeparators,
										  components);
}

bool IsDirectoryComponent(const PathString &component)
{
	if (component.empty())
		return false;
#if defined(OS_WIN)
	return *component.rbegin() == kFilePathSeparators[0] ||
		*component.rbegin() == kFilePathSeparators[1];
#else
	return *component.rbegin() == kFilePathSeparators[0];
#endif // OS_WIN
}

bool FilePathCompose(const PathString &directory_in, 
	                 const PathString &filename_in, 
					 PathString &filepath_out)
{
	PathString directory;
	if (!FilePathApartDirectory(directory_in, directory))
		return false;
	filepath_out = directory + filename_in;
	return true;
}

bool FilePathIsExist(const PathString &filepath_in, bool is_directory)
{
	return FilePathIsExist((const PathChar *)filepath_in.c_str(), is_directory);
}

FILE* OpenFile(const PathString &filepath, const PathChar *mode)
{
	return OpenFile(filepath.c_str(), mode);
}

bool CloseFile(FILE *file)
{
	if (NULL == file)
		return true;
	return fclose(file) == 0;
}

int ReadFile(const PathString &filepath, void *data_out, size_t size)
{
	return ReadFile(filepath.c_str(), data_out, size);
}

int WriteFile(const PathString &filepath, const std::string &data)
{
	return WriteFile(filepath.c_str(), data.c_str(), data.size());
}

bool ReadFileToString(const PathString &filepath, std::string &out)
{
	std::unique_ptr<FILE, nbase::DeleterFileClose> file;
#if defined(OS_WIN)
	file.reset(OpenFile(filepath.c_str(), L"rb"));
#else
	file.reset(OpenFile(filepath.c_str(), "rb"));
#endif

	if (!file)
		return false;

	size_t file_size = (size_t)GetFileSize(filepath);
	if (file_size > (std::numeric_limits<size_t>::max)()) //x64编译条件下，size_t的最大值转为int64_t会溢出，因此用无符号
		return false; // Too large

	bool read_ok = true;
	out.resize(file_size);
	if (!out.empty()) {
		read_ok = fread(&out[0], 1, out.size(), file.get()) == out.size();
	}

	return read_ok;
}

}  // namespace nbase
