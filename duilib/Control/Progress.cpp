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
	m_sProgressImageModify()
{
	m_uTextStyle = DT_SINGLELINE | DT_CENTER;
	SetFixedHeight(12);
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
	else Label::SetAttribute(srName, strValue);
}

void Progress::PaintStatusImage(IRenderContext* pRender)
{
	if (m_nMax <= m_nMin) m_nMax = m_nMin + 1;
	if (m_nValue > m_nMax) m_nValue = m_nMax;
	if (m_nValue < m_nMin) m_nValue = m_nMin;

	UiRect rc = GetProgressPos();
	if (!m_sProgressColor.empty()) {
		DWORD dwProgressColor = GlobalManager::GetTextColor(m_sProgressColor);
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
		else
			m_sProgressImageModify = StringHelper::Printf(_T("destscale='false' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'")
				, rc.left, rc.top, rc.right, rc.bottom
				, rc.left, rc.top, rc.right, rc.bottom);

		DrawImage(pRender, m_progressImage, m_sProgressImageModify);
	}
}

void Progress::ClearImageCache()
{
	__super::ClearImageCache();
	m_progressImage.ClearCache();
}

bool Progress::IsHorizontal()
{
	return m_bHorizontal;
}

void Progress::SetHorizontal(bool bHorizontal)
{
	if( m_bHorizontal == bHorizontal ) return;

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
	ASSERT(GlobalManager::GetTextColor(strProgressColor) != 0);
	if( m_sProgressColor == strProgressColor ) return;

	m_sProgressColor = strProgressColor;
	Invalidate();
}

UiRect Progress::GetProgressPos()
{
	UiRect rc;
	if (m_bHorizontal) {
		rc.right = int((m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin));
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}
	else {
		rc.top = int((m_nMax - m_nValue) * (m_rcItem.bottom - m_rcItem.top) / (m_nMax - m_nMin));
		rc.right = m_rcItem.right - m_rcItem.left;
		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	}

	return rc;
}

}
