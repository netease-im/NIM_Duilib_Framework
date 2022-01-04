#include "stdafx.h"

namespace ui
{

WindowImplBase::WindowImplBase()
{

}


WindowImplBase::~WindowImplBase()
{

}

void WindowImplBase::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	ReapObjects(GetRoot());
	delete this;
}

LONG WindowImplBase::GetStyle()
{
	LONG styleValue = ::GetWindowLong(GetHWND(), GWL_STYLE);
	styleValue &= ~WS_CAPTION;

	return styleValue;
}

UINT WindowImplBase::GetClassStyle() const
{
	return CS_DBLCLKS;
}

Control* WindowImplBase::CreateControl(const std::wstring& pstrClass)
{
	return NULL;
}

LRESULT WindowImplBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return FALSE;
}

LRESULT WindowImplBase::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(GetHWND()) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT WindowImplBase::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT WindowImplBase::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (IsZoomed(m_hWnd)) {
		LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam;
		if (lpPos->flags & SWP_FRAMECHANGED) // 第一次最大化，而不是最大化之后所触发的WINDOWPOSCHANGE
		{
			POINT pt = { 0, 0 };
			HMONITOR hMontorPrimary = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
			HMONITOR hMonitorTo = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY);

			// 先把窗口最大化，再最小化，然后恢复，此时MonitorFromWindow拿到的HMONITOR不准确
			// 判断GetWindowRect的位置如果不正确（最小化时得到的位置信息是-38000），则改用normal状态下的位置，来获取HMONITOR
			RECT rc = { 0 };
			GetWindowRect(m_hWnd, &rc);
			if (rc.left < -10000 && rc.top < -10000 && rc.right < -10000 && rc.right < -10000) {
				WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
				GetWindowPlacement(m_hWnd, &wp);
				hMonitorTo = MonitorFromRect(&wp.rcNormalPosition, MONITOR_DEFAULTTOPRIMARY);
			}
			if (hMonitorTo != hMontorPrimary) {
				// 解决无边框窗口在双屏下面（副屏分辨率大于主屏）时，最大化不正确的问题
				MONITORINFO  miTo = { sizeof(miTo), 0 };
				GetMonitorInfo(hMonitorTo, &miTo);

				lpPos->x = miTo.rcWork.left;
				lpPos->y = miTo.rcWork.top;
				lpPos->cx = miTo.rcWork.right - miTo.rcWork.left;
				lpPos->cy = miTo.rcWork.bottom - miTo.rcWork.top;
			}
		}
	}
	return 0;
}

LRESULT WindowImplBase::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT WindowImplBase::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(GetHWND(), &pt);

	UiRect rcClient;
	::GetClientRect(GetHWND(), &rcClient);
	
	rcClient.Deflate(m_shadow.GetShadowCorner());
	
	if( !::IsZoomed(GetHWND()) ) {
		UiRect rcSizeBox = GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if (pt.y >= rcClient.top) {
				if (pt.x < (rcClient.left + rcSizeBox.left) && pt.x >= rcClient.left) return HTTOPLEFT;
				else if (pt.x >(rcClient.right - rcSizeBox.right) && pt.x <= rcClient.right) return HTTOPRIGHT;
				else return HTTOP;
			}
			else return HTCLIENT;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if (pt.y <= rcClient.bottom) {
				if (pt.x < (rcClient.left + rcSizeBox.left) && pt.x >= rcClient.left) return HTBOTTOMLEFT;
				else if (pt.x > (rcClient.right - rcSizeBox.right) && pt.x <= rcClient.right) return HTBOTTOMRIGHT;
				else return HTBOTTOM;
			}
			else return HTCLIENT;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) {
			if (pt.x >= rcClient.left) return HTLEFT;
			else return HTCLIENT;
		}
		if (pt.x > rcClient.right - rcSizeBox.right) {
			if (pt.x <= rcClient.right) return HTRIGHT;
			else return HTCLIENT;
		}
	}

	UiRect rcCaption = GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcClient.top + rcCaption.top && pt.y < rcClient.top + rcCaption.bottom ) {
			Control* pControl = FindControl(pt);
			if( pControl ) {
				if (dynamic_cast<Button*>(pControl) || dynamic_cast<ButtonBox*>(pControl) || 
					dynamic_cast<RichEdit*>(pControl) || dynamic_cast<Combo*>(pControl))
					return HTCLIENT;
				else
					return HTCAPTION;
			}	
	}

	return HTCLIENT;
}

LRESULT WindowImplBase::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTONEAREST), &oMonitor);
	UiRect rcWork = oMonitor.rcWork;
	UiRect rcMonitor = oMonitor.rcMonitor;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	UiRect rcMaximize = GetMaximizeInfo();
	if (rcMaximize.GetWidth() > 0 && rcMaximize.GetHeight() > 0) {
		lpMMI->ptMaxPosition.x	= rcWork.left + rcMaximize.left;
		lpMMI->ptMaxPosition.y	= rcWork.top + rcMaximize.top;
		lpMMI->ptMaxSize.x = rcMaximize.GetWidth();
		lpMMI->ptMaxSize.y = rcMaximize.GetHeight();
	} 
	else {
		// 计算最大化时，正确的原点坐标
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x = rcWork.GetWidth();
		lpMMI->ptMaxSize.y = rcWork.GetHeight();
	}

	if (GetMaxInfo().cx != 0) {
		lpMMI->ptMaxTrackSize.x = GetMaxInfo(true).cx;
	}
	if (GetMaxInfo().cy != 0) {
		lpMMI->ptMaxTrackSize.y = GetMaxInfo(true).cy;
	}
	if (GetMinInfo().cx != 0) {
		lpMMI->ptMinTrackSize.x = GetMinInfo(true).cx;
	}
	if (GetMinInfo().cy != 0) {
		lpMMI->ptMinTrackSize.y = GetMinInfo(true).cy;
	}

	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CSize szRoundCorner = GetRoundCorner();
	if( !::IsIconic(GetHWND()) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		UiRect rcWnd;
		::GetWindowRect(GetHWND(), &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(GetHWND(), hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)	{
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(GetHWND());
	LRESULT lRes = Window::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(GetHWND()) != bZoomed )
	{

	}

	return lRes;
}

LRESULT WindowImplBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::SetWindowLong(this->GetHWND(), GWL_STYLE, GetStyle());

	Init(m_hWnd);
	SetWindowResourcePath(GetSkinFolder());

	WindowBuilder builder;
	std::wstring strSkinFile = GetWindowResourcePath() + GetSkinFile();

	auto callback = nbase::Bind(&WindowImplBase::CreateControl, this, std::placeholders::_1);
	auto pRoot = (Box*)builder.Create(strSkinFile.c_str(), callback, this);

	ASSERT(pRoot && L"Faield to load xml file.");
	if (pRoot == NULL) {
		TCHAR szErrMsg[MAX_PATH] = { 0 };
		_stprintf_s(szErrMsg, L"Failed to load xml file %s", strSkinFile.c_str());
		MessageBox(NULL, szErrMsg, _T("Duilib"), MB_OK | MB_ICONERROR);
		return -1;
	}
	
	pRoot = m_shadow.AttachShadow(pRoot);
	AttachDialog(pRoot);
	InitWindow();

	if (pRoot->GetFixedWidth() == DUI_LENGTH_AUTO || pRoot->GetFixedHeight() == DUI_LENGTH_AUTO) {
		CSize maxSize(99999, 99999);
		CSize needSize = pRoot->EstimateSize(maxSize);
		if( needSize.cx < pRoot->GetMinWidth() ) needSize.cx = pRoot->GetMinWidth();
		if( pRoot->GetMaxWidth() >= 0 && needSize.cx > pRoot->GetMaxWidth() ) needSize.cx = pRoot->GetMaxWidth();
		if( needSize.cy < pRoot->GetMinHeight() ) needSize.cy = pRoot->GetMinHeight();
		if( needSize.cy > pRoot->GetMaxHeight() ) needSize.cy = pRoot->GetMaxHeight();

		::MoveWindow(m_hWnd, 0, 0, needSize.cx, needSize.cy, FALSE);
	}

	Control *pControl = (Control*)FindControl(L"closebtn");
	if (pControl) {
		Button *pCloseBtn = dynamic_cast<Button*>(pControl);
		ASSERT(pCloseBtn);
		pCloseBtn->AttachClick(nbase::Bind(&WindowImplBase::OnButtonClick, this, std::placeholders::_1));
	}

	pControl = (Control*)FindControl(L"minbtn");
	if (pControl)	{
		Button* pMinBtn = dynamic_cast<Button*>(pControl);
		ASSERT(pMinBtn);
		pMinBtn->AttachClick(nbase::Bind(&WindowImplBase::OnButtonClick, this, std::placeholders::_1));
	}

	pControl = (Control*)FindControl(L"maxbtn");
	if (pControl)	{
		Button* pMaxBtn = dynamic_cast<Button*>(pControl);
		ASSERT(pMaxBtn);
		pMaxBtn->AttachClick(nbase::Bind(&WindowImplBase::OnButtonClick, this, std::placeholders::_1));
	}

	pControl = (Control*)FindControl(L"restorebtn");
	if (pControl)	{
		Button* pRestoreBtn = dynamic_cast<Button*>(pControl);
		ASSERT(pRestoreBtn);
		pRestoreBtn->AttachClick(nbase::Bind(&WindowImplBase::OnButtonClick, this, std::placeholders::_1));
	}

	return 0;
}

LRESULT WindowImplBase::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT WindowImplBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_WINDOWPOSCHANGING: lRes = OnWindowPosChanging(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_CHAR:			lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEHOVER:		lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
	default:				bHandled = FALSE; break;
	}

	if (bHandled) return lRes;

	return Window::HandleMessage(uMsg, wParam, lParam);
}

bool WindowImplBase::OnButtonClick(EventArgs* msg)
{
	std::wstring sCtrlName = msg->pSender->GetName();
	if( sCtrlName == _T("closebtn") ) {
		Close();
	}
	else if( sCtrlName == _T("minbtn")) { 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
	}
	else if( sCtrlName == _T("maxbtn"))	{
		Control* pMaxButton = (Control*)FindControl(L"maxbtn");
		Control* pRestoreButton = (Control*)FindControl(L"restorebtn");
		if (pMaxButton && pRestoreButton) {
			pMaxButton->SetVisible(false);
			pRestoreButton->SetVisible(true);
		}
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else if( sCtrlName == _T("restorebtn"))	{
		Control* pMaxButton = (Control*)FindControl(L"maxbtn");
		Control* pRestoreButton = (Control*)FindControl(L"restorebtn");
		if (pMaxButton && pRestoreButton) {
			pMaxButton->SetVisible(true);
			pRestoreButton->SetVisible(false);
		}
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
	}

	return true;
}


void WindowImplBase::ActiveWindow()
{
	if (::IsWindow(m_hWnd))
	{
		if (::IsIconic(m_hWnd))
		{
			::ShowWindow(m_hWnd, SW_RESTORE);
		}
		else
		{
			if (!::IsWindowVisible(m_hWnd))
				::ShowWindow(m_hWnd, SW_SHOW);
			::SetForegroundWindow(m_hWnd);
		}
	}
}

void WindowImplBase::SetTaskbarTitle(const std::wstring &title)
{
	::SetWindowTextW(m_hWnd, title.c_str());
}


void WindowImplBase::ToTopMost(bool forever)
{
	ASSERT(::IsWindow(m_hWnd));
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (!forever)
	{
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}
}