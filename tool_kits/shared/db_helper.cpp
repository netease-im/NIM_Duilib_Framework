#include "stdafx.h"
#include "shared/db_helper.h"
#include "shared/utf8_file_util.h"
#include "base/util/string_util.h"
namespace shared
{

	bool NimDBPretreatment::FilePathIsExist(const std::string& file_path, bool is_directory)
	{
		return shared::FilePathIsExist(file_path, is_directory);
	}
	bool NimDBPretreatment::DeleteFile(const std::string& file_path)
	{
		return shared::DeleteFile(file_path) && (GetLastError() == 0);
	}
	bool NimDBPretreatment::CopyFile(const std::string &from_path, const std::string &to_path, bool fail_if_exists)
	{
		return shared::CopyFile(from_path, to_path) && (GetLastError() == 0);
	}
	bool NimDBPretreatment::CreateDir(const std::string& dir_path)
	{
		return shared::CreateDirectory(dir_path);
	}
	bool NimDBPretreatment::GetDirFromPath(const std::string& file_path, std::string& dir)
	{
		return shared::FilePathApartDirectory(file_path, dir);
	}
	bool NimDBPretreatment::GetFileNameFromPath(const std::string& file_path, std::string& name, std::string& ext)
	{
		std::string file_name;
		if (!shared::FilePathApartFileName(file_path, name))
			return false;
		if (name.empty())
			return false;
		shared::FilePathExtension(file_name, ext);
		return true;
	}
	bool NimDBPretreatment::MoveFile(const std::string &from_path, const std::string &to_path)
	{
		std::wstring w_from_path = nbase::UTF8ToUTF16(from_path);
		std::wstring w_to_path = nbase::UTF8ToUTF16(to_path);	
		return (::MoveFile(w_from_path.c_str(), w_to_path.c_str()) == TRUE);
		//return (::MoveFileA(from_path.c_str(), to_path.c_str()) == TRUE);
	}
	void NimDBPretreatment::ClearTLSLastError()
	{
		::SetLastError(0);
	}
	unsigned int NimDBPretreatment::GetTLSLastError()
	{
		return (unsigned int)GetLastError();
	}
	void NimDBPretreatment::LockDBFile()
	{
		db_lock_.Lock();
	}
	void NimDBPretreatment::UnLockDBFile()
	{
		db_lock_.Unlock();
	}
}