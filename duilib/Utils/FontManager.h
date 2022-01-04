#ifndef UI_UTILS_FONTMANAGER_H_
#define UI_UTILS_FONTMANAGER_H_

#pragma once
namespace Gdiplus{
	class PrivateFontCollection;
}
namespace ui 
{

/** @class FontManager
  * @brief 自定义字体管理类
  * @copyright (c) 2021, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2021/2/26
  */
class UILIB_API FontManager
{
public:
	static FontManager* GetInstance();

	/** @brief 添加一个字体文件
	  * @param[in] strFontFile 字体文件名
	  * @param[in] strFontName 字体名
	  * @return 无返回值
	  */
	void AddFontResource(const std::wstring& strFontFile, const std::wstring& strFontName);

	/** @brief 获取所有添加的字体的名字
	  * @return 字体名字的集合
	  */
	const std::vector<std::wstring>& GetFontNames();

	/** @brief 清理所有添加的字体
	  * @return 无返回值
	  */
	void Clear();

private:
	FontManager() {};
	~FontManager();
	FontManager(const FontManager&) = delete;
	FontManager& operator = (const FontManager&) = delete;

private:
	std::vector<std::wstring>  m_fontPath;
	std::vector<std::wstring>  m_fontNames;
	std::vector<HANDLE>  m_fontHandles;
};

}
#endif //UI_UTILS_FONTMANAGER_H_
