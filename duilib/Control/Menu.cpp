#include "stdafx.h"

namespace ui {

ui::MenuManager* MenuManager::GetInstance()
{
	static MenuManager manager;
	return &manager;
}

ui::SubjectImpl& MenuManager::GetSubject()
{
	return m_subject;
}

void MenuManager::RegisterMenu(const std::wstring strMenuName, MenuWndEx *menu)
{
	if (strMenuName.empty())
		return;

	auto it = m_menuMap.find(strMenuName);
	ASSERT(it == m_menuMap.end());

	m_menuMap[strMenuName] = menu;
}

void MenuManager::UnRegisterMenu(const std::wstring strMenuName)
{
	if (strMenuName.empty())
		return;

	m_menuMap.erase(strMenuName);
}

ui::MenuWndEx* MenuManager::GetMenu(const std::wstring &strMenuName)
{
	auto it = m_menuMap.find(strMenuName);
	if (it != m_menuMap.end()) {
		if (it->second && ::IsWindow(it->second->GetHWND()))
			return it->second;
	}
	return nullptr;
}

bool MenuManager::ShowMenu(const std::wstring &strMenuName)
{
	auto menu = GetMenu(strMenuName);
	if (menu) {
		::ShowWindow(menu->GetHWND(), SW_SHOWNOACTIVATE);
		return true;
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////
//

MenuBox::MenuBox()
{
	m_cxyFixed.cx = m_cxyFixed.cy = DUI_LENGTH_AUTO;
    this->SetMaxHeight(MulDiv(GetSystemMetrics(SM_CYFULLSCREEN), 100, ui::DpiManager::GetInstance()->GetScale()));
    this->SetVerScrollUnitPixels(100);
    EnableScrollBar(true, GetHorizontalScrollBar() != NULL);
}

MenuBox::~MenuBox()
{
	m_bDelayedDestroy = false;
	RemoveAll();
}

bool MenuBox::Add(Control* pControl)
{
	MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
	if (pMenuItem != NULL) {
		for (int i = 0; i < pMenuItem->GetCount(); ++i) {
			if (dynamic_cast<MenuElement*>(pMenuItem->GetItemAt(i))) {
				pMenuItem->GetItemAt(i)->SetInternVisible(false);
			}
		}
	}
	return __super::Add(pControl);
}

bool MenuBox::AddAt(Control* pControl, std::size_t iIndex)
{
	MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
	if (pMenuItem != NULL) {
		for (int i = 0; i < pMenuItem->GetCount(); ++i) {
			if (dynamic_cast<MenuElement*>(pMenuItem->GetItemAt(i))) {
				pMenuItem->GetItemAt(i)->SetInternVisible(false);
			}
		}
	}
	return __super::AddAt(pControl, iIndex);
}

void MenuBox::RemoveAll()
{
	if (m_bAutoDestroy) {
		for (auto it = m_items.begin(); it != m_items.end(); it++) {
			if (m_bDelayedDestroy && m_pWindow) m_pWindow->AddDelayedCleanup((*it));
			else delete (*it);
		}
	}
	else {
		// 子菜单中的临时菜单项，一定会被销毁
		for (auto it = m_items.begin(); it != m_items.end(); it++) {
			MenuElement* pMenuItem = dynamic_cast<MenuElement*>((*it));
			if (pMenuItem != NULL && pMenuItem->IsTempItem()) {
				delete (*it);
			}
		}
	}

	m_items.clear();
	Arrange();
}

void MenuBox::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("class")) {
		m_class = strValue;
	}

	__super::SetAttribute(strName, strValue);
}

/////////////////////////////////////////////////////////////////////////////////////
//

int StringToMenuAlign(const std::wstring &value)
{
	int flags = 0;
	auto prop = StringHelper::Split(value, L" ");
	for (auto & it : prop) {
		if (it == L"left") {
			flags |= kLeft;
		}
		if (it == L"right") {
			flags |= kRight;
		}
		if (it == L"center") {
			flags |= kCenter;
		}
		if (it == L"top") {
			flags |= kTop;
		}
		if (it == L"bottom") {
			flags |= kBottom;
		}
        if (it == L"vcenter") {
            flags |= kVCenter;
        }
	}

	return flags;
}

CPoint StringToMenuPopup(UiRect rect, const std::wstring &popupInfo)
{
	CPoint point;

	auto prop = StringHelper::Split(popupInfo, L" ");
	for (auto & it : prop) {
		if (it == L"left") {
			point.x = rect.left;
		}
		if (it == L"center") {
			point.x = (rect.left + rect.right) / 2;
		}
		if (it == L"right") {
			point.x = rect.right;
		}
		if (it == L"top") {
			point.y = rect.top;
		}
		if (it == L"vcenter") {
			point.y = (rect.top + rect.bottom) / 2;
		}
		if (it == L"bottom") {
			point.y = rect.bottom;
		}
	}
	return point;
}

void GetMenuPopupInfo(Control *control, CPoint &point, int &align)
{
	if (!control)
		return;

	point = StringToMenuPopup(control->GetPosWithScrollOffset(), control->GetMenuPopup());
	ClientToScreen(control->GetWindow()->GetHWND(), &point);
	align = StringToMenuAlign(control->GetMenuAlign());
}

MenuWndEx::MenuWndEx(const std::wstring &strMenuName /*= L""*/)
	: m_strMenuName(strMenuName)
{

}

MenuWndEx::~MenuWndEx()
{

}

MenuWndEx* MenuWndEx::CreateMenu(STRINGorID xml, CPoint point, const std::wstring &strMenuName/* = L""*/, int align /*= kRight | kBottom*/, HWND hParent, int flags /*= 0*/)
{
	MenuWndEx* pMenu = new MenuWndEx(strMenuName);
	pMenu->InitMenu(nullptr, hParent, xml, point, align, flags);
	return pMenu;
}

void MenuWndEx::InitMenu(MenuElement* pOwner,
	HWND hParent,
	STRINGorID xml,
	CPoint point,
	int align,
	int flags)
{
	m_pOwner = pOwner;
	m_BasedPoint = point;
	m_nAlignment = align;
	m_nFlags = flags;

	if (!m_pOwner) {
		wchar_t dir[_MAX_DIR];
		wchar_t name[_MAX_FNAME];
		wchar_t ext[_MAX_EXT];

		_wsplitpath(xml.m_lpstr, NULL, dir, name, ext);
		m_skinFloder = dir;
		m_skinFile = name;
		m_skinFile += ext;
	}
	else {
		m_skinFloder = (dynamic_cast<MenuWndEx*>(m_pOwner->GetWindow()))->m_skinFloder;
	}

	MenuManager::GetInstance()->GetSubject().AddObserver(this);

	std::wstring title = MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_MENU_TITLE");
	Create((m_pOwner == NULL) ? hParent : m_pOwner->GetWindow()->GetHWND(), title.c_str(), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, true);

	// HACK: Don't deselect the parent's caption
	HWND hWndParent = m_hWnd;
	while (::GetParent(hWndParent) != NULL) hWndParent = ::GetParent(hWndParent);

	::ShowWindow(m_hWnd, CheckFlag(kNoFocus) ? SW_SHOWNOACTIVATE : SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

MenuBox* MenuWndEx::GetMenu()
{
	return m_pMenuRoot;
}

void MenuWndEx::ResizeMenu()
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromPoint(m_BasedPoint, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	UiRect rcWork = oMonitor.rcWork;
	UiRect monitor_rect = oMonitor.rcMonitor;
	ui::CSize szInit = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szInit = GetRoot()->EstimateSize(szInit);
	//必须是Menu标签作为xml的根节点
	MenuBox *pMenuRoot = static_cast<MenuBox*>(GetRoot());
	ASSERT(pMenuRoot);

	UiRect rc;
	CPoint point = m_BasedPoint;
	rc.left = point.x;
	rc.top = point.y;
	rc.right = rc.left + szInit.cx;
	rc.bottom = rc.top + szInit.cy;

	if (m_nAlignment & kLeft) {
		rc.right = point.x;
		rc.left = rc.right - szInit.cx;
	}
	else if (m_nAlignment & kCenter) {
		rc.right = point.x + szInit.cx / 2;
		rc.left = rc.right - szInit.cx;
	}

	if (m_nAlignment & kTop) {
		rc.bottom = point.y;
		rc.top = rc.bottom - szInit.cy;
	}
    else if (m_nAlignment & kVCenter) {
        rc.bottom = point.y + szInit.cy / 2;
        rc.top = rc.bottom - szInit.cy;
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

	if (rc.top > monitor_rect.bottom - szInit.cy)
	{
		rc.top = monitor_rect.bottom - szInit.cy;
	}
	if (rc.top < monitor_rect.top)
	{
		rc.top = monitor_rect.top;
	}
	rc.bottom = rc.top + szInit.cy;

	SetPos(rc, false, SWP_SHOWWINDOW | (CheckFlag(kNoFocus) ? SWP_NOACTIVATE : 0), HWND_TOPMOST, true);
	if (!CheckFlag(kNoFocus))
		SetForegroundWindow(m_hWnd);
}

void MenuWndEx::ResizeSubMenu()
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(m_pOwner->GetWindow()->GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	UiRect rcWork = oMonitor.rcWork;
	UiRect monitor_rect = oMonitor.rcMonitor;
	ui::CSize szInit = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szInit = GetRoot()->EstimateSize(szInit);
	szInit.cx -= GetShadowCorner().left + GetShadowCorner().right;
	szInit.cy -= GetShadowCorner().top + GetShadowCorner().bottom;

	UiRect rcOwnerWindow;
	GetWindowRect(m_pOwner->GetWindow()->GetHWND(), &rcOwnerWindow);

#ifdef SUB_MENU_POPUP_CALC_SHADOW
#else
	rcOwnerWindow.Deflate(m_pOwner->GetWindow()->GetShadowCorner());
#endif

	RECT rcOwner = m_pOwner->GetPos();
	// 初始化子菜单的弹出位置
	UiRect rc = rcOwner;
	rc.top = rcOwner.top;
	rc.bottom = rc.top + szInit.cy;
	::MapWindowRect(m_pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
	rc.left = rcOwnerWindow.right;
	rc.right = rc.left + szInit.cx;

	bool bReachBottom = false;
	bool bReachRight = false;

	RECT rcPreWindow = { 0 };
	SubjectImpl::Iterator iterator(MenuManager::GetInstance()->GetSubject());
	ObserverImplBase* pReceiver = iterator.next();
	while (pReceiver != NULL) {
		MenuWndEx* pContextMenu = dynamic_cast<MenuWndEx*>(pReceiver);
		if (pContextMenu != NULL) {
			GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

			bReachRight = rcPreWindow.left >= rcOwnerWindow.right;
			bReachBottom = rcPreWindow.top >= rcOwnerWindow.bottom;
			if (pContextMenu->GetHWND() == m_pOwner->GetWindow()->GetHWND()
				|| bReachBottom || bReachRight)
				break;
		}
		pReceiver = iterator.next();
	}

	if (bReachBottom) {
		rc.bottom = rcOwner.top;
		rc.top = rc.bottom - szInit.cy;
	}

	if (bReachRight) {
		rc.right = rcOwnerWindow.left;
		rc.left = rc.right - szInit.cx;
	}

	if (rc.left < monitor_rect.left)
	{
		rc.left = rcOwnerWindow.right;
		rc.right = rc.left + szInit.cx;
	}
	if (rc.right > monitor_rect.right)
	{
		rc.right = rcOwnerWindow.left;
		rc.left = rc.right - szInit.cx;
	}
	if (rc.top < monitor_rect.top)
	{
		rc.top = rcOwner.top;
		rc.bottom = rc.top + szInit.cy;
	}
	if (rc.bottom > monitor_rect.bottom)
	{
		rc.bottom = rc.top;
		rc.top = rc.bottom - szInit.cy;
	}

	SetPos(rc, false, SWP_SHOWWINDOW, HWND_TOPMOST, false);
}

bool MenuWndEx::CheckFlag(MenuFlags flag)
{
	return m_nFlags & flag;
}

void MenuWndEx::SetEnableItems(const EnableMap &data)
{
	for (const auto &it : data)	{
		auto *pControl = FindControl(it.first);
		if (pControl)
			pControl->SetEnabled(it.second);
	}
}

void MenuWndEx::SetSelectItems(const SelectMap &data)
{
	for (const auto &it : data)	{
		auto *pControl = dynamic_cast<CheckBox*>(FindControl(it.first));
		if (pControl) {
			pControl->Selected(it.second, false);
		}
		else {
			auto *pBox = dynamic_cast<CheckBoxBox*>(FindControl(it.first));
			if (pBox) {
				pBox->Selected(it.second, false);
			}
		}
	}
}

void MenuWndEx::SetVisibleItems(const VisibleMap &data)
{
	for (const auto &it : data)	{
		auto *pControl = FindControl(it.first);
		if (pControl)
			pControl->SetVisible(it.second);
	}
}

void MenuWndEx::SetTextIdItems(const TextIdMap &data)
{
	for (const auto &it : data)	{
		auto *pControl = dynamic_cast<MenuElement*>(FindControl(it.first));
		if (pControl) {
			pControl->SetTextId(it.second);
		}
	}
}

void MenuWndEx::AttachSelect(const std::wstring& strName, const ui::EventCallback& callback)
{
	auto *pControl = dynamic_cast<MenuElement*>(FindControl(strName));
	if (pControl) {
		pControl->AttachSelect(callback);
	}
}

void MenuWndEx::AttachPopupSubMenu(const std::wstring& strName, const EventCallback& callback)
{
	auto *pControl = dynamic_cast<MenuElement*>(FindControl(strName));
	if (pControl) {
		pControl->AttachPopupSubMenu(callback);
	}
}

void MenuWndEx::Close(UINT nRet /*= IDOK*/)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd)) return;

	PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
}

std::wstring MenuWndEx::GetSkinFolder()
{
	return m_skinFloder;
}

std::wstring MenuWndEx::GetSkinFile()
{
	return m_skinFile;
}

std::wstring MenuWndEx::GetWindowClassName() const
{
	return _T("MenuWnd");
}

void MenuWndEx::OnFinalMessage(HWND hWnd)
{
	MenuManager::GetInstance()->UnRegisterMenu(m_strMenuName);

	RemoveSubject();
	if (m_pOwner != NULL) {
		for (int i = 0; i < m_pOwner->GetCount(); i++) {
			auto pControl = m_pOwner->GetItemAt(i);
			MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
			if (pMenuItem) {
				pMenuItem->SetWindow(m_pOwner->GetWindow(), m_pOwner, false);
				pMenuItem->SetOwner(dynamic_cast<MenuBox*>(m_pOwner->GetParent()));
				pMenuItem->SetVisible(false);
				pMenuItem->SetInternVisible(false);
			}
		}
		m_pOwner->m_pSubMenuWindow = NULL;
		m_pOwner->MouseLeave(ui::EventArgs());
	}
	__super::OnFinalMessage(hWnd);
}

Control* MenuWndEx::CreateControl(const std::wstring& pstrClass)
{
	if (pstrClass == L"Menu")
	{
		return new MenuBox();
	}
	else if (pstrClass == L"MenuElement")
	{
		return new MenuElement();
	}
	return NULL;
}

LRESULT MenuWndEx::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pOwner != NULL) {
		::SetWindowLong(this->GetHWND(), GWL_STYLE, GetStyle());

		Init(m_hWnd);
		SetWindowResourcePath(GetSkinFolder());
		MenuBox *pRoot = new MenuBox();
		pRoot->SetAutoDestroyChild(false);

		for (int i = 0; i < m_pOwner->GetCount(); i++) {
			auto pControl = m_pOwner->GetItemAt(i);
			MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
			if (pMenuItem) {
				pMenuItem->SetOwner(pRoot);
				pRoot->Add(pMenuItem);
			}
		}

		SetRenderTransparent(m_pOwner->GetWindow()->IsRenderTransparent());
		m_pMenuRoot = pRoot;
		ASSERT(m_pMenuRoot);

		if (!m_pOwner->GetSubMenuShadowImage().empty()) {
			m_shadow.SetShadowImage(m_pOwner->GetSubMenuShadowImage());
			m_shadow.SetShadowCorner(m_pOwner->GetSubMenuShadowCorner(), true);
			AttachDialog(m_shadow.AttachShadow(pRoot));
		}
		else if (m_pOwner->GetWindow()->IsShadowAttached()) {
			m_shadow.SetShadowImage(m_pOwner->GetWindow()->GetShadowImage());
			m_shadow.SetShadowCorner(m_pOwner->GetWindow()->GetShadowCorner(), false);
			AttachDialog(m_shadow.AttachShadow(pRoot));
		}
		else{
			AttachDialog(pRoot);
		}
		
		auto classMap = m_pOwner->GetWindow()->GetClassMap();
		if (classMap && !classMap->empty()) {
			for (auto &it : *classMap) {
				this->AddClass(it.first, it.second);
			}
		}
		MenuBox *parentRoot = dynamic_cast<MenuBox*>(m_pOwner->GetParent());
		if (parentRoot) {
			auto strClassName = parentRoot->GetClass();
			if (!strClassName.empty()) {
				pRoot->SetAttribute(L"class", strClassName);
			}
		}
	}
	else {
		::SetWindowLong(this->GetHWND(), GWL_STYLE, GetStyle());

		Init(m_hWnd);
		SetWindowResourcePath(GetSkinFolder());

		WindowBuilder builder;
		auto callback = nbase::Bind(&WindowImplBase::CreateControl, this, std::placeholders::_1);
		auto pRoot = (Box*)builder.Create((GetWindowResourcePath() + GetSkinFile()).c_str(), callback, this);

		ASSERT(pRoot);
		if (pRoot == NULL) {
			// MessageBox(NULL, _T("加载资源文件失败"), _T("Duilib"), MB_OK | MB_ICONERROR);
			return -1;
		}

		if (CheckFlag(kUseDefaultShadow))
			m_shadow.ResetDefaultShadow();

		m_pMenuRoot = dynamic_cast<MenuBox*>(pRoot);
		ASSERT(m_pMenuRoot);
		AttachDialog(m_shadow.AttachShadow(pRoot));

		MenuManager::GetInstance()->RegisterMenu(m_strMenuName, this);
	}

	ui::Box* pRoot = (ui::Box*)GetRoot();
	if (pRoot->GetFixedWidth() == DUI_LENGTH_AUTO || pRoot->GetFixedHeight() == DUI_LENGTH_AUTO) {
		CSize maxSize(99999, 99999);
		CSize needSize = pRoot->EstimateSize(maxSize);
		if (needSize.cx < pRoot->GetMinWidth()) needSize.cx = pRoot->GetMinWidth();
		if (pRoot->GetMaxWidth() >= 0 && needSize.cx > pRoot->GetMaxWidth()) needSize.cx = pRoot->GetMaxWidth();
		if (needSize.cy < pRoot->GetMinHeight()) needSize.cy = pRoot->GetMinHeight();
		if (needSize.cy > pRoot->GetMaxHeight()) needSize.cy = pRoot->GetMaxHeight();

		::MoveWindow(m_hWnd, 0, 0, needSize.cx, needSize.cy, FALSE);
	}

	if (m_pOwner != NULL) {
		ResizeSubMenu();
	} 
	else {
		ResizeMenu();
	}

	return 0;
}

LRESULT MenuWndEx::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hFocusWnd = (HWND)wParam;

	BOOL bInMenuWindowList = FALSE;
	ContextMenuParam param;
	param.hWnd = GetHWND();

	SubjectImpl::Iterator iterator(MenuManager::GetInstance()->GetSubject());
	ObserverImplBase* pReceiver = iterator.next();
	while (pReceiver != NULL) {
		MenuWndEx* pContextMenu = dynamic_cast<MenuWndEx*>(pReceiver);
		if (pContextMenu != NULL && pContextMenu->GetHWND() == hFocusWnd) {
			bInMenuWindowList = TRUE;
			break;
		}
		pReceiver = iterator.next();
	}

	if (!bInMenuWindowList) {
		param.type = ContextMenuParam::kRemoveAllMenu;
		MenuManager::GetInstance()->GetSubject().Notify(param);

		return 0;
	}
	return 0;
}

LRESULT MenuWndEx::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pOwner != NULL) {
		m_pOwner->SetWindow(m_pOwner->GetWindow(), m_pOwner->GetParent(), false);
		m_pOwner->SetPos(m_pOwner->GetPos());
		m_pOwner->SetFocus();
	}
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

LRESULT MenuWndEx::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		return OnCreate(uMsg, wParam, lParam);
	}
	else if (uMsg == WM_KEYDOWN && (wParam == VK_ESCAPE || wParam == VK_LEFT)) {
		Close();
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

BOOL MenuWndEx::OnNotify(ContextMenuParam param)
{
	switch (param.type)
	{
	case ContextMenuParam::kRemoveAllMenu:
		Close();
		break;
	case ContextMenuParam::kRemoveSubMenu:
		{
			HWND hParent = GetParent(m_hWnd);
			while (hParent != NULL)	{
				if (hParent == param.hWnd) {
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

void MenuWndEx::SetWidth(const int width)
{
	auto rootMenu = this->GetMenu();
	if (!rootMenu) return;
	rootMenu->SetFixedWidth(width);
	int shadow_width = this->GetShadowCorner().left + this->GetShadowCorner().right;
	this->GetRoot()->SetFixedWidth(width + shadow_width);
}

/////////////////////////////////////////////////////////////////////////////////////
//

MenuElement::MenuElement()
{
	m_cxyFixed.cy = 30;
	// 菜单项处理WM_POINTER消息，否则WM_POINTER消息就会转换为WM_TOUCH
	// 此时菜单如果消失的话，导致下层窗口收到WM_TOUCH消息而导致Window::m_pEventPointer为非空，导致鼠标消息无法被处理
	SetReceivePointerMsg(true);
}

MenuElement::~MenuElement()
{

}

bool MenuElement::Add(Control* pControl)
{
	if (pControl == NULL) return false;

	MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
	if (pMenuItem != NULL) {
		pMenuItem->SetInternVisible(false);
	}
	return __super::Add(pControl);
}

bool MenuElement::AddAt(Control* pControl, std::size_t iIndex)
{
	if (pControl == NULL) return false;

	MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
	if (pMenuItem != NULL) {
		pMenuItem->SetInternVisible(false);
	}
	return __super::AddAt(pControl, iIndex);
}

bool MenuElement::AddTempItem(MenuElement* pMenuItem)
{
	if (pMenuItem == NULL) return false;

	if (m_pSubMenuWindow) {
		auto menu = m_pSubMenuWindow->GetMenu();
		if (menu) {
			pMenuItem->SetTempItem();
			return menu->Add(pMenuItem);
		}
	}

	return false;
}

void MenuElement::SetAutoCloseWhenClick(bool value)
{
	m_bAutoCloseWhenClick = value;
}

bool MenuElement::IsAutoCloseWhenClick()
{
	return m_bAutoCloseWhenClick;
}

void MenuElement::SetHasSubMenu(bool has)
{
	m_bHasSubMenu = has;
}

bool MenuElement::HasSubMenu()
{
	return m_bHasSubMenu;
}

void MenuElement::SetSubMenuShadowImage(const std::wstring &image)
{
	m_subMenuShadowImage = image;
}

std::wstring MenuElement::GetSubMenuShadowImage() const
{
	return m_subMenuShadowImage;
}

void MenuElement::SetSubMenuShadowCorner(const UiRect &rect)
{
	m_rcSubMenuShadowCorner = rect;
}

ui::UiRect MenuElement::GetSubMenuShadowCorner() const
{
	return m_rcSubMenuShadowCorner;
}

void MenuElement::SetTempItem()
{
	m_bTemp = true;
}

bool MenuElement::IsTempItem()
{
	return m_bTemp;
}

ui::Control* MenuElement::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos /*= CPoint()*/)
{
	// Check if this guy is valid
	if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) return NULL;
	if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) return NULL;
	if ((uFlags & UIFIND_HITTEST) != 0) {
		if (!::PtInRect(&m_rcItem, *(static_cast<LPPOINT>(pData)))) return NULL;
		if (!m_bMouseChildEnabled) {
			Control* pResult = NULL;
			if (pResult == NULL) pResult = Control::FindControl(Proc, pData, uFlags);
			return pResult;
		}
	}

	Control* pResult = NULL;
	if (pResult != NULL) return pResult;

	if ((uFlags & UIFIND_ME_FIRST) != 0) {
		Control* pControl = Control::FindControl(Proc, pData, uFlags);
		if (pControl != NULL) return pControl;
	}
	UiRect rc = m_rcItem;
	rc.left += m_pLayout->GetPadding().left;
	rc.top += m_pLayout->GetPadding().top;
	rc.right -= m_pLayout->GetPadding().right;
	rc.bottom -= m_pLayout->GetPadding().bottom;

	if ((uFlags & UIFIND_TOP_FIRST) != 0) {
		for (int it = (int)m_items.size() - 1; it >= 0; it--) {
			Control* pControl;
			if (m_items[it]->GetWindow() != this->GetWindow())
				continue;
			
			if ((uFlags & UIFIND_HITTEST) != 0) {
				CPoint newPoint = *(static_cast<LPPOINT>(pData));
				newPoint.Offset(scrollPos);
				pControl = m_items[it]->FindControl(Proc, &newPoint, uFlags);
			}
			else {
				pControl = m_items[it]->FindControl(Proc, pData, uFlags);
			}
			if (pControl != NULL) {
				if ((uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
					continue;
				else
					return pControl;
			}
		}
	}
	else {
		for (auto it = m_items.begin(); it != m_items.end(); it++) {
			if ((*it)->GetWindow() != this->GetWindow())
				continue;
			Control* pControl;
			if ((uFlags & UIFIND_HITTEST) != 0) {
				CPoint newPoint = *(static_cast<LPPOINT>(pData));
				newPoint.Offset(scrollPos);
				pControl = (*it)->FindControl(Proc, &newPoint, uFlags);
			}
			else {
				pControl = (*it)->FindControl(Proc, pData, uFlags);
			}
			if (pControl != NULL) {
				if ((uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
					continue;
				else
					return pControl;
			}
		}
	}

	if (pResult == NULL && (uFlags & UIFIND_ME_FIRST) == 0) pResult = Control::FindControl(Proc, pData, uFlags);
	return pResult;
}

void MenuElement::PaintChild(IRenderContext* pRender, const UiRect& rcPaint)
{
	UiRect rcTemp;
	if (!::IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) return;

	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		Control* pControl = *it;
		if (!pControl->IsVisible()) continue;

		MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
		if (!pMenuItem) {
			pControl->AlphaPaint(pRender, rcPaint);
		}
	}
}

bool MenuElement::MouseEnter(EventArgs& msg)
{
	__super::MouseEnter(msg);

	ASSERT(m_pOwner);
	if (!m_pOwner) return true;

	if (IsEnabled()) {
		if (m_pSubMenuWindow) return false;

		if (PrepareCreateMenu()) {
			CreateMenuWnd();
		}
		else {
			ContextMenuParam param;
			param.hWnd = GetWindow()->GetHWND();
			param.type = ContextMenuParam::kRemoveSubMenu;
			MenuManager::GetInstance()->GetSubject().Notify(param);
		}
	}

	return false;
}

bool MenuElement::MouseLeave(EventArgs& msg)
{
	if (m_pSubMenuWindow) return false;
	return __super::MouseLeave(msg);
}

bool MenuElement::ButtonUp(EventArgs& msg)
{
	if (IsEnabled()) {
		std::weak_ptr<nbase::WeakFlag> weakFlag = m_pWindow->GetWeakFlag();
		bool ret = __super::ButtonUp(msg);
		if (!weakFlag.expired()) {
			if (m_pSubMenuWindow) {
				return false;
			}

			if (!m_bAutoCloseWhenClick)
				return ret;

			ContextMenuParam param;
			param.hWnd = GetWindow()->GetHWND();
			param.type = ContextMenuParam::kRemoveAllMenu;
			MenuManager::GetInstance()->GetSubject().Notify(param);

			return ret;
		}
	}
	return false;
}

void MenuElement::HandleMessage(EventArgs& event)
{
	if (!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) {
		if (m_pParent != NULL) m_pParent->HandleMessageTemplate(event);
		else Box::HandleMessage(event);
		return;
	}

	if (IsEnabled() && IsKeyboardEnabled()) {
		if (event.Type == kEventKeyDown && event.chKey == VK_RIGHT) {
			ASSERT(m_pOwner);
			if (!m_pOwner) return;

			if (m_pSubMenuWindow) return;

			if (PrepareCreateMenu()) {
				CreateMenuWnd();
			}
			else {
				ContextMenuParam param;
				param.hWnd = GetWindow()->GetHWND();
				param.type = ContextMenuParam::kRemoveSubMenu;
				MenuManager::GetInstance()->GetSubject().Notify(param);
			}
			return;
		}
	}

	__super::HandleMessage(event);
}

void MenuElement::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("autoclose"))	{
		SetAutoCloseWhenClick(strValue == _T("true"));
		return;
	}
	else if (strName == _T("submenu")) {
		SetHasSubMenu(strValue == _T("true"));
		return;
	}
	else if (strName == _T("shadowimage")) {
		SetSubMenuShadowImage(strValue);
		return;
	}
	else if (strName == _T("shadowcorner")) {
		UiRect rc;
		LPTSTR pstr = NULL;
		rc.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
		rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		SetSubMenuShadowCorner(rc);
		return;
	}

	return __super::SetAttribute(strName, strValue);
}

void MenuElement::PaintStatusColor(IRenderContext* pRender)
{
	// 不绘制Select状态
	Box::PaintStatusColor(pRender);
}

void MenuElement::PaintStatusImage(IRenderContext* pRender)
{
	// 不绘制Select状态
	Box::PaintStatusImage(pRender);
}

void MenuElement::PaintText(IRenderContext* pRender)
{
	// 不绘制Select状态
	LabelBox::PaintText(pRender);
}

bool MenuElement::PrepareCreateMenu()
{
	bool hasSubMenu = false;
	for (int i = 0; i < GetCount(); ++i) {
		auto pControl = GetItemAt(i);
		MenuElement* pMenuItem = dynamic_cast<MenuElement*>(pControl);
		if (pMenuItem) {
			pMenuItem->SetVisible(true);
			pMenuItem->SetInternVisible(true);

			hasSubMenu = true;
		}
	}
	return m_bHasSubMenu || hasSubMenu;
}

void MenuElement::CreateMenuWnd()
{
	if(m_pSubMenuWindow) return;

	m_pSubMenuWindow = new MenuWndEx();
	ASSERT(m_pSubMenuWindow);

	ContextMenuParam param;
	param.hWnd = GetWindow()->GetHWND();
	param.type = ContextMenuParam::kRemoveSubMenu;
	MenuManager::GetInstance()->GetSubject().Notify(param);

	m_pSubMenuWindow->InitMenu(static_cast<MenuElement*>(this), NULL, _T(""), CPoint(), kRight | kBottom);

	m_pWindow->SendNotify(this, ui::kEventNotify, 0, 0);
}

} // namespace ui