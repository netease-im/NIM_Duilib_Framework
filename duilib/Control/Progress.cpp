#include "stdafx.h"
#include "Progress.h"

namespace ui
{

Progress::Progress() :
	m_bHorizontal(true),
	m_bStretchForeImage(true),
	m_nMax(100),
	m_nMin(0),
	m_nValue(0),
	m_sProgressColor(),
	m_progressImage(),
	m_sProgressImageModify(),
	m_bMarquee(false),
	m_nMarqueeWidth(10),
	m_nMarqueeStep(4),
	m_nMarqueeElapsed(50), // for 1s 25fps,will use 20fps default
	m_nMarqueePos(0),
  m_bReverse(false)
{
	m_uTextStyle = DT_SINGLELINE | DT_CENTER;
	SetFixedHeight(12);
}

std::wstring Progress::GetType() const
{
	return DUI_CTR_PROGRESS;
}

UIAControlProvider* Progress::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIAProgressProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

bool Progress::IsHorizontal()
{
	return m_bHorizontal;
}

void Progress::SetHorizontal(bool bHorizontal)
{
	if (m_bHorizontal == bHorizontal) return;

	m_bHorizontal = bHorizontal;
	Invalidate();
}

int Progress::GetMinValue() const
{
	return m_nMin;
}

void Progress::SetMinValue(int nMin)
{
	m_nMin = nMin;
	Invalidate();
}

int Progress::GetMaxValue() const
{
	return m_nMax;
}

void Progress::SetMaxValue(int nMax)
{
	m_nMax = nMax;
	Invalidate();
}

double Progress::GetValue() const
{
	return m_nValue;
}

void Progress::SetValue(double nValue)
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider != nullptr && UiaClientsAreListening()) {
		VARIANT vtOld = { 0 }, vtNew = { 0 };
		vtOld.vt = vtNew.vt = VT_R8;
		vtOld.dblVal = m_nValue;
		vtNew.dblVal = nValue;


		UiaRaiseAutomationPropertyChangedEvent(m_pUIAProvider, UIA_RangeValueValuePropertyId, vtOld, vtNew);
	}
#endif

	m_nValue = nValue;

	Invalidate();
}

bool Progress::IsStretchForeImage()
{
	return m_bStretchForeImage;
}

void Progress::SetStretchForeImage(bool bStretchForeImage /*= true*/)
{
	if (m_bStretchForeImage == bStretchForeImage) return;
	m_bStretchForeImage = bStretchForeImage;
	Invalidate();
}

std::wstring Progress::GetProgressImage() const
{
	return m_progressImage.imageAttribute.simageString;
}

void Progress::SetProgressImage(const std::wstring& strImage)
{
	m_progressImage.SetImageString(strImage);
	Invalidate();
}

std::wstring Progress::GetProgressColor() const
{
	return m_sProgressColor;
}

void Progress::SetProgressColor(const std::wstring& strProgressColor)
{
	ASSERT(this->GetWindowColor(strProgressColor) != 0);
	if (m_sProgressColor == strProgressColor) return;

	m_sProgressColor = strProgressColor;
	Invalidate();
}

void Progress::SetAttribute(const std::wstring& srName, const std::wstring& strValue)
{
  if (srName == _T("hor")) SetHorizontal(strValue == _T("true"));
  else if (srName == _T("min")) SetMinValue(_ttoi(strValue.c_str()));
  else if (srName == _T("max")) SetMaxValue(_ttoi(strValue.c_str()));
  else if (srName == _T("value")) SetValue(_ttoi(strValue.c_str()));
  else if (srName == _T("progressimage")) SetProgressImage(strValue);
  else if (srName == _T("isstretchfore")) SetStretchForeImage(strValue == _T("true"));
  else if (srName == _T("progresscolor")) {
    LPCTSTR pValue = strValue.c_str();
    while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
    SetProgressColor(pValue);
  }
  else if (srName == _T("marquee")) SetMarquee(strValue == _T("true"));
  else if (srName == _T("marqueewidth")) SetMarqueeWidth(_ttoi(strValue.c_str()));
  else if (srName == _T("marqueestep")) SetMarqueeStep(_ttoi(strValue.c_str()));
  else if (srName == _T("reverse")) SetReverse(strValue == _T("true"));
	else Label::SetAttribute(srName, strValue);
}

void Progress::PaintStatusImage(IRenderContext* pRender)
{
	if (m_bMarquee) {
		PaintMarquee(pRender);
		return;
	}

	if (m_nMax <= m_nMin) m_nMax = m_nMin + 1;
	if (m_nValue > m_nMax) m_nValue = m_nMax;
	if (m_nValue < m_nMin) m_nValue = m_nMin;

	UiRect rc = GetProgressPos();
	if (!m_sProgressColor.empty()) {
		DWORD dwProgressColor = this->GetWindowColor(m_sProgressColor);
		if (dwProgressColor != 0) {
			UiRect rcProgressColor = m_rcItem;
			if (m_bHorizontal) {
				rcProgressColor.right = rcProgressColor.left + rc.right;
			}
			else {
				rcProgressColor.top = rcProgressColor.top + rc.top;
			}
			pRender->DrawColor(rcProgressColor, dwProgressColor);
		}
	}

	if (!m_progressImage.imageAttribute.simageString.empty()) {
		m_sProgressImageModify.clear();
		if (m_bStretchForeImage)
			m_sProgressImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom);
		else {
      ui::UiRect m_rcSrc = rc;
      if (m_progressImage.imageCache) {
        if (m_rcSrc.right > m_progressImage.imageCache->nX) {
          m_rcSrc.right = m_progressImage.imageCache->nX;
        }
        if (m_rcSrc.bottom > m_progressImage.imageCache->nY) {
          m_rcSrc.bottom = m_progressImage.imageCache->nY;
        }
      }
      m_sProgressImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'")
        , rc.left, rc.top, rc.right, rc.bottom
        , m_rcSrc.left, m_rcSrc.top, m_rcSrc.right, m_rcSrc.bottom);
    }

		// 让corner的值不超过可绘制范围
		auto& corner = m_progressImage.imageAttribute.rcCorner;
		if (IsHorizontal()) {
			if (corner.left != 0 && corner.left >= rc.right) {
				m_sProgressImageModify += StringHelper::Printf(_T(" corner='%d,%d,%d,%d'"),
					rc.right,
					corner.top,
					0,
					corner.bottom);
			}
		}
		else {
			if (corner.top != 0 && corner.top >= rc.bottom) {
				m_sProgressImageModify += StringHelper::Printf(_T(" corner='%d,%d,%d,%d'"),
					corner.left,
					corner.bottom,
					corner.right,
					0);
			}
		}
		DrawImage(pRender, m_progressImage, m_sProgressImageModify);
	}
}

UiRect Progress::GetProgressPos()
{
	UiRect rc;
	if (m_bHorizontal) {
    if (m_bReverse) {
      rc.right = m_rcItem.GetWidth();
      rc.left = rc.right - static_cast<int>(std::floor(static_cast<double>((m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left)) / static_cast<double>(m_nMax - m_nMin)));
    } else {
      rc.right = static_cast<int>(std::ceil(static_cast<double>((m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left)) / static_cast<double>(m_nMax - m_nMin)));
    }

		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}
	else {
    if (m_bReverse) {
      rc.bottom = static_cast<int>(std::floor(static_cast<double>((m_nMax - m_nValue) * (m_rcItem.bottom - m_rcItem.top)) / static_cast<double>(m_nMax - m_nMin)));
    } else {
      rc.top = static_cast<int>(std::ceil(static_cast<double>((m_nMax - m_nValue) * (m_rcItem.bottom - m_rcItem.top)) / static_cast<double>(m_nMax - m_nMin)));
      rc.bottom = m_rcItem.bottom - m_rcItem.top;
    }

		rc.right = m_rcItem.right - m_rcItem.left;
	}

	return rc;
}

void Progress::ClearImageCache()
{
	__super::ClearImageCache();
	m_progressImage.ClearCache();
}

void Progress::Play() {
	if (!m_bMarquee) {
		m_timer.Cancel();
		return;
	}
	m_nMarqueePos = m_nMarqueePos + m_nMarqueeStep;

	ui::UiRect rc = m_rcItem;
	if (m_bHorizontal) {
		if (m_nMarqueePos > rc.right - rc.left)
			m_nMarqueePos = (m_nMarqueePos - (rc.right - rc.left)) - m_nMarqueeWidth;
	}
	else {
		if (m_nMarqueePos > rc.bottom - rc.top)
			m_nMarqueePos = (m_nMarqueePos - (rc.bottom - rc.top) - m_nMarqueeWidth);
	}

	Invalidate();
}

void Progress::PaintMarquee(IRenderContext* pRender) {
	if (!m_sProgressColor.empty()) {
		DWORD dwProgressColor = GlobalManager::GetTextColor(m_sProgressColor);
		if (dwProgressColor != 0) {
			UiRect rcProgressColor = m_rcItem;
			ui::UiRect rc = m_rcItem;
			if (m_bHorizontal) {
				rc.left = max(m_nMarqueePos, 0) + rc.left;
				rc.right = rc.left + (m_nMarqueePos >= 0 ? m_nMarqueeWidth : (m_nMarqueeWidth + m_nMarqueePos));
			}
			else {
				rc.top = max(m_nMarqueePos, 0) + rc.top;
				rc.bottom = rc.top + (m_nMarqueePos >= 0 ? m_nMarqueeWidth : (m_nMarqueeWidth + m_nMarqueePos));
			}
			DpiManager::GetInstance()->ScaleRect(rc);
			pRender->DrawColor(rc, dwProgressColor);
		}
	}
}

bool Progress::IsMarquee()
{
	return m_bMarquee;
}

void Progress::SetMarquee(bool bMarquee)
{
	if (m_bMarquee == bMarquee)
		return;

	m_bMarquee = bMarquee;
	m_nMarqueePos = 0;

	if (m_bMarquee) {
		auto playCallback = nbase::Bind(&Progress::Play, this);
		TimerManager::GetInstance()->AddCancelableTimer(m_timer.GetWeakFlag(), playCallback, m_nMarqueeElapsed, TimerManager::REPEAT_FOREVER);
	}
	else {
		m_timer.Cancel();
	}

	Invalidate();
}

int Progress::GetMarqueeWidth()
{
	return m_nMarqueeWidth;
}

void Progress::SetMarqueeWidth(int nMarqueeWidth)
{
	if (m_nMarqueeWidth == nMarqueeWidth)
		return;

	m_nMarqueeWidth = nMarqueeWidth;

	Invalidate();
}

int Progress::GetMarqueeStep()
{
	return m_nMarqueeStep;
}

void Progress::SetMarqueeStep(int nMarqueeStep)
{
	if (m_nMarqueeStep == nMarqueeStep)
		return;

	m_nMarqueeStep = nMarqueeStep;

	Invalidate();
}

int Progress::GetMarqueeElapsed()
{
	return m_nMarqueeElapsed;
}

void Progress::SetMarqueeElapsed(int nMarqueeElapsed)
{
	if (!IsMarquee() || m_nMarqueeElapsed == m_nMarqueeElapsed)
		return;

	m_nMarqueeElapsed = nMarqueeElapsed;
	m_timer.Cancel();

	auto playCallback = nbase::Bind(&Progress::Play, this);
	TimerManager::GetInstance()->AddCancelableTimer(m_timer.GetWeakFlag(), playCallback, m_nMarqueeElapsed, TimerManager::REPEAT_FOREVER);

	Invalidate();
}

void Progress::SetReverse(bool bReverse){
  m_bReverse = bReverse;
}

}
