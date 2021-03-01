#include "StdAfx.h"

namespace ui 
{

FontManager* FontManager::GetInstance()
{
	static FontManager fontManager;
	return &fontManager;
}

void FontManager::AddFontResource(const std::wstring& strFontFile, const std::wstring& strFontName)
{
	std::wstring path = GlobalManager::GetResPath(L"font\\" + strFontFile, L"");
	bool res = false;
	if (GlobalManager::IsUseZip()) {
		HGLOBAL hGlobal = GlobalManager::GetZipData(path);
		if (hGlobal) {
			void *data = GlobalLock(hGlobal);
			DWORD len = GlobalSize(hGlobal);
			DWORD dwFonts = 0;
			HANDLE handle = AddFontMemResourceEx(data, len, NULL, &dwFonts);
			res = handle != NULL;
			if (res)
				m_fontHandles.push_back(handle);

			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal);
		}
	}
	else {
		res = AddFontResourceEx(path.c_str(), FR_PRIVATE, 0) != 0;
	}
	assert(res);
	if (res) {
		m_fontPath.push_back(path);
		m_fontNames.push_back(strFontName);
	}
}

const std::vector<std::wstring>& FontManager::GetFontNames()
{
	return m_fontNames;
}

void FontManager::Clear()
{
	for (auto &it : m_fontHandles) {
		assert(RemoveFontMemResourceEx(it));
	}
	for (auto &it : m_fontPath) {
		assert(RemoveFontResourceEx(it.c_str(), FR_PRIVATE, 0));
	}
	m_fontHandles.clear();
	m_fontPath.clear();
	m_fontNames.clear();
}

FontManager::~FontManager()
{
	Clear();
}

}