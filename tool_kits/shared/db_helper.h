#pragma once

#include "base/memory/singleton.h"
#include "base/synchronization/lock.h"
#include "base/callback/callback.h"
#include "db/db_pretreatment.h"
#include "shared/tool.h"
namespace shared
{
typedef int NimDBVersionType;
class NimDBPretreatment : public ndb::DBPretreatment<NimDBVersionType>, public ndb::DBPretreatment<NimDBVersionType>::FileSystem
{
public:
	virtual bool FilePathIsExist(const std::string& file_path, bool is_directory) override;
	virtual bool DeleteFile(const std::string& file_path) override;
	virtual bool CopyFile(const std::string &from_path, const std::string &to_path, bool fail_if_exists) override;
	virtual bool CreateDir(const std::string& dir_path) override;
	virtual bool GetDirFromPath(const std::string& file_path, std::string& dir) override;//dir º¬Î´Î²µÄ"/" or "\\"
	virtual bool GetFileNameFromPath(const std::string& file_path, std::string& name, std::string& ext) override;
	virtual bool MoveFile(const std::string &from_path, const std::string &to_path) override;
	virtual void ClearTLSLastError() override;
	virtual unsigned int GetTLSLastError() override;
	virtual void LockDBFile() override;
	virtual void UnLockDBFile() override;
protected:
	nbase::NLock    db_lock_;
};
class NimDatabase : public NimDBPretreatment, virtual public nbase::SupportWeakCallback
{
public:
	NimDatabase(NimDBVersionType base_version, NimDBVersionType newest_version) : 
		base_version_(base_version), newest_version_(newest_version)
	{
	}
public:
	bool CreateDB()
	{ 
		PretreatmentConfig&& config = CreatePretreatmentConfig();
		config.db_path_ = std::move(GetDBPath());
		config.base_version_ = base_version_;
		config.newest_version_ = newest_version_;
		if (config.newest_version_ < config.base_version_)
			config.newest_version_ = config.base_version_;
		if (!config.db_path_.empty() && OnBeforCreateDatabase(config))
		{			
			NimDBPretreatment::Init(config);
			return NimDBPretreatment::DoPretreatment(this, GetEncryptKey(), create_db_sqls_, update_function_list_);
		}
		return false;
	};
	virtual bool CloseDB() override
	{ 
		OnBeforCloseDatabase(); 
		return NimDBPretreatment::CloseDB();;
	};
protected:
	virtual void AddCreateDBSql(const std::string& sql)
	{
		create_db_sqls_.push_back(sql);
	}
	virtual void AddCreateDBSql(std::string&& sql)
	{
		create_db_sqls_.push_back(std::forward<std::string>(sql));
	}
	virtual void AddUpdateFunction(NimDBVersionType version, const DBUpdateFunc& opration)
	{
		update_function_list_.insert(std::make_pair(version, opration));
	}
	virtual void AddUpdateFunction(NimDBVersionType version, DBUpdateFunc&& opration)
	{
		update_function_list_.insert(std::make_pair(version, std::forward<DBUpdateFunc>(opration)));
	}
	virtual void OnClear() override
	{
		create_db_sqls_.clear();
		update_function_list_.clear();
	}
	virtual bool OnBeforCreateDatabase(PretreatmentConfig& config)
	{
		return true;
	}
	virtual bool OnBeforCloseDatabase()
	{
		return true;
	}
	virtual std::string GetEncryptKey() const
	{
		return "";
	}
	virtual std::string GetDBPath() const = 0;
private:
	inline PretreatmentConfig CreatePretreatmentConfig()
	{
		PretreatmentConfig config;
		config.back_db_dir_ = "";
		config.enable_backup_ = true;
		config.enable_restore_ = false;
		config.enable_def_restore_ = false;
		return config;
	}
private:
	std::list<std::string> create_db_sqls_;
	DBUpdateFuncList update_function_list_;
	NimDBVersionType base_version_;
	NimDBVersionType newest_version_;
};
}