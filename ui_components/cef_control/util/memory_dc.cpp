#include "stdafx.h"
#include "memory_dc.h"
 
MemoryDC::MemoryDC() 
{
	bitmap_ = NULL;
	old_bitmap_ = NULL;
	mem_dc_ = NULL;
	memset(&bitmap_info_, 0, sizeof(bitmap_info_));

	bitmap_size_.cx = bitmap_size_.cy = 0;
	valid_ = false;
}

MemoryDC::~MemoryDC()
{
	DeleteDC();
}

void MemoryDC::DeleteDC()
{
	if (old_bitmap_ != NULL)
	{
		::SelectObject(mem_dc_, old_bitmap_);
		old_bitmap_ = NULL;
	}
	if (bitmap_ != NULL)
	{
		::DeleteObject(bitmap_);
		bitmap_ = NULL;
	}
	if (mem_dc_ != NULL)
	{
		::DeleteDC(mem_dc_);
		mem_dc_ = NULL;
	}
	memset(&bitmap_info_, 0, sizeof(bitmap_info_));
	valid_ = false;
}

bool MemoryDC::Init(HDC src_dc, int width, int height, const LPVOID pBits/*=NULL*/)
{
	DeleteDC();

	LPVOID pBmpBits = NULL;
	bool bRet = CreateMemoryDC(src_dc, width, height, &pBmpBits);
	if (bRet)
	{
		bitmap_size_.cx = width;
		bitmap_size_.cy = height;
		int stride = bitmap_size_.cx * 4;
		if (pBits)
		{
			memcpy(pBmpBits, pBits, stride*bitmap_size_.cy);
		}
		else
		{
			memset(pBmpBits, 0, stride*bitmap_size_.cy);
		}
	}

	return bRet;
}

bool MemoryDC::CreateMemoryDC(HDC src_dc, int width, int height, void** pBits)
{
	if (mem_dc_ != NULL || src_dc == NULL || width == 0 || height == 0)
		return false;

	mem_dc_ = ::CreateCompatibleDC(src_dc);

	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width * height * 4;

	bitmap_ = ::CreateDIBSection(src_dc, &bmi, DIB_RGB_COLORS, pBits, NULL, 0);
	old_bitmap_ = (HBITMAP)::SelectObject(mem_dc_, bitmap_);

	::GetObject(bitmap_, sizeof(bitmap_info_), &bitmap_info_);
	valid_ = true;

	return true;
}

BYTE* MemoryDC::GetBits()
{
	ASSERT(valid_);
	BYTE *pPiexl = LPBYTE(bitmap_info_.bmBits);
	ASSERT(pPiexl != NULL);
	return pPiexl;
}

int MemoryDC::GetWidth()
{
	return bitmap_size_.cx;
}

int MemoryDC::GetHeight()
{
	return bitmap_size_.cy;
}

HBITMAP MemoryDC::GetBitmap()
{
	ASSERT(bitmap_ != NULL && valid_);
	return bitmap_;
}

HDC MemoryDC::GetDC()
{
	ASSERT(mem_dc_ != NULL);
	return mem_dc_;
}

bool MemoryDC::IsValid()
{
	return valid_ && mem_dc_ != NULL && bitmap_ != NULL;
}