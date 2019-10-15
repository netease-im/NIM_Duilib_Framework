#include "stdafx.h"
#include "ui_menu.h"

namespace nim_comp {

/////////////////////////////////////////////////////////////////////////////////////
//

Control* CMenuWnd::CreateControl(const std::wstring& pstrClass)
{
	if (pstrClass == kMenuElementUIInterfaceName)
	{
		return new CMenuElementUI();
	}
	return NULL;
}

BOOL CMenuWnd::Receive(ContextMenuParam param)
{
	switch (param.wParam)
	{
	case eMenuCloseAll:
		Close();
		break;
	case eMenuCloseThis:
	{
		HWND hParent = GetParent(m_hWnd);
		while (hParent != NULL)
		{
			if (hParent == param.hWnd)
			{
				Close();
				break;
			}
			hParent = GetParent(hParent);
		}
	}
		break;
	default:
		break;
	}

	return TRUE;
}

CMenuWnd::CMenuWnd(HWND hParent) :
	m_hParent(hParent),
	m_xml(_T("")),
	no_focus_(false),
	m_pOwner(nullptr),
	m_pLayout(nullptr)
{
}

void CMenuWnd::Init(STRINGorID xml, LPCTSTR pSkinType, POINT point, PopupPosType popupPosType, bool no_focus, CMenuElementUI* pOwner)
{
	m_BasedPoint = point;
	m_popupPosType = popupPosType;

	m_xml = xml;
	no_focus_ = no_focus;
	m_pOwner = pOwner;

	CMenuWnd::GetMenuObserver().AddReceiver(this);

	Create(m_hParent, L"NIM_DUILIB_MENU_WINDOW", WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, true, UiRect());
	// HACK: Don't deselect the parent's caption
	HWND hWndParent = m_hWnd;
	while (::GetParent(hWndParent) != NULL) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, no_focus ? SW_SHOWNOACTIVATE : SW_SHOW);
	if (m_pOwner)
	{
		ResizeSubMenu();
	}
	else
	{
		ResizeMenu();
	}
	//::SetWindowPos(m_hWnd, NULL, m_BasedPoint.x, m_BasedPoint.y, 0, 0, SWP_NOSIZE | (no_focus ? SWP_NOACTIVATE : 0));
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	if (m_pOwner != NULL) {
		for (int i = 0; i < m_pLayout->GetCount(); i++) {
			CMenuElementUI* pItem = static_cast<CMenuElementUI*>(m_pLayout->GetItemAt(i)); //这里确定是CMenuElementUI*，static_cast效率高
			if (pItem)
			{
				pItem->SetOwner(dynamic_cast<IListOwner*>(m_pOwner->GetParent()));//这里的父控件可能仍然是menuitem,那么置空即可
				pItem->SetWindow(m_pOwner->GetWindow(), m_pOwner, false);         //更改item的归属
				pItem->SetVisible(false);
				pItem->SetInternVisible(false);
			}
		}
		m_pLayout->RemoveAll();
		m_pOwner->m_pSubWindow = NULL;
		//m_pOwner->m_uButtonState &= ~UISTATE_PUSHED;  这里可能需要替换，暂时注释
		m_pOwner->Invalidate();
	}
	delete this;
}

std::wstring CMenuWnd::GetWindowClassName() const
{
	return _T("MenuWnd");
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
 	case WM_KILLFOCUS:
 	{
		HWND hFocusWnd = (HWND)wParam;

		BOOL bInMenuWindowList = FALSE;
		ContextMenuParam param;
		param.hWnd = GetHWND();

		ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(GetMenuObserver());
		ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
		while (pReceiver != NULL) {
			CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
			if (pContextMenu != NULL && pContextMenu->GetHWND() == hFocusWnd) {
				bInMenuWindowList = TRUE;
				break;
			}
			pReceiver = iterator.next();
		}

		if (!bInMenuWindowList) {
			param.wParam = eMenuCloseAll;
			GetMenuObserver().RBroadcast(param);

			return 0;
		}
	}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE || wParam == VK_LEFT)
			Close();
		break;
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		return 0L;

	default:
		break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CMenuWnd::ResizeMenu()
{
	Control* pRoot =GetRoot();
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	UiRect rcWork = oMonitor.rcWork;

	CSize szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szAvailable = pRoot->EstimateSize(szAvailable);
	SetInitSize(szAvailable.cx, szAvailable.cy);

	CSize szInit = GetInitSize();
	UiRect rc;
	CPoint point = m_BasedPoint;
	rc.left = point.x;
	rc.top = point.y;
	rc.right = rc.left + szInit.cx;
	rc.bottom = rc.top + szInit.cy;

	int nWidth = rc.GetWidth();
	int nHeight = rc.GetHeight();

// 	if (m_popupPosType & eMenuAlignment_Right)
// 	{
// 		rc.right = point.x;
// 		rc.left = rc.right - nWidth;
// 	}
// 
// 	if (m_popupPosType & eMenuAlignment_Bottom)
// 	{
// 		rc.bottom = point.y;
// 		rc.top = rc.bottom - nHeight;
// 	}

	SetForegroundWindow(m_hWnd);
	//MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top,
		rc.GetWidth(), rc.GetHeight(),
		SWP_SHOWWINDOW);
}

void CMenuWnd::ResizeSubMenu()
{
	// Position the popup window in absolute space
	RECT rcOwner = m_pOwner->GetPos();
	RECT rc = rcOwner;

	int cxFixed = 0;
	int cyFixed = 0;

	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	UiRect rcWork = oMonitor.rcWork;
	CSize szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };

	for (int it = 0; it < m_pLayout->GetCount(); it++) {
		//取子菜单项中的最大值作为菜单项
		CMenuElementUI* pItem = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(it));
		if (pItem)
		{
			SIZE sz = pItem->EstimateSize(szAvailable);
			cyFixed += sz.cy;

			if (cxFixed < sz.cx)
				cxFixed = sz.cx;
		}
	}

	RECT rcWindow;
	GetWindowRect(m_pOwner->GetWindow()->GetHWND(), &rcWindow);

	rc.top = rcOwner.top;
	rc.bottom = rc.top + cyFixed;
	::MapWindowRect(m_pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
	rc.left = rcWindow.right;
	rc.left -= GetShadowCorner().left + GetShadowCorner().right;
	rc.top -= GetShadowCorner().top;    //这里有很大的阴影窗口，待准确计算
	rc.right = rc.left + cxFixed;
	//rc.right += 2;

	bool bReachBottom = false;
	bool bReachRight = false;
	LONG chRightAlgin = 0;
	LONG chBottomAlgin = 0;

	RECT rcPreWindow = { 0 };
	ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(GetMenuObserver());
	ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
	while (pReceiver != NULL) {
		CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
		if (pContextMenu != NULL) {
			GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

			bReachRight = rcPreWindow.left >= rcWindow.right;
			bReachBottom = rcPreWindow.top >= rcWindow.bottom;
			if (pContextMenu->GetHWND() == m_pOwner->GetWindow()->GetHWND()
				|| bReachBottom || bReachRight)
				break;
		}
		pReceiver = iterator.next();
	}

	if (bReachBottom)
	{
		rc.bottom = rcWindow.top;
		rc.top = rc.bottom - cyFixed;
	}

	if (bReachRight)
	{
		rc.right = rcWindow.left;
		rc.left = rc.right - cxFixed;
	}

	if (bReachBottom)
	{
		rc.bottom = rcWindow.top;
		rc.top = rc.bottom - cyFixed;
	}

	if (bReachRight)
	{
		rc.right = rcWindow.left;
		rc.left = rc.right - cxFixed;
	}

	if (rc.bottom > rcWork.bottom)
	{
		rc.bottom = rc.top;
		rc.top = rc.bottom - cyFixed;
	}

	if (rc.right > rcWork.right)
	{
		rc.right = rcWindow.left;
		rc.left = rc.right - cxFixed;
	}

	if (rc.top < rcWork.top)
	{
		rc.top = rcOwner.top;
		rc.bottom = rc.top + cyFixed;
	}

	if (rc.left < rcWork.left)
	{
		rc.left = rcWindow.right;
		rc.right = rc.left + cxFixed;
	}

	//MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top , FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top,
		SWP_SHOWWINDOW);
}

void CMenuWnd::Show()
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	UiRect rcWork = oMonitor.rcWork;
	UiRect monitor_rect = oMonitor.rcMonitor;
	ui::CSize szInit = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szInit = GetRoot()->EstimateSize(szInit);
	szInit.cx -= GetShadowCorner().left + GetShadowCorner().right;
	szInit.cy -= GetShadowCorner().top + GetShadowCorner().bottom;
	if (m_popupPosType == RIGHT_BOTTOM)
	{
		if (m_BasedPoint.y + szInit.cy > monitor_rect.bottom)
		{
			m_BasedPoint.y -= szInit.cy;
		}
	}
	else if (m_popupPosType == RIGHT_TOP)
	{
		if (m_BasedPoint.y - szInit.cy >= monitor_rect.top)
		{
			m_BasedPoint.y -= szInit.cy;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	UiRect rc;
	rc.left = m_BasedPoint.x;
	rc.top = m_BasedPoint.y;
	if (rc.top < monitor_rect.top)
	{
		rc.top = monitor_rect.top;
	}

	//判断是否超出屏幕
	if (rc.left > monitor_rect.right - szInit.cx)
	{
		rc.left = monitor_rect.right - szInit.cx;
	}
	if (rc.left < monitor_rect.left)
	{
		rc.left = monitor_rect.left;
	}
	rc.right = rc.left + szInit.cx;
	rc.bottom = rc.top + szInit.cy;

	SetPos(rc, false, SWP_SHOWWINDOW | (no_focus_ ? SWP_NOACTIVATE : 0), HWND_TOPMOST, false);
	if (!no_focus_)
		SetForegroundWindow(m_hWnd);
}


void CMenuWnd::InitWindow()
{
	if (m_pOwner)
	{
		m_pLayout = static_cast<ListBox*>(FindControl(L"submenu"));
		ASSERT(m_pLayout);
		m_pLayout->SetAutoDestroy(false);

		for (int i = 0; i < m_pOwner->GetCount(); i++) {
			CMenuElementUI* subMenuItem = dynamic_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i));
			if (subMenuItem)
			{
				//此时子菜单item属于2个子菜单，注意生命周期的维护，子菜单窗口退出不能销毁控件，需要归还原控件，
				//此时子菜单item的父控件是准的，但父控件可能不是Vlist，SetOwner的入参是Vlist，这时owner置空
				//见OnFinalMessage
				m_pLayout->Add(subMenuItem); //内部会调用subMenuItem->SetOwner(m_pLayout); 会调用SetWindows，改变了归属窗口、父控件。
			}
		}
	}
}



// MenuElementUI
const TCHAR* const kMenuElementUIInterfaceName = _T("MenuElement");

CMenuElementUI::CMenuElementUI() :
m_pSubWindow(nullptr)
{
	m_bMouseChildEnabled = false;
}

CMenuElementUI::~CMenuElementUI()
{}

bool CMenuElementUI::ButtonUp(EventArgs& msg)
{
	std::weak_ptr<nbase::WeakFlag> weakFlag = m_pWindow->GetWeakFlag();
	bool ret = __super::ButtonUp(msg);
	if (ret && !weakFlag.expired()) {
		//这里处理下如果有子菜单则显示子菜单
		if (!CheckSubMenuItem())
		{
			ContextMenuParam param;
			param.hWnd = GetWindow()->GetHWND();
			param.wParam = eMenuCloseAll;
			CMenuWnd::GetMenuObserver().RBroadcast(param);
		}
	}

	return ret;
}


bool CMenuElementUI::MouseEnter(EventArgs& msg)
{
	std::weak_ptr<nbase::WeakFlag> weakFlag = m_pWindow->GetWeakFlag();
	bool ret = __super::MouseEnter(msg);
	if (ret && !weakFlag.expired()) {
		//这里处理下如果有子菜单则显示子菜单
		if (!CheckSubMenuItem())
		{
			ContextMenuParam param;
			param.hWnd = GetWindow()->GetHWND();
			param.wParam = eMenuCloseThis;
			CMenuWnd::GetMenuObserver().RBroadcast(param);
			m_pOwner->SelectItem(GetIndex(), true);
		}
	}

	return ret;
}

void CMenuElementUI::PaintChild(IRenderContext* pRender, const UiRect& rcPaint)
{
	UiRect rcTemp;
	if (!::IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) return;

	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		//尝试转CMenuElementUI
		CMenuElementUI* subMenuItem = dynamic_cast<CMenuElementUI*>(*it);
		if (subMenuItem)
		{
			continue;
		}
		Control* pControl = *it;
		if (!pControl->IsVisible()) continue;
		pControl->AlphaPaint(pRender, rcPaint);
	}
}

bool CMenuElementUI::CheckSubMenuItem()
{
	bool hasSubMenu = false;
	for (int i = 0; i < GetCount(); ++i)
	{
		CMenuElementUI* subMenuItem = dynamic_cast<CMenuElementUI*>(GetItemAt(i));
		if (subMenuItem)
		{
			subMenuItem->SetVisible(true);
			subMenuItem->SetInternVisible(true);
			hasSubMenu = true;
		}
	}
	if (hasSubMenu)
	{
		m_pOwner->SelectItem(GetIndex(), true);
		CreateMenuWnd();
	}
	return hasSubMenu;
}

void CMenuElementUI::CreateMenuWnd()
{
	if (m_pSubWindow) return;
	m_pSubWindow = new CMenuWnd(GetWindow()->GetHWND());

	ContextMenuParam param;
	param.hWnd =GetWindow()->GetHWND();
	param.wParam = eMenuCloseThis;
	CMenuWnd::GetMenuObserver().RBroadcast(param);

	m_pSubWindow->Init(_T("submenu.xml"), _T(""), CPoint(), CMenuWnd::RIGHT_BOTTOM, false, this);
}

} // namespace ui
