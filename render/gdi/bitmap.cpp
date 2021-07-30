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
	// �˺���������GDI��API��Ⱦλͼ�����¶�ʧalphaͨ������������԰�alphaͨ��������
	// ������Ⱦλͼʱ������GDI+��AlphaBlend��API��λͼ�����˰�͸����alphaͨ��ʱ�����ܵ���û����ȷ������alphaͨ��
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

				// ClearAlphaʱ����alphaͨ������Ϊĳ��ֵ
				// �����ֵû�б仯����֤������û�л����κ����ݣ���alpha��Ϊ0
				if (alpha != 0 && *a == alpha)
					*a = 0;
				// �����ֵ��Ϊ0����֤�����汻����DrawText��GDI�������ƹ�����alpha����Ϊ0����ʱalpha��Ϊ255
				else if (*a == 0)
					*a = 255;
			}
		}
	}
}

void Bitmap::RestoreAlpha(const UiRect& rc_dirty, const UiRect& rc_shadow_padding)
{
	// ����ʲô��������Ѵ������alphaaͨ������Ϊ255
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