#include "stdafx.h"
#include "FilterCombo.h"

namespace nim_comp
{

class CFilterComboWnd : public Window
{
public:
    void Init(FilterCombo* pOwner);
    virtual std::wstring GetWindowClassName() const override;
	virtual void OnFinalMessage(HWND hWnd) override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void OnSeleteItem();

private:
    FilterCombo *m_pOwner;
    int m_iOldSel;
	bool m_bClosing = false;
};

void CFilterComboWnd::Init(FilterCombo* pOwner)
{
    m_pOwner = pOwner;
    m_iOldSel = m_pOwner->GetCurSel();

    // Position the popup window in absolute space
    CSize szDrop = m_pOwner->GetDropBoxSize();
    UiRect rcOwner = pOwner->GetPosWithScrollOffset();
    UiRect rc = rcOwner;
    rc.top = rc.bottom + 1;		// 父窗口left、bottom位置作为弹出窗口起点
    rc.bottom = rc.top + szDrop.cy;	// 计算弹出窗口高度
    if( szDrop.cx > 0 ) rc.right = rc.left + szDrop.cx;	// 计算弹出窗口宽度

    CSize szAvailable(rc.right - rc.left, rc.bottom - rc.top);
    int cyFixed = 0;
	for (int it = 0; it < pOwner->GetListBox()->GetCount(); it++) {
		Control* pControl = pOwner->GetListBox()->GetItemAt(it);
        if( !pControl->IsVisible() ) continue;
        CSize sz = pControl->EstimateSize(szAvailable);
        cyFixed += sz.cy;
    }
    cyFixed += 2; // VBox 默认的Padding 调整
    rc.bottom = rc.top + min(cyFixed, szDrop.cy);

    ::MapWindowRect(pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);

    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    UiRect rcWork = oMonitor.rcWork;
    if( rc.bottom > rcWork.bottom || m_pOwner->IsPopupTop()) {
        rc.left = rcOwner.left;
        rc.right = rcOwner.right;
        if( szDrop.cx > 0 ) rc.right = rc.left + szDrop.cx;
        rc.top = rcOwner.top - min(cyFixed, szDrop.cy);
        rc.bottom = rcOwner.top;
        ::MapWindowRect(pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
    }
    
    Create(pOwner->GetWindow()->GetHWND(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, true, rc);
    // HACK: Don't deselect the parent's caption
    HWND hWndParent = m_hWnd;
    while( ::GetParent(hWndParent) != NULL ) 
		hWndParent = ::GetParent(hWndParent);
    ::ShowWindow(m_hWnd, SW_SHOW);
    //::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

std::wstring CFilterComboWnd::GetWindowClassName() const
{
    return _T("ComboWnd");
}

void CFilterComboWnd::OnFinalMessage(HWND hWnd)
{
	m_pOwner->m_pComboWnd = NULL;
    m_pOwner->m_uButtonState = kControlStateNormal;
    m_pOwner->Invalidate();
    delete this;
}

void CFilterComboWnd::OnSeleteItem()
{
	PostMessage(WM_KILLFOCUS);
}

LRESULT CFilterComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if( uMsg == WM_CREATE ) {
        this->Window::Init(m_hWnd);
		Box* pRoot = new Box;
		pRoot->SetAutoDestroyChild(false);
		pRoot->Add(m_pOwner->GetListBox());
		m_pOwner->GetListBox()->SetFilterComboWnd(this);
		this->AttachDialog(pRoot);
		this->SetWindowResourcePath(m_pOwner->GetWindow()->GetWindowResourcePath());
		this->SetShadowAttached(false);

        return 0;
    }
    else if( uMsg == WM_CLOSE ) {
        m_pOwner->SetWindow(m_pOwner->GetWindow(), m_pOwner->GetParent(), false);
        m_pOwner->SetPos(m_pOwner->GetPos());
        m_pOwner->SetFocus();
    }
    else if( uMsg == WM_KILLFOCUS ) {
		if (m_hWnd != (HWND)wParam)	{ 
			m_bClosing = true;
			PostMessage(WM_CLOSE);
			m_pOwner->SelectItem(m_pOwner->GetListBox()->GetCurSel());
			((Box*)this->GetRoot())->RemoveAt(0);
			m_pOwner->GetListBox()->PlaceHolder::SetWindow(nullptr, nullptr, false);
			m_pOwner->GetListBox()->SetFilterComboWnd(nullptr);
		}
    }
#if 1
	else if (uMsg == WM_CHAR || uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
	{
		if (m_pOwner)
		{
			EventArgs args;
			args.pSender = m_pOwner->GetListBox();
			args.chKey = wParam;
			if (uMsg == WM_CHAR)
				args.Type = kEventChar;
			else if (uMsg == WM_KEYDOWN)
				args.Type = kEventKeyDown;
			else if (uMsg == WM_KEYUP)
				args.Type = kEventKeyUp;
			args.wParam = wParam;
			args.lParam = lParam;
			args.dwTimestamp = ::GetTickCount();
			m_pOwner->HandleMessage(args);
		}
	}
	else if (uMsg == WM_SYSKEYDOWN)
	{
		if (m_pOwner)
		{
			EventArgs args;
			args.pSender = m_pOwner->GetListBox();
			args.chKey = wParam;
			args.Type = kEventSystemKey;
			args.wParam = 0;
			args.lParam = 0;
			args.dwTimestamp = ::GetTickCount();
			m_pOwner->HandleMessage(args);
		}
	}
#endif
	if (m_bClosing)	{
		return CallWindowProc(uMsg, wParam, lParam);
	}
	else {
		bool handled = false;
		LRESULT ret = this->DoHandlMessage(uMsg, wParam, lParam, handled);
		if (handled) {
			return ret;
		}
		else {
			return CallWindowProc(uMsg, wParam, lParam);
		}
	}
}

/////////////////////FilterListBox//////////////////////////
/////////////////////FilterListBox//////////////////////////

bool ListElementMatch::StringMatch(const std::string& utf8str)
{
	std::string utf8text = nbase::MakeLowerString(GetUTF8Text());
	std::string utf8date = nbase::MakeLowerString(GetUTF8DataID());
	std::string searchkey = nbase::MakeLowerString(utf8str);

	if (utf8text.find(searchkey) != std::string::npos || utf8date.find(searchkey) != std::string::npos)
	{
		return true;
	}
	return false;
}


bool FilterListBox::SelectItem(int iIndex, bool bTakeFocus, bool bTrigger)
{
	if (iIndex == m_iCurSel) return true;
	int iOldSel = m_iCurSel;
	// We should first unselect the currently selected item
	if (m_iCurSel >= 0) {
		Control* pControl = GetItemAt(m_iCurSel);
		if (pControl != NULL) {
			ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(pControl);
			if (pListItem != NULL) pListItem->OptionTemplate<Box>::Selected(false, bTrigger);
		}

		m_iCurSel = -1;
	}
	if (iIndex < 0) {
		if (m_pWindow != NULL && bTrigger) {
			m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, iOldSel);
		}
		return false;
	}
	Control* pControl = GetItemAt(iIndex);
	if (pControl == NULL) return false;
	if (!pControl->IsVisible()) return false;
	if (!pControl->IsEnabled()) return false;

	ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(pControl);
	if (pListItem == NULL) return false;
	m_iCurSel = iIndex;
	pListItem->OptionTemplate<Box>::Selected(true, bTrigger);

	if (GetItemAt(m_iCurSel)) {
		UiRect rcItem = GetItemAt(m_iCurSel)->GetPos();
		EnsureVisible(rcItem);
	}

	if (bTakeFocus) pControl->SetFocus();
	if (m_pWindow != NULL && bTrigger) {
		m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, iOldSel);
	}

	return true;
}

void FilterListBox::Filter(std::string utf8_str)
{
	ListElementMatch *item = nullptr;
	for (size_t i = 0; i < GetCount(); i++)
	{
		if (item = dynamic_cast<ListElementMatch*>(GetItemAt(i)))
		{
			if (item->StringMatch(utf8_str))
				item->SetVisible();
			else
				item->SetVisible(false);
		}
	}
}

//////////////////////////FilterCombo////////////////////////////
//////////////////////////FilterCombo////////////////////////////

FilterCombo::FilterCombo() :
	m_pComboWnd(nullptr),
	m_iCurSel(-1),
	m_szDropBox(0, 150),
	m_uButtonState(kControlStateNormal),
	m_sDropBoxAttributes(),
	m_bPopupTop(false)
{
	// The trick is to add the items to the new container. Their owner gets
	// reassigned by this operation - which is why it is important to reassign
	// the items back to the righfull owner/manager when the window closes.
	m_pLayout.reset(new FilterListBox(new VLayout));
	m_pLayout->GetLayout()->SetPadding(UiRect(1, 1, 1, 1));
	m_pLayout->SetBkColor(L"bk_wnd_lightcolor");
	m_pLayout->SetBorderColor(L"splitline_level1");
	m_pLayout->SetBorderSize(UiRect(1, 1, 1, 1));
	m_pLayout->EnableScrollBar();
	m_pLayout->ApplyAttributeList(GetDropBoxAttributeList());
	m_pLayout->AttachSelect(nbase::Bind(&FilterCombo::OnSelectItem, this, std::placeholders::_1));

	m_pRichEdit = new RichEdit;
	m_pRichEdit->AttachTextChange(nbase::Bind(&FilterCombo::OnRichEditTextChanged, this, std::placeholders::_1));
	m_pRichEdit->AttachButtonDown(nbase::Bind(&FilterCombo::OnRichEditButtonDown, this, std::placeholders::_1));
	m_pRichEdit->SetClass(L"simple");
	m_pRichEdit->SetFixedWidth(DUI_LENGTH_STRETCH);
	m_pRichEdit->SetFixedHeight(DUI_LENGTH_STRETCH);
	m_pRichEdit->SetMargin({ 1, 1, 1, 1 });
	m_pRichEdit->SetAttribute(L"padding", L"2,3");
	m_pRichEdit->SetFont(L"system_14");
	//m_pRichEdit->SetMouseChildEnabled(false);
	//m_pRichEdit->EnableScrollBar();
	Box::Add(m_pRichEdit);
}

void FilterCombo::HandleMessage(EventArgs& args)
{
	if (args.Type == kEventChar || args.Type == kEventKeyDown)
	{
		m_pRichEdit->HandleMessage(args);
	}
	return __super::HandleMessage(args);
}

bool FilterCombo::Add(Control* pControl)
{
	ListElementMatch *pListElementMatch = dynamic_cast<ListElementMatch*>(pControl);
	if (pListElementMatch)
	{
		
	}
	else
	{
		printf("CheckCombo::Add pControl is not CheckBox object\n");
		assert(0);
		return true;
	}

	m_pLayout->Add(pControl);
	pControl->SetReceivePointerMsg(true);
	m_iCurSel = m_pLayout->GetCurSel();
	return true;
}

bool FilterCombo::Remove(Control * pControl)
{
	bool ret = m_pLayout->Remove(pControl);
	m_iCurSel = m_pLayout->GetCurSel();
	return ret;
}

bool FilterCombo::RemoveAt(std::size_t iIndex)
{
	bool ret = m_pLayout->RemoveAt((int)iIndex);
	m_iCurSel = m_pLayout->GetCurSel();
	return ret;
}

void FilterCombo::RemoveAll()
{
	m_pLayout->RemoveAll();
	m_iCurSel = -1;
}

void FilterCombo::Activate()
{
    if( !IsActivatable() ) return;
	if (m_pComboWnd) return;

	m_pComboWnd = new CFilterComboWnd();
	ASSERT(m_pComboWnd);
	m_pComboWnd->Init(this);

	//if (m_pComboWnd != NULL) m_pComboWnd->SendNotify(this, kEventClick);
    Invalidate();
}

void FilterCombo::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("dropbox")) SetDropBoxAttributeList(strValue);
	else if (strName == _T("vscrollbar")) {}
	else if (strName == _T("dropboxsize"))
	{
		CSize szDropBoxSize;
		LPTSTR pstr = NULL;
		szDropBoxSize.cx = _tcstol(strValue.c_str(), &pstr, 10); ASSERT(pstr);
		szDropBoxSize.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		SetDropBoxSize(szDropBoxSize);
	}
	else if (strName == _T("popuptop")) SetPopupTop(strValue == _T("true"));
	else Box::SetAttribute(strName, strValue);
}

std::wstring FilterCombo::GetText() const
{
#if 0
    if( m_iCurSel < 0 ) return _T("");
	ListContainerElement* pControl = static_cast<ListContainerElement*>(m_pLayout->GetItemAt(m_iCurSel));
    return pControl->GetText();
#else
	if (m_pRichEdit)
		return m_pRichEdit->GetText();
	return L"";
#endif
}

FilterListBox* FilterCombo::GetListBox() {
	return m_pLayout.get(); 
}


std::wstring FilterCombo::GetDropBoxAttributeList()
{
    return m_sDropBoxAttributes;
}

void FilterCombo::SetDropBoxAttributeList(const std::wstring& pstrList)
{
    m_sDropBoxAttributes = pstrList;
}

CSize FilterCombo::GetDropBoxSize() const
{
    return m_szDropBox;
}

void FilterCombo::SetDropBoxSize(CSize szDropBox)
{
	DpiManager::GetInstance()->ScaleSize(szDropBox);
    m_szDropBox = szDropBox;
}

bool FilterCombo::SelectItem(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_pLayout->GetCount() || m_iCurSel == iIndex)
		return false;

	m_iCurSel = iIndex;
	m_pLayout->SelectItem(m_iCurSel);

	return true;
}

Control* FilterCombo::GetItemAt(int iIndex)
{
	return m_pLayout->GetItemAt(iIndex);
}

int FilterCombo::GetCount() const {
	return m_pLayout->GetCount(); 
}

void FilterCombo::AttachSelect(const EventCallback& callback) { 
	m_pLayout->AttachSelect(callback); 
}

bool FilterCombo::OnSelectItem(EventArgs* args)
{
	m_pComboWnd->OnSeleteItem();
	m_iCurSel = m_pLayout->GetCurSel();
	auto pControl = m_pLayout->GetItemAt(m_iCurSel);
	if (pControl != NULL) {
		pControl->SetState(kControlStateNormal);
	}
	if (m_pComboWnd != NULL) {
		m_pComboWnd->SendNotify(this, kEventSelect, m_iCurSel, -1);
	}
	ListContainerElement *ele = dynamic_cast<ListContainerElement*>(pControl);
	if (m_pRichEdit && ele)
	{
		m_pRichEdit->SetText(ele->GetText());
	}
	return true;
}

bool FilterCombo::OnRichEditTextChanged(EventArgs* args)
{
	if (m_pLayout)
	{
		m_pLayout->Filter(m_pRichEdit->GetUTF8Text());

		if (m_pComboWnd)
		{
			CSize szDrop = GetDropBoxSize();
			UiRect rcOwner = GetPosWithScrollOffset();
			UiRect rc = rcOwner;
			rc.top = rc.bottom + 1;		// 父窗口left、bottom位置作为弹出窗口起点
			rc.bottom = rc.top + szDrop.cy;	// 计算弹出窗口高度
			if (szDrop.cx > 0) rc.right = rc.left + szDrop.cx;	// 计算弹出窗口宽度

			CSize szAvailable(rc.right - rc.left, rc.bottom - rc.top);
			int cyFixed = 0;
			for (int it = 0; it < GetListBox()->GetCount(); it++) {
				Control* pControl = GetListBox()->GetItemAt(it);
				if (!pControl->IsVisible()) continue;
				CSize sz = pControl->EstimateSize(szAvailable);
				cyFixed += sz.cy;
			}
			cyFixed += 2; // VBox 默认的Padding 调整
			rc.bottom = rc.top + min(cyFixed, szDrop.cy);

			::MapWindowRect(GetWindow()->GetHWND(), HWND_DESKTOP, &rc);

			MONITORINFO oMonitor = {};
			oMonitor.cbSize = sizeof(oMonitor);
			::GetMonitorInfo(::MonitorFromWindow(m_pComboWnd->GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
			UiRect rcWork = oMonitor.rcWork;
			if (rc.bottom > rcWork.bottom || IsPopupTop()) {
				rc.left = rcOwner.left;
				rc.right = rcOwner.right;
				if (szDrop.cx > 0) rc.right = rc.left + szDrop.cx;
				rc.top = rcOwner.top - min(cyFixed, szDrop.cy);
				rc.bottom = rcOwner.top;
				::MapWindowRect(GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
			}

			m_pComboWnd->SetPos(rc, true, SWP_NOACTIVATE);
			m_pLayout->SetPos(m_pLayout->GetPos());
		}
	}
	return true;
}

bool FilterCombo::OnRichEditButtonDown(EventArgs* args)
{
	Activate();
	return true;
}

} // namespace ui
