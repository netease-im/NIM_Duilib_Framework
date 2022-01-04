 #include "stdafx.h"

namespace ui
{

HBITMAP GdiBitmap::CreateDIBBitmap(HDC hdc, int width, int height, bool flipBItmap, LPVOID* pBits)
{
	if (hdc == NULL || width <= 0 || height <= 0)
		return NULL;

	BITMAPINFO bmi = { 0 };
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = flipBItmap ? -height : height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width * height * sizeof(DWORD);

	return ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, pBits, NULL, 0);
}

GdiBitmap::GdiBitmap() 
{
	m_hBitmap = NULL;
	CleanUp();
}

GdiBitmap::~GdiBitmap()
{
	CleanUp();
}

void GdiBitmap::CleanUp()
{
	if (m_hBitmap != NULL)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	m_pPiexl = NULL;
	m_nWidth = m_nHeight = 0;
}

bool GdiBitmap::Init(HDC hSrcDC, int width, int height, bool flipBItmap)
{
	CleanUp();

	LPVOID pBmpBits = NULL;
	m_hBitmap = CreateDIBBitmap(hSrcDC, width, height, flipBItmap, &pBmpBits);
	if (m_hBitmap != NULL)
	{
		m_nWidth = width;
		m_nHeight = height;

		BITMAP info;
		::GetObject(m_hBitmap, sizeof(info), &info);
		m_pPiexl = LPBYTE(info.bmBits);
		return true;
	}
	else
	{
		DWORD err = GetLastError();
		ASSERT(FALSE);
		return false;
	}
}

void GdiBitmap::Clear()
{
	ASSERT(m_hBitmap && m_pPiexl != NULL);
	if (m_pPiexl == NULL)
		return;

	::memset(m_pPiexl, 0, m_nWidth * m_nHeight * 4);
}

HBITMAP GdiBitmap::DetachBitmap()
{
	HBITMAP hBitmap = m_hBitmap;
	m_hBitmap = NULL;
	CleanUp();

	return hBitmap;
}

BYTE* GdiBitmap::GetBits()
{
	ASSERT(m_hBitmap && m_pPiexl != NULL);
	return m_pPiexl;
}

int GdiBitmap::GetWidth()
{
	return m_nWidth;
}

int GdiBitmap::GetHeight()
{
	return m_nHeight;
}

void GdiBitmap::ClearAlpha(const UiRect& rcDirty, int alpha)
{
	ASSERT(m_hBitmap && m_pPiexl != NULL);
	if (m_pPiexl == NULL)
		return;

	unsigned int * pBmpBits = (unsigned int *)m_pPiexl;
	int nTop = MAX(rcDirty.top, 0);
	int nBottom = MIN(rcDirty.bottom, m_nHeight);
	int nLeft = MAX(rcDirty.left, 0);
	int nRight = MIN(rcDirty.right, m_nWidth);
	if (nRight > nLeft)	{
		for (int i = nTop; i < nBottom; ++i) {
			::memset(pBmpBits + i * m_nWidth + nLeft, alpha, (nRight - nLeft) * 4);
		}
	}
}

void GdiBitmap::RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha)
{
	// 此函数适用于GDI等API渲染位图，导致丢失alpha通道的情况，可以把alpha通道补回来
	// 但是渲染位图时，还有GDI+、AlphaBlend等API给位图设置了半透明的alpha通道时，可能导致没法正确的修正alpha通道
	ASSERT(m_hBitmap && m_pPiexl != NULL);
	if (m_pPiexl == NULL)
		return;

	unsigned int * pBmpBits = (unsigned int *)m_pPiexl;
	int nTop = MAX(rcDirty.top, 0);
	int nBottom = MIN(rcDirty.bottom, m_nHeight);
	int nLeft = MAX(rcDirty.left, 0);
	int nRight = MIN(rcDirty.right, m_nWidth);

	for (int i = nTop; i < nBottom; i++) {
		for (int j = nLeft; j < nRight; j++) {
			BYTE* a = (BYTE*)(pBmpBits + i * m_nWidth + j) + 3;

			if (((j >= rcShadowPadding.left && j < m_nWidth - rcShadowPadding.right)
				|| (i >= rcShadowPadding.top && i < m_nHeight - rcShadowPadding.bottom))) {

				// ClearAlpha时，把alpha通道设置为某个值
				// 如果此值没有变化，则证明上面没有绘制任何内容，把alpha设为0
				if (alpha != 0 && *a == alpha)
					*a = 0;
				// 如果此值变为0，则证明上面被类似DrawText等GDI函数绘制过导致alpha被设为0，此时alpha设为255
				else if (*a == 0)
					*a = 255;
			}
		}
	}
}

void GdiBitmap::RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding)
{
	// 无论什么情况，都把此区域的alphaa通道设置为255
	ASSERT(m_hBitmap && m_pPiexl != NULL);
	if (m_pPiexl == NULL)
		return;

	unsigned int * pBmpBits = (unsigned int *)m_pPiexl;
	int nTop = MAX(rcDirty.top, 0);
	int nBottom = MIN(rcDirty.bottom, m_nHeight);
	int nLeft = MAX(rcDirty.left, 0);
	int nRight = MIN(rcDirty.right, m_nWidth);

	for (int i = nTop; i < nBottom; i++) {
		for (int j = nLeft; j < nRight; j++) {
			BYTE* a = (BYTE*)(pBmpBits + i * m_nWidth + j) + 3;

			if (((j >= rcShadowPadding.left && j < m_nWidth - rcShadowPadding.right)
				|| (i >= rcShadowPadding.top && i < m_nHeight - rcShadowPadding.bottom))) {
					*a = 255;
			}
		}
	}
}

HBITMAP GdiBitmap::GetBitmap()
{
	ASSERT(m_hBitmap);
	return m_hBitmap;
}

}