#ifndef CHINESE_PIN_YIN_H_
#define CHINESE_PIN_YIN_H_

/*
 *
 *	Author       tom|yuzj
 *	Date         2013-10-10
 *	Brief        拼音
 *
 */

using namespace std;

class PinYinHelper
{
public:
	SINGLETON_DEFINE(PinYinHelper);

	PinYinHelper();
	~PinYinHelper();
	const char* ConvertToFullSpell(const std::wstring& chinese);
	const char* ConvertToSimpleSpell(const std::wstring& chinese);
private:
	std::wstring GetPinyinFilePath();
	bool LoadPinyinTable();
	
private:
	std::unique_ptr<FILE, nbase::DeleterFileClose> file_handle_;
	std::unique_ptr<int[]> code_index_; 
	std::unique_ptr<char[]> pinyin_;
	std::string str_pinyin_;
	std::string str_firstLetters_;
};
#endif//CHINESE_PIN_YIN_H_