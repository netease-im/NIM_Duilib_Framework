// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: rl
// Date: 2011/6/12
//
// Modified by wrt(guangguang)
// Date: 2013/9/5
//
// Utilities for file and filepath operation

#include "file_util.h"
#include "base/util/string_util.h"

#if defined(OS_WIN)
#include <windows.h>
#include "base/win32/scoped_win_handle.h"

namespace nbase
{

bool FilePathCurrentDirectory(PathString &directory_out)
{
	PathChar directory[MAX_PATH];
	directory[0] = 0;
	DWORD len = ::GetCurrentDirectoryW(MAX_PATH, directory);
	if (len == 0 || len > MAX_PATH)
	{
		return false;
	}
	directory_out = directory;
	directory_out.append(1, kFilePathSeparators[0]);

	return true;
}

bool FilePathIsExist(const PathChar *filepath_in, bool is_directory)
{
	const DWORD file_attr = ::GetFileAttributesW(filepath_in);
	if (file_attr != INVALID_FILE_ATTRIBUTES)
	{
		if (is_directory)
			return (file_attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
		else
			return true;
	}
	return false;
}

bool CreateDirectory(const PathString& full_path)
{
	return CreateDirectory(full_path.c_str());
}

bool CreateDirectory(const PathChar* full_path)
{
	if (full_path == nullptr)
		return false;

	if (FilePathIsExist(full_path, true))
		return true;

	std::list<PathString> subpaths;
	ParsePathComponents(full_path, subpaths);
	if (subpaths.empty())
		return false;

	// Collect a list of all parent directories.
	auto curr = subpaths.begin();
	for (auto prev = curr++; curr != subpaths.end(); curr++) {
		*curr = *prev + *curr;
		prev = curr;
	}

	// Iterate through the parents and create the missing ones.
	for (auto i = subpaths.begin(); i != subpaths.end(); ++i) {
		if (FilePathIsExist(i->c_str(), true))
			continue;
		if (!::CreateDirectoryW(i->c_str(), NULL)) {
			DWORD error = ::GetLastError();
			if (error == ERROR_ALREADY_EXISTS &&
				FilePathIsExist(i->c_str(), true)) {
				continue;
			}
			return false;
		}
	}
	return true;
}

FILE* OpenFile(const PathChar *filepath, const PathChar *mode)
{
	return _wfsopen(filepath, mode, _SH_DENYNO);
}

int ReadFile(const PathChar *filepath, void *data_out, size_t size)
{
	win32::ScopedWinHandle file(CreateFileW(filepath,
		                                    GENERIC_READ,
		                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
		                                    NULL,
		                                    OPEN_EXISTING,
		                                    FILE_FLAG_SEQUENTIAL_SCAN,
		                                    NULL));
	if (!file)
		return -1;

	DWORD read;
	if (::ReadFile(file, data_out, (DWORD)size, &read, NULL) &&
		static_cast<int>(read) == size)
		return read;
	return -1;
}

int WriteFile(const PathChar *filepath, const void *data, size_t size)
{
	win32::ScopedWinHandle file(CreateFileW(filepath,
		                                    GENERIC_WRITE,
		                                    0,
		                                    NULL,
		                                    CREATE_ALWAYS,
		                                    0,
		                                    NULL));
	if (!file)
		return -1;

	DWORD written;
	BOOL result = ::WriteFile(file, data, (DWORD)size, &written, NULL);
	if (result && static_cast<int>(written) == size)
		return written;

	return -1;
}

bool CopyFile(const PathString &from_path, const PathString &to_path)
{
	if (from_path.size() >= MAX_PATH ||
		to_path.size() >= MAX_PATH) {
			return false;
	}
	return (::CopyFileW(from_path.c_str(), to_path.c_str(),
		false) != 0);
}

bool DeleteFile(const PathString &filepath)
{
	if (::DeleteFileW(filepath.c_str()) != 0)
		return true;
	return false;
}

int64_t GetFileSize(const PathString &filepath)
{
	WIN32_FIND_DATAW file_data;
	HANDLE file = FindFirstFileW(filepath.c_str(), &file_data);

	if (file == INVALID_HANDLE_VALUE)
		return -1;
	LARGE_INTEGER li = { file_data.nFileSizeLow, (LONG)file_data.nFileSizeHigh };
	FindClose(file);
	return li.QuadPart;
}

}  // namespace nbase

#endif  // OS_WIN

