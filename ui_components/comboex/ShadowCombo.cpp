#include "stdafx.h"
#include "ShadowCombo.h"

namespace nim_comp
{
CShadowComboWnd::CShadowComboWnd() {}

CShadowComboWnd::~CShadowComboWnd() {}

void CShadowComboWnd::Init(ShadowCombo* pOwner) {
  m_pOwner = pOwner;
  m_iOldSel = m_pOwner->GetCurSel();

  // Position the popup window in absolute space
  CSize szDrop = m_pOwner->GetDropBoxSize();
  UiRect rcOwner = pOwner->GetPosWithScrollOffset();
  int iItemHeight = m_iOldSel > -1 ? pOwner->GetItemAt(m_iOldSel)->GetFixedHeight() : 0;
  int iOffset = iItemHeight * (m_iOldSel + 1);
  iOffset = max(iOffset, 0);
  int iScrollPos = pOwner->GetCustomLayout()->GetScrollPos().cy;
  if (iScrollPos > iItemHeight) {
    iOffset = iItemHeight;
  }

  UiRect rc = rcOwner;
  rc.top = rc.bottom;		// 父窗口left、bottom位置作为弹出窗口起点
  rc.top = rc.top - iOffset;
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

  int padding = 2;
  auto listBox = m_pOwner->GetListBox();
  if (listBox) {
    padding = listBox->GetLayout()->GetPadding().top + listBox->GetLayout()->GetPadding().bottom;
  }

  cyFixed += padding; // VBox 默认的Padding 调整
  rc.bottom = rc.top + min(cyFixed, szDrop.cy);
  ui::UiRect shadow_corner = m_pOwner->GetShadowCorner();
  rc.left = rc.left - shadow_corner.left;
  rc.right = rc.right + shadow_corner.right;
  rc.top = rc.top - shadow_corner.top;
  rc.bottom = rc.bottom + shadow_corner.bottom;

  ::MapWindowRect(pOwner->GetWindow()->GetHWND(), HWND_DESKTOP, &rc);

  MONITORINFO oMonitor = {};
  oMonitor.cbSize = sizeof(oMonitor);
  ::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
  UiRect rcWork = oMonitor.rcWork;
  if (rc.bottom > rcWork.bottom || m_pOwner->IsPopupTop()) {
    rc.left = rcOwner.left;
    rc.right = rcOwner.right;
    rc.top = rcOwner.bottom - min(cyFixed, szDrop.cy);
    rc.bottom = rcOwner.bottom;

    rc.left = rc.left - shadow_corner.left;
    rc.right = rc.right + shadow_corner.right;
    rc.top = rc.top - shadow_corner.top;
    rc.bottom = rc.bottom + shadow_corner.bottom;
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

std::wstring CShadowComboWnd::GetWindowClassName() const
{
  return _T("CShadowComboWnd");
}

void CShadowComboWnd::OnFinalMessage(HWND hWnd)
{
  if (m_pOwner)
  {
    m_pOwner->SetCShadowComboWnd(nullptr);
    m_pOwner->Invalidate();
  }
  __super::OnFinalMessage(hWnd);
  delete this;
}

void CShadowComboWnd::OnSeleteItem()
{
  PostMessage(WM_KILLFOCUS);
}

LRESULT CShadowComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CREATE) {
    this->Window::Init(m_hWnd);
    Box* pRoot = new Box;
    pRoot->SetAutoDestroyChild(false);
    pRoot->Add(m_pOwner->GetListBox());
    if (!m_pOwner->GetShadowImage().empty()) {
      pRoot->SetBkImage(m_pOwner->GetShadowImage());
    }
    pRoot->GetLayout()->SetPadding(m_pOwner->GetShadowCorner(), false);
    this->AttachDialog(pRoot);
    this->SetWindowResourcePath(m_pOwner->GetWindow()->GetWindowResourcePath());
    this->SetShadowAttached(false);
    this->SetRenderTransparent(true);

    return 0;
  }
  else if (uMsg == WM_CLOSE) {
    m_pOwner->SetWindow(m_pOwner->GetWindow(), m_pOwner->GetParent(), false);
    m_pOwner->SetPos(m_pOwner->GetPos());
    m_pOwner->SetFocus();
  }
  else if (uMsg == WM_KILLFOCUS) {
    if (m_hWnd != (HWND)wParam) {
      m_bClosing = true;
      PostMessage(WM_CLOSE);
      m_pOwner->SelectItemInternal(m_pOwner->GetListBox()->GetCurSel());
      ((Box*)this->GetRoot())->RemoveAt(0);
      m_pOwner->GetListBox()->PlaceHolder::SetWindow(nullptr, nullptr, false);
    }
  }

  if (m_bClosing) {
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

/////////////////////////////////////////////////////////////////////////////
// ShadowCombo
ShadowCombo::ShadowCombo()
    : m_pWindow(nullptr),
     m_iCurSel(-1),
     m_sDropBoxAttributes(),
     m_bPopupTop(false),
     m_sShadowImage(L"file = '../public/bk/bk_combo_shadow.png' corner = '5,4,5,6'"),
     m_cArrow (nullptr),
     m_bInit(false) {
  SetDropBoxSize(ui::CSize(0, 150));
  SetShadowCorner({ 5,4,5,6 });
  SetArrowOffset(10);

  m_pLayout.reset(new ListBox(new VLayout));
  m_pLayout->SetBkColor(L"white");
  m_pLayout->SetAutoDestroyChild(false);
  m_pLayout->EnableScrollBar();
  m_pLayout->ApplyAttributeList(GetDropBoxAttributeList());
  m_pLayout->AttachSelect(nbase::Bind(&ShadowCombo::OnSelectItem, this, std::placeholders::_1));
  SetStateImage(kControlStateNormal, L"../public/combo/combo_normal.svg");
  SetStateImage(kControlStateHot, L"../public/combo/combo_hot.svg");

  m_cArrow = new ui::Control;
  m_cArrow->SetStateImage(kControlStateNormal, L"../public/combo/arrow_normal.svg");
  m_cArrow->SetStateImage(kControlStateHot, L"../public/combo/arrow_hot.svg");
  m_cArrow->SetFixedWidth(DUI_LENGTH_AUTO);
  m_cArrow->SetFixedHeight(DUI_LENGTH_AUTO);
}

ShadowCombo::~ShadowCombo() {
  delete m_cArrow;
  m_cArrow = nullptr;
}

void ShadowCombo::DoInit() {
  __super::DoInit();

  if (!m_bInit) {
    m_bInit = true;

    m_cArrow->SetWindow(GetWindow());
    AttachResize(ToWeakCallback([this](ui::EventArgs* args) {
      ui::UiRect rect = m_rcItem;
      ui::CSize ArrrowSize = m_cArrow->EstimateSize(CSize(m_rcItem.GetWidth(), m_rcItem.GetHeight()));
      rect.top = m_rcItem.top + (m_rcItem.GetHeight() - ArrrowSize.cy) / 2;
      rect.bottom = rect.top + ArrrowSize.cy;
      rect.left = m_rcItem.right - ArrrowSize.cx - m_iArrowOffset;
      rect.right = rect.left + ArrrowSize.cx;
      m_cArrow->SetPos(rect);
      return true;
    }));
  }
}

std::wstring ShadowCombo::GetType() const {
  return L"ShadowCombo";
}

UIAControlProvider* ShadowCombo::GetUIAProvider()
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


bool ShadowCombo::Add(Control* pControl)
{
  m_pLayout->Add(pControl);
  pControl->SetReceivePointerMsg(true);
  m_iCurSel = m_pLayout->GetCurSel();
  return true;
}

bool ShadowCombo::Remove(Control* pControl)
{
  bool ret = m_pLayout->Remove(pControl);
  m_iCurSel = m_pLayout->GetCurSel();
  return ret;
}

bool ShadowCombo::RemoveAt(std::size_t iIndex)
{
  bool ret = m_pLayout->RemoveAt((int)iIndex);
  m_iCurSel = m_pLayout->GetCurSel();
  return ret;
}

void ShadowCombo::RemoveAll()
{
  m_pLayout->RemoveAll();
  m_iCurSel = -1;
}

void ShadowCombo::Activate()
{
  if (!IsActivatable()) return;
  if (m_pWindow) return;

  m_pWindow  = new CShadowComboWnd;
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

void ShadowCombo::Deactivate()
{
  if (!IsActivatable()) return;
  if (!IsActivated()) return;

  m_pWindow->PostMessage(WM_KILLFOCUS);
  Invalidate();
}

bool ShadowCombo::IsActivated()
{
  return (m_pWindow && !m_pWindow->IsClosing());
}

void ShadowCombo::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
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
  } else if (strName == _T("shadowcorner")) {
    UiRect rcShadowCorner;
    LPTSTR pstr = NULL;
    rcShadowCorner.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
    rcShadowCorner.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
    rcShadowCorner.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
    rcShadowCorner.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
    SetShadowCorner(rcShadowCorner);
  }
  else if (strName == _T("arrownormalimage")) {
    m_cArrow->SetStateImage(kControlStateNormal, strValue);
  } else if (strName == _T("arrowhotimage")) {
    m_cArrow->SetStateImage(kControlStateHot, strValue);
  } else if (strName == _T("arrowpushedimage")) {
    m_cArrow->SetStateImage(kControlStatePushed, strValue);
  } else if (strName == _T("arrowdisabledimage")) {
    m_cArrow->SetStateImage(kControlStateDisabled, strValue);
  } else if (strName == _T("arrowoffset")) {
    SetArrowOffset(_ttoi(strValue.c_str()));
  }

  else Box::SetAttribute(strName, strValue);
}

void ShadowCombo::PaintText(IRenderContext* pRender)
{
  UiRect rcText = m_rcItem;
  rcText.right = m_cArrow->GetPos().left;

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

void ShadowCombo::PaintChild(IRenderContext* pRender, const UiRect& rcPaint) {
  __super::PaintChild(pRender, rcPaint);
  if (m_cArrow->GetState() != GetState()) {
    m_cArrow->SetState(GetState());
  }
  m_cArrow->Paint(pRender, rcPaint);
}

std::wstring ShadowCombo::GetText() const
{
  if (m_iCurSel < 0) return _T("");
  ListContainerElement* pControl = static_cast<ListContainerElement*>(m_pLayout->GetItemAt(m_iCurSel));
  return pControl ? pControl->GetText() : _T("");
}

UiRect ShadowCombo::GetTextPadding() const
{
  return m_rcTextPadding;
}

void ShadowCombo::SetTextPadding(UiRect rc)
{
  DpiManager::GetInstance()->ScaleRect(rc);
  m_rcTextPadding = rc;
  this->Invalidate();
}

std::wstring ShadowCombo::GetDropBoxAttributeList()
{
  return m_sDropBoxAttributes;
}

void ShadowCombo::SetDropBoxAttributeList(const std::wstring& pstrList)
{
  m_sDropBoxAttributes = pstrList;
  m_pLayout->ApplyAttributeList(pstrList);
}

CSize ShadowCombo::GetDropBoxSize() const
{
  return m_szDropBox;
}

void ShadowCombo::SetDropBoxSize(CSize szDropBox)
{
  DpiManager::GetInstance()->ScaleSize(szDropBox);
  m_szDropBox = szDropBox;
}

bool ShadowCombo::SelectItemInternal(int iIndex)
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
bool ShadowCombo::SelectItem(int iIndex, bool bTrigger)
{
  m_pLayout->SelectItem(iIndex, false, false);
  if (!SelectItemInternal(iIndex))
    return false;
  Invalidate();
  if (m_pWindow != NULL && bTrigger) {
    m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, -1);
  }
  return true;
}

Control* ShadowCombo::GetItemAt(int iIndex)
{
  return m_pLayout->GetItemAt(iIndex);
}

bool ShadowCombo::OnSelectItem(EventArgs* args)
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

void ShadowCombo::SetShadowCorner(const UiRect& rect, bool bNeedDpiScale) {
  ui::UiRect rc = rect;
  if (bNeedDpiScale) {
    ui::DpiManager::GetInstance()->ScaleRect(rc);
  }
  m_rcShadowCorner = rc;
}

void ShadowCombo::SetArrowOffset(int offset, bool bNeedDpiScale) {
  if (bNeedDpiScale) {
    ui::DpiManager::GetInstance()->ScaleInt(offset);
  }
  m_iArrowOffset = offset;
}

}
