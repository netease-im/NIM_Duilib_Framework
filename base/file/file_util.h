// Utilities for file and filepath operation

#ifndef BASE_FILE_FILE_UTIL_H_
#define BASE_FILE_FILE_UTIL_H_

#include <list>
#include <string>
#include "base/base_export.h"
#include "base/base_types.h"
#include "base/file/file_path.h"

namespace nbase
{

// Null-terminated array of separators used to separate components in path.
// Each character in this array is a valid separator
extern const PathChar kFilePathSeparators[];
// A special path component meaning "this directory."
extern const PathChar kFilePathCurrentDirectory[];
// A special path component meaning "the parent directory."
extern const PathChar kFilePathParentDirectory[];
// The character used to identify a file extension.
extern const PathChar kFilePathExtensionSeparator;

// Check the character is filepath separator
BASE_EXPORT bool IsFilePathSeparator(const PathChar separator);
BASE_EXPORT bool IsFilePathSeparator(const PathString &separator);

// Get the file extension from filepath
BASE_EXPORT bool FilePathExtension(const PathString &filepath_in,
								   PathString &extension_out);

// Get the directory from the whole filepath
BASE_EXPORT bool FilePathApartDirectory(const PathString &filepath_in,
										PathString &directory_out);
// Get the filename from the whole filepath
BASE_EXPORT bool FilePathApartFileName(const PathString &filepath_in,
									   PathString &filename_out);

// Parse and extract all components of a path
BASE_EXPORT bool ParsePathComponents(const PathChar *PathChar,
									 std::list<PathString> &components);

// Check if |component| is a directory
BASE_EXPORT bool IsDirectoryComponent(const PathString &component);

// Compose the filepath from directory and filename
BASE_EXPORT bool FilePathCompose(const PathString &directory_in,
								 const PathString &filename_in,
								 PathString &filepath_out);

// Get the current filepath
BASE_EXPORT bool FilePathCurrentDirectory(PathString &directory_out);

// Check the filepath is exist
// If |is_directory| is true, check a directory, or check the path
BASE_EXPORT bool FilePathIsExist(const PathChar *filepath_in,
								 bool is_directory);
BASE_EXPORT bool FilePathIsExist(const PathString &filepath_in,
								 bool is_directory);

// Create a directory, all subdirs will be created if not existing
BASE_EXPORT bool CreateDirectory(const PathChar* full_path);
BASE_EXPORT bool CreateDirectory(const PathString& full_path);

// Wrapper for fopen-like calls. Returns non-NULL FILE* on success
BASE_EXPORT FILE* OpenFile(const PathChar *filepath, const PathChar *mode);
BASE_EXPORT FILE* OpenFile(const PathString &filepath, const PathChar *mode);

// Closes file opened by OpenFile. Returns true on success
BASE_EXPORT bool CloseFile(FILE *file);

// Reads the given number of bytes from the file into the buffer
BASE_EXPORT int ReadFile(const PathChar *filepath,
						 void *data_out,
						 size_t size);
BASE_EXPORT int ReadFile(const PathString &filepath,
						 void *data_out,
						 size_t size);


// Read the file at |path| into |contents|, returning true on success.
BASE_EXPORT bool ReadFileToString(const PathString &filepath,
								  std::string &contents_out);

// Writes the content of given buffer into the file
BASE_EXPORT int WriteFile(const PathChar *filepath,
						  const void *data,
						  size_t size);
BASE_EXPORT int WriteFile(const PathString &filepath, const std::string &data);

// Copies a single file.
BASE_EXPORT bool CopyFile(const PathString &from_path,
						  const PathString &to_path);

// Deletes the given path.
BASE_EXPORT bool DeleteFile(const PathString &filepath);

// Get file size.
BASE_EXPORT int64_t GetFileSize(const PathString &filepath);

}  // namespace nbase

#endif  // BASE_FILE_FILE_UTIL_H_