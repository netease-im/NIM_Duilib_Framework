#pragma once
//#include "util.h"


/*  结合c#与cout的log
 *  示例：
 *  QLOG_ERR(L"I am {0},this is {1} year,you can also call me {2}") <<L"天外飞仙" <<2015 <<"Tom Stiven";
 */

enum LOG_LEVEL
{
	LV_ERR = 0,
	LV_WAR = 1,	
	LV_APP = 2,	
	LV_PRO = 3
};

class QLogHelper;

class QLogImpl
{
	friend class QLogHelper;
public:
	SINGLETON_DEFINE(QLogImpl);
	QLogImpl() { }
public:
	void SetLogFile( const std::wstring &file_path ); //full path
	std::wstring GetLogFile();
	void SetLogLevel( LOG_LEVEL lv );
	void HalfTo(long max, long to);
private:
	void WriteLog( LOG_LEVEL lv, const std::wstring &log );
private:
	std::wstring log_file_;
	LOG_LEVEL	 log_level_;

	nbase::NLock lock_;
};

class QLogHelper
{
public:
	QLogHelper( const char* file, long line );
	~QLogHelper();
	QLogHelper& VLog( LOG_LEVEL lv, const std::wstring &fmt );
	QLogHelper& operator<<(const std::wstring &str);
	QLogHelper& operator<<(const std::string &str);
	QLogHelper& operator<<(const long long lld);
private:
	size_t FindArg( int &len ); //返回"{x}"的索引，len代表"{x}"的长度
private:
	std::wstring fmt_;
	std::wstring string_;
	int			 index_;

	std::wstring time_;
	std::wstring file_line_;
	LOG_LEVEL	 level_;
};

#define QLOG_PRO(fmt) QLogHelper(__FILE__, __LINE__).VLog(LV_PRO, fmt)
#define QLOG_APP(fmt) QLogHelper(__FILE__, __LINE__).VLog(LV_APP, fmt)
#define QLOG_WAR(fmt) QLogHelper(__FILE__, __LINE__).VLog(LV_WAR, fmt)
#define QLOG_ERR(fmt) QLogHelper(__FILE__, __LINE__).VLog(LV_ERR, fmt)
