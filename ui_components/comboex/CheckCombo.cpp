#include "stdafx.h"
#include "CheckCombo.h"

namespace nim_comp
{

	class CCheckComboWnd : public Window
	{
	public:
		void Init(CheckCombo* pOwner);
		virtual std::wstring GetWindowClassName() const override;
		virtual void OnFinalMessage(HWND hWnd) override;
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		void OnSeleteItem();

	private:
		CheckCombo *m_pOwner;
		int m_iOldSel;
		bool m_bClosing = false;
	};


	void CCheckComboWnd::Init(CheckCombo* pOwner)
	{
		m_pOwner = pOwner;
		//m_iOldSel = m_pOwner->GetCurSel();

		// Position the popup window in absolute space
		CSize szDrop = m_pOwner->GetDropBoxSize();
		UiRect rcOwner = m_pOwner->GetOrgPos();
		UiRect rc = rcOwner;
		rc.top = rc.bottom + 1;		// 父窗口left、bottom位置作为弹出窗口起点
		rc.bottom = rc.top + szDrop.cy;	// 计算弹出窗口高度
		if (szDrop.cx > 0) rc.right = rc.left + szDrop.cx;	// 计算弹出窗口宽度

		CSize szAvailable(rc.right - rc.left, rc.bottom - rc.top);
		int cyFixed = 0;
		for (int it = 0; it < pOwner->GetListBox()->GetCount(); it++) {
			Control* pControl = pOwner->GetListBox()->GetItemAt(it);
			if (!pControl->IsVisible()) continue;
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
		if (rc.bottom > rcWork.bottom || m_pOwner->IsPopupTop()) {
			rc.left = rcOwner.left;
			rc.right = rcOwner.right;
			if (szDrop.cx > 0) rc.right = rc.left + szDrop.cx;
			rc.top = rcOwner.top - min(cyFixed, szDrop.cy);
			rc.bottom = rcOwner.top;
			::MapWindowRect(pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);
		}

		Create(pOwner->GetWindow()->GetHWND(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, true, rc);
		// HACK: Don't deselect the parent's caption
		HWND hWndParent = m_hWnd;
		while (::GetParent(hWndParent) != NULL)
			hWndParent = ::GetParent(hWndParent);
		::ShowWindow(m_hWnd, SW_SHOW);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	}

	std::wstring CCheckComboWnd::GetWindowClassName() const
	{
		return _T("ComboWnd");
	}

	void CCheckComboWnd::OnFinalMessage(HWND hWnd)
	{
		m_pOwner->m_pCheckComboWnd = NULL;
		m_pOwner->m_uButtonState = kControlStateNormal;
		m_pOwner->Invalidate();
		delete this;
	}

	void CCheckComboWnd::OnSeleteItem()
	{
		PostMessage(WM_KILLFOCUS);
	}

	LRESULT CCheckComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CREATE) {
			this->Window::Init(m_hWnd);
			Box* pRoot = new Box;
			pRoot->SetAutoDestroyChild(false);
			pRoot->Add(m_pOwner->GetListBox());
			this->AttachDialog(pRoot);
			this->SetWindowResourcePath(m_pOwner->GetWindow()->GetWindowResourcePath());
			this->SetShadowAttached(false);

			return 0;
		}
		else if (uMsg == WM_CLOSE) {
			m_pOwner->SetWindow(m_pOwner->GetWindow(), m_pOwner->GetParent(), false);
			m_pOwner->SetPos(m_pOwner->GetPos());
			m_pOwner->SetFocus();
		}
		else if (uMsg == WM_KILLFOCUS) {
			if (m_hWnd != (HWND)wParam)	{
				m_bClosing = true;
				PostMessage(WM_CLOSE);
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

	CheckCombo::CheckCombo() :
		m_pCheckComboWnd(nullptr),
		m_szDropBox(0, 150),
		m_uButtonState(kControlStateNormal),
		m_sDropBoxAttributes(),
		m_bPopupTop(false),
		m_iOrgHeight(20)
	{
		// The trick is to add the items to the new container. Their owner gets
		// reassigned by this operation - which is why it is important to reassign
		// the items back to the righfull owner/manager when the window closes.
		m_pDropList.reset(new ListBox(new VLayout));
		m_pDropList->GetLayout()->SetPadding(UiRect(1, 1, 1, 1));
		m_pDropList->SetBkColor(L"bk_wnd_lightcolor");
		m_pDropList->SetBorderColor(L"splitline_level1");
		m_pDropList->SetBorderSize(UiRect(1, 1, 1, 1));
		m_pDropList->EnableScrollBar();
		m_pDropList->ApplyAttributeList(GetDropBoxAttributeList());

		m_pList.reset(new ListBox(new VLayout));
		//m_pList->SetMouseEnabled(false);
		m_pList->AttachButtonDown(std::bind(&CheckCombo::OnListButtonDown, this, std::placeholders::_1));
		m_pList->SetMouseChildEnabled(false);
		//m_pList->SetAutoDestroyChild(false);
		m_pList->EnableScrollBar();
		Box::Add(m_pList.get());

		SetMaxHeight(m_iOrgHeight * 3);
		SetMinHeight(m_iOrgHeight);
	}

	CheckCombo::~CheckCombo()
	{
		Box::Remove(m_pList.get());
	}

	bool CheckCombo::Add(Control* pControl)
	{
		CheckBoxBox *pCheckBoxBox = nullptr;
		CheckBox *pCheckBox = dynamic_cast<CheckBox*>(pControl);
		if (pCheckBox)
		{
			pCheckBox->AttachSelect(std::bind(&CheckCombo::OnSelectItem, this, std::placeholders::_1));
			pCheckBox->AttachUnSelect(std::bind(&CheckCombo::OnUnSelectItem, this, std::placeholders::_1));
		}
		else if (pCheckBoxBox = dynamic_cast<CheckBoxBox*>(pControl)) {
			pCheckBoxBox->AttachSelect(std::bind(&CheckCombo::OnSelectItem, this, std::placeholders::_1));
			pCheckBoxBox->AttachUnSelect(std::bind(&CheckCombo::OnUnSelectItem, this, std::placeholders::_1));
		}
		else
		{
			printf("CheckCombo::Add pControl is not CheckBox object\n");
			assert(0);
			return true;
		}
		m_pDropList->Add(pControl);
		pControl->SetReceivePointerMsg(true);
		return true;
	}

	bool CheckCombo::Remove(Control * pControl)
	{
		bool ret = m_pDropList->Remove(pControl);
		return ret;
	}

	bool CheckCombo::RemoveAt(std::size_t iIndex)
	{
		bool ret = m_pDropList->RemoveAt((int)iIndex);
		return ret;
	}

	void CheckCombo::RemoveAll()
	{
		m_pDropList->RemoveAll();
	}

	void CheckCombo::Activate()
	{
		if (!IsActivatable()) return;
		if (m_pCheckComboWnd) return;

		m_pCheckComboWnd = new CCheckComboWnd();
		ASSERT(m_pCheckComboWnd);
		m_pCheckComboWnd->Init(this);

		if (m_pCheckComboWnd != NULL) m_pCheckComboWnd->SendNotify(this, kEventClick);
		Invalidate();
	}

	void CheckCombo::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
	{
		if (strName == _T("dropbox")) SetDropBoxAttributeList(strValue);
		else if (strName == _T("vscrollbar")) {}
		else if (strName == _T("dropboxsize")){
			CSize szDropBoxSize;
			LPTSTR pstr = NULL;
			szDropBoxSize.cx = _tcstol(strValue.c_str(), &pstr, 10); ASSERT(pstr);
			szDropBoxSize.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetDropBoxSize(szDropBoxSize);
		}
		else if (strName == _T("popuptop")) SetPopupTop(strValue == _T("true"));
		else if (strName == _T("height")) {
			__super::SetAttribute(strName, strValue);
			if (strValue != _T("stretch") && strValue != _T("auto")) {
				ASSERT(_ttoi(strValue.c_str()) >= 0);
				m_iOrgHeight = _ttoi(strValue.c_str());
				SetMaxHeight(m_iOrgHeight * 3);
				SetMinHeight(m_iOrgHeight);
			}
		}
		else Box::SetAttribute(strName, strValue);
	}

	UiRect CheckCombo::GetOrgPos() const
	{
		UiRect rc = GetPosWithScrollOffset();
		rc.bottom = rc.top + m_iOrgHeight;
		return UiRect(rc);
	}

	std::wstring CheckCombo::GetDropBoxAttributeList()
	{
		return m_sDropBoxAttributes;
	}

	void CheckCombo::SetDropBoxAttributeList(const std::wstring& pstrList)
	{
		m_sDropBoxAttributes = pstrList;
	}

	CSize CheckCombo::GetDropBoxSize() const
	{
		return m_szDropBox;
	}

	void CheckCombo::SetDropBoxSize(CSize szDropBox)
	{
		DpiManager::GetInstance()->ScaleSize(szDropBox);
		m_szDropBox = szDropBox;
	}

	bool CheckCombo::SelectItem(int iIndex)
	{
		/*if (iIndex < 0 || iIndex >= m_pDropList->GetCount() || m_iCurSel == iIndex)
			return false;

		m_iCurSel = iIndex;
		m_pDropList->SelectItem(m_iCurSel);*/

		return true;
	}

	Control* CheckCombo::GetItemAt(int iIndex)
	{
		return m_pDropList->GetItemAt(iIndex);
	}

	bool CheckCombo::OnSelectItem(EventArgs* args)
	{
		std::string date = args->pSender->GetUTF8DataID();
		std::string text = date;
		CheckBox *check = dynamic_cast<CheckBox*>(args->pSender);
		if (check)
		{
			text = check->GetUTF8Text();
		}
		if (date.empty())
		{
#ifdef _DEBUG
			printf("CheckCombo::OnSelectItem date.empty()\n");
			assert(0);
#endif
			return true;
		}
		m_vecDate.push_back(date);

		Label *item = new Label;
		item->SetFixedWidth(DUI_LENGTH_AUTO);
		item->SetFixedHeight(22);
		item->SetMargin({ 4, 2, 4, 2 });
		item->SetBkColor(L"bk_menuitem_selected");
		item->SetTextPadding({ 2, 3, 2, 3 });
		item->SetUTF8Name(date);
		item->SetUTF8Text(text);

		m_pList->Add(item);

		SetFixedHeight(m_pList->GetCount() * m_iOrgHeight);

		return true;
	}

	bool CheckCombo::OnUnSelectItem(EventArgs* args)
	{
		std::string date = args->pSender->GetUTF8DataID();
		if (date.empty())
		{
#ifdef _DEBUG
			printf("CheckCombo::OnSelectItem date.empty()\n");
			assert(0);
#endif
			return true;
		}

		assert(std::find(m_vecDate.cbegin(), m_vecDate.cend(), date) != m_vecDate.cend());
		if (std::find(m_vecDate.cbegin(), m_vecDate.cend(), date) != m_vecDate.cend())
		{
			m_vecDate.erase(std::find(m_vecDate.cbegin(), m_vecDate.cend(), date));
		}
		std::wstring utf16;
		StringHelper::MBCSToUnicode(date, utf16, CP_UTF8);
		Control *pRemove = m_pList->FindSubControl(utf16);
		assert(pRemove);
		if (pRemove)
			m_pList->Remove(pRemove);

		SetFixedHeight(m_pList->GetCount() * m_iOrgHeight);
		return true;
	}

	void CheckCombo::ClearAllDate()
	{
		m_pList->RemoveAll();
		m_pDropList->RemoveAll();
		SetFixedHeight(m_iOrgHeight);
		m_vecDate.clear();
	}

	bool CheckCombo::OnListButtonDown(EventArgs* args)
	{
		Activate();
		return true;
	}

}