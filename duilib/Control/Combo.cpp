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
    Combo *m_pOwner;
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
    cyFixed += 2; // VBox 默认的Padding 调整
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
    m_pOwner->m_pWindow = NULL;
    m_pOwner->m_uButtonState = kControlStateNormal;
    m_pOwner->Invalidate();
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
		pRoot->SetAutoDestroy(false);
		pRoot->Add(m_pOwner->GetListBox());
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
	m_pLayout->SetBorderSize(UiRect(1, 1, 1, 1));
	m_pLayout->SetAutoDestroy(false);
	m_pLayout->EnableScrollBar();
	m_pLayout->ApplyAttributeList(GetDropBoxAttributeList());
	m_pLayout->AttachSelect(nbase::Bind(&Combo::OnSelectItem, this, std::placeholders::_1));
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
    if( m_pWindow ) return;

    m_pWindow = new CComboWnd();
    ASSERT(m_pWindow);
    m_pWindow->Init(this);

    if( m_pWindow != NULL ) m_pWindow->SendNotify(this, kEventClick);
    Invalidate();
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
	else Box::SetAttribute(strName, strValue);
}

void Combo::PaintText(IRenderContext* pRender)
{
	UiRect rcText = m_rcItem;

	if (m_iCurSel >= 0) {
		Control* pControl = static_cast<Control*>((m_pLayout->GetItemAt(m_iCurSel)));
		ListContainerElement* pElement = dynamic_cast<ListContainerElement*>(pControl);
		UiRect rcTextPadding = pElement->GetTextPadding();
		rcText.left += rcTextPadding.left;
		rcText.right -= rcTextPadding.right;
		rcText.top += rcTextPadding.top;
		rcText.bottom -= rcTextPadding.bottom;

		if (pElement != NULL) {
			if (GetText().empty())
				return;

			if (pElement->GetOwner() == NULL)
				return;

			DWORD dwTextColor = 0xFF000000;
			dwTextColor = GlobalManager::GetTextColor(pElement->GetStateTextColor(kControlStateNormal));
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
    return pControl->GetText();
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

bool Combo::SelectItem(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_pLayout->GetCount() || m_iCurSel == iIndex)
		return false;

	m_iCurSel = iIndex;
	m_pLayout->SelectItem(m_iCurSel, false, false);

	return true;
}

Control* Combo::GetItemAt(int iIndex)
{
	return m_pLayout->GetItemAt(iIndex);
}

bool Combo::OnSelectItem(EventArgs* args)
{
	m_pWindow->OnSeleteItem();
	m_iCurSel = m_pLayout->GetCurSel();
	auto pControl = m_pLayout->GetItemAt(m_iCurSel);
	if (pControl != NULL) {
		pControl->SetState(kControlStateNormal);
	}
	if (m_pWindow != NULL) {
		m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, -1);
	}
	return true;
}

} // namespace ui
