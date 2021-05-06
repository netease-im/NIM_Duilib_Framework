#include "stdafx.h"
#include "ScrollBar.h"

namespace ui
{

ScrollBar::ScrollBar() :
	m_bHorizontal(false),
	m_bShowButton1(true),
	m_bShowButton2(true),
	m_bAutoHide(true),
	m_nRange(100),
	m_nScrollPos(0),
	m_nLineSize(8),
	m_nThumbMinLength(30),
	m_nLastScrollPos(0),
	m_nLastScrollOffset(0),
	m_nScrollRepeatDelay(0),
	m_pOwner(nullptr),
	m_ptLastMouse(),
	m_rcButton1(0, 0, 0, 0),
	m_rcButton2(0, 0, 0, 0),
	m_rcThumb(0, 0, 0, 0),
	m_uButton1State(kControlStateNormal),
	m_uButton2State(kControlStateNormal),
	m_uThumbState(kControlStateNormal),
	m_sImageModify(),
	m_bkStateImage(),
	m_button1StateImage(),
	m_button2StateImage(),
	m_thumbStateImage(),
	m_railStateImage(),
	m_weakFlagOwner()
{
	m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
	m_cxyFixed.cy = 0;
	m_ptLastMouse.x = m_ptLastMouse.y = 0;
	m_bkStateImage.SetControl(this);
	m_thumbStateImage.SetControl(this);
	m_bFloat = true;
	SetNeedButtonUpWhenKillFocus(true);
}

Box* ScrollBar::GetOwner() const
{
	return m_pOwner;
}

void ScrollBar::SetOwner(ScrollableBox* pOwner)
{
	m_pOwner = pOwner;
}

std::wstring ScrollBar::GetType() const
{
	return DUI_CTR_SCROLLBAR;
}

UIAControlProvider* ScrollBar::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIAScrollBarProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}


void ScrollBar::SetEnabled(bool bEnable)
{
	Control::SetEnabled(bEnable);
	if( bEnable ) {
		m_uButton1State = kControlStateNormal;
		m_uButton2State = kControlStateNormal;
		m_uThumbState = kControlStateNormal;
	}
	else {
		m_uButton1State = kControlStateDisabled;
		m_uButton2State = kControlStateDisabled;
		m_uThumbState = kControlStateDisabled;
	}
}

void ScrollBar::SetFocus()
{
	if (m_pOwner != NULL) m_pOwner->SetFocus();
	else Control::SetFocus();
}

void ScrollBar::SetVisible_(bool bVisible)
{
	if( m_bVisible == bVisible ) return;
	bool v = IsVisible();
	m_bVisible = bVisible;
	if( m_bFocused ) m_bFocused = false;
	if (!bVisible && m_pWindow && m_pWindow->GetFocus() == this) {
		m_pWindow->SetFocus(NULL) ;
	}
	if( IsVisible() != v ) {
		ArrangeSelf();
	}
}

bool ScrollBar::ButtonUp(EventArgs& msg)
{
	bool ret = false;
	if( IsMouseFocused() ) {
		SetMouseFocused(false);
		Invalidate();
		UiRect pos = GetPos();
		if (::PtInRect(&pos, msg.ptMouse)) {
			m_uButtonState = kControlStateHot;
			m_nHotAlpha = 255;
			Activate();
			ret = true;
		}
		else {
			m_uButtonState = kControlStateNormal;
			m_nHotAlpha = 0;
		}
	}

	UiRect ownerPos = m_pOwner->GetPos();
	if (m_bAutoHide && !::PtInRect(&ownerPos, msg.ptMouse)) {
		SetVisible(false);
	}

	return ret;
}

bool ScrollBar::HasHotState()
{
	return true;
}

bool ScrollBar::MouseEnter(EventArgs& msg)
{
	bool ret = __super::MouseEnter(msg);
	if (ret) {
		m_uButton1State = kControlStateHot;
		m_uButton2State = kControlStateHot;
		m_uThumbState = kControlStateHot;
	}

	return ret;
}

bool ScrollBar::MouseLeave(EventArgs& msg)
{
	bool ret = __super::MouseLeave(msg);
	if (ret) {
		m_uButton1State = kControlStateNormal;
		m_uButton2State = kControlStateNormal;
		m_uThumbState = kControlStateNormal;
	}

	return ret;
}

void ScrollBar::SetPos(UiRect rc)
{
	Control::SetPos(rc);
	rc = m_rcItem;

	if (m_bHorizontal) {
		int cx = rc.right - rc.left;
		if (m_bShowButton1) cx -= m_cxyFixed.cy;
		if (m_bShowButton2) cx -= m_cxyFixed.cy;
		if (cx > m_cxyFixed.cy) {
			m_rcButton1.left = rc.left;
			m_rcButton1.top = rc.top;
			if (m_bShowButton1) {
				m_rcButton1.right = rc.left + m_cxyFixed.cy;
				m_rcButton1.bottom = rc.top + m_cxyFixed.cy;
			}
			else {
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.top = rc.top;
			m_rcButton2.right = rc.right;
			if (m_bShowButton2) {
				m_rcButton2.left = rc.right - m_cxyFixed.cy;
				m_rcButton2.bottom = rc.top + m_cxyFixed.cy;
			}
			else {
				m_rcButton2.left = m_rcButton2.right;
				m_rcButton2.bottom = m_rcButton2.top;
			}

			m_rcThumb.top = rc.top;
			m_rcThumb.bottom = rc.top + m_cxyFixed.cy;
			if (m_nRange > 0) {
				int cxThumb = cx * (rc.right - rc.left) / (m_nRange + rc.right - rc.left);
				if (cxThumb < m_nThumbMinLength) cxThumb = m_nThumbMinLength;

				m_rcThumb.left = m_nScrollPos * (cx - cxThumb) / m_nRange + m_rcButton1.right;
				m_rcThumb.right = m_rcThumb.left + cxThumb;
				if (m_rcThumb.right > m_rcButton2.left) {
					m_rcThumb.left = m_rcButton2.left - cxThumb;
					m_rcThumb.right = m_rcButton2.left;
				}
			}
			else {
				m_rcThumb.left = m_rcButton1.right;
				m_rcThumb.right = m_rcButton2.left;
			}
		}
		else {
			int cxButton = (rc.right - rc.left) / 2;
			if (cxButton > m_cxyFixed.cy) cxButton = m_cxyFixed.cy;
			m_rcButton1.left = rc.left;
			m_rcButton1.top = rc.top;
			if (m_bShowButton1) {
				m_rcButton1.right = rc.left + cxButton;
				m_rcButton1.bottom = rc.top + m_cxyFixed.cy;
			}
			else {
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.top = rc.top;
			m_rcButton2.right = rc.right;
			if (m_bShowButton2) {
				m_rcButton2.left = rc.right - cxButton;
				m_rcButton2.bottom = rc.top + m_cxyFixed.cy;
			}
			else {
				m_rcButton2.left = m_rcButton2.right;
				m_rcButton2.bottom = m_rcButton2.top;
			}

			::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
		}
	}
	else {
		int cy = rc.bottom - rc.top;
		if (m_bShowButton1) cy -= m_cxyFixed.cx;
		if (m_bShowButton2) cy -= m_cxyFixed.cx;
		if (cy > m_cxyFixed.cx) {
			m_rcButton1.left = rc.left;
			m_rcButton1.top = rc.top;
			if (m_bShowButton1) {
				m_rcButton1.right = rc.left + m_cxyFixed.cx;
				m_rcButton1.bottom = rc.top + m_cxyFixed.cx;
			}
			else {
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.left = rc.left;
			m_rcButton2.bottom = rc.bottom;
			if (m_bShowButton2) {
				m_rcButton2.top = rc.bottom - m_cxyFixed.cx;
				m_rcButton2.right = rc.left + m_cxyFixed.cx;
			}
			else {
				m_rcButton2.top = m_rcButton2.bottom;
				m_rcButton2.right = m_rcButton2.left;
			}

			m_rcThumb.left = rc.left;
			m_rcThumb.right = rc.left + m_cxyFixed.cx;
			if (m_nRange > 0) {
				int cyThumb = cy * (rc.bottom - rc.top) / (m_nRange + rc.bottom - rc.top);
				if (cyThumb < m_nThumbMinLength) cyThumb = m_nThumbMinLength;

				m_rcThumb.top = m_nScrollPos * (cy - cyThumb) / m_nRange + m_rcButton1.bottom;
				m_rcThumb.bottom = m_rcThumb.top + cyThumb;
				if (m_rcThumb.bottom > m_rcButton2.top) {
					m_rcThumb.top = m_rcButton2.top - cyThumb;
					m_rcThumb.bottom = m_rcButton2.top;
				}
			}
			else {
				m_rcThumb.top = m_rcButton1.bottom;
				m_rcThumb.bottom = m_rcButton2.top;
			}
		}
		else {
			int cyButton = (rc.bottom - rc.top) / 2;
			if (cyButton > m_cxyFixed.cx) cyButton = m_cxyFixed.cx;
			m_rcButton1.left = rc.left;
			m_rcButton1.top = rc.top;
			if (m_bShowButton1) {
				m_rcButton1.right = rc.left + m_cxyFixed.cx;
				m_rcButton1.bottom = rc.top + cyButton;
			}
			else {
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.left = rc.left;
			m_rcButton2.bottom = rc.bottom;
			if (m_bShowButton2) {
				m_rcButton2.top = rc.bottom - cyButton;
				m_rcButton2.right = rc.left + m_cxyFixed.cx;
			}
			else {
				m_rcButton2.top = m_rcButton2.bottom;
				m_rcButton2.right = m_rcButton2.left;
			}

			::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
		}
	}
}

void ScrollBar::HandleMessage(EventArgs& event)
{
	ASSERT(m_pOwner);

	if (!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) {
		if (m_pOwner != NULL) m_pOwner->HandleMessageTemplate(event);
		return;
	}

	if (event.Type == kEventInternalSetFocus) {
		return;
	}
	else if (event.Type == kEventInternalKillFocus) {
		return;
	}
	else if (event.Type == kEventMouseButtonDown || event.Type == kEventInternalDoubleClick || event.Type == kEventPointDown) {
		if (!IsEnabled()) return;

		m_nLastScrollOffset = 0;
		m_nScrollRepeatDelay = 0;

		auto callback = nbase::Bind(&ScrollBar::ScrollTimeHandle, this);
		TimerManager::GetInstance()->AddCancelableTimer(m_weakFlagOwner.GetWeakFlag(), callback, 50, TimerManager::REPEAT_FOREVER);

		if (::PtInRect(&m_rcButton1, event.ptMouse)) {
			m_uButton1State = kControlStatePushed;
			if (!m_bHorizontal) {
				if (m_pOwner != NULL) m_pOwner->LineUp();
				else SetScrollPos(m_nScrollPos - m_nLineSize);
			}
			else {
				if (m_pOwner != NULL) m_pOwner->LineLeft();
				else SetScrollPos(m_nScrollPos - m_nLineSize);
			}
		}
		else if (::PtInRect(&m_rcButton2, event.ptMouse)) {
			m_uButton2State = kControlStatePushed;
			if (!m_bHorizontal) {
				if (m_pOwner != NULL) m_pOwner->LineDown();
				else SetScrollPos(m_nScrollPos + m_nLineSize);
			}
			else {
				if (m_pOwner != NULL) m_pOwner->LineRight();
				else SetScrollPos(m_nScrollPos + m_nLineSize);
			}
		}
		else if (::PtInRect(&m_rcThumb, event.ptMouse)) {
			m_uThumbState = kControlStatePushed;
			SetMouseFocused(true);
			m_ptLastMouse = event.ptMouse;
			m_nLastScrollPos = m_nScrollPos;
		}
		else {
			if (!m_bHorizontal) {
				if (event.ptMouse.y < m_rcThumb.top) {
					if (m_pOwner != NULL) m_pOwner->PageUp();
					else SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
				}
				else if (event.ptMouse.y > m_rcThumb.bottom){
					if (m_pOwner != NULL) m_pOwner->PageDown();
					else SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);
				}
			}
			else {
				if (event.ptMouse.x < m_rcThumb.left) {
					if (m_pOwner != NULL) m_pOwner->PageLeft();
					else SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
				}
				else if (event.ptMouse.x > m_rcThumb.right){
					if (m_pOwner != NULL) m_pOwner->PageRight();
					else SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);
				}
			}
		}

		ButtonDown(event);
		return;
	}
	else if (event.Type == kEventMouseButtonUp || event.Type == kEventPointUp) {
		m_nScrollRepeatDelay = 0;
		m_nLastScrollOffset = 0;

		m_weakFlagOwner.Cancel();

		if (IsMouseFocused()) {
			if (::PtInRect(&m_rcItem, event.ptMouse)) {
				m_uThumbState = kControlStateHot;
			}
			else {
				m_uThumbState = kControlStateNormal;
			}
		}
		else if (m_uButton1State == kControlStatePushed) {
			m_uButton1State = kControlStateNormal;
			Invalidate();
		}
		else if (m_uButton2State == kControlStatePushed) {
			m_uButton2State = kControlStateNormal;
			Invalidate();
		}

		ButtonUp(event);
		return;
	}
	else if (event.Type == kEventMouseEnter) {
		MouseEnter(event);
	}
	else if (event.Type == kEventMouseLeave) {
		MouseLeave(event);
	}
	else if (event.Type == kEventMouseMove || event.Type == kEventPointMove) {
		if (IsMouseFocused()) {
			if (!m_bHorizontal) {

				int vRange = m_rcItem.bottom - m_rcItem.top - m_rcThumb.bottom + m_rcThumb.top;
				if (m_bShowButton1) {
					vRange -= m_cxyFixed.cx;
				}
				if (m_bShowButton2) {
					vRange -= m_cxyFixed.cx;
				}

				if (vRange != 0)
					m_nLastScrollOffset = (event.ptMouse.y - m_ptLastMouse.y) * m_nRange / vRange;
			}
			else {

				int hRange = m_rcItem.right - m_rcItem.left - m_rcThumb.right + m_rcThumb.left;
				if (m_bShowButton1) {
					hRange -= m_cxyFixed.cy;
				}
				if (m_bShowButton2) {
					hRange -= m_cxyFixed.cy;
				}

				if (hRange != 0)
					m_nLastScrollOffset = (event.ptMouse.x - m_ptLastMouse.x) * m_nRange / hRange;
			}
		}

		return;
	}
	else if (event.Type == kEventInternalMenu)	{
		return;
	}
	else if (event.Type == kEventSetCursor) {
		if (m_cursorType == kCursorHand) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		else if (m_cursorType == kCursorArrow){
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			return;
		}
		else {
			ASSERT(FALSE);
		}
	}

	if (m_pOwner != NULL) m_pOwner->HandleMessageTemplate(event);
}

void ScrollBar::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("button1normalimage")) SetButton1StateImage(kControlStateNormal, strValue);
	else if (strName == _T("button1hotimage")) SetButton1StateImage(kControlStateHot, strValue);
	else if (strName == _T("button1pushedimage")) SetButton1StateImage(kControlStatePushed, strValue);
	else if (strName == _T("button1disabledimage")) SetButton1StateImage(kControlStateDisabled, strValue);
	else if (strName == _T("button2normalimage")) SetButton2StateImage(kControlStateNormal, strValue);
	else if (strName == _T("button2hotimage")) SetButton2StateImage(kControlStateHot, strValue);
	else if (strName == _T("button2pushedimage")) SetButton2StateImage(kControlStatePushed, strValue);
	else if (strName == _T("button2disabledimage")) SetButton2StateImage(kControlStateDisabled, strValue);
	else if (strName == _T("thumbnormalimage")) SetThumbStateImage(kControlStateNormal, strValue);
	else if (strName == _T("thumbhotimage")) SetThumbStateImage(kControlStateHot, strValue);
	else if (strName == _T("thumbpushedimage")) SetThumbStateImage(kControlStatePushed, strValue);
	else if (strName == _T("thumbdisabledimage")) SetThumbStateImage(kControlStateDisabled, strValue);
	else if (strName == _T("railnormalimage")) SetRailStateImage(kControlStateNormal, strValue);
	else if (strName == _T("railhotimage")) SetRailStateImage(kControlStateHot, strValue);
	else if (strName == _T("railpushedimage")) SetRailStateImage(kControlStatePushed, strValue);
	else if (strName == _T("raildisabledimage")) SetRailStateImage(kControlStateDisabled, strValue);
	else if (strName == _T("bknormalimage")) SetBkStateImage(kControlStateNormal, strValue);
	else if (strName == _T("bkhotimage")) SetBkStateImage(kControlStateHot, strValue);
	else if (strName == _T("bkpushedimage")) SetBkStateImage(kControlStatePushed, strValue);
	else if (strName == _T("bkdisabledimage")) SetBkStateImage(kControlStateDisabled, strValue);
	else if (strName == _T("hor")) SetHorizontal(strValue == _T("true"));
	else if (strName == _T("linesize")) SetLineSize(_ttoi(strValue.c_str()));
	else if (strName == _T("thumbminlength")) SetThumbMinLength(_ttoi(strValue.c_str()));
	else if (strName == _T("range")) SetScrollRange(_ttoi(strValue.c_str()));
	else if (strName == _T("value")) SetScrollPos(_ttoi(strValue.c_str()));
	else if (strName == _T("showbutton1")) SetShowButton1(strValue == _T("true"));
	else if (strName == _T("showbutton2")) SetShowButton2(strValue == _T("true"));
	else if (strName == _T("autohidescroll")) SetAutoHideScroll(strValue == _T("true"));
	else Control::SetAttribute(strName, strValue);
}

void ScrollBar::Paint(IRenderContext* pRender, const UiRect& rcPaint)
{
	if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;
	PaintBk(pRender);
	PaintButton1(pRender);
	PaintButton2(pRender);
	PaintThumb(pRender);
	PaintRail(pRender);
}

void ScrollBar::ClearImageCache()
{
	__super::ClearImageCache();
	m_bkStateImage.ClearCache();
	m_button1StateImage.ClearCache();
	m_button2StateImage.ClearCache();
	m_thumbStateImage.ClearCache();
	m_railStateImage.ClearCache();
}

bool ScrollBar::IsHorizontal()
{
	return m_bHorizontal;
}

void ScrollBar::SetHorizontal(bool bHorizontal)
{
	if( m_bHorizontal == bHorizontal ) return;

	m_bHorizontal = bHorizontal;
	if( m_bHorizontal ) {
		if( m_cxyFixed.cy == 0 ) {
			m_cxyFixed.cx = 0;
			m_cxyFixed.cy = DEFAULT_SCROLLBAR_SIZE;
		}
	}
	else {
		if( m_cxyFixed.cx == 0 ) {
			m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
			m_cxyFixed.cy = 0;
		}
	}

	if( m_pOwner != NULL )
		m_pOwner->Arrange();
	else 
		ArrangeAncestor();
}

int64_t ScrollBar::GetScrollRange() const
{
	return m_nRange;
}

void ScrollBar::SetScrollRange(int64_t nRange)
{
	if( m_nRange == nRange ) return;

	m_nRange = nRange;
	if( m_nRange < 0 ) m_nRange = 0;
	if( m_nScrollPos > m_nRange ) m_nScrollPos = m_nRange;

	if (m_nRange == 0) {
		SetVisible_(false);
	}
	else if (!m_bAutoHide && !IsVisible()) {
		SetVisible(true);
	}
	SetPos(m_rcItem);
}

int64_t ScrollBar::GetScrollPos() const
{
	return m_nScrollPos;
}

void ScrollBar::SetScrollPos(int64_t nPos)
{
	if( m_nScrollPos == nPos ) return;

	m_nScrollPos = nPos;
	if( m_nScrollPos < 0 ) m_nScrollPos = 0;
	if( m_nScrollPos > m_nRange ) m_nScrollPos = m_nRange;
	SetPos(m_rcItem);
}

int ScrollBar::GetLineSize() const
{
	return m_nLineSize;
}

void ScrollBar::SetLineSize(int nSize)
{
	DpiManager::GetInstance()->ScaleInt(nSize);
	m_nLineSize = nSize;
}

int ScrollBar::GetThumbMinLength() const
{
	return m_nThumbMinLength;
}

void ScrollBar::SetThumbMinLength(int nThumbMinLength)
{
	DpiManager::GetInstance()->ScaleInt(nThumbMinLength);
	m_nThumbMinLength = nThumbMinLength;
}

bool ScrollBar::IsShowButton1()
{
	return m_bShowButton1;
}

void ScrollBar::SetShowButton1(bool bShow)
{
	m_bShowButton1 = bShow;
	SetPos(m_rcItem);
}

std::wstring ScrollBar::GetButton1StateImage(ControlStateType stateType)
{
	return m_button1StateImage[stateType].imageAttribute.simageString;
}

void ScrollBar::SetButton1StateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_button1StateImage[stateType].SetImageString(pStrImage);
	Invalidate();
}

bool ScrollBar::IsShowButton2()
{
	return m_bShowButton2;
}

void ScrollBar::SetShowButton2(bool bShow)
{
	m_bShowButton2 = bShow;
	SetPos(m_rcItem);
}

std::wstring ScrollBar::GetButton2StateImage(ControlStateType stateType)
{
	return m_button2StateImage[stateType].imageAttribute.simageString;
}

void ScrollBar::SetButton2StateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_button2StateImage[stateType].SetImageString(pStrImage);
	Invalidate();
}

std::wstring ScrollBar::GetThumbStateImage(ControlStateType stateType)
{
	return m_thumbStateImage[stateType].imageAttribute.simageString;
}

void ScrollBar::SetThumbStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_thumbStateImage[stateType].SetImageString(pStrImage);
	Invalidate();
}

std::wstring ScrollBar::GetRailStateImage(ControlStateType stateType)
{
	return m_railStateImage[stateType].imageAttribute.simageString;
}

void ScrollBar::SetRailStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_railStateImage[stateType].SetImageString(pStrImage);
	Invalidate();
}

std::wstring ScrollBar::GetBkStateImage(ControlStateType stateType)
{
	return m_bkStateImage[stateType].imageAttribute.simageString;
}

void ScrollBar::SetBkStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_bkStateImage[stateType].SetImageString(pStrImage);
	Invalidate();
}

void ScrollBar::SetAutoHideScroll(bool hide)
{
	if (m_bAutoHide != hide) {
		m_bAutoHide = hide;
	}
}

void ScrollBar::ScrollTimeHandle()
{
	++m_nScrollRepeatDelay;
	if(m_uThumbState == kControlStatePushed) {
		if( !m_bHorizontal ) {
			if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CSize(m_pOwner->GetScrollPos().cx, \
				m_nLastScrollPos + m_nLastScrollOffset)); 
			else SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
		}
		else {
			if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CSize(m_nLastScrollPos + m_nLastScrollOffset, \
				m_pOwner->GetScrollPos().cy)); 
			else SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
		}
	}
	else if( m_uButton1State == kControlStatePushed ) {
		if( m_nScrollRepeatDelay <= 5 ) return;
		if( !m_bHorizontal ) {
			if( m_pOwner != NULL ) m_pOwner->LineUp(); 
			else SetScrollPos(m_nScrollPos - m_nLineSize);
		}
		else {
			if( m_pOwner != NULL ) m_pOwner->LineLeft(); 
			else SetScrollPos(m_nScrollPos - m_nLineSize);
		}
	}
	else if( m_uButton2State == kControlStatePushed ) {
		if( m_nScrollRepeatDelay <= 5 ) return;
		if( !m_bHorizontal ) {
			if( m_pOwner != NULL ) m_pOwner->LineDown(); 
			else SetScrollPos(m_nScrollPos + m_nLineSize);
		}
		else {
			if( m_pOwner != NULL ) m_pOwner->LineRight(); 
			else SetScrollPos(m_nScrollPos + m_nLineSize);
		}
	}
	else {
		if( m_nScrollRepeatDelay <= 5 ) return;
		POINT pt = { 0 };
		::GetCursorPos(&pt);
		::ScreenToClient(m_pWindow->GetHWND(), &pt);
		if( !m_bHorizontal ) {
			if( pt.y < m_rcThumb.top ) {
				if( m_pOwner != NULL ) m_pOwner->PageUp(); 
				else SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
			}
			else if ( pt.y > m_rcThumb.bottom ){
				if( m_pOwner != NULL ) m_pOwner->PageDown(); 
				else SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);                    
			}
		}
		else {
			if( pt.x < m_rcThumb.left ) {
				if( m_pOwner != NULL ) m_pOwner->PageLeft(); 
				else SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
			}
			else if ( pt.x > m_rcThumb.right ){
				if( m_pOwner != NULL ) m_pOwner->PageRight(); 
				else SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);                    
			}
		}
	}
	return;
}

void ScrollBar::PaintBk(IRenderContext* pRender)
{
	m_bkStateImage.PaintStatusImage(pRender, m_uButtonState);
}

void ScrollBar::PaintButton1(IRenderContext* pRender)
{
	if (!m_bShowButton1) return;

	m_sImageModify.clear();
	m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), m_rcButton1.left - m_rcItem.left, \
		m_rcButton1.top - m_rcItem.top, m_rcButton1.right - m_rcItem.left, m_rcButton1.bottom - m_rcItem.top);

	if (m_uButton1State == kControlStateDisabled) {
		if (!DrawImage(pRender, m_button1StateImage[kControlStateDisabled], m_sImageModify)) {

		}
		else return;
	}
	else if (m_uButton1State == kControlStatePushed) {
		if (!DrawImage(pRender, m_button1StateImage[kControlStatePushed], m_sImageModify)) {

		}
		else return;

		if (!DrawImage(pRender, m_button1StateImage[kControlStateHot], m_sImageModify)) {

		}
		else return;
	}
	else if (m_uButton1State == kControlStateHot || m_uThumbState == kControlStatePushed) {
		if (!DrawImage(pRender, m_button1StateImage[kControlStateHot], m_sImageModify)) {

		}
		else return;
	}

	if (!DrawImage(pRender, m_button1StateImage[kControlStateNormal], m_sImageModify)) {

	}
	else return;
}

void ScrollBar::PaintButton2(IRenderContext* pRender)
{
	if (!m_bShowButton2) return;

	m_sImageModify.clear();
	m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), m_rcButton2.left - m_rcItem.left, \
		m_rcButton2.top - m_rcItem.top, m_rcButton2.right - m_rcItem.left, m_rcButton2.bottom - m_rcItem.top);

	if (m_uButton2State == kControlStateDisabled) {
		if (!DrawImage(pRender, m_button2StateImage[kControlStateDisabled], m_sImageModify)) {

		}
		else return;
	}
	else if (m_uButton2State == kControlStatePushed) {
		if (!DrawImage(pRender, m_button2StateImage[kControlStatePushed], m_sImageModify)) {

		}
		else return;

		if (!DrawImage(pRender, m_button2StateImage[kControlStateHot], m_sImageModify)) {

		}
		else return;
	}
	else if (m_uButton2State == kControlStateHot || m_uThumbState == kControlStatePushed) {
		if (!DrawImage(pRender, m_button2StateImage[kControlStateHot], m_sImageModify)) {

		}
		else return;
	}

	if (!DrawImage(pRender, m_button2StateImage[kControlStateNormal], m_sImageModify)) {

	}
	else return;
}

void ScrollBar::PaintThumb(IRenderContext* pRender)
{
	if (m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0) return;

	m_sImageModify.clear();
	m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), m_rcThumb.left - m_rcItem.left, \
		m_rcThumb.top - m_rcItem.top, m_rcThumb.right - m_rcItem.left, m_rcThumb.bottom - m_rcItem.top);

	m_thumbStateImage.PaintStatusImage(pRender, m_uThumbState, m_sImageModify);
}

void ScrollBar::PaintRail(IRenderContext* pRender)
{
	if (m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0) return;

	m_sImageModify.clear();
	if (!m_bHorizontal) {
		m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), m_rcThumb.left - m_rcItem.left, \
			(m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top - m_cxyFixed.cx / 2, \
			m_rcThumb.right - m_rcItem.left, \
			(m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top + m_cxyFixed.cx - m_cxyFixed.cx / 2);
	}
	else {
		m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), \
			(m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left - m_cxyFixed.cy / 2, \
			m_rcThumb.top - m_rcItem.top, \
			(m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left + m_cxyFixed.cy - m_cxyFixed.cy / 2, \
			m_rcThumb.bottom - m_rcItem.top);
	}

	if (m_uThumbState == kControlStateDisabled) {
		if (!DrawImage(pRender, m_railStateImage[kControlStateDisabled], m_sImageModify)) {

		}
		else return;
	}
	else if (m_uThumbState == kControlStatePushed) {
		if (!DrawImage(pRender, m_railStateImage[kControlStatePushed], m_sImageModify)) {

		}
		else return;

		if (!DrawImage(pRender, m_railStateImage[kControlStateHot], m_sImageModify)) {

		}
		else return;
	}
	else if (m_uThumbState == kControlStateHot) {
		if (!DrawImage(pRender, m_railStateImage[kControlStateHot], m_sImageModify)) {

		}
		else return;
	}

	if (!DrawImage(pRender, m_railStateImage[kControlStateNormal], m_sImageModify)) {

	}
	else return;
}

}
