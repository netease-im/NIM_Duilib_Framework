#include "stdafx.h"
#include "pin_yin_helper.h"
#include "shared/tool.h"

using namespace nbase;
using namespace shared;

#define HANZI_MIN 0x4E00
#define HANZI_MAX 0x9FA5

unsigned int pyrange[][3] = {
	{0xB0A1, 0xB0C4, 'a'},
	{0xB0C5, 0xB2C0, 'b'},
	{0xB2C1, 0xB4ED, 'c'},
	{0xB4EE, 0xB6E9, 'd'},
	{0xB6EA, 0xB7A1, 'e'},
	{0xB7A2, 0xB8C0, 'f'},
	{0xB8C1, 0xB9FD, 'g'},
	{0xB9FE, 0xBBF6, 'h'},
	{0xBBF7, 0xBFA5, 'j'},
	{0xBFA6, 0xC0AB, 'k'},
	{0xC0AC, 0xC2E7, 'l'},
	{0xC2E8, 0xC4C2, 'm'},
	{0xC4C3, 0xC5B5, 'n'},
	{0xC5B6, 0xC5BD, 'o'},
	{0xC5BE, 0xC6D9, 'p'},
	{0xC6DA, 0xC8BA, 'q'},
	{0xC8BB, 0xC8F5, 'r'},
	{0xC8F6, 0xCBF9, 's'},
	{0xCBFA, 0xCDD9, 't'},
	{0xCDDA, 0xCEF3, 'w'},
	{0xCEF4, 0xD1B8, 'x'},
	{0xD1B9, 0xD4D0, 'y'},
	{0xD4D1, 0xD7F9, 'z'}
};

const wchar_t  kFileName[] = L"translation.bin";

PinYinHelper::PinYinHelper() : file_handle_(nullptr), code_index_(nullptr), pinyin_(nullptr)
{
	LoadPinyinTable();
}

PinYinHelper::~PinYinHelper()
{
}

const char* PinYinHelper::ConvertToSimpleSpell(const std::wstring& chinese)
{
	if (chinese.empty() || !code_index_.get() || !pinyin_.get())
	{
		return "";
	}
	str_firstLetters_.clear();
	for (std::wstring::size_type i = 0; i < chinese.length(); i++)
	{
		wchar_t ch = chinese[i];
		const char *pinyin_char = NULL;
		if(ch >= HANZI_MIN && ch <= HANZI_MAX)
		{
			pinyin_char =  pinyin_.get() + code_index_[ch - HANZI_MIN];
			str_firstLetters_ += pinyin_char[0];
		}
		else
		{
			str_firstLetters_.push_back((char)ch);
		}
	}

	return str_firstLetters_.c_str();
}


std::wstring PinYinHelper::GetPinyinFilePath()
{
	std::wstring full_path;
	std::wstring path = nbase::win32::GetCurrentModuleDirectory();
	full_path = path + kFileName;
	return full_path;
}

bool PinYinHelper::LoadPinyinTable()
{
	if (code_index_.get() && pinyin_.get())
		return true;
	std::wstring path = GetPinyinFilePath();

	file_handle_.reset(nbase::OpenFile(path, L"rb"));
	if (!file_handle_)
	{
		//LOG_ERR("Open translator file error!");
		return false;
	}
	int hanzi_num = 0;
	int pinyin_size = 0;
	fread(&hanzi_num, 4, 1, file_handle_.get());
	fread(&pinyin_size, 4, 1, file_handle_.get());
	code_index_.reset(new int[4 * hanzi_num]);
	if (!code_index_.get())
	{
		//LOG_ERR("code_index_ New memory error!");
		return false;
	}
	pinyin_.reset(new char[pinyin_size]);
	if (!pinyin_.get())
	{
		//LOG_ERR("pinyin_ New memory error!");
		return false;
	}
	int ret;
	ret = fread(code_index_.get(), 4, hanzi_num, file_handle_.get());
	if (ret != hanzi_num)
	{
		//LOG_ERR("code_index_ Read file error!");
		return false;
	}
	ret = fread(pinyin_.get(), 1, pinyin_size, file_handle_.get());
	if (ret != pinyin_size)
	{
		//LOG_ERR("pinyin_ Read file error!");
		return false;
	}
	return true;
}

const char* PinYinHelper::ConvertToFullSpell(const std::wstring& chinese)
{
	if (chinese.empty() || !code_index_.get() || !pinyin_.get())
	{
		return "";
	}
	str_pinyin_.clear();
	for(unsigned int i = 0; i < chinese.length(); i++)
	{
		wchar_t ch = chinese[i];
		const char *pinyin_char = NULL;
		if(ch >= HANZI_MIN && ch <= HANZI_MAX)
		{
			pinyin_char =  pinyin_.get() + code_index_[ch - HANZI_MIN];
			str_pinyin_.append(pinyin_char);
		}
		else
		{
			str_pinyin_.push_back((char)ch);
		}
	}

	return str_pinyin_.c_str();
}
