#include "framework.h"

namespace dui {
namespace render {

static inline void DrawFunction(HDC hDC, bool bTransparent, UiRect rcDest, HDC hdcSrc, UiRect rcSrc, bool bAlphaChannel, int uFade)
{
	if (bTransparent || bAlphaChannel || uFade < 255
		|| (rcSrc.GetWidth() == rcDest.GetWidth() && rcSrc.GetHeight() == rcDest.GetHeight())) {
		BLENDFUNCTION ftn = { AC_SRC_OVER, 0, uFade, AC_SRC_ALPHA };
		::AlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.GetWidth(), rcDest.GetHeight(),
			hdcSrc, rcSrc.left, rcSrc.top, rcSrc.GetWidth(), rcSrc.GetHeight(), ftn);
	}
	else {
		::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.GetWidth(), rcDest.GetHeight(),
			hdcSrc, rcSrc.left, rcSrc.top, rcSrc.GetWidth(), rcSrc.GetHeight(), SRCCOPY);
	}
}

RenderContext::RenderContext()
	: m_hDC(NULL)
	, m_hOldBitmap(NULL)
	, m_bTransparent(false)
	, clip_(new common::RefCountedObject<Clicp>())
	, bitmap_(new common::RefCountedObject<Bitmap>())
{
	HDC hdc = ::GetDC(NULL);
	m_hDC = ::CreateCompatibleDC(hdc);
	::ReleaseDC(NULL, hdc);
}

RenderContext::~RenderContext()
{
	if (m_hOldBitmap != NULL)
	{
		::SelectObject(m_hDC, m_hOldBitmap);
		m_hOldBitmap = NULL;
	}

	if (m_hDC != NULL)
	{
		::DeleteDC(m_hDC);
		m_hDC = NULL;
	}
}

HDC RenderContext::GetDC()
{
	return m_hDC;
}

bool RenderContext::Resize(int width, int height, bool flipBItmap)
{
	if (width <= 0)
		width = 1;
	if (height <= 0)
		height = 1;

	if (bitmap_->GetWidth() == width && bitmap_->GetHeight() == height)
		return false;

	if (m_hOldBitmap != NULL)
	{
		::SelectObject(m_hDC, m_hOldBitmap);
	}

	bool ret = bitmap_->Init(m_hDC, width, height, flipBItmap);
	m_hOldBitmap = (HBITMAP)::SelectObject(m_hDC, bitmap_->GetBitmap());
	return ret;
}

void RenderContext::Clear()
{
	bitmap_->Clear();
}

HBITMAP RenderContext::DetachBitmap()
{
	if (m_hOldBitmap == NULL)
		return NULL;

	::SelectObject(m_hDC, m_hOldBitmap);
	return bitmap_->DetachBitmap();
}

BYTE* RenderContext::GetBits()
{
	return bitmap_->GetBits();
}

HBITMAP RenderContext::GetBitmap()
{
	return bitmap_->GetBitmap();
}

int RenderContext::GetWidth()
{
	return bitmap_->GetWidth();
}

int RenderContext::GetHeight()
{
	return bitmap_->GetHeight();
}

void RenderContext::ClearAlpha(const UiRect& rcDirty, int alpha)
{
	bitmap_->ClearAlpha(rcDirty, alpha);
}

void RenderContext::RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha)
{
	bitmap_->RestoreAlpha(rcDirty, rcShadowPadding, alpha);
}

void RenderContext::RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding /*= UiRect()*/)
{
	bitmap_->RestoreAlpha(rcDirty, rcShadowPadding);
}

bool RenderContext::IsRenderTransparent() const
{
	return m_bTransparent;
}

bool RenderContext::SetRenderTransparent(bool bTransparent)
{
	bool oldValue = m_bTransparent;
	m_bTransparent = bTransparent;
	return oldValue;
}

void RenderContext::Save()
{
	m_saveDC = SaveDC(m_hDC);
}

void RenderContext::Restore()
{
	RestoreDC(m_hDC, m_saveDC);
}

CPoint RenderContext::OffsetWindowOrg(CPoint ptOffset)
{
	CPoint ptOldWindowOrg;
	GetWindowOrgEx(m_hDC, &ptOldWindowOrg);
	ptOffset.Offset(ptOldWindowOrg.x, ptOldWindowOrg.y);
	::SetWindowOrgEx(m_hDC, ptOffset.x, ptOffset.y, NULL);
	return ptOldWindowOrg;
}

CPoint RenderContext::SetWindowOrg(CPoint ptOffset)
{
	CPoint ptOldWindowOrg;
	GetWindowOrgEx(m_hDC, &ptOldWindowOrg);
	::SetWindowOrgEx(m_hDC, ptOffset.x, ptOffset.y, NULL);
	return ptOldWindowOrg;
}

CPoint RenderContext::GetWindowOrg() const
{
	CPoint ptWindowOrg;
	GetWindowOrgEx(m_hDC, &ptWindowOrg);
	return ptWindowOrg;
}

void RenderContext::SetClip(const UiRect& rc)
{
	clip_->CreateClip(m_hDC, rc);
}

void RenderContext::SetRoundClip(const UiRect& rc, int width, int height)
{
	clip_->CreateRoundClip(m_hDC, rc, width, height);
}

void RenderContext::ClearClip()
{
	clip_->ClearClip(m_hDC);
}

bool RenderContext::BitBlt(int x, int y, int cx, int cy, HDC hdcSrc, int xSrc /*= 0*/, int yScr /*= 0*/, DWORD rop /*= SRCCOPY*/)
{
	return ::BitBlt(m_hDC, x, y, cx, cy, hdcSrc, xSrc, yScr, rop);
}

bool RenderContext::StretchBlt(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, DWORD rop /*= SRCCOPY*/)
{
	int stretchBltMode = ::SetStretchBltMode(m_hDC, HALFTONE);
	bool ret = (TRUE == ::StretchBlt(m_hDC, xDest, yDest, widthDest, heightDest,
		hdcSrc, xSrc, yScr, widthSrc, heightSrc, rop));
	::SetStretchBltMode(m_hDC, stretchBltMode);
	return ret;
}

bool RenderContext::AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, BYTE uFade /*= 255*/)
{
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, uFade, AC_SRC_ALPHA };
	return (TRUE == ::AlphaBlend(m_hDC, xDest, yDest, widthDest, heightDest, hdcSrc, xSrc, yScr, widthSrc, heightSrc, bf));
}

void RenderContext::DrawImage(const UiRect& rcPaint,
	const CSize& scrollPos,
	HBITMAP hBitmap,
	bool bAlphaChannel,
	const UiRect& rcImageDest,
	const UiRect& rcImageSource,
	UiRect rcCorners,
	UiRect rcDestCorners,
	bool bBitmapDpiScale,
	BYTE uFade /*= 255*/,
	bool xtiled /*= false*/,
	bool ytiled /*= false*/,
	bool fullxtiled /*= true*/,
	bool fullytiled /*= true*/,
	int nTiledMargin /*=0*/)
{
	UiRect rcTestTemp;
	if (!::IntersectRect(&rcTestTemp, &rcImageDest, &rcPaint)) return;

	if (hBitmap == NULL) return;

	HDC hCloneDC = ::CreateCompatibleDC(m_hDC);
	HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, hBitmap);
	int stretchBltMode = ::SetStretchBltMode(m_hDC, HALFTONE);

	UiRect rcTemp;
	UiRect rcSource;
	UiRect rcDest;
	// ���Դλͼ�Ѿ�����DPI���Ź�����ô��Ӧ��cornerҲ����һ��
	if (bBitmapDpiScale)
		rcCorners = rcDestCorners;

	// middle
	rcDest.left = rcImageDest.left + rcDestCorners.left;
	rcDest.top = rcImageDest.top + rcDestCorners.top;
	rcDest.right = rcImageDest.right - rcDestCorners.right;
	rcDest.bottom = rcImageDest.bottom - rcDestCorners.bottom;
	rcSource.left = rcImageSource.left + rcCorners.left;
	rcSource.top = rcImageSource.top + rcCorners.top;
	rcSource.right = rcImageSource.right - rcCorners.right;
	rcSource.bottom = rcImageSource.bottom - rcCorners.bottom;
	if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
		if (!xtiled && !ytiled) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
		else if (xtiled && ytiled) {
			LONG lWidth = rcImageSource.right - rcImageSource.left - rcCorners.left - rcCorners.right;
			LONG lHeight = rcImageSource.bottom - rcImageSource.top - rcCorners.top - rcCorners.bottom;
			int iTimesX = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
			int iTimesY = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
			for (int j = 0; j < iTimesY; ++j) {
				LONG lDestTop = rcDest.top + lHeight * j;
				LONG lDestBottom = rcDest.top + lHeight * (j + 1);
				LONG lDrawHeight = lHeight;
				if (lDestBottom > rcDest.bottom) {
					lDrawHeight -= lDestBottom - rcDest.bottom;
					lDestBottom = rcDest.bottom;
				}
				for (int i = 0; i < iTimesX; ++i) {
					LONG lDestLeft = rcDest.left + lWidth * i;
					LONG lDestRight = rcDest.left + lWidth * (i + 1);
					LONG lDrawWidth = lWidth;
					if (lDestRight > rcDest.right) {
						lDrawWidth -= lDestRight - rcDest.right;
						lDestRight = rcDest.right;
					}
					rcDest.left = rcDest.left + lWidth * i;
					rcDest.top = rcDest.top + lHeight * j;
					rcDest.right = rcDest.left + lDestRight - lDestLeft;
					rcDest.bottom = rcDest.top + lDestBottom - lDestTop;
					rcSource.left = rcImageSource.left + rcCorners.left;
					rcSource.top = rcImageSource.top + rcCorners.top;
					rcSource.right = rcSource.left + lDrawWidth;
					rcSource.bottom = rcSource.top + lDrawHeight;
					DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
				}
			}
		}
		else if (xtiled) { // supp
			LONG lWidth = rcImageSource.right - rcImageSource.left - rcCorners.left - rcCorners.right;
			int iTimes = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
			for (int i = 0; i < iTimes; ++i) {
				LONG lDestLeft = rcDest.left + lWidth * i;
				LONG lDestRight = rcDest.left + lWidth * (i + 1);
				LONG lDrawWidth = lWidth;
				if (lDestRight > rcDest.right) {
					lDrawWidth -= lDestRight - rcDest.right;
					lDestRight = rcDest.right;
				}
				rcDest.left = lDestLeft;
				rcDest.top = rcDest.top;
				rcDest.right = lDestRight;
				rcDest.bottom = rcDest.top + rcDest.bottom;
				rcSource.left = rcImageSource.left + rcCorners.left;
				rcSource.top = rcImageSource.top + rcCorners.top;
				rcSource.right = rcSource.left + lDrawWidth;
				rcSource.bottom = rcImageSource.bottom - rcCorners.bottom;
				DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
			}
		}
		else { // ytiled
			rcDest.left -= scrollPos.cx;
			rcDest.top -= scrollPos.cy;

			LONG lHeight = rcImageSource.bottom - rcImageSource.top - rcCorners.top - rcCorners.bottom;
			int iTimes = 0;
			if (fullytiled)
				iTimes = (rcDest.bottom - rcDest.top + lHeight + nTiledMargin - 1) / (lHeight + nTiledMargin);
			else
				iTimes = (rcDest.bottom - rcDest.top + nTiledMargin) / (lHeight + nTiledMargin);

			UiRect rcDestTemp;
			rcDestTemp.left = rcDest.left;
			rcDestTemp.right = rcDest.left + rcSource.GetWidth();

			for (int i = 0; i < iTimes; ++i) {
				LONG lDestTop = rcDest.top + lHeight * i + i * nTiledMargin;
				LONG lDestBottom = lDestTop + lHeight;
				LONG lDrawHeight = lHeight;
				if (lDestBottom > rcDest.bottom) {
					lDrawHeight -= lDestBottom - rcDest.bottom;
					lDestBottom = rcDest.bottom;
				}

				rcSource.left = rcImageSource.left + rcCorners.left;
				rcSource.top = rcImageSource.top + rcCorners.top;
				rcSource.right = rcImageSource.right - rcCorners.right;
				rcSource.bottom = rcSource.top + lDrawHeight;

				rcDestTemp.top = lDestTop;
				rcDestTemp.bottom = lDestBottom;

				DrawFunction(m_hDC, m_bTransparent, rcDestTemp, hCloneDC, rcSource, bAlphaChannel, uFade);
			}
		}
	}

	// left-top
	if (rcCorners.left > 0 && rcCorners.top > 0) {
		rcDest.left = rcImageDest.left;
		rcDest.top = rcImageDest.top;
		rcDest.right = rcImageDest.left + rcDestCorners.left;
		rcDest.bottom = rcImageDest.top + rcDestCorners.top;
		rcSource.left = rcImageSource.left;
		rcSource.top = rcImageSource.top;
		rcSource.right = rcImageSource.left + rcCorners.left;
		rcSource.bottom = rcImageSource.top + rcCorners.top;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}
	// top
	if (rcCorners.top > 0) {
		rcDest.left = rcImageDest.left + rcDestCorners.left;
		rcDest.top = rcImageDest.top;
		rcDest.right = rcImageDest.right - rcDestCorners.right;
		rcDest.bottom = rcImageDest.top + rcDestCorners.top;
		rcSource.left = rcImageSource.left + rcCorners.left;
		rcSource.top = rcImageSource.top;
		rcSource.right = rcImageSource.right - rcCorners.right;
		rcSource.bottom = rcImageSource.top + rcCorners.top;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}
	// right-top
	if (rcCorners.right > 0 && rcCorners.top > 0) {
		rcDest.left = rcImageDest.right - rcDestCorners.right;
		rcDest.top = rcImageDest.top;
		rcDest.right = rcImageDest.right;
		rcDest.bottom = rcImageDest.top + rcDestCorners.top;
		rcSource.left = rcImageSource.right - rcCorners.right;
		rcSource.top = rcImageSource.top;
		rcSource.right = rcImageSource.right;
		rcSource.bottom = rcImageSource.top + rcCorners.top;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}
	// left
	if (rcCorners.left > 0) {
		rcDest.left = rcImageDest.left;
		rcDest.top = rcImageDest.top + rcDestCorners.top;
		rcDest.right = rcImageDest.left + rcDestCorners.left;
		rcDest.bottom = rcImageDest.bottom - rcDestCorners.bottom;
		rcSource.left = rcImageSource.left;
		rcSource.top = rcImageSource.top + rcCorners.top;
		rcSource.right = rcImageSource.left + rcCorners.left;
		rcSource.bottom = rcImageSource.bottom - rcCorners.bottom;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}
	// right
	if (rcCorners.right > 0) {
		rcDest.left = rcImageDest.right - rcDestCorners.right;
		rcDest.top = rcImageDest.top + rcDestCorners.top;
		rcDest.right = rcImageDest.right;
		rcDest.bottom = rcImageDest.bottom - rcDestCorners.bottom;
		rcSource.left = rcImageSource.right - rcCorners.right;
		rcSource.top = rcImageSource.top + rcCorners.top;
		rcSource.right = rcImageSource.right;
		rcSource.bottom = rcImageSource.bottom - rcCorners.bottom;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}
	// left-bottom
	if (rcCorners.left > 0 && rcCorners.bottom > 0) {
		rcDest.left = rcImageDest.left;
		rcDest.top = rcImageDest.bottom - rcDestCorners.bottom;
		rcDest.right = rcImageDest.left + rcDestCorners.left;
		rcDest.bottom = rcImageDest.bottom;
		rcSource.left = rcImageSource.left;
		rcSource.top = rcImageSource.bottom - rcCorners.bottom;
		rcSource.right = rcImageSource.left + rcCorners.left;
		rcSource.bottom = rcImageSource.bottom;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}
	// bottom
	if (rcCorners.bottom > 0) {
		rcDest.left = rcImageDest.left + rcDestCorners.left;
		rcDest.top = rcImageDest.bottom - rcDestCorners.bottom;
		rcDest.right = rcImageDest.right - rcDestCorners.right;
		rcDest.bottom = rcImageDest.bottom;
		rcSource.left = rcImageSource.left + rcCorners.left;
		rcSource.top = rcImageSource.bottom - rcCorners.bottom;
		rcSource.right = rcImageSource.right - rcCorners.right;
		rcSource.bottom = rcImageSource.bottom;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}
	// right-bottom
	if (rcCorners.right > 0 && rcCorners.bottom > 0) {
		rcDest.left = rcImageDest.right - rcDestCorners.right;
		rcDest.top = rcImageDest.bottom - rcDestCorners.bottom;
		rcDest.right = rcImageDest.right;
		rcDest.bottom = rcImageDest.bottom;
		rcSource.left = rcImageSource.right - rcCorners.right;
		rcSource.top = rcImageSource.bottom - rcCorners.bottom;
		rcSource.right = rcImageSource.right;
		rcSource.bottom = rcImageSource.bottom;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
			DrawFunction(m_hDC, m_bTransparent, rcDest, hCloneDC, rcSource, bAlphaChannel, uFade);
		}
	}

	::SetStretchBltMode(m_hDC, stretchBltMode);
	::SelectObject(hCloneDC, hOldBitmap);
	::DeleteDC(hCloneDC);
}

void RenderContext::DrawColor(const UiRect& rc, DWORD dwColor, BYTE uFade)
{
	DWORD dwNewColor = dwColor;
	if (uFade < 255) {
		int alpha = dwColor >> 24;
		dwNewColor = dwColor & 0xffffff;
		alpha *= double(uFade) / 255;
		dwNewColor += alpha << 24;
	}

	Gdiplus::Graphics graphics(m_hDC);
	Gdiplus::Color color(dwNewColor);
	Gdiplus::SolidBrush brush(color);
	Gdiplus::RectF rcFill(rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
	graphics.FillRectangle(&brush, rcFill);
}

void RenderContext::DrawLine(const UiRect& rc, int nSize, DWORD dwPenColor)
{
	Gdiplus::Graphics graphics(m_hDC);
	Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
	graphics.DrawLine(&pen, Gdiplus::Point(rc.left, rc.top), Gdiplus::Point(rc.right, rc.bottom));
}

void RenderContext::DrawLine(common::dui_refptr<IPen> pen, int x1, int y1, int x2, int y2)
{
	if (!pen) return;
	Gdiplus::Graphics graphics(m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.DrawLine(reinterpret_cast<Pen*>(pen.get())->GetPen(), x1, y1, x2, y2);
}

void RenderContext::DrawBezier(common::dui_refptr<IPen> pen, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	if (!pen) return;
	Gdiplus::Graphics graphics(m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.DrawBezier(reinterpret_cast<Pen*>(pen.get())->GetPen(), x1, y1, x2, y2, x3, y3, x4, y4);
}

void RenderContext::DrawRect(const UiRect& rc, int nSize, DWORD dwPenColor)
{
	Gdiplus::Graphics graphics(m_hDC);
	Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
	graphics.DrawRectangle(&pen, rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
}


void RenderContext::DrawRoundRect(const UiRect& rc, const CSize& roundSize, int nSize, DWORD dwPenColor)
{
	Gdiplus::Graphics graphics(m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);

	// �ü����������������±߿���ʱ��ȫ����������һ������
	// UiRect rcInflate = rc;
	// rcInflate.Inflate({ -1, -1, -1, -1 });

	Gdiplus::GraphicsPath pPath;
	pPath.AddArc((int)rc.left, (int)rc.top, (int)roundSize.cx, (int)roundSize.cy, 180, 90);
	pPath.AddLine((int)rc.left + (int)roundSize.cx, (int)rc.top, (int)rc.right - (int)roundSize.cx, (int)rc.top);
	pPath.AddArc((int)rc.right - (int)roundSize.cx, (int)rc.top, (int)roundSize.cx, (int)roundSize.cy, 270, 90);
	pPath.AddLine((int)rc.right, (int)rc.top + (int)roundSize.cy, (int)rc.right, (int)rc.bottom - (int)roundSize.cy);
	pPath.AddArc((int)rc.right - (int)roundSize.cx, (int)rc.bottom - (int)roundSize.cy, (int)roundSize.cx, (int)roundSize.cy, 0, 90);
	pPath.AddLine((int)rc.right - (int)roundSize.cx, (int)rc.bottom, (int)rc.left + (int)roundSize.cx, (int)rc.bottom);
	pPath.AddArc((int)rc.left, (int)rc.bottom - (int)roundSize.cy, (int)roundSize.cx, (int)roundSize.cy, 90, 90);
	pPath.AddLine((int)rc.left, (int)rc.bottom - (int)roundSize.cy, (int)rc.left, (int)rc.top + (int)roundSize.cy);
	pPath.CloseFigure();

	graphics.DrawPath(&pen, &pPath);
}

void RenderContext::DrawText(const UiRect& rc,
	const std::wstring& strText,
	DWORD dwTextColor,
	const HFONT hfont,
	UINT uStyle,
	BYTE uFade /*= 255*/,
	bool bLineLimit /*= false*/,
	bool bFillPath /*= false*/)
{
	if (strText.empty()) return;

	Gdiplus::InstalledFontCollection installedFontCollection;

	// How many font families are installed?
	int count = installedFontCollection.GetFamilyCount();
	if (count == 0) {

		::SetBkMode(m_hDC, TRANSPARENT);
		::SetTextColor(m_hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor),
			GetRValue(dwTextColor)));
		HFONT hOldFont = (HFONT)::SelectObject(m_hDC, hfont);

		RECT rcGdi = { rc.left,rc.top,rc.right,rc.bottom };
		::DrawText(m_hDC, strText.c_str(), -1, &rcGdi, uStyle);
		::SelectObject(m_hDC, hOldFont);

		return;
	}

	Gdiplus::Graphics graphics(m_hDC);
	Gdiplus::Font font(m_hDC, hfont);

	Gdiplus::RectF rcPaint((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top, (Gdiplus::REAL)(rc.right - rc.left), (Gdiplus::REAL)(rc.bottom - rc.top));
	int alpha = dwTextColor >> 24;
	uFade *= double(alpha) / 255;
	if (uFade == 255) {
		uFade = 254;
	}
	Gdiplus::SolidBrush tBrush(Gdiplus::Color(uFade, GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

	Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();
	if ((uStyle & DT_END_ELLIPSIS) != 0) {
		stringFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
	}

	int formatFlags = 0;
	if ((uStyle & DT_NOCLIP) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoClip;
	}
	if ((uStyle & DT_SINGLELINE) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
	}
	if (bLineLimit) {
		formatFlags |= Gdiplus::StringFormatFlagsLineLimit;
	}
	stringFormat.SetFormatFlags(formatFlags);

	if ((uStyle & DT_LEFT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_CENTER) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_RIGHT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}

	if ((uStyle & DT_TOP) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_VCENTER) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_BOTTOM) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}

	// try to fix font issue
#if 1
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintSystemDefault);
#else
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
#endif
	if (bFillPath) {
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::GraphicsPath path;
		Gdiplus::FontFamily font_family;
		font.GetFamily(&font_family);
		path.AddString(strText.c_str(), (int)strText.length(), &font_family, font.GetStyle(),
			font.GetSize(), rcPaint, &stringFormat);
		graphics.FillPath(&tBrush, &path);
		return;
	}
	graphics.DrawString(strText.c_str(), (int)strText.length(), &font, rcPaint, &stringFormat, &tBrush);
}

void RenderContext::DrawEllipse(const UiRect& rc, int nSize, DWORD dwColor)
{
	Gdiplus::Graphics graphics(m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gdiplus::Pen pen(dwColor, nSize);
	graphics.DrawEllipse(&pen, rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
}

void RenderContext::FillEllipse(const UiRect& rc, DWORD dwColor)
{
	Gdiplus::Graphics graphics(m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gdiplus::SolidBrush brush(dwColor);
	graphics.FillEllipse(&brush, rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
}

void RenderContext::DrawPath(common::dui_refptr<IPath> path, common::dui_refptr<IPen> pen)
{
	if (!path || !pen)
		return;

	Gdiplus::Graphics graphics(m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.DrawPath(reinterpret_cast<Pen*>(pen.get())->GetPen(), reinterpret_cast<Path*>(path.get())->GetPath());
}

void RenderContext::FillPath(common::dui_refptr<IPath> path, common::dui_refptr<IBrush> brush)
{
	if (!path || !brush)
		return;

	Gdiplus::Graphics graphics(m_hDC);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.FillPath(reinterpret_cast<Brush*>(brush.get())->GetBrush(), reinterpret_cast<Path*>(path.get())->GetPath());
}

void RenderContext::DrawBoxShadow(const UiRect& rc,
	const CSize& roundSize,
	const CPoint& cpOffset,
	int nBlurRadius,
	int nBlurSize,
	int nSpreadSize,
	DWORD dwColor,
	bool bExclude)
{
#define USE_BLUR 1
#define USE_COLOR_MATRIX 0

	UiRect destRc = rc;
	destRc.Offset(cpOffset);
	destRc.left -= nBlurSize;
	destRc.top -= nBlurSize;
	destRc.right += nBlurSize;
	destRc.bottom += nBlurSize;

	Gdiplus::REAL scale = 1.0f;

	Gdiplus::RectF srcRc(0.0f, 0.0f, destRc.GetWidth() / scale, destRc.GetHeight() / scale);
	Gdiplus::RectF excludeRc(abs(nBlurSize) / scale,
		abs(nBlurSize) / scale,
		srcRc.Width - abs(nBlurSize) * 2.0f / scale,
		srcRc.Height - abs(nBlurSize) * 2.0f / scale);

	Gdiplus::GraphicsPath shadowPath;
	Gdiplus::GraphicsPath excludePath;

	auto add_roundcorner_path = [](Gdiplus::GraphicsPath& path, const Gdiplus::RectF& rect, const CSize& roundSize) {
		path.AddArc(rect.GetLeft(), rect.GetTop(), (Gdiplus::REAL)roundSize.cx, (Gdiplus::REAL)roundSize.cy, 180, 90);
		path.AddLine(rect.GetLeft() + (Gdiplus::REAL)roundSize.cx, rect.GetTop(), rect.GetRight() - (Gdiplus::REAL)roundSize.cx, rect.GetTop());
		path.AddArc(rect.GetRight() - (Gdiplus::REAL)roundSize.cx, rect.GetTop(), (Gdiplus::REAL)roundSize.cx, (Gdiplus::REAL)roundSize.cy, 270, 90);
		path.AddLine(rect.GetRight(), rect.GetTop() + (Gdiplus::REAL)roundSize.cy, rect.GetRight(), rect.GetBottom() - (Gdiplus::REAL)roundSize.cy);
		path.AddArc(rect.GetRight() - (Gdiplus::REAL)roundSize.cx, rect.GetBottom() - (Gdiplus::REAL)roundSize.cy, (Gdiplus::REAL)roundSize.cx, (Gdiplus::REAL)roundSize.cy, 0, 90);
		path.AddLine(rect.GetRight() - (Gdiplus::REAL)roundSize.cx, rect.GetBottom(), rect.GetLeft() + (Gdiplus::REAL)roundSize.cx, rect.GetBottom());
		path.AddArc(rect.GetLeft(), rect.GetBottom() - (Gdiplus::REAL)roundSize.cy, (Gdiplus::REAL)roundSize.cx, (Gdiplus::REAL)roundSize.cy, 90, 90);
		path.AddLine(rect.GetLeft(), rect.GetBottom() - (Gdiplus::REAL)roundSize.cy, rect.GetLeft(), rect.GetTop() + (Gdiplus::REAL)roundSize.cy);
		path.CloseFigure();
	};

	add_roundcorner_path(shadowPath, srcRc, roundSize);

	if (bExclude)
		add_roundcorner_path(excludePath, excludeRc, roundSize);

	Gdiplus::PathGradientBrush gradientPathBrush(&shadowPath);
	gradientPathBrush.SetWrapMode(Gdiplus::WrapMode::WrapModeClamp);

	Gdiplus::Color colors[] = { Gdiplus::Color::Transparent,Gdiplus::Color(dwColor) ,Gdiplus::Color(dwColor) };
	Gdiplus::REAL pos[] = { 0.0f,0.7f,1.0f };

	gradientPathBrush.SetInterpolationColors(colors, pos, 3);

	Gdiplus::Bitmap tempBitmap(srcRc.Width, srcRc.Height);
	Gdiplus::Graphics tempRender(&tempBitmap);

	if (bExclude) {
		Gdiplus::Region excludeRegion(&excludePath);
		tempRender.ExcludeClip(&excludeRegion);
	}

	tempRender.FillPath(&gradientPathBrush, &shadowPath);

#if USE_BLUR
	// blur effect
	Gdiplus::BlurParams blurParams;
	blurParams.expandEdge = false;
	blurParams.radius = nBlurRadius;

	Gdiplus::Blur blurEffect;
	blurEffect.SetParameters(&blurParams);

	RECT rcBlurEffect{ nBlurSize,nBlurSize,srcRc.Width - 2 * nBlurSize,srcRc.Height - 2 * nBlurSize };
	tempBitmap.ApplyEffect(&blurEffect, &rcBlurEffect);
#endif

#if USE_COLOR_MATRIX
	// color matrix
	//https://docs.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-using-a-color-matrix-to-transform-a-single-color-use
	//https://docs.microsoft.com/en-us/windows/win32/api/gdipluscolormatrix/ns-gdipluscolormatrix-colormatrix
	//A 5��5 color matrix is a homogeneous matrix for a 4 - space transformation.
	//The element in the fifth row and fifth column of a 5��5 homogeneous matrix must be 1, 
	//and all of the other elements in the fifth column must be 0. 
	//Color matrices are used to transform color vectors.
	//The first four components of a color vector hold the red, green, blue,
	//and alpha components(in that order) of a color.
	//The fifth component of a color vector is always 1.
	Gdiplus::ColorMatrix colorMatrix = {
	0.99215,0,0,0,0,
	0,0.24313,0,0,0,
	0,0,0.24705,0,0,
	0,0,0,0.53923,0,
	0,0,0,0,1
	};

	Gdiplus::ColorMatrixEffect colorEffect;
	colorEffect.SetParameters(&colorMatrix);
	tempBitmap.ApplyEffect(&colorEffect, NULL);
#endif


	Gdiplus::Graphics graphics(m_hDC);
	graphics.DrawImage(&tempBitmap,
		Gdiplus::RectF(destRc.left, destRc.top, destRc.GetWidth(), destRc.GetHeight()),
		0, 0, srcRc.Width, srcRc.Height,
		Gdiplus::UnitPixel);
}

UiRect RenderContext::MeasureText(const std::wstring& strText,
	const HFONT hfont,
	UINT uStyle,
	int width /*= -1*/)
{

	Gdiplus::InstalledFontCollection installedFontCollection;

	// How many font families are installed?
	int count = installedFontCollection.GetFamilyCount();
	if (count == 0) {
		::SetBkMode(m_hDC, TRANSPARENT);
		HFONT hOldFont = (HFONT)::SelectObject(m_hDC, hfont);

		SIZE size = { 0 };
		GetTextExtentPoint32(m_hDC, strText.c_str(), _tcslen(strText.c_str()), &size);
		::SelectObject(m_hDC, hOldFont);

		UiRect rc(int(0), int(0), int(size.cx + 1), int(size.cy + 1));
		return rc;
	}

	Gdiplus::Graphics graphics(m_hDC);
	Gdiplus::Font font(m_hDC, hfont);
	Gdiplus::RectF bounds;

	Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();
	int formatFlags = 0;
	if ((uStyle & DT_SINGLELINE) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
	}
	stringFormat.SetFormatFlags(formatFlags);

	if (width == -1) {
		graphics.MeasureString(strText.c_str(), (int)strText.length(), &font, Gdiplus::PointF(), &stringFormat, &bounds);
	}
	else {
		Gdiplus::REAL height = 0;
		if ((uStyle & DT_SINGLELINE) != 0) {
			Gdiplus::RectF rcEmpty((Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)0);
			graphics.MeasureString(L"����", 2, &font, rcEmpty, &stringFormat, &bounds);
			height = bounds.Height;
		}
		Gdiplus::RectF rcText((Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)width, height);
		graphics.MeasureString(strText.c_str(), (int)strText.length(), &font, rcText, &stringFormat, &bounds);
	}

	UiRect rc(int(bounds.GetLeft()), int(bounds.GetTop()), int(bounds.GetRight() + 1), int(bounds.GetBottom() + 1));
	return rc;
}

}
}