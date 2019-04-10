// Copyright (c) 2011, NetEase Inc. All rights reserved.
//
// Author: Ruan Liang <ruanliang@corp.netease.com>
// Date: 2011/6/23
//
// File utility function Unittest

#if defined(WITH_UNITTEST)

#include "base/file/file_util.h"
#include "base/util/string_util.h"
#include "gtest/gtest.h"

TEST(FileUtil, IsSeparator)
{
#if defined(OS_WIN)
	EXPECT_TRUE(nbase::IsFilePathSeparator("\\"));
	EXPECT_TRUE(nbase::IsFilePathSeparator(L"\\"));
#endif
	EXPECT_TRUE(nbase::IsFilePathSeparator("/"));
	EXPECT_TRUE(nbase::IsFilePathSeparator(L"/"));
	EXPECT_TRUE(nbase::IsFilePathSeparator("3") == false);
}

TEST(FileUtil, Extension)
{
	std::wstring extension_w;
	nbase::FilePathExtension(L"D:/123.txt", extension_w);
	EXPECT_STREQ(L".txt", extension_w.c_str());

	std::string extension;
	nbase::FilePathExtension("D:/123.exe", extension);
	EXPECT_STREQ(".exe", extension.c_str());
}

TEST(FileUtil, PathApart)
{
	std::wstring directory_w, filename_w;
	nbase::FilePathApartDirectory(L"D:/testcase/001/hello.txt", directory_w);
	nbase::FilePathApartFileName(L"D:/testcase/001/hello.txt", filename_w);

	EXPECT_STREQ(L"hello.txt", filename_w.c_str());
	EXPECT_STREQ(L"D:/testcase/001/", directory_w.c_str());

	std::string directory, filename;
	nbase::FilePathApartDirectory("D:/testcase/001/hello.txt", directory);
	nbase::FilePathApartFileName("D:/testcase/001/hello.txt", filename);

	EXPECT_STREQ("hello.txt", filename.c_str());
	EXPECT_STREQ("D:/testcase/001/", directory.c_str());
}

TEST(FileUtil, FilePathCompose)
{
	std::wstring directory_w = L"D:/testcase/001/";
	std::wstring filename_w  = L"hello.txt";
	std::wstring filepath_w;
	nbase::FilePathCompose(directory_w, filename_w, filepath_w);
	EXPECT_STREQ(L"D:/testcase/001/hello.txt", filepath_w.c_str());

	std::string directory = "D:/testcase/001/";
	std::string filename  = "hello.txt";
	std::string filepath;
	nbase::FilePathCompose(directory, filename, filepath);
	EXPECT_STREQ("D:/testcase/001/hello.txt", filepath.c_str());
}

TEST(FileUtil, FilePathExist)
{
	std::wstring directory_w;
	nbase::FilePathCurrentDirectory(directory_w);
	EXPECT_TRUE(nbase::FilePathIsExist(directory_w, true));	
	std::wstring filepath_w;
	nbase::FilePathCompose(directory_w, L"hello.txt", filepath_w);
	EXPECT_TRUE(nbase::FilePathIsExist(filepath_w, false) == false);

	std::string directory;
	nbase::FilePathCurrentDirectory(directory);
	EXPECT_TRUE(nbase::FilePathIsExist(directory, true));
	std::string filepath;
	nbase::FilePathCompose(directory, "hello.txt", filepath);
	EXPECT_TRUE(nbase::FilePathIsExist(filepath, false) == false);
}

TEST(FileUtil, FileSize)
{
#if !defined(OS_IOS)    
	std::string directory;
	nbase::FilePathCurrentDirectory(directory);
	std::string filepath;
	nbase::FilePathCompose(directory, "test.log", filepath);
	EXPECT_TRUE(nbase::FilePathIsExist(filepath, false) == true);

	int64_t filesize = nbase::GetFileSize(filepath);
	EXPECT_TRUE(filesize > 0);
#endif
}

TEST(FileUtil, ParsePathComponents)
{
#if defined(OS_WIN)
	std::string file_path = "f:\\a\\b\\c.exe";
#else
	std::string file_path = "/user/a/b/c.exe";
#endif
	std::list<std::string> components;
	EXPECT_TRUE(nbase::ParsePathComponents(file_path.c_str(), components));
	EXPECT_FALSE(nbase::IsDirectoryComponent(components.back()));
	EXPECT_TRUE(nbase::IsDirectoryComponent(std::string("test/")));
}

#endif  // WITH_UNITTEST
