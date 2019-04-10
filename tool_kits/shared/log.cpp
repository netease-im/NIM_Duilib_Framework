#include "stdafx.h"
#include "log.h"



void QLogImpl::SetLogFile( const std::wstring &file_path )
{
	log_file_ = file_path;
}

std::wstring QLogImpl::GetLogFile()
{
	return log_file_;
}

void QLogImpl::SetLogLevel( LOG_LEVEL lv )
{
	log_level_ = lv;
}

void QLogImpl::WriteLog( LOG_LEVEL lv, const std::wstring &log )
{
#ifdef _DEBUG
	fwprintf_s( stdout, L"%s", log.c_str() );
#endif

#ifdef _WINDOWS
	OutputDebugStringW( log.c_str() );
#endif

	if( lv > log_level_ )
		return;

	if( log_file_.empty() )
		return;

	std::string str = nbase::UTF16ToUTF8(log);

	nbase::NAutoLock auto_lock( &lock_ );

	bool exist = nbase::FilePathIsExist(log_file_, false);

	std::unique_ptr<FILE, nbase::DeleterFileClose> fp;
	FILE* fp_file = nullptr;
	_wfopen_s(&fp_file, log_file_.c_str(), L"a+");
	fp.reset(fp_file);
	if( !fp )
		return;

	if( !exist )
	{
		const char utf8[3] = { (char) 0xEF, (char) 0xBB, (char) 0xBF };
		fwrite(utf8, sizeof(char), 3, fp.get() );
	}

	fwrite(str.c_str(), sizeof(char), str.size(), fp.get());
	fp.reset(NULL);
}

void QLogImpl::HalfTo( long max, long to )
{
	nbase::NAutoLock auto_lock( &lock_);

	//打开文件
	std::unique_ptr<FILE, nbase::DeleterFileClose> fp;
	FILE* fp_file = nullptr;
	_wfopen_s(&fp_file, log_file_.c_str(), L"r");
	fp.reset(fp_file);
	if(!fp)
	{
		fp.reset(NULL);
		return;
	}

	//获取长度
	int ret = fseek(fp.get(), 0L, SEEK_END);
	if (ret != 0)
	{
		return;
	}

	//小于max则直接返回
	long len = ftell(fp.get());
	if(len <= max)
	{
		return;
	}

	//大于max，只留下最后to
	len = max * (-1);
	ret = fseek(fp.get(), len, SEEK_END);
	if (ret != 0)
	{
		return;
	}

	//创建新文件
	nbase::PathString new_file = log_file_ + L".old";
	std::unique_ptr<FILE, nbase::DeleterFileClose> fp2;
	FILE* fp_file2 = nullptr;
	_wfopen_s(&fp_file2, new_file.c_str(), L"w");
	fp2.reset(fp_file2);
	if(!fp2)
	{
		return;
	}

	//写入新文件
	char cbuf[12 * 1024] = { 0 };
	int cn = sizeof(cbuf), n = 0;
	while(!feof(fp.get()))
	{
		n = fread_s(cbuf, cn, sizeof(char), cn, fp.get());
		if(n > 0)
		{
			fwrite(cbuf, sizeof(char), n, fp2.get());
		}
		else
		{
			break;
		}
	}
	fp.reset(NULL);
	fp2.reset(NULL);

	//文件替换
	bool del = nbase::DeleteFileW(log_file_);
	if(del)
	{
		::_wrename(new_file.c_str(), log_file_.c_str());
	}
	else
	{
		nbase::DeleteFileW(new_file);
	}
}


QLogHelper::QLogHelper( const char* file, long line )
{
	index_ = 0;
	level_ = LV_PRO;

	nbase::Time::TimeStruct qt = nbase::Time::Now().ToTimeStruct(true);
	time_ = nbase::StringPrintf( L"[%04d-%02d-%02d %02d:%02d:%02d", 
		qt.year_, qt.month_, qt.day_of_month_, qt.hour_, qt.minute_, qt.second_);

	std::wstring str = nbase::UTF8ToUTF16(file);
	std::wstring file_name;
	nbase::FilePathApartFileName(str, file_name);

	file_line_ = nbase::StringPrintf( L" %s:%ld ", file_name.c_str(), line );
}

QLogHelper::~QLogHelper()
{
	std::wstring lv;
	switch( level_ )
	{
	case LV_ERR:
		lv = L"LV_ERR";
		break;
	case LV_WAR:
		lv = L"LV_WAR";
		break;
	case LV_APP:
		lv = L"LV_APP";
		break;
	case LV_PRO:
		lv = L"LV_PRO";
		break;
	default:
		assert( 0 );
		break;
	}
	lv.append( L"] " );

	if( string_.empty() )
		string_ = fmt_;
	else if( !fmt_.empty() )
		string_.append(fmt_);

	std::wstring log = time_ + file_line_ + lv + string_ + L"\r\n";
	QLogImpl::GetInstance()->WriteLog( level_, log );
}

QLogHelper& QLogHelper::VLog( LOG_LEVEL lv, const std::wstring &fmt )
{
	level_ = lv;
	fmt_ = fmt;
	return *this;
}

QLogHelper& QLogHelper::operator<<(const std::wstring &str)
{
	int len = 0;
	size_t pos = FindArg( len );
	if( pos == fmt_.npos )
	{
		assert( 0 );
	}
	else
	{
		string_.append( fmt_.substr( 0, pos ) );
		string_.append( str );

		fmt_.erase( 0, pos + len );
	}

	index_++;
	return *this;
}

QLogHelper& QLogHelper::operator<<(const std::string &str)
{
	std::wstring wstr = nbase::UTF8ToUTF16(str);
	return this->operator<<(wstr);
}

QLogHelper& QLogHelper::operator<<(const long long lld)
{
	int len = 0;
	size_t pos = FindArg( len );
	if( pos == fmt_.npos )
	{
		assert( 0 );
	}
	else
	{
		std::wstring data = nbase::Int64ToString16(lld);

		string_.append( fmt_.substr( 0, pos ) );
		string_.append( data );

		fmt_.erase( 0, pos + len );
	}

	index_++;
	return *this;
}

size_t QLogHelper::FindArg( int &len )
{
	assert( !fmt_.empty() );
	assert( index_ >= 0 && index_ <= 20 );

	std::wstring str = nbase::StringPrintf(L"{%d}", index_);
	len = str.size();

	return fmt_.find( str );
}