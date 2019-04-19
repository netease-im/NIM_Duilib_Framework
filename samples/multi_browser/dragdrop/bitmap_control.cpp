#include "bitmap_control.h"

using namespace ui;

BitmapControl::BitmapControl()
{
	bitmap_ = NULL;
}

void BitmapControl::Paint(ui::IRenderContext* pRender, const ui::UiRect& rcPaint)
{
	if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;
	__super::Paint(pRender, rcPaint);

	if (NULL == bitmap_)
		return;

	HDC hCloneDC = ::CreateCompatibleDC(pRender->GetDC());
	HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, bitmap_);

	pRender->AlphaBlend(m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, hCloneDC,
		0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);

	::SelectObject(hCloneDC, hOldBitmap);
	::DeleteDC(hCloneDC);
}

void BitmapControl::SetBitmapImage(HBITMAP bitmap)
{
	bitmap_ = bitmap;
}
