#include "framework.h"

namespace dui {
namespace render {

Bitmap::Bitmap()
	:bitmap_(nullptr), pixel_(nullptr), width_(0), height_(0)
{

}

Bitmap::~Bitmap()
{
	CleanUp();
}

bool Bitmap::Init(HDC hdc, int width, int height, bool flip)
{
	CleanUp();

	LPVOID bmpbits = NULL;
	bitmap_ = CreateDIBBitmap(hdc, width, height, flip, &bmpbits);
	if (bitmap_ != NULL)
	{
		width_ = width;
		height_ = height;

		BITMAP info;
		::GetObject(bitmap_, sizeof(info), &info);
		pixel_ = LPBYTE(info.bmBits);
		return true;
	}
	else
	{
		DWORD err = GetLastError();
		return false;
	}
}

void Bitmap::Clear()
{
	if (pixel_ == NULL)
		return;

	::memset(pixel_, 0, width_ * height_ * 4);
}

HBITMAP Bitmap::DetachBitmap()
{
	HBITMAP temp_bitmap = bitmap_;
	bitmap_ = NULL;
	CleanUp();

	return temp_bitmap;
}

HBITMAP Bitmap::GetBitmap()
{
	return bitmap_;
}

BYTE* Bitmap::GetBits()
{
	return pixel_;
}

int Bitmap::GetWidth()
{
	return width_;
}

int Bitmap::GetHeight()
{
	return height_;
}

void Bitmap::ClearAlpha(const UiRect& rc_dirty, int alpha)
{
	if (pixel_ == NULL)
		return;

	unsigned int* pBmpBits = (unsigned int*)pixel_;
	int nTop = max(rc_dirty.top, 0);
	int nBottom = min(rc_dirty.bottom, height_);
	int nLeft = max(rc_dirty.left, 0);
	int nRight = min(rc_dirty.right, width_);
	if (nRight > nLeft) {
		for (int i = nTop; i < nBottom; ++i) {
			::memset(pBmpBits + i * width_ + nLeft, alpha, (nRight - nLeft) * 4);
		}
	}
}

void Bitmap::RestoreAlpha(const UiRect& rc_dirty, const UiRect& rc_shadow_padding, int alpha)
{
	// 此函数适用于GDI等API渲染位图，导致丢失alpha通道的情况，可以把alpha通道补回来
	// 但是渲染位图时，还有GDI+、AlphaBlend等API给位图设置了半透明的alpha通道时，可能导致没法正确的修正alpha通道
	if (pixel_ == NULL)
		return;

	unsigned int* pBmpBits = (unsigned int*)pixel_;
	int nTop = max(rc_dirty.top, 0);
	int nBottom = min(rc_dirty.bottom, height_);
	int nLeft = max(rc_dirty.left, 0);
	int nRight = min(rc_dirty.right, width_);

	for (int i = nTop; i < nBottom; i++) {
		for (int j = nLeft; j < nRight; j++) {
			BYTE* a = (BYTE*)(pBmpBits + i * width_ + j) + 3;

			if (((j >= rc_shadow_padding.left && j < width_ - rc_shadow_padding.right)
				|| (i >= rc_shadow_padding.top && i < height_ - rc_shadow_padding.bottom))) {

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

void Bitmap::RestoreAlpha(const UiRect& rc_dirty, const UiRect& rc_shadow_padding)
{
	// 无论什么情况，都把此区域的alphaa通道设置为255
	if (pixel_ == NULL)
		return;

	unsigned int* pBmpBits = (unsigned int*)pixel_;
	int nTop = max(rc_dirty.top, 0);
	int nBottom = min(rc_dirty.bottom, height_);
	int nLeft = max(rc_dirty.left, 0);
	int nRight = min(rc_dirty.right, width_);

	for (int i = nTop; i < nBottom; i++) {
		for (int j = nLeft; j < nRight; j++) {
			BYTE* a = (BYTE*)(pBmpBits + i * width_ + j) + 3;

			if (((j >= rc_shadow_padding.left && j < width_ - rc_shadow_padding.right)
				|| (i >= rc_shadow_padding.top && i < height_ - rc_shadow_padding.bottom))) {
				*a = 255;
			}
		}
	}
}

HBITMAP Bitmap::CreateDIBBitmap(HDC hdc, int width, int height, bool flip, LPVOID* pBits)
{
	if (hdc == NULL || width <= 0 || height <= 0)
		return NULL;

	BITMAPINFO bmi = { 0 };
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = flip ? -height : height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width * height * sizeof(DWORD);

	return ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, pBits, NULL, 0);
}

void Bitmap::CleanUp()
{
	if (bitmap_ != NULL)
	{
		::DeleteObject(bitmap_);
		bitmap_ = NULL;
	}

	pixel_ = NULL;
	width_ = height_ = 0;
}

}
}