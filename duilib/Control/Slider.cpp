#include "StdAfx.h"

namespace ui
{

Slider::Slider() :
	m_szThumb(10, 10),
	m_uButtonState(kControlStateNormal),
	m_nStep(1),
	m_sImageModify(),
	m_thumbStateImage(),
	m_rcProgressBarPadding()
{
	m_uTextStyle = DT_SINGLELINE | DT_CENTER;
}

std::wstring Slider::GetType() const
{
	return DUI_CTR_SLIDER;
}

UiRect Slider::GetProgressPos()
{
	UiRect rc;
	if (m_bHorizontal) {
		rc.right = int((m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left - m_szThumb.cx) / (m_nMax - m_nMin) + m_szThumb.cx / 2 + 0.5);
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}
	else {
		rc.top = int((m_nMax - m_nValue) * (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy) / (m_nMax - m_nMin) + m_szThumb.cy / 2 + 0.5);
		rc.right = m_rcItem.right - m_rcItem.left;
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}

	return rc;
}

void Slider::HandleMessage(EventArgs& event)
{
	if (!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) {
		if (m_pParent != NULL) m_pParent->HandleMessageTemplate(event);
		else Progress::HandleMessage(event);
		return;
	}

	if (event.Type == kEventMouseButtonDown || event.Type == kEventInternalDoubleClick || event.Type == kEventPointDown) {
		if( IsEnabled() ) {
			CPoint newPtMouse = event.ptMouse;
			newPtMouse.Offset(GetScrollOffset());
			UiRect rcThumb = GetThumbRect();
			if (rcThumb.IsPointIn(newPtMouse)) {
				SetMouseFocused(true);
			}
		}
		return;
	}
	if (event.Type == kEventMouseButtonUp || event.Type == kEventPointUp) {
		if(IsMouseFocused()) {
			SetMouseFocused(false);
		}
		if (IsEnabled()) {
			if (m_bHorizontal) {
				if (event.ptMouse.x >= m_rcItem.right - m_szThumb.cx / 2) m_nValue = m_nMax;
				else if (event.ptMouse.x <= m_rcItem.left + m_szThumb.cx / 2) m_nValue = m_nMin;
				else m_nValue = m_nMin + double((m_nMax - m_nMin) * (event.ptMouse.x - m_rcItem.left - m_szThumb.cx / 2)) / (m_rcItem.right - m_rcItem.left - m_szThumb.cx);
			}
			else {
				if (event.ptMouse.y >= m_rcItem.bottom - m_szThumb.cy / 2) m_nValue = m_nMin;
				else if (event.ptMouse.y <= m_rcItem.top + m_szThumb.cy / 2) m_nValue = m_nMax;
				else m_nValue = m_nMin + double((m_nMax - m_nMin) * (m_rcItem.bottom - event.ptMouse.y - m_szThumb.cy / 2)) / (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy);
			}
			m_pWindow->SendNotify(this, kEventValueChange);
			Invalidate();
		}
		return;
	}
	if (event.Type == kEventInternalMenu) {
		return;
	}
	if (event.Type == kEventMouseScrollWheel) {
		int detaValue = event.wParam;
		if (detaValue > 0) {
			SetValue(GetValue() + GetChangeStep());
			m_pWindow->SendNotify(this, kEventValueChange);
			return;
		}
		else {
			SetValue(GetValue() - GetChangeStep());
			m_pWindow->SendNotify(this, kEventValueChange);
			return;
		}
	}
	if (event.Type == kEventMouseMove || event.Type == kEventPointMove) {
		if (IsMouseFocused()) {
			if (m_bHorizontal) {
				if (event.ptMouse.x >= m_rcItem.right - m_szThumb.cx / 2) m_nValue = m_nMax;
				else if (event.ptMouse.x <= m_rcItem.left + m_szThumb.cx / 2) m_nValue = m_nMin;
				else m_nValue = m_nMin + double((m_nMax - m_nMin) * (event.ptMouse.x - m_rcItem.left - m_szThumb.cx / 2)) / (m_rcItem.right - m_rcItem.left - m_szThumb.cx);
			}
			else {
				if (event.ptMouse.y >= m_rcItem.bottom - m_szThumb.cy / 2) m_nValue = m_nMin;
				else if (event.ptMouse.y <= m_rcItem.top + m_szThumb.cy / 2) m_nValue = m_nMax;
				else m_nValue = m_nMin + double((m_nMax - m_nMin) * (m_rcItem.bottom - event.ptMouse.y - m_szThumb.cy / 2)) / (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy);
			}
			m_pWindow->SendNotify(this, kEventValueChange);
			Invalidate();
		}
		return;
	}

	Progress::HandleMessage(event);
}

void Slider::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("step")) SetChangeStep(_ttoi(strValue.c_str()));
	else if (strName == _T("thumbnormalimage")) SetThumbStateImage(kControlStateNormal, strValue);
	else if (strName == _T("thumbhotimage")) SetThumbStateImage(kControlStateHot, strValue);
	else if (strName == _T("thumbpushedimage")) SetThumbStateImage(kControlStatePushed, strValue);
	else if (strName == _T("thumbdisabledimage")) SetThumbStateImage(kControlStateDisabled, strValue);
	else if (strName == _T("thumbsize")) {
		CSize szXY;
		LPTSTR pstr = NULL;
		szXY.cx = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
		szXY.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		SetThumbSize(szXY);
	}
	else if (strName == _T("progressbarpadding")) {
		UiRect rcPadding;
		LPTSTR pstr = NULL;
		rcPadding.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
		rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		SetProgressBarPadding(rcPadding);
	}
	else Progress::SetAttribute(strName, strValue);
}

void Slider::PaintBkColor(IRenderContext* pRender)
{
	m_rcItem.Deflate(m_rcProgressBarPadding);
	m_rcPaint.Deflate(m_rcProgressBarPadding);
	Control::PaintBkColor(pRender);
	m_rcPaint.Inflate(m_rcProgressBarPadding);
	m_rcItem.Inflate(m_rcProgressBarPadding);
}

void Slider::PaintStatusImage(IRenderContext* pRender)
{
	m_rcItem.Deflate(m_rcProgressBarPadding);
	Progress::PaintStatusImage(pRender);
	m_rcItem.Inflate(m_rcProgressBarPadding);

	UiRect rcThumb = GetThumbRect();
	rcThumb.left -= m_rcItem.left;
	rcThumb.top -= m_rcItem.top;
	rcThumb.right -= m_rcItem.left;
	rcThumb.bottom -= m_rcItem.top;

	if (IsMouseFocused()) {
		m_sImageModify.clear();
		m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
		if (!DrawImage(pRender, m_thumbStateImage[kControlStatePushed], m_sImageModify)) {

		}
		else return;
	}
	else if (m_uButtonState == kControlStateHot) {
		m_sImageModify.clear();
		m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
		if (!DrawImage(pRender, m_thumbStateImage[kControlStateHot], m_sImageModify)) {

		}
		else return;
	}

	m_sImageModify.clear();
	m_sImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
	if (!DrawImage(pRender, m_thumbStateImage[kControlStateNormal], m_sImageModify)) {

	}
	else return;
}

void Slider::ClearImageCache()
{
	__super::ClearImageCache();
	m_thumbStateImage.ClearCache();
}

int Slider::GetChangeStep()
{
	return m_nStep;
}

void Slider::SetChangeStep(int step)
{
	m_nStep = step;
}

void Slider::SetThumbSize(CSize szXY)
{
	DpiManager::GetInstance()->ScaleSize(szXY);
	m_szThumb = szXY;
}

UiRect Slider::GetThumbRect() const
{
	if( m_bHorizontal ) {
		int left = int(m_rcItem.left + (m_rcItem.right - m_rcItem.left - m_szThumb.cx) * (m_nValue - m_nMin) / (m_nMax - m_nMin));
		int top = (m_rcItem.bottom + m_rcItem.top - m_szThumb.cy) / 2;
		return UiRect(left, top, left + m_szThumb.cx, top + m_szThumb.cy); 
	}
	else {
		int left = (m_rcItem.right + m_rcItem.left - m_szThumb.cx) / 2;
		int top = int(m_rcItem.bottom - m_szThumb.cy - (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy) * (m_nValue - m_nMin) / (m_nMax - m_nMin));
		return UiRect(left, top, left + m_szThumb.cx, top + m_szThumb.cy); 
	}
}

std::wstring Slider::GetThumbStateImage(ControlStateType stateType)
{
	return m_thumbStateImage[stateType].imageAttribute.simageString;
}

void Slider::SetThumbStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_thumbStateImage[stateType].SetImageString(pStrImage);
	Invalidate();
}

UiRect Slider::GetProgressBarPadding() const
{
	return m_rcProgressBarPadding;
}

void Slider::SetProgressBarPadding(UiRect rc)
{
	DpiManager::GetInstance()->ScaleRect(rc);
	m_rcProgressBarPadding = rc;
	if (GetFixedWidth() == DUI_LENGTH_AUTO || GetFixedHeight() == DUI_LENGTH_AUTO) {
		ArrangeAncestor();
	}
	else {
		Invalidate();
	}
}

}
