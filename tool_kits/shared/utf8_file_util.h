// File operation utilities for UTF-8 encoding support

#ifndef SHARED_UTF8_FILE_UTIL_H_
#define SHARED_UTF8_FILE_UTIL_H_

#include <list>
#include "base/util/unicode.h"
#include "base/file/file_path.h" // for PathChar

namespace shared
{

// Check the character is filepath separator
bool IsFilePathSeparator(const UTF8Char separator);
bool IsFilePathSeparator(const UTF8String &separator);

// Get the file extension from filepath
bool FilePathExtension(const UTF8String &filepath_in,
					   UTF8String &extension_out);

// Get the directory from the whole filepath
bool FilePathApartDirectory(const UTF8String &filepath_in,
							UTF8String &directory_out);
// Get the filename from the whole filepath
bool FilePathApartFileName(const UTF8String &filepath_in,
						   UTF8String &filename_out);

// Parse and extract all components of a path
bool ParsePathComponents(const UTF8Char *PathChar,
						 std::list<UTF8String> &components);

// Check if |component| is a directory
bool IsDirectoryComponent(const UTF8String &component);

// Compose the filepath from directory and filename
bool FilePathCompose(const UTF8String &directory_in,
					 const UTF8String &filename_in,
					 UTF8String &filepath_out);

// Check the filepath is exist
// If |is_directory| is true, check a directory, or check the path
bool FilePathIsExist(const UTF8String &filepath_in, bool is_directory);

// Create a directory, all subdirs will be created if not existing
bool CreateDirectory(const UTF8String& full_path);

// Wrapper for fopen-like calls. Returns non-NULL FILE* on success
FILE* OpenFile(const UTF8String &filepath, const nbase::PathChar *mode);

// Reads the given number of bytes from the file into the buffer
int ReadFile(const UTF8String &filepath, void *data_out, int size);

// Read the file at |path| into |contents|, returning true on success.
bool ReadFileToString(const UTF8String &filepath, std::string &contents_out);

// Writes the content of given buffer into the file
int WriteFile(const UTF8String &filepath, const std::string &data);

// Copies a single file.
bool CopyFile(const UTF8String &from_path, const UTF8String &to_path);

// Deletes the given path.
bool DeleteFile(const UTF8String &filepath);

// Get file size.
int64_t GetFileSize(const UTF8String &filepath);

} // namespace shared

#endif // SHARED_UTF8_FILE_UTIL_H_
