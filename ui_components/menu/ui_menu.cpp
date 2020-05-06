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
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void CMenuWnd::DetouchOwner()
{
	if (m_pOwner)
	{
		m_pLayout->SelectItem(-1);

		for (int i = 0; i < m_pOwner->GetSubMenuItemCount(); i++) {
			CMenuElementUI* pItem = static_cast<CMenuElementUI*>(m_pOwner->GetSubMenuItemAt(i));
			if (pItem)
			{
				pItem->SetWindow(nullptr, nullptr, false);
			}
		}

		m_pLayout->RemoveAll();
		m_pOwner->m_pSubWindow = NULL;
		//m_pOwner->m_uButtonState &= ~UISTATE_PUSHED;  这里可能需要替换，暂时注释
		m_pOwner->Invalidate();
		m_pOwner = NULL;
	}
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	Window::OnFinalMessage(hWnd);
	RemoveObserver();
	DetouchOwner();
	ReapObjects(GetRoot());
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
		else if (wParam == VK_RIGHT)
		{
			if (m_pLayout)
			{
				int index = m_pLayout->GetCurSel();
				CMenuElementUI* pItem = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(index));
				if (pItem)
				{
					pItem->CheckSubMenuItem();
				}
			}
		}
		else if (wParam == VK_RETURN || wParam == VK_SPACE)
		{
			if (m_pLayout)
			{
				int index = m_pLayout->GetCurSel();
				CMenuElementUI* pItem = dynamic_cast<CMenuElementUI*>(m_pLayout->GetItemAt(index));
				if (pItem)
				{
					if (!pItem->CheckSubMenuItem())
					{
						ContextMenuParam param;
						param.hWnd = m_hWnd;
						param.wParam = eMenuCloseAll;
						CMenuWnd::GetMenuObserver().RBroadcast(param);
					}
				}
			}
		}
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
	Control* pRoot = GetRoot();
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	//点击在哪里，以哪里的屏幕为主
	::GetMonitorInfo(::MonitorFromPoint(m_BasedPoint, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	UiRect rcWork = oMonitor.rcWork;

	CSize szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szAvailable = pRoot->EstimateSize(szAvailable);   //这里带上了阴影窗口
	SetInitSize(szAvailable.cx, szAvailable.cy);
	UiRect rcCorner = GetShadowCorner();
	CSize szInit=szAvailable;   
	szInit.cx -= rcCorner.left + rcCorner.right;
	szInit.cy -= rcCorner.top + rcCorner.bottom; //这里去掉阴影窗口，即用户的视觉有效面积 szInit<=szAvailable
	
	CPoint point = m_BasedPoint;  //这里有个bug，由于坐标点与包含在窗口内，会直接出发mouseenter导致出来子菜单，偏移1个像素
	if (m_popupPosType & eMenuAlignment_Right)
	{
		point.x += -szAvailable.cx + rcCorner.right + rcCorner.left;
		point.x -= 1;
	}
	else if (m_popupPosType & eMenuAlignment_Left)
	{
		point.x += 1;
	}
	if (m_popupPosType & eMenuAlignment_Bottom)
	{
		point.y += -szAvailable.cy + rcCorner.bottom + rcCorner.top;
		point.y += 1;
	}
	else if (m_popupPosType & eMenuAlignment_Top)
	{
		point.y += 1;
	}
	if (m_popupPosType&eMenuAlignment_Intelligent)
	{
		if (point.x < rcWork.left)
		{
			point.x = rcWork.left;
		}
		else if (point.x + szInit.cx> rcWork.right)
		{
			point.x = rcWork.right - szInit.cx;
		}
		if (point.y < rcWork.top)
		{
			point.y = rcWork.top ;
		}
		else if (point.y + szInit.cy > rcWork.bottom)
		{
			point.y = rcWork.bottom - szInit.cy;
		}
	}
	if (!no_focus_)
	{
		SetForegroundWindow(m_hWnd);
		SetFocus(m_pLayout);
	}
	SetWindowPos(m_hWnd, HWND_TOPMOST, point.x - rcCorner.left, point.y-rcCorner.top,
		szAvailable.cx, szAvailable.cy,
		SWP_SHOWWINDOW | (no_focus_ ? SWP_NOACTIVATE : 0));
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
	::GetMonitorInfo(::MonitorFromPoint(m_BasedPoint, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
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
	UiRect rcCorner = GetShadowCorner();
	RECT rcWindow;
	GetWindowRect(m_pOwner->GetWindow()->GetHWND(), &rcWindow);
	//去阴影
	{
		rcWindow.left += rcCorner.left;
		rcWindow.right -= rcCorner.right;
		rcWindow.top += rcCorner.top;
		rcWindow.bottom -=  rcCorner.bottom;
	}

	::MapWindowRect(m_pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
	
	rc.left = rcWindow.right;
	rc.right = rc.left + cxFixed;
	rc.bottom = rc.top + cyFixed;

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
			GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);  //需要减掉阴影

			bReachRight = (rcPreWindow.left + rcCorner.left) >= rcWindow.right;
			bReachBottom = (rcPreWindow.top + rcCorner.top) >= rcWindow.bottom;
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

	SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left-rcCorner.left, rc.top-rcCorner.top,
		rc.right - rc.left, rc.bottom - rc.top,
		SWP_SHOWWINDOW);

	SetForegroundWindow(m_hWnd);
	SetFocus(m_pLayout);
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
		//兼容老版本
		return;
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
		m_pLayout = dynamic_cast<ListBox*>(FindControl(L"submenu"));
		ASSERT(m_pLayout);
		m_pLayout->SetAutoDestroy(false);

		for (int i = 0; i < m_pOwner->GetSubMenuItemCount(); i++) {
			CMenuElementUI* subMenuItem = dynamic_cast<CMenuElementUI*>(m_pOwner->GetSubMenuItemAt(i));
			subMenuItem->SetInternVisible(true);		//add by djj 20200506
			if (subMenuItem && subMenuItem->IsVisible())
			{
				m_pLayout->Add(subMenuItem); //内部会调用subMenuItem->SetOwner(m_pLayout); 会调用SetWindows，改变了归属窗口、父控件。
			}
		}
	}
	else
	{
		m_pLayout = dynamic_cast<ListBox*>(m_pRoot);
		if (m_pLayout == NULL)
		{
			//允许外面套层阴影
			if (m_pRoot->GetCount()>0)
			{
				m_pLayout = dynamic_cast<ListBox*>(m_pRoot->GetItemAt(0));
			}
		}
		ASSERT(m_pLayout);
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
{

	if (m_child_menus.size() > 0)
	{
		for (size_t i = 0; i < m_child_menus.size(); i++)
		{
			delete m_child_menus[i];
		}
		m_child_menus.clear();
	}
}

bool CMenuElementUI::Add(Control* pControl)
{
	CMenuElementUI *pMenuItem = dynamic_cast<CMenuElementUI*>(pControl);
	if (!pMenuItem)
		return __super::Add(pControl);
	if (std::find(m_child_menus.cbegin(), m_child_menus.cend(), pMenuItem) != m_child_menus.cend())
	{
		assert(0);
		return false;
	}
	m_child_menus.push_back(pMenuItem);
	return true;
}

bool CMenuElementUI::AddSubMenuItem(CMenuElementUI* pMenuItem)
{
	if (pMenuItem == NULL) return false;
	if (std::find(m_child_menus.cbegin(), m_child_menus.cend(), pMenuItem) != m_child_menus.cend())
	{
		assert(0);
		return false;
	}
	m_child_menus.push_back(pMenuItem);
	return true;
}
bool CMenuElementUI::AddSubMenuItemAt(CMenuElementUI* pMenuItem, std::size_t iIndex)
{
	if (pMenuItem == NULL) return false;
	if (iIndex < 0 || iIndex > m_child_menus.size()) {
		ASSERT(FALSE);
		return false;
	}
	if (std::find(m_child_menus.cbegin(), m_child_menus.cend(), pMenuItem) != m_child_menus.cend())
	{
		assert(0);
		return false;
	}
	m_child_menus.insert(m_child_menus.begin() + iIndex, pMenuItem);
	return true;
}
bool CMenuElementUI::RemoveSubMenuItem(CMenuElementUI* pMenuItem)
{
	if (pMenuItem == NULL) return false;

	for (auto it = m_child_menus.begin(); it != m_child_menus.end(); it++) {
		if (*it == pMenuItem) {
			delete pMenuItem;			//
			m_child_menus.erase(it);
			return true;
		}
	}
	return false;
}
bool CMenuElementUI::RemoveSubMenuItemAt(std::size_t iIndex)
{
	if (iIndex < 0 || iIndex >= m_child_menus.size()) return false;
	return RemoveSubMenuItem(m_child_menus[iIndex]);
}
bool CMenuElementUI::RemoveAllSubMenuItem()
{
	for (auto it = m_child_menus.begin(); it != m_child_menus.end(); it++)
	{
		delete (*it);
	}
	m_child_menus.clear();
	return true;
}
CMenuElementUI* CMenuElementUI::GetSubMenuItemAt(std::size_t iIndex) const
{
	if (iIndex < 0 || iIndex >= m_child_menus.size()) return nullptr;
	return m_child_menus[iIndex];
}

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
			//m_pOwner->SelectItem(GetIndex(), true);  有些老版本attachselect会触发
			//这里得把之前选中的置为未选中
			m_pOwner->SelectItem(-1, false);
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
	bool hasSubMenu = m_child_menus.size() > 0;
	if (hasSubMenu)
	{
		m_pOwner->SelectItem(GetIndex(), true);
		CreateMenuWnd();
	}
	return hasSubMenu;
}

void CMenuElementUI::CreateMenuWnd()
{
#if 0
	if (m_pSubWindow)
		return;
#else		//add by djj 20200506 快速切换子菜单情况下, 可以使子菜单弹出及时
	if (m_pSubWindow)
	{
		if (m_pSubWindow->IsClosing())
		{
			m_pSubWindow->DetouchOwner();
		}
		else
		{
			return;
		}
	}
#endif
	m_pSubWindow = new CMenuWnd(GetWindow()->GetHWND());

	ContextMenuParam param;
	param.hWnd =GetWindow()->GetHWND();
	param.wParam = eMenuCloseThis;
	CMenuWnd::GetMenuObserver().RBroadcast(param);

	m_pSubWindow->Init(_T("submenu.xml"), _T(""), CPoint(), CMenuWnd::RIGHT_BOTTOM, false, this);
}

} // namespace ui
