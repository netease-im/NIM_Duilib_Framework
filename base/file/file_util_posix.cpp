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

#if defined(OS_POSIX)
#include <string>
#include <vector>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/stat.h>

namespace nbase
{

typedef struct stat stat_wrapper_t;
static int CallStat(const char *path, stat_wrapper_t *sb)
{
	return stat(path, sb);
}

static int CallLstat(const char *path, stat_wrapper_t *sb)
{
	return lstat(path, sb);
}

bool FilePathCurrentDirectory(std::string &directory_out)
{
	char directory[512] = "";
	if (!getcwd(directory, sizeof(directory)))
	{
		return false;
	}

	directory_out = directory;
	directory_out.append(1, kFilePathSeparators[0]);
	return true;
}

bool FilePathIsExist(const char *filepath_in, bool is_directory)
{
	if (!is_directory)
		return access(filepath_in, F_OK) == 0;
	else
	{
		DIR *directory = opendir(filepath_in);
		if (directory != NULL)
		{
			closedir(directory);
			return true;
		}
	}
	return false;
}

bool CreateDirectory(const std::string& full_path)
{
	return CreateDirectory(full_path.c_str());
}

bool CreateDirectory(const char* full_path)
{
	if (full_path == NULL)
		return false;

	std::list<std::string> subpaths;
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
		if (mkdir(i->c_str(), 0700) == 0)
			continue;
		// Mkdir failed, but it might have failed with EEXIST, or some other
		// error due to the the directory appearing out of thin air. This can
		// occur if two processes are trying to create the same file system tree
		// at the same time. Check to see if it exists and make sure it is a
		// directory.
		if (!FilePathIsExist(i->c_str(), true))
			return false;
	}
	return true;
}

FILE* OpenFile(const char *filepath, const char *mode)
{
	FILE* file = NULL;
	do
	{
		file = fopen(filepath, mode);
	} while (!file && errno == EINTR);
	return file;
}

int ReadFile(const char *filepath, void *data_out, size_t size)
{
	int fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return -1;

	ssize_t bytes_read = read(fd, data_out, size);
	if (int ret = close(fd) < 0)
		return ret;
	return bytes_read;
}

int WriteFile(const char *filepath, const void *data, size_t size)
{
	int fd = creat(filepath, 0666);
	if (fd < 0)
		return -1;

	ssize_t bytes_written_total = 0;
	for (ssize_t bytes_written_partial = 0; bytes_written_total < size;
		bytes_written_total += bytes_written_partial)
	{
		bytes_written_partial =
			write(fd, data + bytes_written_total,
			size - bytes_written_total);
		if (bytes_written_partial < 0)
			return -1;
	}

	if (int ret = close(fd) < 0)
		return ret;

	return bytes_written_total;
}

bool CopyFile(const std::string &from_path, const std::string &to_path)
{
	int infile = open(from_path.c_str(), O_RDONLY);
	if (infile < 0)
		return false;

	int outfile = creat(to_path.c_str(), 0666);
	if (outfile < 0)
	{
		close(infile);
		return false;
	}

	const size_t kBufferSize = 32768;
	std::vector<char> buffer(kBufferSize);
	bool result = true;

	while (result)
	{
		ssize_t bytes_read = read(infile, &buffer[0], buffer.size());
		if (bytes_read < 0)
		{
			result = false;
			break;
		}
		if (bytes_read == 0)
			break;
		// Allow for partial writes
		ssize_t bytes_written_per_read = 0;
		do 
		{
			ssize_t bytes_written_partial = write(
				outfile,
				&buffer[bytes_written_per_read],
				bytes_read - bytes_written_per_read);
			if (bytes_written_partial < 0)
			{
				result = false;
				break;
			}
			bytes_written_per_read += bytes_written_partial;
		} while (bytes_written_per_read < bytes_read);
	}

	if (close(infile) < 0)
		result = false;
	if (close(outfile) < 0)
		result = false;

	return result;
}

bool DeleteFile(const std::string &filepath)
{
	const char* path_str = filepath.c_str();
	stat_wrapper_t file_info;
	int test = CallStat(path_str, &file_info);
	if (test != 0)
	{
		// The Windows version defines this condition as success.
		bool ret = (errno == ENOENT || errno == ENOTDIR);
		return ret;
	}
	if (!S_ISDIR(file_info.st_mode))
		return (unlink(path_str) == 0);

	return true;	
}

int64_t GetFileSize(const std::string &filepath)
{
	const char* path_str = filepath.c_str();
	stat_wrapper_t file_info;
	int test = CallStat(path_str, &file_info);
	if (test != 0)
		return -1;
	if (!S_ISDIR(file_info.st_mode))
		return file_info.st_size;
	return -1;
}

}  // namespace nbase

#endif  // OS_POSIX

