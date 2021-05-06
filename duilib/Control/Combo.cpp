#include "StdAfx.h"

namespace ui
{

class CComboWnd : public Window
{
public:
    void Init(Combo* pOwner);
    virtual std::wstring GetWindowClassName() const override;
	virtual void OnFinalMessage(HWND hWnd) override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void OnSeleteItem();

private:
    Combo *m_pOwner = nullptr;
    int m_iOldSel;
	bool m_bClosing = false;
};


void CComboWnd::Init(Combo* pOwner)
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

	int padding = 2;
	auto listBox = m_pOwner->GetListBox();
	if (listBox)
		padding = listBox->GetLayout()->GetPadding().top + listBox->GetLayout()->GetPadding().bottom;

	cyFixed += padding; // VBox 默认的Padding 调整
    rc.bottom = rc.top + MIN(cyFixed, szDrop.cy);

    ::MapWindowRect(pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);

    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    UiRect rcWork = oMonitor.rcWork;
    if( rc.bottom > rcWork.bottom || m_pOwner->IsPopupTop()) {
        rc.left = rcOwner.left;
        rc.right = rcOwner.right;
        if( szDrop.cx > 0 ) rc.right = rc.left + szDrop.cx;
        rc.top = rcOwner.top - MIN(cyFixed, szDrop.cy);
        rc.bottom = rcOwner.top;
        ::MapWindowRect(pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
    }
    
    Create(pOwner->GetWindow()->GetHWND(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, true, rc);
    // HACK: Don't deselect the parent's caption
    HWND hWndParent = m_hWnd;
    while( ::GetParent(hWndParent) != NULL ) 
		hWndParent = ::GetParent(hWndParent);
    ::ShowWindow(m_hWnd, SW_SHOW);
    ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

std::wstring CComboWnd::GetWindowClassName() const
{
    return _T("ComboWnd");
}

void CComboWnd::OnFinalMessage(HWND hWnd)
{
	if (m_pOwner)
	{
		m_pOwner->m_pWindow = NULL;
		m_pOwner->m_uButtonState = kControlStateNormal;
		m_pOwner->Invalidate();
	}
	__super::OnFinalMessage(hWnd);
    delete this;
}

void CComboWnd::OnSeleteItem()
{
	PostMessage(WM_KILLFOCUS);
}

LRESULT CComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if( uMsg == WM_CREATE ) {
        this->Window::Init(m_hWnd);
		Box* pRoot = new Box;
		pRoot->SetAutoDestroyChild(false);
		pRoot->Add(m_pOwner->GetListBox());
		this->AttachDialog(pRoot);
		this->SetWindowResourcePath(m_pOwner->GetWindow()->GetWindowResourcePath());
		this->SetShadowAttached(false);
		this->SetRenderTransparent(true);

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
			m_pOwner->SelectItemInternal(m_pOwner->GetListBox()->GetCurSel());
			((Box*)this->GetRoot())->RemoveAt(0);
			m_pOwner->GetListBox()->PlaceHolder::SetWindow(nullptr, nullptr, false);
		}
    }

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

////////////////////////////////////////////////////////

Combo::Combo() :
    m_pWindow(nullptr),
	m_iCurSel(-1),
	m_szDropBox(0, 150),
	m_uButtonState(kControlStateNormal),
	m_sDropBoxAttributes(),
	m_bPopupTop(false)
{
	// The trick is to add the items to the new container. Their owner gets
	// reassigned by this operation - which is why it is important to reassign
	// the items back to the righfull owner/manager when the window closes.
	m_pLayout.reset(new ListBox(new VLayout));
	m_pLayout->GetLayout()->SetPadding(UiRect(1, 1, 1, 1));
	m_pLayout->SetBkColor(L"bk_wnd_lightcolor");
	m_pLayout->SetBorderColor(L"combobox_border");
	m_pLayout->SetBorderSize(UiRect(1, 1, 1, 1));
	m_pLayout->SetBkColor(L"white");
	m_pLayout->SetBorderColor(L"gray");
	m_pLayout->SetAutoDestroyChild(false);
	m_pLayout->EnableScrollBar();
	m_pLayout->ApplyAttributeList(GetDropBoxAttributeList());
	m_pLayout->AttachSelect(nbase::Bind(&Combo::OnSelectItem, this, std::placeholders::_1));
}

std::wstring Combo::GetType() const
{
	return DUI_CTR_COMBO;
}

UIAControlProvider* Combo::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIAComboBoxProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

bool Combo::Add(Control* pControl)
{
	m_pLayout->Add(pControl);
	pControl->SetReceivePointerMsg(true);
	m_iCurSel = m_pLayout->GetCurSel();
	return true;
}

bool Combo::Remove(Control * pControl)
{
	bool ret = m_pLayout->Remove(pControl);
	m_iCurSel = m_pLayout->GetCurSel();
	return ret;
}

bool Combo::RemoveAt(std::size_t iIndex)
{
	bool ret = m_pLayout->RemoveAt((int)iIndex);
	m_iCurSel = m_pLayout->GetCurSel();
	return ret;
}

void Combo::RemoveAll()
{
	m_pLayout->RemoveAll();
	m_iCurSel = -1;
}

void Combo::Activate()
{
    if( !IsActivatable() ) return;
	if (m_pWindow) return;

    m_pWindow = new CComboWnd();
    ASSERT(m_pWindow);
    m_pWindow->Init(this);
	m_pWindow->AttachWindowClose(ToWeakCallback([this](ui::EventArgs* msg) {
		auto callback = OnEvent.find(msg->Type);
		if (callback != OnEvent.end()) {
			callback->second(msg);
		}
		return true;
	}));

	if (m_pWindow != NULL) m_pWindow->SendNotify(this, kEventClick);
    Invalidate();
}

void Combo::Deactivate()
{
	if (!IsActivatable()) return;
	if (!m_pWindow) return;

	m_pWindow->Close();
	Invalidate();
}

bool Combo::IsActivated()
{
	return (m_pWindow && !m_pWindow->IsClosing());
}

void Combo::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
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
	else if (strName == _T("textpadding")) {
		UiRect rcTextPadding;
		LPTSTR pstr = NULL;
		rcTextPadding.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
		rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		SetTextPadding(rcTextPadding);
	}
	else Box::SetAttribute(strName, strValue);
}

void Combo::PaintText(IRenderContext* pRender)
{
	UiRect rcText = m_rcItem;

	if (m_iCurSel >= 0) {
		Control* pControl = static_cast<Control*>((m_pLayout->GetItemAt(m_iCurSel)));
		ListContainerElement* pElement = dynamic_cast<ListContainerElement*>(pControl);
		assert(pElement);		
		if (!pElement)
			return;
		UiRect rcPadding = m_rcTextPadding;

		if (pElement) {
			if (GetText().empty())
				return;

			if (pElement->GetOwner() == NULL)
				return;

			if (rcPadding.left == 0 && rcPadding.top == 0 && rcPadding.right == 0 && rcPadding.bottom == 0)
				rcPadding = pElement->GetTextPadding();
			rcText.left += rcPadding.left;
			rcText.right -= rcPadding.right;
			rcText.top += rcPadding.top;
			rcText.bottom -= rcPadding.bottom;

			DWORD dwTextColor = 0xFF000000;
			dwTextColor = this->GetWindowColor(pElement->GetStateTextColor(kControlStateNormal));
			pRender->DrawText(rcText, GetText(), dwTextColor, \
				pElement->GetFont(), DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
		}
		else {	
			UiRect rcOldPos = pControl->GetPos();
			pControl->SetPos(rcText);
			pControl->AlphaPaint(pRender, rcText);
			pControl->SetPos(rcOldPos);
		}
	}
}

std::wstring Combo::GetText() const
{
    if( m_iCurSel < 0 ) return _T("");
	ListContainerElement* pControl = static_cast<ListContainerElement*>(m_pLayout->GetItemAt(m_iCurSel));
	return pControl ? pControl->GetText() : _T("");
}

UiRect Combo::GetTextPadding() const
{
	return m_rcTextPadding;
}

void Combo::SetTextPadding(UiRect rc)
{
	DpiManager::GetInstance()->ScaleRect(rc);
	m_rcTextPadding = rc;
	this->Invalidate();
}

std::wstring Combo::GetDropBoxAttributeList()
{
    return m_sDropBoxAttributes;
}

void Combo::SetDropBoxAttributeList(const std::wstring& pstrList)
{
    m_sDropBoxAttributes = pstrList;
    m_pLayout->ApplyAttributeList(pstrList);
}

CSize Combo::GetDropBoxSize() const
{
    return m_szDropBox;
}

void Combo::SetDropBoxSize(CSize szDropBox)
{
	DpiManager::GetInstance()->ScaleSize(szDropBox);
    m_szDropBox = szDropBox;
}

bool Combo::SelectItemInternal(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_pLayout->GetCount())
		return false;

	int iOldSel = m_iCurSel;
	m_iCurSel = iIndex;
	m_pLayout->SelectItem(m_iCurSel, false, false);

	//add by djj below
	if (m_pWindow != NULL) {
		m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, iOldSel);
	}

#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider != nullptr && UiaClientsAreListening()) {
		VARIANT vtOld = { 0 }, vtNew = { 0 };
		vtOld.vt = vtNew.vt = VT_BSTR;
		ListContainerElement* pControl = static_cast<ListContainerElement*>(m_pLayout->GetItemAt(m_iCurSel));
		vtOld.bstrVal = SysAllocString(pControl ? pControl->GetText().c_str() : L"");
		vtNew.bstrVal = SysAllocString(GetText().c_str());

		UiaRaiseAutomationPropertyChangedEvent(m_pUIAProvider, UIA_ValueValuePropertyId, vtOld, vtNew);
	}
#endif

	Invalidate();	

	return true;
}

bool Combo::SelectItem(int iIndex, bool bTrigger)
{
    m_pLayout->SelectItem(iIndex, false, false);
    if (!SelectItemInternal(iIndex))
        return false;
    Invalidate();
    if (m_pWindow != NULL && bTrigger) {
        m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, -1);
    }
}

Control* Combo::GetItemAt(int iIndex)
{
	return m_pLayout->GetItemAt(iIndex);
}

bool Combo::OnSelectItem(EventArgs* args)
{
	if (m_pWindow)
        m_pWindow->OnSeleteItem();
	int iOldSel = m_iCurSel;
	m_iCurSel = m_pLayout->GetCurSel();
	auto pControl = m_pLayout->GetItemAt(m_iCurSel);
	if (pControl != NULL) {
		pControl->SetState(kControlStateNormal);
	}
	if (m_pWindow != NULL) {
		m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, iOldSel);
	}
	return true;
}

} // namespace ui
