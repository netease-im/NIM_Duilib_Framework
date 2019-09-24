#include "StdAfx.h"
#include <zmouse.h>
#include <shlwapi.h>
#include "Utils/UnZip.h"

namespace ui
{

std::wstring GlobalManager::m_pStrResourcePath;
std::vector<Window*> GlobalManager::m_aPreMessages;
std::map<std::wstring, std::unique_ptr<WindowBuilder>> GlobalManager::m_builderMap;
CreateControlCallback GlobalManager::m_createControlCallback;

GlobalManager::MapStringToImagePtr GlobalManager::m_mImageHash;
std::map<std::wstring, DWORD> GlobalManager::m_mapTextColor;
std::map<std::wstring, std::wstring> GlobalManager::m_mGlobalClass;
std::map<std::wstring, TFontInfo*> GlobalManager::m_mCustomFonts;
std::wstring GlobalManager::m_sDefaultFontId;

short GlobalManager::m_H = 180;
short GlobalManager::m_S = 100;
short GlobalManager::m_L = 100;

std::wstring GlobalManager::m_strDefaultFontName;
std::wstring GlobalManager::m_strDefaultDisabledColor = L"textdefaultdisablecolor";
std::wstring GlobalManager::m_strDefaultFontColor = L"textdefaultcolor";
DWORD GlobalManager::m_dwDefaultLinkFontColor = 0xFF0000FF;
DWORD GlobalManager::m_dwDefaultLinkHoverFontColor = 0xFFD3215F;
DWORD GlobalManager::m_dwDefaultSelectedBkColor = 0xFFBAE4FF;

std::unique_ptr<IRenderFactory> GlobalManager::m_renderFactory;

static ULONG_PTR g_gdiplusToken;
static Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
static HZIP g_hzip = NULL;

void GlobalManager::Startup(const std::wstring& strResourcePath, const CreateControlCallback& callback, bool bAdaptDpi, const std::wstring& theme, const std::wstring& language)
{
	m_renderFactory = std::make_unique<RenderFactory_GdiPlus>();
	GlobalManager::SetResourcePath(strResourcePath + theme);
	m_createControlCallback = callback;

	// ����DPI
	if (bAdaptDpi) {
		DpiManager::GetInstance()->SetAdaptDPI();
		DpiManager::GetInstance()->SetScale(DpiManager::GetMainMonitorDPI());
	}

	// ����ȫ����Դ��Ϣ
	LoadGlobalResource();

	// ���ض������ļ������ʹ������Դѹ��������ڴ��м��������ļ�
	if (g_hzip) {
		HGLOBAL hGlobal = GetData(strResourcePath + language + L"\\gdstrings.ini");
		if (hGlobal) {
			ui::MutiLanSupport::GetInstance()->LoadStringTable(hGlobal);
			GlobalFree(hGlobal);
		}
	}
	else {
		MutiLanSupport::GetInstance()->LoadStringTable(strResourcePath + language + L"\\gdstrings.ini");
	}

	GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
	// Boot Windows Common Controls (for the ToolTip control)
	::InitCommonControls();
}

void GlobalManager::Shutdown()
{
	if (g_hzip)
	{
		CloseZip(g_hzip);
		g_hzip = NULL;
	}
	m_renderFactory.reset();
	RemoveAllFonts();
	Gdiplus::GdiplusShutdown(g_gdiplusToken);
}

std::wstring GlobalManager::GetCurrentPath()
{
	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	::GetCurrentDirectory(MAX_PATH, tszModule);
	return tszModule;
}

std::wstring GlobalManager::GetResourcePath()
{
	return m_pStrResourcePath;
}

void GlobalManager::SetCurrentPath(const std::wstring& strPath)
{
	::SetCurrentDirectory(strPath.c_str());
}

void GlobalManager::SetResourcePath(const std::wstring& strPath)
{
	m_pStrResourcePath = strPath;
	if (m_pStrResourcePath.empty()) return;
	TCHAR cEnd = m_pStrResourcePath.at(m_pStrResourcePath.length() - 1);
	if (cEnd != _T('\\') && cEnd != _T('/')) m_pStrResourcePath += _T('\\');
}

void GlobalManager::LoadGlobalResource()
{
	ui::WindowBuilder dialog_builder;
	ui::Window paint_manager;
	dialog_builder.Create(L"global.xml", CreateControlCallback(), &paint_manager);
}

void GlobalManager::ReloadSkin(const std::wstring& resourcePath)
{
	RemoveAllFonts();
	RemoveAllTextColors();
	RemoveAllClasss();
	RemoveAllImages();

	SetResourcePath(resourcePath);
	LoadGlobalResource();

	for (auto it = m_aPreMessages.begin(); it != m_aPreMessages.end(); it++) {
		(*it)->GetRoot()->Invalidate();
	}
}

ui::IRenderFactory* GlobalManager::GetRenderFactory()
{
	return m_renderFactory.get();
}

std::unique_ptr<ui::IRenderContext> GlobalManager::CreateRenderContext()
{
	std::unique_ptr<ui::IRenderContext> p;
	p.reset(m_renderFactory->CreateRenderContext());
	return p;
}

std::unique_ptr<ui::IPen> GlobalManager::CreatePen(DWORD color, int width /*= 1*/)
{
	std::unique_ptr<ui::IPen> p;
	p.reset(m_renderFactory->CreatePen(color, width));
	return p;
}

std::unique_ptr<ui::IBrush> GlobalManager::CreateBrush(DWORD color)
{
	std::unique_ptr<ui::IBrush> p;
	p.reset(m_renderFactory->CreateBrush(color));
	return p;
}

std::unique_ptr<ui::IMatrix> GlobalManager::CreateMatrix()
{
	std::unique_ptr<ui::IMatrix> p;
	p.reset(m_renderFactory->CreateMatrix());
	return p;
}

std::unique_ptr<ui::IPath> GlobalManager::CreatePath()
{
	std::unique_ptr<ui::IPath> p;
	p.reset(m_renderFactory->CreatePath());
	return p;
}

void GlobalManager::AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList)
{
	ASSERT(!strClassName.empty());
	ASSERT(!strControlAttrList.empty());
	m_mGlobalClass[strClassName] = strControlAttrList;
}

std::wstring GlobalManager::GetClassAttributes(const std::wstring& strClassName)
{
	auto it = m_mGlobalClass.find(strClassName);
	if (it != m_mGlobalClass.end()) {
		return it->second;
	}

	return L"";
}

void GlobalManager::RemoveAllClasss()
{
	m_mGlobalClass.clear();
}

void GlobalManager::AddTextColor(const std::wstring& strName, const std::wstring& strValue)
{
	std::wstring strColor = strValue.substr(1);
	LPTSTR pstr = NULL;
	DWORD dwBackColor = _tcstoul(strColor.c_str(), &pstr, 16);

	m_mapTextColor[strName] = dwBackColor;
}

DWORD GlobalManager::GetTextColor(const std::wstring& strName)
{
	// ������global.xml����ǰ���嵽��ɫֵ
	ASSERT(m_mapTextColor[strName] != 0);
	return m_mapTextColor[strName];
}

void GlobalManager::RemoveAllTextColors()
{
	m_mapTextColor.clear();
}

std::shared_ptr<ImageInfo> GlobalManager::IsImageCached(const std::wstring& strImagePath)
{
	std::wstring imageFullPath = StringHelper::ReparsePath(strImagePath);
	std::shared_ptr<ImageInfo> sharedImage;
	auto it = m_mImageHash.find(imageFullPath);
	if (it != m_mImageHash.end()) {
		sharedImage = it->second.lock();
	}

	return sharedImage;
}

std::shared_ptr<ImageInfo> GlobalManager::AddImageCached(const std::shared_ptr<ImageInfo>& sharedImage)
{
	ASSERT(m_mImageHash[sharedImage->sImageFullPath].expired() == true);
	m_mImageHash[sharedImage->sImageFullPath] = sharedImage;
	sharedImage->SetCached(true);
	return sharedImage;
}

void GlobalManager::RemoveFromImageCache(const std::wstring& imageFullPath)
{
	auto it = m_mImageHash.find(imageFullPath);
	if (it != m_mImageHash.end()) {
		m_mImageHash.erase(it);
	}
	else {
		ASSERT(FALSE);
	}
}

void GlobalManager::OnImageInfoDestroy(ImageInfo* pImageInfo)
{
	ASSERT(pImageInfo);
	if (pImageInfo && pImageInfo->IsCached()) {
		if (!pImageInfo->sImageFullPath.empty()) {
			GlobalManager::RemoveFromImageCache(pImageInfo->sImageFullPath);
		}
	}
	delete pImageInfo;
}

std::shared_ptr<ImageInfo> GlobalManager::GetImage(const std::wstring& bitmap)
{
	std::wstring imageFullPath = StringHelper::ReparsePath(bitmap);
	if (IsUseZip())
	{
		imageFullPath = GetZipFilePath(imageFullPath);
	}
	std::shared_ptr<ImageInfo> sharedImage;
	auto it = m_mImageHash.find(imageFullPath);
	if (it == m_mImageHash.end()) {
		std::unique_ptr<ImageInfo> data;
		if (IsUseZip())
		{
			data = ImageInfo::LoadImage(GetData(imageFullPath), imageFullPath);
		}
		if (!data)
		{
			data = ImageInfo::LoadImage(imageFullPath);
		}
		if (!data) return sharedImage;
		sharedImage.reset(data.release(), &OnImageInfoDestroy);
		m_mImageHash[imageFullPath] = sharedImage;
		sharedImage->SetCached(true);
	}
	else {
		sharedImage = it->second.lock();
	}

	return sharedImage;
}

std::wstring GlobalManager::GetDefaultFontName()
{
	return m_strDefaultFontName;
}

void GlobalManager::RemoveAllImages()
{
	for (auto it = m_aPreMessages.begin(); it != m_aPreMessages.end(); it++) {
		(*it)->GetRoot()->ClearImageCache();
	}

	m_mImageHash.clear();
}

HFONT GlobalManager::AddFont(const std::wstring& strFontId, const std::wstring& strFontName, int nSize, bool bBold, bool bUnderline, bool bStrikeOut, bool bItalic, bool bDefault)
{
	std::wstring strNewFontId = strFontId;
	if (strNewFontId.empty())
	{
		strNewFontId = std::to_wstring(m_mCustomFonts.size());
	}

	auto iter = m_mCustomFonts.find(strNewFontId);
	ASSERT(iter == m_mCustomFonts.end());

	static bool bOsOverXp = IsWindowsVistaOrGreater();
	std::wstring fontName = strFontName;
	if (fontName == L"system") {
		fontName = bOsOverXp ? L"΢���ź�" : L"������";
	}

	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	_tcscpy(lf.lfFaceName, fontName.c_str());
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = -DpiManager::GetInstance()->ScaleInt(nSize);
	if (bBold) lf.lfWeight += FW_BOLD;
	if (bUnderline) lf.lfUnderline = TRUE;
	if (bStrikeOut) lf.lfStrikeOut = TRUE;
	if (bItalic) lf.lfItalic = TRUE;
	HFONT hFont = ::CreateFontIndirect(&lf);
	if (hFont == NULL) return NULL;

	TFontInfo* pFontInfo = new TFontInfo;
	if (!pFontInfo) return false;
	pFontInfo->hFont = hFont;
	pFontInfo->sFontName = fontName;
	pFontInfo->iSize = nSize;
	pFontInfo->bBold = bBold;
	pFontInfo->bUnderline = bUnderline;
	pFontInfo->bStrikeOut = bStrikeOut;
	pFontInfo->bItalic = bItalic;
	::ZeroMemory(&pFontInfo->tm, sizeof(pFontInfo->tm));

	m_mCustomFonts.insert(std::make_pair(strNewFontId, pFontInfo));

	if (bDefault) m_sDefaultFontId = strNewFontId;

	return hFont;
}

TFontInfo* GlobalManager::GetTFontInfo(const std::wstring& strFontId)
{
	std::wstring strFindId = strFontId;
	if (strFindId.empty())
	{
		ASSERT(!m_sDefaultFontId.empty());
		strFindId = m_sDefaultFontId;
	}

	auto iter = m_mCustomFonts.find(strFindId);
	ASSERT(iter != m_mCustomFonts.end());

	TFontInfo* pFontInfo = static_cast<TFontInfo*>(iter->second);
	return pFontInfo;
}

HFONT GlobalManager::GetFont(const std::wstring& strFontId)
{
	TFontInfo* pFontInfo = GetTFontInfo(strFontId);
	if (pFontInfo)
		return pFontInfo->hFont;
	return nullptr;
}

HFONT GlobalManager::GetFont(const std::wstring& strFontName, int nSize, bool bBold, bool bUnderline, bool bStrikeOut, bool bItalic)
{
	for (auto it = m_mCustomFonts.begin(); it != m_mCustomFonts.end(); it++) {
		auto pFontInfo = it->second;
		if (pFontInfo->sFontName == strFontName && pFontInfo->iSize == nSize &&
			pFontInfo->bBold == bBold && pFontInfo->bUnderline == bUnderline && 
			pFontInfo->bStrikeOut == bStrikeOut && pFontInfo->bItalic == bItalic)
			return pFontInfo->hFont;
	}
	return NULL;
}

TFontInfo* GlobalManager::GetFontInfo(const std::wstring& strFontId, HDC hDcPaint)
{
	TFontInfo* pFontInfo = GetTFontInfo(strFontId);
	if (pFontInfo->tm.tmHeight == 0) {
		HFONT hOldFont = (HFONT) ::SelectObject(hDcPaint, pFontInfo->hFont);
		::GetTextMetrics(hDcPaint, &pFontInfo->tm);
		::SelectObject(hDcPaint, hOldFont);
	}
	return pFontInfo;
}

TFontInfo* GlobalManager::GetFontInfo(HFONT hFont, HDC hDcPaint)
{
	for (auto it = m_mCustomFonts.begin(); it != m_mCustomFonts.end(); it++) {
		auto pFontInfo = it->second;
		if (pFontInfo->hFont == hFont) {
			if (pFontInfo->tm.tmHeight == 0) {
				HFONT hOldFont = (HFONT) ::SelectObject(hDcPaint, pFontInfo->hFont);
				::GetTextMetrics(hDcPaint, &pFontInfo->tm);
				::SelectObject(hDcPaint, hOldFont);
			}
			return pFontInfo;
		}
	}

	ASSERT(FALSE);
	return NULL;
}

bool GlobalManager::FindFont(HFONT hFont)
{
	for (auto it = m_mCustomFonts.begin(); it != m_mCustomFonts.end(); it++) {
		auto pFontInfo = it->second;
		if (pFontInfo->hFont == hFont)
			return true;
	}
	return false;
}

bool GlobalManager::FindFont(const std::wstring& strFontName, int nSize, bool bBold, bool bUnderline, bool bStrikeOut, bool bItalic)
{
	for (auto it = m_mCustomFonts.begin(); it != m_mCustomFonts.end(); it++) {
		auto pFontInfo = it->second;
		if (pFontInfo->sFontName == strFontName && pFontInfo->iSize == nSize &&
			pFontInfo->bBold == bBold && pFontInfo->bUnderline == bUnderline && 
			pFontInfo->bStrikeOut == bStrikeOut && pFontInfo->bItalic == bItalic)
			return true;
	}
	return false;
}

bool GlobalManager::RemoveFontAt(const std::wstring& strFontId)
{
	auto iter = m_mCustomFonts.find(strFontId);
	if (iter == m_mCustomFonts.end()) return false;

	TFontInfo* pFontInfo = static_cast<TFontInfo*>(iter->second);
	::DeleteObject(pFontInfo->hFont);
	delete pFontInfo;

	m_mCustomFonts.erase(iter);

	return true;
}

void GlobalManager::RemoveAllFonts()
{
	for (auto it = m_mCustomFonts.begin(); it != m_mCustomFonts.end(); it++) {
		auto pFontInfo = it->second;
		::DeleteObject(pFontInfo->hFont);
		delete pFontInfo;
	}
	m_mCustomFonts.clear();
}

std::wstring GlobalManager::GetDefaultDisabledTextColor()
{
	return m_strDefaultDisabledColor;
}

void GlobalManager::SetDefaultDisabledTextColor(const std::wstring& strColor)
{
	m_strDefaultDisabledColor = strColor;
}

std::wstring GlobalManager::GetDefaultTextColor()
{
	return m_strDefaultFontColor;
}

void GlobalManager::SetDefaultTextColor(const std::wstring& strColor)
{
	m_strDefaultFontColor = strColor;
}

DWORD GlobalManager::GetDefaultLinkFontColor()
{
	return m_dwDefaultLinkFontColor;
}

void GlobalManager::SetDefaultLinkFontColor(DWORD strColor)
{
	m_dwDefaultLinkFontColor = strColor;
}

DWORD GlobalManager::GetDefaultLinkHoverFontColor()
{
	return m_dwDefaultLinkHoverFontColor;
}

void GlobalManager::SetDefaultLinkHoverFontColor(DWORD dwColor)
{
	m_dwDefaultLinkHoverFontColor = dwColor;
}

DWORD GlobalManager::GetDefaultSelectedBkColor()
{
	return m_dwDefaultSelectedBkColor;
}

void GlobalManager::SetDefaultSelectedBkColor(DWORD dwColor)
{
	m_dwDefaultSelectedBkColor = dwColor;
}

Box* GlobalManager::CreateBox(const std::wstring& strXmlPath, CreateControlCallback callback)
{
	WindowBuilder builder;
	Box* box = builder.Create(strXmlPath.c_str(), callback);
	ASSERT(box);

	return box;
}

Box* GlobalManager::CreateBoxWithCache(const std::wstring& strXmlPath, CreateControlCallback callback)
{
	Box* box = nullptr;
	auto it = m_builderMap.find(strXmlPath);
	if (it == m_builderMap.end()) {
		WindowBuilder* builder = new WindowBuilder();
		box = builder->Create(strXmlPath.c_str(), callback);
		if (box) {
			m_builderMap[strXmlPath].reset(builder);
		}
		else {
			ASSERT(FALSE);
		}
	}
	else {
		box = it->second->Create(callback);
	}

	return box;
}

void GlobalManager::FillBox(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback)
{
	WindowBuilder winBuilder;
	Box* box = winBuilder.Create(strXmlPath.c_str(), callback, pUserDefinedBox->GetWindow(), nullptr, pUserDefinedBox);
	ASSERT(box);

	return;
}

void GlobalManager::FillBoxWithCache(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback)
{
	Box* box = nullptr;
	auto it = m_builderMap.find(strXmlPath);
	if (it == m_builderMap.end()) {
		WindowBuilder* winBuilder = new WindowBuilder();
		box = winBuilder->Create(strXmlPath.c_str(), callback, pUserDefinedBox->GetWindow(), nullptr, pUserDefinedBox);
		if (box) {
			m_builderMap[strXmlPath].reset(winBuilder);
		}
		else {
			ASSERT(FALSE);
		}
	}
	else {
		box = it->second->Create(callback, pUserDefinedBox->GetWindow(), nullptr, pUserDefinedBox);
	}

	return;
}

Control* GlobalManager::CreateControl(const std::wstring& strControlName)
{
	if (m_createControlCallback) {
		return m_createControlCallback(strControlName);
	}

	return nullptr;
}

bool GlobalManager::IsUseZip()
{
	return g_hzip != NULL;
}

bool GlobalManager::OpenResZip(LPCTSTR  resource_name, LPCTSTR  resource_type, const std::string& password)
{
	HRSRC   rsc = FindResource(NULL, resource_name, resource_type);
	HGLOBAL resource = LoadResource(NULL, rsc);
	DWORD   size = SizeofResource(NULL, rsc);
	if (resource && size > 0)
	{

	}

	if (g_hzip)
	{
		CloseZip(g_hzip);
		g_hzip = NULL;
	}
	g_hzip = OpenZip(resource, size, password.c_str());
	return g_hzip != NULL;
}

bool GlobalManager::OpenResZip(const std::wstring& path, const std::string& password)
{
	if (g_hzip)
	{
		CloseZip(g_hzip);
		g_hzip = NULL;
	}
	g_hzip = OpenZip(path.c_str(), password.c_str());
	return g_hzip != NULL;
}

HGLOBAL GlobalManager::GetData(const std::wstring& path)
{
	HGLOBAL hGlobal = NULL;
	std::wstring file_path = GetZipFilePath(path);
	if (g_hzip && !file_path.empty())
	{
		ZIPENTRY ze;
		int i = 0;
		if (FindZipItem(g_hzip, file_path.c_str(), true, &i, &ze) == ZR_OK)
		{
			if (ze.index >= 0)
			{
				hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, ze.unc_size);
				if (hGlobal)
				{
					TCHAR *pData = (TCHAR*)GlobalLock(hGlobal);
					if (pData)
					{
						ZRESULT res = UnzipItem(g_hzip, ze.index, pData, ze.unc_size);
						GlobalUnlock(hGlobal);
						if (res != ZR_OK)
						{
							GlobalFree(hGlobal);
							hGlobal = NULL;
						}
					}
					else
					{
						GlobalFree(hGlobal);
						hGlobal = NULL;
					}
				}
			}
		}
	}

	return hGlobal;
}

std::wstring GlobalManager::GetZipFilePath(const std::wstring& path)
{
	std::wstring file_path = path;
	StringHelper::ReplaceAll(L"\\", L"/", file_path);
	StringHelper::ReplaceAll(L"//", L"/", file_path);
	for (unsigned int i = 0; i < file_path.size();)
	{
		bool start_node = false;
		if (i == 0 || file_path.at(i - 1) == L'/')
		{
			start_node = true;
		}
		WCHAR wch = file_path.at(i);
		if (start_node && wch == L'/')//"//"
		{
			file_path.erase(i, 1);
			continue;
		}
		if (start_node && wch == L'.')
		{
			if (i + 1 < file_path.size() && file_path.at(i + 1) == L'/')// "./"
			{
				file_path.erase(i, 2);
				continue;
			}
			else if (i + 2 < file_path.size() && file_path.at(i + 1) == L'.' && file_path.at(i + 2) == L'/')// "../"
			{
				file_path.erase(i, 2);
				int i_erase = i - 2;
				if (i_erase < 0)
				{
					ASSERT(0);
				}
				while (i_erase > 0 && file_path.at(i_erase) != L'/')
					i_erase--;
				file_path.erase(i_erase, i - i_erase);
				i = i_erase;
				continue;
			}
		}
		i++;
	}
	return file_path;
}

bool GlobalManager::ImageCacheKeyCompare::operator()(const std::wstring& key1, const std::wstring& key2) const
{
	int nLen1 = (int)key1.length();
	int nLen2 = (int)key2.length();
	if (nLen1 != nLen2)
		return nLen1 < nLen2;

	LPCWSTR pStr1Begin = key1.c_str();
	LPCWSTR pStr2Begin = key2.c_str();
	LPCWSTR pStr1End = pStr1Begin + nLen1;
	LPCWSTR pStr2End = pStr2Begin + nLen2;

	// ����Ƚ�
	while (--pStr1End >= pStr1Begin && --pStr2End >= pStr2Begin && *pStr1End == *pStr2End);

	// ���������Ѿ��ȹ��ˣ���ô�϶���ȣ�����false
	if (pStr1End < pStr1Begin) {
		return false;
	}
	return *pStr1End < *pStr2End;
}

} // namespace ui
