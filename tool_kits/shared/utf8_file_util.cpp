// File operation utilities for UTF-8 encoding support
#include "stdafx.h"
#include "shared/utf8_file_util.h"

#include "base/file/file_util.h"
#include "base/util/string_util.h"

#ifndef OS_WIN
#error "Only used on MS-Windows"
#endif

namespace shared
{

static const UTF8Char kEndChar = '\0';
static const UTF8Char kFilePathSeparators[] = "\\/";
static const UTF8Char kFilePathCurrentDirectory[]  = ".";
static const UTF8Char kFilePathParentDirectory[]   = "..";
static const UTF8Char kFilePathExtensionSeparator  = '.';

bool IsFilePathSeparator(const UTF8Char separator)
{
	if (separator == kEndChar)
		return false;

	size_t len = sizeof(kFilePathSeparators)/sizeof(UTF8Char);
	for (size_t i = 0; i < len; i++)
	{
		if (separator == kFilePathSeparators[i])
			return true;
	}

	return false;
}

bool IsFilePathSeparator(const UTF8String &separator)
{
	if (separator.empty())
		return false;
	UTF8Char c = separator[0];
	return IsFilePathSeparator(c);
}

bool FilePathExtension(const UTF8String &filepath_in,
					   UTF8String &extension_out)
{
	if (filepath_in.size() == 0)
		return false;
	bool ret = false;
	UTF8String file_name;
	if (FilePathApartFileName(filepath_in, file_name))
	{
		size_t pos = file_name.rfind(kFilePathExtensionSeparator);
		if (pos != UTF8String::npos)
		{
			extension_out = file_name.substr(pos, UTF8String::npos);
			ret = true;
		}			
	}	
	return ret;
}

bool FilePathApartDirectory(const UTF8String &filepath_in,
							UTF8String &directory_out)
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

// Get the filename from the whole filepath
bool FilePathApartFileName(const UTF8String &filepath_in,
						   UTF8String &filename_out)
{
	if (filepath_in.size() == 0)
		return false;
	bool ret = true;
	size_t separator_pos = UTF8String::npos;
	size_t separators_count = sizeof(kFilePathSeparators) / sizeof(UTF8Char);
	for (size_t index = 0; index < separators_count; index++)
	{
		separator_pos = filepath_in.rfind(kFilePathSeparators[index]);
		if (separator_pos != UTF8String::npos)
			break;
	}
	if (separator_pos++ != UTF8String::npos && separator_pos < filepath_in.size())
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

bool ParsePathComponents(const UTF8Char *path,
						 std::list<UTF8String> &components)
{
	return ParsePathComponentsT<UTF8Char>(path,
										  kFilePathSeparators,
										  components);
}

bool IsDirectoryComponent(const UTF8String &component)
{
	if (component.empty())
		return false;
	return *component.rbegin() == kFilePathSeparators[0] ||
		*component.rbegin() == kFilePathSeparators[1];
}

bool FilePathCompose(const UTF8String &directory_in,
					 const UTF8String &filename_in,
					 UTF8String &filepath_out)
{
	UTF8String directory;
	if (!FilePathApartDirectory(directory_in, directory))
		return false;
	filepath_out = directory + filename_in;
	return true;
}

bool FilePathIsExist(const UTF8String &filepath, bool is_directory)
{
	UTF16String filepath2 = nbase::UTF8ToUTF16(filepath);
	return nbase::FilePathIsExist(filepath2, is_directory);
}

bool CreateDirectory(const UTF8String& full_path)
{
	UTF16String full_path2 = nbase::UTF8ToUTF16(full_path);
	return nbase::CreateDirectory(full_path2);
}

FILE* OpenFile(const UTF8String &filepath, const nbase::PathChar *mode)
{
	UTF16String filepath2 = nbase::UTF8ToUTF16(filepath);
	return nbase::OpenFile(filepath2, mode);
}

int ReadFile(const UTF8String &filepath, void *data_out, int size)
{
	UTF16String filepath2 = nbase::UTF8ToUTF16(filepath);
	return nbase::ReadFile(filepath2, data_out, size);
}

bool ReadFileToString(const UTF8String &filepath, std::string &contents_out)
{
	UTF16String filepath2 = nbase::UTF8ToUTF16(filepath);
	return nbase::ReadFileToString(filepath2, contents_out);
}

int WriteFile(const UTF8String &filepath, const std::string &data)
{
	UTF16String filepath2 = nbase::UTF8ToUTF16(filepath);
	return nbase::WriteFile(filepath2, data);
}

bool CopyFile(const UTF8String &from_path, const UTF8String &to_path)
{
	UTF16String from_path2 = nbase::UTF8ToUTF16(from_path);
	UTF16String to_path2 = nbase::UTF8ToUTF16(to_path);
	return nbase::CopyFile(from_path2, to_path2);
}

bool DeleteFile(const UTF8String &filepath)
{
	UTF16String filepath2 = nbase::UTF8ToUTF16(filepath);
	return nbase::DeleteFile(filepath2);
}

int64_t GetFileSize(const UTF8String &filepath)
{
	UTF16String filepath2 = nbase::UTF8ToUTF16(filepath);
	return nbase::GetFileSize(filepath2);
}

} // namespace shared
