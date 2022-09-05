#include "StdAfx.h"
#include <shlwapi.h>

namespace ui 
{

typedef struct tagFINDTABINFO
{
	Control* pFocus;
	Control* pLast;
	bool bForward;
	bool bNextIsIt;
} FINDTABINFO;

typedef struct tagFINDSHORTCUT
{
	TCHAR ch;
	bool bPickNext;
} FINDSHORTCUT;


//////////////////////////////////////////////////////////////////////////
///
Window::Window() :
	m_hWnd(nullptr),
	m_pRoot(nullptr),
	OnEvent(),
	m_OldWndProc(::DefWindowProc),
	m_bSubclassed(false),
	m_nAlpha(255),
	m_renderOffset(),
	m_hDcPaint(nullptr),
	m_hwndTooltip(nullptr),
	m_ToolTip(),
	m_pFocus(nullptr),
	m_pNewHover(nullptr),
	m_pEventHover(nullptr),
	m_pEventClick(nullptr),
	m_pEventKey(nullptr),
	m_ptLastMousePos(-1, -1),
	m_pEventPointer(nullptr),
	m_bHandlePointer(true),
	m_szMinWindow(),
	m_szMaxWindow(),
	m_szInitWindowSize(),
	m_rcMaximizeInfo(0, 0, 0, 0),
	m_rcSizeBox(),
	m_rcAlphaFix(0, 0, 0, 0),
	m_szRoundCorner(),
	m_rcCaption(),
	m_uTimerID(0x1000),
	m_bFirstLayout(true),
	m_bIsArranged(false),
	m_bFocusNeeded(false),
	m_bMouseTracking(false),
	m_bMouseCapture(false),
	m_bIsLayeredWindow(true),
	m_aPreMessageFilters(),
	m_aMessageFilters(),
	m_aDelayedCleanup(),
	m_aFoundControls(),
	m_mNameHash(),
	m_mOptionGroup(),
	m_defaultFontInfo(),
	m_defaultAttrHash(),
	m_strWindowResourcePath(),
	m_aTranslateAccelerator(),
	m_heightPercent(0),
	m_closeFlag(),
	m_pUIAProvider(nullptr)
{
	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	lf.lfCharSet = DEFAULT_CHARSET;
	if (GlobalManager::GetDefaultFontName().length() > 0)
		_tcscpy_s(lf.lfFaceName, LF_FACESIZE, GlobalManager::GetDefaultFontName().c_str());

	HFONT hDefaultFont = ::CreateFontIndirect(&lf);
	m_defaultFontInfo.hFont = hDefaultFont;
	m_defaultFontInfo.sFontName = lf.lfFaceName;
	m_defaultFontInfo.iSize = -lf.lfHeight;
	m_defaultFontInfo.iWeight = FW_NORMAL;
	m_defaultFontInfo.bBold = (lf.lfWeight >= FW_BOLD);
	m_defaultFontInfo.bUnderline = (lf.lfUnderline == TRUE);
	m_defaultFontInfo.bStrikeout = (lf.lfStrikeOut == TRUE);
	m_defaultFontInfo.bItalic = (lf.lfItalic == TRUE);
	::ZeroMemory(&m_defaultFontInfo.tm, sizeof(m_defaultFontInfo.tm));
}

Window::~Window()
{
	// Delete the control-tree structures
	for (auto it = m_aDelayedCleanup.begin(); it != m_aDelayedCleanup.end(); it++) delete *it;

	delete m_pRoot;
	::DeleteObject(m_defaultFontInfo.hFont);
	RemoveAllClass();
	RemoveAllOptionGroups();

	// Reset other parts...
	if (m_hwndTooltip != NULL) ::DestroyWindow(m_hwndTooltip);
	if (m_hDcPaint != NULL) ::ReleaseDC(m_hWnd, m_hDcPaint);
}

HWND Window::GetHWND() const 
{ 
    return m_hWnd; 
}

bool Window::RegisterWindowClass()
{
	WNDCLASS wc = { 0 };
	wc.style = GetClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = Window::__WndProc;
	wc.hInstance = ::GetModuleHandle(NULL);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	std::wstring className = GetWindowClassName();
	wc.lpszClassName = className.c_str();
	ATOM ret = ::RegisterClass(&wc);
	ASSERT(ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

bool Window::RegisterSuperClass()
{
	// Get the class information from an existing
	// window so we can subclass it later on...
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	std::wstring superClassName = GetSuperClassName();
	if (!::GetClassInfoEx(NULL, superClassName.c_str(), &wc)) {
		if (!::GetClassInfoEx(::GetModuleHandle(NULL), superClassName.c_str(), &wc)) {
			ASSERT(!"Unable to locate window class");
			return false;
		}
	}
	m_OldWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = Window::__ControlProc;
	wc.hInstance = ::GetModuleHandle(NULL);
	std::wstring className = GetWindowClassName();
	wc.lpszClassName = className.c_str();
	ATOM ret = ::RegisterClassEx(&wc);
	ASSERT(ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

std::wstring Window::GetWindowClassName() const
{
	ASSERT(FALSE);
	return L"";
}

std::wstring Window::GetSuperClassName() const
{
    return std::wstring();
}

UINT Window::GetClassStyle() const
{
	return 0;
}

HWND Window::Subclass(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(m_hWnd == NULL);
	m_OldWndProc = SubclassWindow(hWnd, __WndProc);
	if (m_OldWndProc == NULL) return NULL;
	m_bSubclassed = true;
	m_hWnd = hWnd;
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));
	return m_hWnd;
}

void Window::Unsubclass()
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd)) return;
	if (!m_bSubclassed) return;
	SubclassWindow(m_hWnd, m_OldWndProc);
	m_OldWndProc = ::DefWindowProc;
	m_bSubclassed = false;
}

HWND Window::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, bool isLayeredWindow, const UiRect& rc)
{
    if( !GetSuperClassName().empty() && !RegisterSuperClass() ) return NULL;
    if( GetSuperClassName().empty() && !RegisterWindowClass() ) return NULL;
	std::wstring className = GetWindowClassName();
	m_bIsLayeredWindow = isLayeredWindow;
	if (m_bIsLayeredWindow) {
		dwExStyle |= WS_EX_LAYERED;
	}

	m_hWnd = ::CreateWindowEx(dwExStyle, className.c_str(), pstrName, dwStyle, 
		rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), hwndParent, NULL, ::GetModuleHandle(NULL), this);
	LONG nWindowLong = GetWindowLong( m_hWnd, GWL_STYLE );
	if( nWindowLong & WS_CAPTION ) {
		SetWindowLong( m_hWnd, GWL_STYLE, nWindowLong & ~WS_CAPTION );
	}

	ASSERT(m_hWnd!=NULL);
    return m_hWnd;
}

void Window::Close(UINT nRet)
{
	if (m_bFakeModal)
	{
		auto parent_hwnd = GetWindowOwner(m_hWnd);
		ASSERT(::IsWindow(parent_hwnd));
		::EnableWindow(parent_hwnd, TRUE);
		::SetFocus(parent_hwnd);
		m_bFakeModal = false;
	}
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd)) return;

	if (m_pRoot && IsWindowsVistaOrGreater()) {
		m_pRoot->SetVisible(false);
		auto closeCallback = [this, nRet]() {
			this->PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
		};
		TimerManager::GetInstance()->AddCancelableTimer(m_closeFlag.GetWeakFlag(), closeCallback, 300, 1);
	}
	else {
		PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
	}
	m_bCloseing = true;
}

void Window::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
    ASSERT(::IsWindow(m_hWnd));
    if( !::IsWindow(m_hWnd) ) return;
    ::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

void Window::ShowModalFake(HWND parent_hwnd)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(::IsWindow(parent_hwnd));
	auto p_hwnd = GetWindowOwner(m_hWnd);
	ASSERT(::IsWindow(p_hwnd));
	ASSERT(p_hwnd == parent_hwnd);
	::EnableWindow(parent_hwnd, FALSE);
	ShowWindow();
	m_bFakeModal = true;
}

bool Window::IsFakeModal()
{
	return m_bFakeModal;
}

void Window::CenterWindow()
{
    ASSERT(::IsWindow(m_hWnd));
    ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
    UiRect rcDlg;
    ::GetWindowRect(m_hWnd, &rcDlg);
    UiRect rcArea;
    UiRect rcCenter;
	HWND hWnd = GetHWND();
    HWND hWndCenter = ::GetWindowOwner(m_hWnd);
	if (hWndCenter!=NULL)
		hWnd=hWndCenter;

	// 处理多显示器模式下屏幕居中
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
	rcArea = oMonitor.rcWork;

    if( hWndCenter == NULL )
		rcCenter = rcArea;
	else if( ::IsIconic(hWndCenter) )
		rcCenter = rcArea;
	else
		::GetWindowRect(hWndCenter, &rcCenter);

    int DlgWidth = rcDlg.right - rcDlg.left;
    int DlgHeight = rcDlg.bottom - rcDlg.top;

    // Find dialog's upper left based on rcCenter
    int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
    int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

    // The dialog is outside the screen, move it inside
    if( xLeft < rcArea.left ) xLeft = rcArea.left;
    else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
    if( yTop < rcArea.top ) yTop = rcArea.top;
    else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
    ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Window::SetIcon(UINT nRes)
{
    HICON hIcon = (HICON)::LoadImage(::GetModuleHandle(NULL), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR | LR_SHARED);
    ASSERT(hIcon);
    ::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
    hIcon = (HICON)::LoadImage(::GetModuleHandle(NULL), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR | LR_SHARED);
    ASSERT(hIcon);
    ::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
}

LRESULT Window::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

LRESULT Window::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

void Window::AttachWindowClose(const EventCallback& callback)
{
	OnEvent[kEventWindowClose] += callback;
}

void Window::OnFinalMessage(HWND hWnd)
{
	UnregisterTouchWindowWrapper(m_hWnd);
	SendNotify(kEventWindowClose);

#if defined(ENABLE_UIAUTOMATION)
	if (nullptr != m_pUIAProvider) {
		// Coz UiaDisconnectProviderd require at least win8
		// UiaDisconnectProvider(m_pUIAProvider);

		m_pUIAProvider->ResetWindow();
		m_pUIAProvider->Release();

		m_pUIAProvider = nullptr;
	}
#endif
}

LRESULT CALLBACK Window::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pThis = NULL;
    if( uMsg == WM_NCCREATE ) {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<Window*>(lpcs->lpCreateParams);
        pThis->m_hWnd = hWnd;
        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
    }
    else {
        pThis = reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if( uMsg == WM_NCDESTROY && pThis != NULL ) {
            LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
            ::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
            if( pThis->m_bSubclassed ) pThis->Unsubclass();
            pThis->OnFinalMessage(hWnd);
            return lRes;
        }
    }

    if( pThis != NULL) {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    } 
    else {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK Window::__ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pThis = NULL;
    if( uMsg == WM_NCCREATE ) {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<Window*>(lpcs->lpCreateParams);
        ::SetProp(hWnd, _T("WndX"), (HANDLE) pThis);
        pThis->m_hWnd = hWnd;
    } 
    else {
        pThis = reinterpret_cast<Window*>(::GetProp(hWnd, _T("WndX")));
        if( uMsg == WM_NCDESTROY && pThis != NULL ) {
            LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
            if( pThis->m_bSubclassed ) pThis->Unsubclass();
            ::SetProp(hWnd, _T("WndX"), NULL);
            pThis->m_hWnd = NULL;
            pThis->OnFinalMessage(hWnd);
            return lRes;
        }
    }
    if( pThis != NULL ) {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    } 
    else {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

#if defined(ENABLE_UIAUTOMATION)
UIAWindowProvider* Window::GetUIAProvider()
{
	if (m_pUIAProvider == NULL)
	{
		m_pUIAProvider = new (std::nothrow) UIAWindowProvider(this);
	}
	return m_pUIAProvider;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////
//
//

void Window::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	ASSERT(::IsWindow(hWnd));
	// Remember the window context we came from
	m_hDcPaint = ::GetDC(hWnd);
	m_renderContext = GlobalManager::CreateRenderContext();

	RegisterTouchWindowWrapper(hWnd, 0);
}

bool Window::AttachDialog(Box* pRoot)
{
	ASSERT(::IsWindow(m_hWnd));
	// Reset any previous attachment
	SetFocus(NULL);
	m_pEventKey = NULL;
	m_pNewHover = NULL;
	m_pEventHover = NULL;
	m_pEventClick = NULL;
	m_pEventPointer = NULL;
	// Remove the existing control-tree. We might have gotten inside this function as
	// a result of an event fired or similar, so we cannot just delete the objects and
	// pull the internal memory of the calling code. We'll delay the cleanup.
	if (m_pRoot != NULL) {
		AddDelayedCleanup(m_pRoot);
	}
	// Set the dialog root element
	m_pRoot = pRoot;
	// Go ahead...
	m_bIsArranged = true;
	m_bFirstLayout = true;
	m_bFocusNeeded = true;
	// Initiate all control
	return InitControls(m_pRoot);
}

bool Window::InitControls(Control* pControl, Box* pParent /*= NULL*/)
{
	ASSERT(pControl);
	if (pControl == NULL) return false;
	pControl->SetWindow(this, pParent != NULL ? pParent : pControl->GetParent(), true);
	pControl->FindControl(__FindControlFromNameHash, this, UIFIND_ALL);
	return true;
}

void Window::ReapObjects(Control* pControl)
{
	if (!pControl) {
		return;
	}
	if (pControl == m_pEventKey) m_pEventKey = NULL;
	if (pControl == m_pEventHover) m_pEventHover = NULL;
	if (pControl == m_pNewHover) m_pNewHover = NULL;
	if (pControl == m_pEventClick) m_pEventClick = NULL;
	if (pControl == m_pEventPointer) m_pEventPointer = NULL;
	if (pControl == m_pFocus) m_pFocus = NULL;
	std::wstring sName = pControl->GetName();
	if (!sName.empty()) {
		auto it = m_mNameHash.find(sName);
		if (it != m_mNameHash.end())
		{
			m_mNameHash.erase(it);
		}
	}
}

std::wstring Window::GetWindowResourcePath()
{
	return m_strWindowResourcePath;
}

void Window::SetWindowResourcePath(const std::wstring& strPath)
{
	m_strWindowResourcePath = strPath;
	if (m_strWindowResourcePath.empty()) return;
	TCHAR cEnd = m_strWindowResourcePath.at(m_strWindowResourcePath.length() - 1);
	if (cEnd != _T('\\') && cEnd != _T('/')) m_strWindowResourcePath += _T('\\');
}

TFontInfo* Window::GetDefaultFontInfo()
{
	if (m_defaultFontInfo.tm.tmHeight == 0) {
		HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, m_defaultFontInfo.hFont);
		::GetTextMetrics(m_hDcPaint, &m_defaultFontInfo.tm);
		::SelectObject(m_hDcPaint, hOldFont);
	}
	return &m_defaultFontInfo;
}

void Window::AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList)
{
	ASSERT(!strClassName.empty());
	ASSERT(!strControlAttrList.empty());
	m_defaultAttrHash[strClassName] = strControlAttrList;
}

const std::map<std::wstring, std::wstring>* Window::GetClassMap()
{
	return &m_defaultAttrHash;
}

std::wstring Window::GetClassAttributes(const std::wstring& strClassName) const
{
	auto it = m_defaultAttrHash.find(strClassName);
	if (it != m_defaultAttrHash.end())
	{
		return it->second;
	}

	return L"";
}

bool Window::RemoveClass(const std::wstring& strClassName)
{
	auto it = m_defaultAttrHash.find(strClassName);
	if (it != m_defaultAttrHash.end())
	{
		m_defaultAttrHash.erase(it);
		return true;
	}
	return false;
}

void Window::RemoveAllClass()
{
	m_defaultAttrHash.clear();
}

void Window::AddTextColor(const std::wstring& strName, const std::wstring& strValue)
{
	std::wstring strColor = strValue.substr(1);
	LPTSTR pstr = NULL;
	DWORD dwBackColor = _tcstoul(strColor.c_str(), &pstr, 16);

	m_mapTextColor[strName] = dwBackColor;
}

void Window::AddTextColor(const std::wstring& strName, DWORD argb)
{
	m_mapTextColor[strName] = argb;
}

DWORD Window::GetTextColor(const std::wstring& strName)
{
	auto it = m_mapTextColor.find(strName);
	if (it != m_mapTextColor.end()) {
		return it->second;
	}

	return 0;
}

bool Window::AddOptionGroup(const std::wstring& strGroupName, Control* pControl)
{
	auto it = m_mOptionGroup.find(strGroupName);
	if (it != m_mOptionGroup.end()) {
		auto it2 = std::find(it->second.begin(), it->second.end(), pControl);
		if (it2 != it->second.end())
		{
			return false;
		}
		it->second.push_back(pControl);
	}
	else {
		m_mOptionGroup[strGroupName].push_back(pControl);
	}
	return true;
}

std::vector<Control*>* Window::GetOptionGroup(const std::wstring& strGroupName)
{
	auto it = m_mOptionGroup.find(strGroupName);
	if (it != m_mOptionGroup.end()) return &(it->second);
	return NULL;
}

void Window::RemoveOptionGroup(const std::wstring& strGroupName, Control* pControl)
{
	ASSERT(!strGroupName.empty());
	ASSERT(pControl);
	auto it = m_mOptionGroup.find(strGroupName);
	if (it != m_mOptionGroup.end()) {
		auto it2 = std::find(it->second.begin(), it->second.end(), pControl);
		if (it2 != it->second.end())
		{
			it->second.erase(it2);
		}

		if (it->second.empty()) {
			m_mOptionGroup.erase(it);
		}
	}
}

void Window::RemoveAllOptionGroups()
{
	m_mOptionGroup.clear();
}

void Window::ClearImageCache()
{
	Control *pRoot = m_shadow.GetRoot();
	if (pRoot)
		pRoot->ClearImageCache();
	else
		m_pRoot->ClearImageCache();
}

POINT Window::GetLastMousePos() const
{
	return m_ptLastMousePos;
}

UiRect Window::GetSizeBox()
{
	return m_rcSizeBox;
}

void Window::SetSizeBox(const UiRect& rcSizeBox)
{
	m_rcSizeBox = rcSizeBox;
}

UiRect Window::GetCaptionRect() const
{
	return m_rcCaption;
}

void Window::SetCaptionRect(UiRect& rcCaption)
{
	DpiManager::GetInstance()->ScaleRect(rcCaption);
	m_rcCaption = rcCaption;
}

CSize Window::GetRoundCorner() const
{
	return m_szRoundCorner;
}

void Window::SetRoundCorner(int cx, int cy)
{
	DpiManager::GetInstance()->ScaleInt(cx);
	DpiManager::GetInstance()->ScaleInt(cy);
	m_szRoundCorner.cx = cx;
	m_szRoundCorner.cy = cy;
}

UiRect Window::GetMaximizeInfo() const
{
	return m_rcMaximizeInfo;
}

void Window::SetMaximizeInfo(UiRect& rcMaximize)
{
	DpiManager::GetInstance()->ScaleRect(rcMaximize);
	m_rcMaximizeInfo = rcMaximize;
}

UiRect Window::GetAlphaFixCorner() const
{
	return m_rcAlphaFix;
}

void Window::SetAlphaFixCorner(UiRect& rc)
{
	DpiManager::GetInstance()->ScaleRect(rc);
	m_rcAlphaFix = rc;
}

double Window::GetHeightPercent() const
{
	return m_heightPercent;
}

void Window::SetHeightPercent(double heightPercent)
{
	m_heightPercent = heightPercent;
}

void Window::SetTextId(const std::wstring& strTextId)
{
	::SetWindowText(m_hWnd, MutiLanSupport::GetInstance()->GetStringViaID(strTextId).c_str());
}

void Window::SetShadowAttached(bool bShadowAttached)
{
	m_shadow.SetShadowAttached(bShadowAttached);
}

std::wstring Window::GetShadowImage() const
{
	return m_shadow.GetShadowImage();
}

void Window::SetShadowImage(const std::wstring &strImage)
{
	m_shadow.SetShadowImage(strImage);
}

UiRect Window::GetShadowCorner() const
{
	return m_shadow.GetShadowCorner();
}

bool Window::IsShadowAttached()
{
    return m_shadow.IsShadowAttached();
}

void Window::SetShadowCorner(const UiRect rect, bool bNeedDpiScale)
{
	m_shadow.SetShadowCorner(rect, bNeedDpiScale);
}

UiRect Window::GetPos(bool bContainShadow) const
{
	UiRect rcPos;
	::GetWindowRect(m_hWnd, &rcPos);

	if (!bContainShadow) {
		UiRect padding = m_shadow.GetShadowCorner();
		rcPos.left += padding.left;
		rcPos.right -= padding.right;
		rcPos.top += padding.top;
		rcPos.bottom -= padding.bottom;
	}
	return rcPos;
}

void Window::SetPos(const UiRect& rc, bool bNeedDpiScale, UINT uFlags, HWND hWndInsertAfter, bool bContainShadow)
{
	UiRect rcNewPos = rc;
	if (bNeedDpiScale)
		DpiManager::GetInstance()->ScaleRect(rcNewPos);

	ASSERT(::IsWindow(m_hWnd));
	if (!bContainShadow) {
		rcNewPos.Inflate(m_shadow.GetShadowCorner());
	}
	::SetWindowPos(m_hWnd, hWndInsertAfter, rcNewPos.left, rcNewPos.top, rcNewPos.GetWidth(), rcNewPos.GetHeight(), uFlags);
}

CSize Window::GetMinInfo(bool bContainShadow) const
{
	CSize xy = m_szMinWindow;
	if (!bContainShadow) {
		UiRect rcShadow = m_shadow.GetShadowCorner();
		if (xy.cx != 0) {
			xy.cx -= rcShadow.left + rcShadow.right;
		}
		if (xy.cy != 0) {
			xy.cy -= rcShadow.top + rcShadow.bottom;
		}
	}

	return xy;
}

void Window::SetMinInfo(int cx, int cy, bool bContainShadow, bool bNeedDpiScale)
{
	if (bNeedDpiScale) {
		DpiManager::GetInstance()->ScaleInt(cx);
		DpiManager::GetInstance()->ScaleInt(cy);
	}
	ASSERT(cx >= 0 && cy >= 0);

	if (!bContainShadow) {
		UiRect rcShadow = m_shadow.GetShadowCorner();
		if (cx != 0) {
			cx += rcShadow.left + rcShadow.right;
		}
		if (cy != 0) {
			cy += rcShadow.top + rcShadow.bottom;
		}
	}
	m_szMinWindow.cx = cx;
	m_szMinWindow.cy = cy;
}

CSize Window::GetMaxInfo(bool bContainShadow) const
{
	CSize xy = m_szMaxWindow;
	if (!bContainShadow) {
		UiRect rcShadow = m_shadow.GetShadowCorner();
		if (xy.cx != 0) {
			xy.cx -= rcShadow.left + rcShadow.right;
		}
		if (xy.cy != 0) {
			xy.cy -= rcShadow.top + rcShadow.bottom;
		}
	}

	return xy;
}

void Window::SetMaxInfo(int cx, int cy, bool bContainShadow, bool bNeedDpiScale)
{
	if (bNeedDpiScale) {
		DpiManager::GetInstance()->ScaleInt(cx);
		DpiManager::GetInstance()->ScaleInt(cy);
	}
	ASSERT(cx >= 0 && cy >= 0);

	if (!bContainShadow) {
		UiRect rcShadow = m_shadow.GetShadowCorner();
		if (cx != 0) {
			cx += rcShadow.left + rcShadow.right;
		}
		if (cy != 0) {
			cy += rcShadow.top + rcShadow.bottom;
		}
	}
	m_szMaxWindow.cx = cx;
	m_szMaxWindow.cy = cy;
}

CSize Window::GetInitSize(bool bContainShadow) const
{
	CSize xy = m_szInitWindowSize;
	if (!bContainShadow) {
		UiRect rcShadow = m_shadow.GetShadowCorner();
		if (xy.cx != 0) {
			xy.cx -= rcShadow.left + rcShadow.right;
		}
		if (xy.cy != 0) {
			xy.cy -= rcShadow.top + rcShadow.bottom;
		}
	}

	return xy;
}

void Window::Resize(int cx, int cy, bool bContainShadow, bool bNeedDpiScale)
{
	if (bNeedDpiScale) {
		DpiManager::GetInstance()->ScaleInt(cy);
		DpiManager::GetInstance()->ScaleInt(cx);
	}

	if (!bContainShadow) {
		UiRect rcShadow = m_shadow.GetShadowCorner();
		cx += rcShadow.left + rcShadow.right;
		cy += rcShadow.top + rcShadow.bottom;
	}
	m_szInitWindowSize.cx = cx;
	m_szInitWindowSize.cy = cy;
	if(m_hWnd != NULL ) {
		::SetWindowPos(m_hWnd, NULL, 0, 0, m_szInitWindowSize.cx, m_szInitWindowSize.cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
	}
}

void Window::SetInitSize(int cx, int cy, bool bContainShadow, bool bNeedDpiScale)
{
	if(m_pRoot == NULL) {
		Resize(cx, cy, bContainShadow, bNeedDpiScale);
	}
}

bool Window::AddMessageFilter(IUIMessageFilter* pFilter)
{
	ASSERT(std::find(m_aMessageFilters.begin(), m_aMessageFilters.end(), pFilter) == m_aMessageFilters.end());
	m_aMessageFilters.push_back(pFilter);
	return true;
}
bool Window::RemoveMessageFilter(IUIMessageFilter* pFilter)
{
	for (auto it = m_aMessageFilters.begin(); it != m_aMessageFilters.end(); it++) {
		if (*it == pFilter) {
			m_aMessageFilters.erase(it);
			return true;
		}
	}
	return false;
}
bool Window::AddControlFromPointFinder(IControlFromPointFinder* pFinder)
{
	ASSERT(std::find(m_aIControlFromPointFinder.begin(), m_aIControlFromPointFinder.end(), pFinder) == m_aIControlFromPointFinder.end());
	m_aIControlFromPointFinder.emplace_back(pFinder);
	return true;
}
bool Window::RemoveControlFromPointFinder(IControlFromPointFinder* pFinder)
{
	auto it = std::find_if(m_aIControlFromPointFinder.begin(), m_aIControlFromPointFinder.end(), [&](IControlFromPointFinder* item) {
		return pFinder == item;
	});
	if (it != m_aIControlFromPointFinder.end())
	{
		m_aIControlFromPointFinder.erase(it);
		return true;
	}
	return false;
}

bool Window::AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
{
	ASSERT(std::find(m_aTranslateAccelerator.begin(), m_aTranslateAccelerator.end(), pTranslateAccelerator) == m_aTranslateAccelerator.end());
	m_aTranslateAccelerator.push_back(pTranslateAccelerator);
	return true;
}

bool Window::RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
{
	for (auto it = m_aTranslateAccelerator.begin(); it != m_aTranslateAccelerator.end(); it++) {
		if (*it == pTranslateAccelerator)
		{
			m_aTranslateAccelerator.erase(it);
			return true;
		}
	}
	return false;
}

bool Window::TranslateAccelerator(LPMSG pMsg)
{
	for (auto it = m_aTranslateAccelerator.begin(); it != m_aTranslateAccelerator.end(); it++) {
		LRESULT lResult = (*it)->TranslateAccelerator(pMsg);
		if (lResult == S_OK) return true;
	}
	return false;
}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;
	LRESULT ret = DoHandlMessage(uMsg, wParam, lParam, handled);
	if (handled) {
		return ret;
	}
	else {
		return CallWindowProc(uMsg, wParam, lParam);
	}
}

LRESULT Window::DoHandlMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled)
{
	handled = false;
	// Cycle through listeners
	for (auto it = m_aMessageFilters.begin(); it != m_aMessageFilters.end(); it++)
	{
		BOOL bHandled = false;
		LRESULT lResult = (*it)->MessageHandler(uMsg, wParam, lParam, bHandled);
		if (bHandled && uMsg != WM_MOUSEMOVE) {
			handled = true;
			return lResult;
		}
	}
	// Custom handling of events
	switch (uMsg) {
	case WM_APP + 1:
	{
		for (auto it = m_aDelayedCleanup.begin(); it != m_aDelayedCleanup.end(); it++)	delete *it;
		m_aDelayedCleanup.clear();
	}
	break;
	case WM_CLOSE:
	{
		// Make sure all matching "closing" events are sent
		if (m_pEventHover != NULL) {
			m_pEventHover->HandleMessageTemplate(kEventMouseLeave);
		}
		if (m_pEventClick != NULL) {
			m_pEventClick->HandleMessageTemplate(kEventMouseButtonUp);
		}
		if (m_pEventPointer != NULL) {
			m_pEventPointer->HandleMessageTemplate(kEventMouseButtonUp);
		}
		SetFocus(NULL);

		// Hmmph, the usual Windows tricks to avoid
		// focus loss...
		HWND hwndParent = GetWindowOwner(m_hWnd);
		if (hwndParent != NULL) ::SetFocus(hwndParent);
	}
	break;
	case WM_ERASEBKGND:
	{
		handled = true;
		return 1;
	}
	case WM_PAINT:
	{
		Paint();
		handled = true;
		return 0;

	}
	// If any of the painting requested a resize again, we'll need
	// to invalidate the entire window once more.
	if (m_bIsArranged) {
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}
	handled = true;
	return 0;
	case WM_SIZE:
	{
		if (m_pFocus != NULL) {
			m_pFocus->HandleMessageTemplate(kEventWindowSize);
		}
		if (m_pRoot != NULL) m_pRoot->Arrange();

		if (wParam == SIZE_MAXIMIZED) {
			m_shadow.MaximizedOrRestored(true);
		}
		else if (wParam == SIZE_RESTORED) {
			m_shadow.MaximizedOrRestored(false);
		}
	}
	handled = true;
	return 0;
	case WM_MOUSEHOVER:
	{
		m_bMouseTracking = false;
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		Control* pHover = FindControl(pt);
		if (pHover == NULL) break;
		// Generate mouse hover event
		if (m_pEventHover != NULL) {
			m_pEventHover->HandleMessageTemplate(kEventMouseHover, 0, 0, 0, pt);
		}
		// Create tooltip information
		std::wstring sToolTip = pHover->GetToolTipText();
		if (sToolTip.empty())
			break;

		if (m_hwndTooltip != NULL && IsWindowVisible(m_hwndTooltip)) {
			TOOLINFO toolTip = {0};
			toolTip.cbSize = sizeof(TOOLINFO);
			toolTip.hwnd = m_hWnd;
			toolTip.uId = (UINT_PTR)m_hWnd;
			std::wstring toolTipText;
			toolTipText.resize(MAX_PATH);
			toolTip.lpszText = const_cast<LPTSTR>((LPCTSTR)toolTipText.c_str());
			::SendMessage(m_hwndTooltip, TTM_GETTOOLINFO, 0, (LPARAM)&toolTip);
			if (pHover == m_pEventHover && sToolTip == std::wstring(toolTipText.c_str()))
				break;
		}
		
		::ZeroMemory(&m_ToolTip, sizeof(TOOLINFO));
		m_ToolTip.cbSize = sizeof(TOOLINFO);
		m_ToolTip.uFlags = TTF_IDISHWND;
		m_ToolTip.hwnd = m_hWnd;
		m_ToolTip.uId = (UINT_PTR)m_hWnd;
		m_ToolTip.hinst = ::GetModuleHandle(NULL);
		m_ToolTip.lpszText = const_cast<LPTSTR>((LPCTSTR)sToolTip.c_str());
		m_ToolTip.rect = pHover->GetPos();
		if (m_hwndTooltip == NULL) {
			m_hwndTooltip = ::CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hWnd, NULL, ::GetModuleHandle(NULL), NULL);
			::SendMessage(m_hwndTooltip, TTM_ADDTOOL, 0, (LPARAM)&m_ToolTip);
			::SetWindowPos(m_hwndTooltip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
		if (!::IsWindowVisible(m_hwndTooltip)) {
			::SendMessage(m_hwndTooltip, TTM_SETMAXTIPWIDTH, 0, pHover->GetToolTipWidth());
			::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolTip);
			::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ToolTip);
		}
		::SendMessage(m_hwndTooltip, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD)MAKELONG(pt.x, pt.y));
	}
	handled = true;
	return 0;
	case WM_MOUSELEAVE:
	{
		if (m_hwndTooltip != NULL) ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolTip);
		if (m_bMouseTracking) ::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, (LPARAM)-1);
		m_bMouseTracking = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (m_pEventPointer != NULL)
			break;
	
		// Start tracking this entire window again...
		if (!m_bMouseTracking) {
			TRACKMOUSEEVENT tme = { 0 };
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = m_hwndTooltip == NULL ? 400UL : (DWORD) ::SendMessage(m_hwndTooltip, TTM_GETDELAYTIME, TTDT_INITIAL, 0L);
			_TrackMouseEvent(&tme);
			m_bMouseTracking = true;
		}
		// Generate the appropriate mouse messages
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;

		// Do not move the focus to the new control when the mouse is pressed
		if (!IsCaptured())
			if (!HandleMouseEnterLeave(pt, wParam, lParam)) break;

		if (m_pEventClick != NULL) {
			m_pEventClick->HandleMessageTemplate(kEventMouseMove, wParam, lParam, 0, pt);
		}
		else if (m_pNewHover != NULL) {
			m_pNewHover->HandleMessageTemplate(kEventMouseMove, wParam, lParam, 0, pt);
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (m_pEventPointer != NULL)
			break;

		// We alway set focus back to our app (this helps
		// when Win32 child windows are placed on the dialog
		// and we need to remove them on focus change).
		::SetFocus(m_hWnd);
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		Control* pControl = FindControl(pt);
		if (pControl == NULL) break;
		if (pControl->GetWindow() != this) break;
		m_pEventClick = pControl;
		pControl->SetFocus();
		SetCapture();

		pControl->HandleMessageTemplate(kEventMouseButtonDown, wParam, lParam, 0, pt);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		if (m_pEventPointer != NULL)
			break;

		::SetFocus(m_hWnd);
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		Control* pControl = FindControl(pt);
		if (pControl == NULL) break;
		if (pControl->GetWindow() != this) break;
		m_pEventClick = pControl;
		pControl->SetFocus();
		SetCapture();

		pControl->HandleMessageTemplate(kEventMouseRightButtonDown, wParam, lParam, 0, pt);
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		if (m_pEventPointer != NULL)
			break;

		::SetFocus(m_hWnd);
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		Control* pControl = FindControl(pt);
		if (pControl == NULL) break;
		if (pControl->GetWindow() != this) break;
		m_pEventClick = pControl;
		SetCapture();

		pControl->HandleMessageTemplate(kEventInternalDoubleClick, wParam, lParam, 0, pt);
	}
	break;
	case WM_LBUTTONUP:
	{
		ReleaseEventClick(false, wParam, lParam);
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		ReleaseCapture();
		if (m_pEventClick == NULL) break;

		m_pEventClick->HandleMessageTemplate(kEventMouseButtonUp, wParam, lParam, 0, pt);
		m_pEventClick = NULL;
	}
	break;
	case WM_RBUTTONUP:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		ReleaseCapture();
		if (m_pEventClick == NULL) break;
 
 		m_pEventClick->HandleMessageTemplate(kEventMouseRightButtonUp, wParam, lParam, 0, pt);
		// WM_CONTEXTMENU消息还会用到m_pEventClick
 		// m_pEventClick = NULL;
	}
	break;
	case WM_IME_STARTCOMPOSITION:
	{
		if (m_pFocus == NULL) break;
		m_pFocus->HandleMessageTemplate(kEventImeStartComposition, wParam, lParam, wParam);
	}
	break;
	case WM_IME_ENDCOMPOSITION:
	{
		if (m_pFocus == NULL) break;
		m_pFocus->HandleMessageTemplate(kEventImeEndComposition, wParam, lParam, wParam);
	}
	break;
	case WM_MOUSEWHEEL:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		::ScreenToClient(m_hWnd, &pt);
		m_ptLastMousePos = pt;
		Control* pControl = FindControl(pt);
		if (pControl == NULL) break;
		if (pControl->GetWindow() != this) break;
		int zDelta = (int)(short)HIWORD(wParam);

		pControl->HandleMessageTemplate(kEventMouseScrollWheel, zDelta, lParam);
	}
	break;
	case WM_TOUCH:
	{
		unsigned int nNumInputs = (int)wParam;
		TOUCHINPUT* pInputs = new TOUCHINPUT[nNumInputs]; 

		// 只关心第一个触摸位置
		if (nNumInputs >= 1 && GetTouchInputInfoWrapper((HTOUCHINPUT)lParam, nNumInputs, pInputs, sizeof(TOUCHINPUT)))
		{
			if (pInputs[0].dwID != 0)
			{
				POINT pt;
				pt.x = TOUCH_COORD_TO_PIXEL(pInputs[0].x);
				pt.y = TOUCH_COORD_TO_PIXEL(pInputs[0].y);
				ScreenToClient(m_hWnd, &pt);

				if (pInputs[0].dwFlags & TOUCHEVENTF_DOWN)
				{
					if (m_pEventClick != NULL)
						goto endtouch;

					::SetFocus(m_hWnd);
					m_ptLastMousePos = pt;
					Control *pControl = FindControl(pt);
					if (pControl == NULL) goto endtouch;
					if (pControl->GetWindow() != this) goto endtouch;
					m_pEventPointer = pControl;
					pControl->SetFocus();
					SetCapture();

					pControl->HandleMessageTemplate(kEventTouchDown, 0, lParam, 0, pt);
				}
				else if (pInputs[0].dwFlags & TOUCHEVENTF_MOVE)
				{
					if (m_pEventClick != NULL)
						goto endtouch;

					if (m_ptLastMousePos.x == pt.x && m_ptLastMousePos.y == pt.y)
						goto endtouch;

					m_ptLastMousePos = pt;
					if (m_pEventPointer == NULL) goto endtouch;

					if (!HandleMouseEnterLeave(pt, wParam, lParam)) goto endtouch;

					m_pEventPointer->HandleMessageTemplate(kEventTouchMove, 0, 0, 0, pt);
				}
				else if (pInputs[0].dwFlags & TOUCHEVENTF_UP)
				{
					ReleaseEventClick(true, wParam, lParam);
					m_ptLastMousePos = pt;
					ReleaseCapture();
					if (m_pEventPointer == NULL) goto endtouch;

					m_pEventPointer->HandleMessageTemplate(kEventTouchUp, 0, lParam, 0, pt);
					m_pEventPointer = NULL;
				}
			}
		}
	endtouch:
		CloseTouchInputHandleWrapper((HTOUCHINPUT)lParam);
		delete[] pInputs;
	}
		break;
	case WM_POINTERDOWN:
	case WM_POINTERUP:
	case WM_POINTERUPDATE:
	case WM_POINTERLEAVE:
	case WM_POINTERCAPTURECHANGED:
	{
		if (!m_bHandlePointer) {
			break;
		}

		// 只关心第一个触摸点
		if (!IS_POINTER_PRIMARY_WPARAM(wParam)) {
			handled = true;
			break;
		}

		UINT32 pointerId = GET_POINTERID_WPARAM(wParam);
		POINTER_INPUT_TYPE type;
		if (!GetPointerTypeWrapper(pointerId, &type)) {
			return 0;
		}

		POINT pt = { 0 };
		FLOAT pressure = 0.0f;

		switch (type)
		{
		case PT_TOUCH:
			POINTER_TOUCH_INFO touchInfo;
			if (!GetPointerTouchInfoWrapper(pointerId, &touchInfo)) {
				return 0;
			}

			pt = touchInfo.pointerInfo.ptPixelLocationRaw;
			pressure = (float)touchInfo.pressure / 1024;
			break;
		case PT_PEN:
			POINTER_PEN_INFO penInfo;
			if (!GetPointerPenInfoWrapper(pointerId, &penInfo)) {
				return 0;
			}

			pt = penInfo.pointerInfo.ptPixelLocationRaw;
			pressure = (float)penInfo.pressure / 1024;
			break;
		default:
			return 0;
			break;
		}
		ScreenToClient(m_hWnd, &pt);

		switch (uMsg)
		{
		case WM_POINTERDOWN:
		{
			if (m_pEventClick != NULL) {
				handled = true;
				break;
			}

			::SetFocus(m_hWnd);
			m_ptLastMousePos = pt;
			Control *pControl = FindControl(pt);
			if (pControl == NULL) break;
			if (pControl->GetWindow() != this) break;
			m_pEventPointer = pControl;
			pControl->SetFocus();
			SetCapture();

			pControl->HandleMessageTemplate(kEventPointDown, 0, lParam, 0, pt, pressure);

			// 如果控件不支持处理WM_POINTERUPDATE消息，则不设置handled，程序会进入WM_BUTTON处理流程
			if (m_pEventPointer && m_pEventPointer->IsReceivePointerMsg()) {
				handled = true;
			}
		}
			break;
		case WM_POINTERUPDATE:
			if (m_pEventClick != NULL) {
				handled = true;
				break;
			}

			if (m_ptLastMousePos.x == pt.x && m_ptLastMousePos.y == pt.y)
				break;

			m_ptLastMousePos = pt;
			// 如果没有按下，则不设置handled，程序会转换为WM_BUTTON类消息
			if (m_pEventPointer == NULL) break;

			if (!HandleMouseEnterLeave(pt, wParam, lParam)) break;

			m_pEventPointer->HandleMessageTemplate(kEventPointMove, 0, 0, 0, pt, pressure);

			// 如果控件不支持处理WM_POINTERUPDATE消息，则不设置handled，程序会进入WM_MOUSEMOVE处理流程
			if (m_pEventPointer && m_pEventPointer->IsReceivePointerMsg()) {
				handled = true;
			}
			break;
		case WM_POINTERUP:
		case WM_POINTERLEAVE:
		case WM_POINTERCAPTURECHANGED:
			ReleaseEventClick(true, wParam, lParam);
			m_ptLastMousePos = pt;
			// 如果没有按下，则不设置handled，程序会转换为WM_BUTTON类消息
			 if (uMsg != WM_POINTERLEAVE) {
                		// Refer to LBUTTONUP and MOUSELEAVE，LBUTTOUP ReleaseCapture while MOUSELEAVE DONOT ReleaseCapture
                		ReleaseCapture();
                        }
			if (m_pEventPointer == NULL) break;

			m_pEventPointer->HandleMessageTemplate(kEventPointUp, 0, lParam, 0, pt, pressure);

			// 如果控件不支持处理WM_POINTERUPDATE消息，则不设置handled，程序会进入WM_BUTTON处理流程
			if (m_pEventPointer && m_pEventPointer->IsReceivePointerMsg()) {
				handled = true;
			}
			m_pEventPointer = NULL;
			break;
		default:
			break;
		}

		return 0;
	}
		break;
	case WM_SETFOCUS:
	{

	}
	break;
	case WM_KILLFOCUS:
	{
		Control *pControl = m_pEventClick ? m_pEventClick : NULL;
		pControl = m_pEventPointer ? m_pEventPointer : pControl;

		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		if (pControl == NULL || !pControl->IsNeedButtonUpWhenKillFocus()) break;
		ReleaseCapture();

		pControl->HandleMessageTemplate(kEventMouseButtonUp, wParam, lParam, 0, pt);
		m_pEventClick = NULL;
		m_pEventPointer = NULL;
	}
	break;
	case WM_CONTEXTMENU:
	{
		ReleaseEventClick(false, wParam, lParam);
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		::ScreenToClient(m_hWnd, &pt);
		m_ptLastMousePos = pt;
		if (m_pEventClick == NULL) break;
		ReleaseCapture();

		m_pEventClick->HandleMessageTemplate(kEventInternalMenu, wParam, (LPARAM)m_pEventClick, 0, pt);
		m_pEventClick = NULL;
	}
	break;
	case WM_CHAR:
	{
		if (m_pFocus == NULL) break;

		m_pFocus->HandleMessageTemplate(kEventChar, wParam, lParam, wParam);
	}
	break;
	case WM_KEYDOWN:
	{
		if (m_pFocus == NULL) break;

		// Tabbing between controls
		if (wParam == VK_TAB) {
			if (m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && dynamic_cast<RichEdit*>(m_pFocus) != nullptr) {
				if (dynamic_cast<RichEdit*>(m_pFocus)->IsWantTab()) return false;
			}
			SetNextTabControl(::GetKeyState(VK_SHIFT) >= 0);
			return true;
		}

		m_pEventKey = m_pFocus;
		m_pFocus->HandleMessageTemplate(kEventKeyDown, wParam, lParam, wParam);
	}
	break;
	case WM_KEYUP:
	{
		if (m_pEventKey == NULL) break;

		m_pEventKey->HandleMessageTemplate(kEventKeyUp, wParam, lParam, wParam);
		m_pEventKey = NULL;
	}
	break;
	case WM_SETCURSOR:
	{
		if (LOWORD(lParam) != HTCLIENT) break;
		if (m_pEventClick != NULL || m_pEventPointer != NULL) {
			handled = true;
			return 0;
		}

		POINT pt = { 0 };
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		m_ptLastMousePos = pt;
		Control* pControl = FindControl(pt);
		if (pControl == NULL) break;
		pControl->HandleMessageTemplate(kEventSetCursor, wParam, lParam, 0, pt);
	}
	handled = true;
	return 0;
	case WM_NOTIFY:
	{
		LPNMHDR lpNMHDR = (LPNMHDR)lParam;
		if (lpNMHDR != NULL) return ::SendMessage(lpNMHDR->hwndFrom, OCM__BASE + uMsg, wParam, lParam);
		handled = true;
		return 0;
	}
	break;
	case WM_COMMAND:
	{
		if (lParam == 0) break;
		HWND hWndChild = (HWND)lParam;
		handled = true;
		return ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
	}
	break;
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
	{
		// Refer To: http://msdn.microsoft.com/en-us/library/bb761691(v=vs.85).aspx
		// Read-only or disabled edit controls do not send the WM_CTLCOLOREDIT message; instead, they send the WM_CTLCOLORSTATIC message.
		if (lParam == 0) break;
		HWND hWndChild = (HWND)lParam;
		handled = true;
		return ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
	}
	break;
#if defined(ENABLE_UIAUTOMATION)
	case WM_GETOBJECT:
	{
		if (static_cast<long>(lParam) == static_cast<long>(UiaRootObjectId) && GlobalManager::IsAutomationEnabled()) {
			handled = TRUE;


			return UiaReturnRawElementProvider(m_hWnd, wParam, lParam, GetUIAProvider());
		}
	}
	break;
#endif
	default:
		break;
	}

	return 0;
}

LRESULT Window::CallWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
}

bool Window::HandleMouseEnterLeave(const POINT &pt, WPARAM wParam, LPARAM lParam)
{
	m_pNewHover = FindControl(pt);
	if (m_pNewHover != NULL && m_pNewHover->GetWindow() != this) return false;;

	if (m_pNewHover != m_pEventHover && m_pEventHover != NULL) {
		m_pEventHover->HandleMessageTemplate(kEventMouseLeave, 0, 0, 0, pt);
		m_pEventHover = NULL;
		if (m_hwndTooltip != NULL) ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ToolTip);
	}
	if (m_pNewHover != m_pEventHover && m_pNewHover != NULL) {
		m_pNewHover->HandleMessageTemplate(kEventMouseEnter, wParam, lParam, 0, pt);
		m_pEventHover = m_pNewHover;
	}
	return true;
}

void Window::ReleaseEventClick(bool bClickOrPointer, WPARAM wParam, LPARAM lParam)
{
	if (bClickOrPointer) {
		if (m_pEventClick) {
			m_pEventClick->HandleMessageTemplate(kEventMouseButtonUp, wParam, lParam, 0, m_ptLastMousePos);
			m_pEventClick = NULL;
		}
	}
	else {
		if (m_pEventPointer) {
			m_pEventPointer->HandleMessageTemplate(kEventPointUp, 0, lParam, 0, m_ptLastMousePos);
			m_pEventPointer = NULL;
		}
	}
}

Control* Window::GetFocus() const
{
	return m_pFocus;
}

void Window::SetFocus(Control* pControl)
{
	// Paint manager window has focus?
	HWND hFocusWnd = ::GetFocus();
	if (hFocusWnd != m_hWnd && pControl != m_pFocus && pControl != NULL) ::SetFocus(m_hWnd);
	// Already has focus?
	if (pControl == m_pFocus) return;
	// Remove focus from old control
	if (m_pFocus != NULL) {
		m_pFocus->HandleMessageTemplate(kEventInternalKillFocus);
		Control* tmp = m_pFocus;
		m_pFocus = NULL;
		SendNotify(tmp, kEventKillFocus);
	}
	if (pControl == NULL) return;
	// Set focus to new control
	if (pControl != NULL
		&& pControl->GetWindow() == this
		&& pControl->IsVisible()
		&& pControl->IsEnabled())
	{
		m_pFocus = pControl;

		if (m_pFocus) {
			m_pFocus->HandleMessageTemplate(kEventInternalSetFocus);
		}
		if (m_pFocus) {
			SendNotify(m_pFocus, kEventSetFocus);
		}
	}
}

void Window::SetFocusNeeded(Control* pControl)
{
	::SetFocus(m_hWnd);
	if (pControl == NULL) return;
	if (m_pFocus != NULL) {
		m_pFocus->HandleMessageTemplate(kEventInternalKillFocus);
		SendNotify(m_pFocus, kEventKillFocus);
		m_pFocus = NULL;
	}
	FINDTABINFO info = { 0 };
	info.pFocus = pControl;
	info.bForward = false;
	m_pFocus = m_pRoot->FindControl(__FindControlFromTab, &info, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
	m_bFocusNeeded = true;
	if (m_pRoot != NULL) m_pRoot->Arrange();
}

void Window::KillFocus()
{
	if (m_pFocus != NULL)
	{
		m_pFocus->HandleMessageTemplate(kEventInternalKillFocus);
		SendNotify(m_pFocus, kEventKillFocus);
		m_pFocus = NULL;
	}
}

void Window::SetCapture()
{
	::SetCapture(m_hWnd);
	m_bMouseCapture = true;
}

void Window::ReleaseCapture()
{
	::ReleaseCapture();
	m_bMouseCapture = false;
}

bool Window::IsCaptureControl(const ui::Control* pControl)
{
	return m_pEventClick == pControl || m_pEventPointer == pControl;
}

bool Window::IsCaptured() const
{
	return m_bMouseCapture;
}

ui::Control* Window::GetNewHover()
{
	return m_pNewHover;
}

HWND Window::GetTooltipWindow() const
{
	return m_hwndTooltip;
}

bool Window::SetNextTabControl(bool bForward)
{
	// If we're in the process of restructuring the layout we can delay the
	// focus calulation until the next repaint.
	if (m_bIsArranged && bForward) {
		m_bFocusNeeded = true;
		::InvalidateRect(m_hWnd, NULL, FALSE);
		return true;
	}
	// Find next/previous tabbable control
	FINDTABINFO info1 = { 0 };
	info1.pFocus = m_pFocus;
	info1.bForward = bForward;
	Control* pControl = m_pRoot->FindControl(__FindControlFromTab, &info1, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
	if (pControl == NULL) {
		if (bForward) {
			// Wrap around
			FINDTABINFO info2 = { 0 };
			info2.pFocus = bForward ? NULL : info1.pLast;
			info2.bForward = bForward;
			pControl = m_pRoot->FindControl(__FindControlFromTab, &info2, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
		}
		else {
			pControl = info1.pLast;
		}
	}
	if (pControl != NULL) SetFocus(pControl);
	m_bFocusNeeded = false;
	return true;
}

Control* Window::GetRoot() const
{
	return m_pRoot;
}

void Window::SetArrange(bool bArrange)
{
	m_bIsArranged = bArrange;
}

void Window::AddDelayedCleanup(Control* pControl)
{
	pControl->SetWindow(this, NULL, false);
	m_aDelayedCleanup.push_back(pControl);
	::PostMessage(m_hWnd, WM_APP + 1, 0, 0L);
}

Control* Window::FindControl(POINT pt) const
{
	ASSERT(m_pRoot);
	Control* pFindedControl = NULL;
	for (auto it : m_aIControlFromPointFinder)
	{
		if ((pFindedControl = it->FindControlFromPoint(pt)) != NULL)
			return pFindedControl;		
	}
	return m_pRoot->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
}

Control* Window::FindControl(const std::wstring& strName) const
{
	ASSERT(m_pRoot);
	Control* pFindedControl = NULL;
	auto it = m_mNameHash.find(strName);
	if (it != m_mNameHash.end()) {
		pFindedControl = it->second;
	}

	return pFindedControl;
}

Control* Window::FindSubControlByPoint(Control* pParent, POINT pt) const
{
	if (pParent == NULL) pParent = GetRoot();
	ASSERT(pParent);
	return pParent->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
}

Control* Window::FindSubControlByName(Control* pParent, const std::wstring& strName) const
{
	if (pParent == NULL) pParent = GetRoot();
	ASSERT(pParent);
	return pParent->FindControl(__FindControlFromName, (LPVOID)strName.c_str(), UIFIND_ALL);
}

Control* Window::FindSubControlByClass(Control* pParent, const type_info& typeinfo, int iIndex)
{
	if (pParent == NULL) pParent = GetRoot();
	ASSERT(pParent);
	m_aFoundControls.clear();
	m_aFoundControls.resize(iIndex + 1);
	return pParent->FindControl(__FindControlFromClass, (LPVOID)&typeinfo, UIFIND_ALL);
}

std::vector<Control*>* Window::FindSubControlsByClass(Control* pParent, const type_info& typeinfo)
{
	if (pParent == NULL) pParent = GetRoot();
	ASSERT(pParent);
	m_aFoundControls.clear();
	pParent->FindControl(__FindControlsFromClass, (LPVOID)&typeinfo, UIFIND_ALL);
	return &m_aFoundControls;
}

std::vector<Control*>* Window::GetSubControlsByClass()
{
	return &m_aFoundControls;
}

bool Window::SendNotify(EventType eventType, WPARAM wParam, LPARAM lParam)
{
	EventArgs msg;
	msg.pSender = nullptr;
	msg.Type = eventType;
	msg.ptMouse = GetLastMousePos();
	msg.dwTimestamp = ::GetTickCount();
	msg.wParam = wParam;
	msg.lParam = lParam;

	auto callback = OnEvent.find(msg.Type);
	if (callback != OnEvent.end()) {
		callback->second(&msg);
	}

	callback = OnEvent.find(kEventAll);
	if (callback != OnEvent.end()) {
		callback->second(&msg);
	}

	return true;
}

bool Window::SendNotify(Control* pControl, EventType msgType, WPARAM wParam, LPARAM lParam)
{
	EventArgs msg;
	msg.pSender = pControl;
	msg.Type = msgType;
	msg.ptMouse = GetLastMousePos();
	msg.dwTimestamp = ::GetTickCount();
	msg.wParam = wParam;
	msg.lParam = lParam;
	pControl->HandleMessageTemplate(msg);

	return true;
}

HDC Window::GetPaintDC() const
{
	return m_hDcPaint;
}

ui::IRenderContext* Window::GetRenderContext() const
{
	return m_renderContext.get();
}

void Window::Invalidate(const UiRect& rcItem)
{
	GlobalManager::AssertUIThread();

	::InvalidateRect(m_hWnd, &rcItem, FALSE);
	// Invalidating a layered window will not trigger a WM_PAINT message,
	// thus we have to post WM_PAINT by ourselves.
	if (m_bIsLayeredWindow) {
		::PostMessage(m_hWnd, WM_PAINT, (LPARAM)&rcItem, (WPARAM)FALSE);
	}
}

void Window::Paint()
{
	GlobalManager::AssertUIThread();

	if (::IsIconic(m_hWnd) || !m_pRoot) {
		PAINTSTRUCT ps = { 0 };
		::BeginPaint(m_hWnd, &ps);
		::EndPaint(m_hWnd, &ps);
		return;
	}

	if (m_bIsArranged && m_pRoot->IsArranged() && (m_pRoot->GetFixedWidth() == DUI_LENGTH_AUTO || m_pRoot->GetFixedHeight() == DUI_LENGTH_AUTO)) {
		CSize maxSize(99999, 99999);
		CSize needSize = m_pRoot->EstimateSize(maxSize);
		if (needSize.cx < m_pRoot->GetMinWidth()) needSize.cx = m_pRoot->GetMinWidth();
		if (m_pRoot->GetMaxWidth() >= 0 && needSize.cx > m_pRoot->GetMaxWidth()) needSize.cx = m_pRoot->GetMaxWidth();
		if (needSize.cy < m_pRoot->GetMinHeight()) needSize.cy = m_pRoot->GetMinHeight();
		if (needSize.cy > m_pRoot->GetMaxHeight()) needSize.cy = m_pRoot->GetMaxHeight();
		UiRect rect;
		::GetWindowRect(m_hWnd, &rect);
		::MoveWindow(m_hWnd, rect.left, rect.top, needSize.cx, needSize.cy, TRUE);
	}

	// Should we paint?
	UiRect rcPaint;
	if (!::GetUpdateRect(m_hWnd, &rcPaint, FALSE) && !m_bFirstLayout) {
		return;
	}

	UiRect rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	UiRect rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);

	//使用层窗口时，窗口部分在屏幕外时，获取到的无效区域仅仅是屏幕内的部分，这里做修正处理
	if (m_bIsLayeredWindow) {
		int xScreen = GetSystemMetrics(SM_XVIRTUALSCREEN);
		int yScreen = GetSystemMetrics(SM_YVIRTUALSCREEN);
		int cxScreen = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int cyScreen = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		if (rcWindow.left < xScreen && rcWindow.left + rcPaint.left == xScreen)
			rcPaint.left = rcClient.left;
		if (rcWindow.top < yScreen && rcWindow.top + rcPaint.top == yScreen)
			rcPaint.top = rcClient.top;
		if (rcWindow.right > cxScreen && rcWindow.left + rcPaint.right == xScreen + cxScreen)
			rcPaint.right = rcClient.right;
		if (rcWindow.bottom > cyScreen && rcWindow.top + rcPaint.bottom == yScreen + cyScreen)
			rcPaint.bottom = rcClient.bottom;
	}

	PAINTSTRUCT ps = { 0 };
	::BeginPaint(m_hWnd, &ps);

	if (m_bIsArranged)
	{
		m_bIsArranged = false;
		if (!::IsRectEmpty(&rcClient))
		{
			if (m_pRoot->IsArranged())
			{
				m_pRoot->SetPos(rcClient);
			}
			else
			{
				Control* pControl = NULL;
				while ((pControl = m_pRoot->FindControl(__FindControlFromUpdate, NULL, UIFIND_VISIBLE | UIFIND_ME_FIRST)) != nullptr)
				{
					pControl->SetPos(pControl->GetPos());
				}
			}

			if (m_bFirstLayout) {
				m_bFirstLayout = false;
				OnInitLayout();
			}
		}
	}

	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;
	if (m_renderContext->Resize(width, height))
	{
		rcPaint.left = 0;
		rcPaint.top = 0;
		rcPaint.right = width;
		rcPaint.bottom = height;
	}

	// 去掉alpha通道
	if (m_bIsLayeredWindow) {
		m_renderContext->ClearAlpha(rcPaint);
	}

	// 绘制
	AutoClip rectClip(m_renderContext.get(), rcPaint, true);
	CPoint ptOldWindOrg = m_renderContext->OffsetWindowOrg(m_renderOffset);
	m_pRoot->Paint(m_renderContext.get(), rcPaint);
	m_pRoot->PaintChild(m_renderContext.get(), rcPaint);
	m_renderContext->SetWindowOrg(ptOldWindOrg);

	// alpha修复
	if (m_bIsLayeredWindow) {
		if (m_shadow.IsShadowAttached() && m_renderOffset.x == 0 && m_renderOffset.y == 0) {
			//补救由于Gdi绘制造成的alpha通道为0
			UiRect rcNewPaint = rcPaint;
			rcNewPaint.Intersect(m_pRoot->GetPaddingPos());
			UiRect rcRootPadding = m_pRoot->GetLayout()->GetPadding();

			//考虑圆角
			rcRootPadding.left += 1;
			rcRootPadding.top += 1;
			rcRootPadding.right += 1;
			rcRootPadding.bottom += 1;

			m_renderContext->RestoreAlpha(rcNewPaint, rcRootPadding);
		}
		else {
			UiRect rcAlphaFixCorner = GetAlphaFixCorner();
			if (rcAlphaFixCorner.left > 0 || rcAlphaFixCorner.top > 0 || rcAlphaFixCorner.right > 0 || rcAlphaFixCorner.bottom > 0)
			{
				UiRect rcNewPaint = rcPaint;
				UiRect rcRootPaddingPos = m_pRoot->GetPaddingPos();
				rcRootPaddingPos.Deflate(rcAlphaFixCorner);
				rcNewPaint.Intersect(rcRootPaddingPos);

				UiRect rcRootPadding;
				m_renderContext->RestoreAlpha(rcNewPaint, rcRootPadding);
			}
		}
	}

	// 渲染到窗口
	if (m_bIsLayeredWindow) {
		CPoint pt(rcWindow.left, rcWindow.top);
		CSize szWindow(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
		CPoint ptSrc;
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, m_nAlpha, AC_SRC_ALPHA };
		::UpdateLayeredWindow(m_hWnd, NULL, &pt, &szWindow, m_renderContext->GetDC(), &ptSrc, 0, &bf, ULW_ALPHA);
	}
	else {
		::BitBlt(ps.hdc, rcPaint.left, rcPaint.top, rcPaint.GetWidth(),
			rcPaint.GetHeight(), m_renderContext->GetDC(), rcPaint.left, rcPaint.top, SRCCOPY);
	}

	::EndPaint(m_hWnd, &ps);
}

void Window::SetAlpha(int nAlpha)
{
	ASSERT(nAlpha >= 0 && nAlpha <= 255);
	if (m_pRoot) {
		m_nAlpha = nAlpha;
		Invalidate(m_pRoot->GetPos());
	}
}

bool Window::IsRenderTransparent() const
{
	return m_renderContext->IsRenderTransparent();
}

bool Window::SetRenderTransparent(bool bCanvasTransparent)
{
	return m_renderContext->SetRenderTransparent(bCanvasTransparent);
}

bool Window::IsLayeredWindow()
{
	return m_bIsLayeredWindow;
}

void Window::SetRenderOffset(CPoint renderOffset)
{
	if (m_pRoot) {
		m_renderOffset = renderOffset;
		Invalidate(m_pRoot->GetPos());
	}
}

void Window::SetRenderOffsetX(int renderOffsetX) 
{
	if (m_pRoot) {
		m_renderOffset.x = renderOffsetX;
		Invalidate(m_pRoot->GetPos());
	}
}

void Window::SetRenderOffsetY(int renderOffsetY) 
{
	if (m_pRoot) {
		m_renderOffset.y = renderOffsetY;
		Invalidate(m_pRoot->GetPos());
	}
}

void Window::OnInitLayout()
{
	if (m_pRoot && IsWindowsVistaOrGreater()) {
		AnimationPlayer* animationArgs = m_pRoot->GetAnimationManager().SetFadeAlpha(true);
		m_nAlpha = 0;
		std::function<void(int)> playCallback = nbase::Bind(&Window::SetAlpha, this, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);
		m_pRoot->SetVisible(true);
	}

	//auto fromLeftAnimationPlayer = m_pRoot->GetAnimationManager().GetAnimationPlayer(kAnimationInoutXFromLeft);
	//if (!fromLeftAnimationPlayer && m_shadow.IsShadowAttached()) {
	//	Control* tmpRoot = m_pRoot->GetItemAt(0);
	//	fromLeftAnimationPlayer = tmpRoot->GetAnimationManager().GetAnimationPlayer(kAnimationInoutXFromLeft);
	//}
	//auto fromRightAnimationPlayer = m_pRoot->GetAnimationManager().GetAnimationPlayer(kAnimationInoutXFromRight);
	//if (!fromRightAnimationPlayer && m_shadow.IsShadowAttached()) {
	//	Control* tmpRoot = m_pRoot->GetItemAt(0);
	//	fromRightAnimationPlayer = tmpRoot->GetAnimationManager().GetAnimationPlayer(kAnimationInoutXFromRight);
	//}
	//AnimationPlayer* fadeInoutXPlayer = nullptr;
	//if (fromLeftAnimationPlayer) {
	//	fadeInoutXPlayer = m_pRoot->GetAnimationManager().SetFadeInOutX(true, false);
	//}
	//if (fromRightAnimationPlayer) {
	//	fadeInoutXPlayer = m_pRoot->GetAnimationManager().SetFadeInOutX(true, true);
	//}
	//if (fadeInoutXPlayer) {
	//	std::function<void(int)> playCallback = nbase::Bind(&Window::SetRenderOffsetX, this, std::placeholders::_1);
	//	fadeInoutXPlayer->SetCallback(playCallback);
	//	fadeInoutXPlayer->SetMaxTotalMillSeconds(250);
	//	m_pRoot->SetVisible(true);
	//}
}

Control* CALLBACK Window::__FindControlFromNameHash(Control* pThis, LPVOID pData)
{
	Window* pManager = static_cast<Window*>(pData);
	std::wstring sName = pThis->GetName();
	if( sName.empty() ) return NULL;
	// Add this control to the hash list
	pManager->m_mNameHash[sName] = pThis;
	return NULL; // Attempt to add all controls
}

Control* CALLBACK Window::__FindControlFromCount(Control* /*pThis*/, LPVOID pData)
{
	int* pnCount = static_cast<int*>(pData);
	(*pnCount)++;
	return NULL;  // Count all controls
}

Control* CALLBACK Window::__FindControlFromPoint(Control* pThis, LPVOID pData)
{
	LPPOINT pPoint = static_cast<LPPOINT>(pData);
	UiRect pos = pThis->GetPos();
	return ::PtInRect(&pos, *pPoint) ? pThis : NULL;
}

Control* CALLBACK Window::__FindControlFromTab(Control* pThis, LPVOID pData)
{
	FINDTABINFO* pInfo = static_cast<FINDTABINFO*>(pData);
	if (pInfo->pFocus == pThis) {
		if (pInfo->bForward) pInfo->bNextIsIt = true;
		return pInfo->bForward ? NULL : pInfo->pLast;
	}
	if ((pThis->GetControlFlags() & UIFLAG_TABSTOP) == 0) return NULL;
	pInfo->pLast = pThis;
	if (pInfo->bNextIsIt) return pThis;
	if (pInfo->pFocus == NULL) return pThis;
	return NULL;  // Examine all controls
}

//Control* CALLBACK Window::__FindControlFromShortcut(Control* pThis, LPVOID pData)
//{
//	if( !pThis->IsVisible() ) return NULL; 
//	FINDSHORTCUT* pFS = static_cast<FINDSHORTCUT*>(pData);
//	if( pFS->ch == toupper(pThis->GetShortcut()) ) pFS->bPickNext = true;
//	if( typeid(*pThis) == typeid(Label) ) return NULL;   // Labels never get focus!
//	return pFS->bPickNext ? pThis : NULL;
//}

Control* CALLBACK Window::__FindControlFromUpdate(Control* pThis, LPVOID pData)
{
	return pThis->IsArranged() ? pThis : NULL;
}

Control* CALLBACK Window::__FindControlFromName(Control* pThis, LPVOID pData)
{
	LPCTSTR pstrName = static_cast<LPCTSTR>(pData);
	const std::wstring& sName = pThis->GetName();
	if( sName.empty() ) return NULL;
	return (_tcsicmp(sName.c_str(), pstrName) == 0) ? pThis : NULL;
}

Control* CALLBACK Window::__FindControlFromClass(Control* pThis, LPVOID pData)
{
	type_info* pTypeInfo = static_cast<type_info*>(pData);
	std::vector<Control*>* pFoundControls = pThis->GetWindow()->GetSubControlsByClass();
	if( typeid(*pThis) == *pTypeInfo ) {
		int iIndex = -1;
		while( (*pFoundControls)[++iIndex] != NULL ) ;
		if( (std::size_t)iIndex < pFoundControls->size() ) (*pFoundControls)[iIndex] = pThis;
	}
	if( (*pFoundControls)[pFoundControls->size() - 1] != NULL ) return pThis; 
	return NULL;
}

Control* CALLBACK Window::__FindControlsFromClass(Control* pThis, LPVOID pData)
{
	type_info* pTypeInfo = static_cast<type_info*>(pData);
	if( typeid(*pThis) == *pTypeInfo ) 
		pThis->GetWindow()->GetSubControlsByClass()->push_back(pThis);
	return NULL;
}

} // namespace ui
