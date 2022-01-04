#include "StdAfx.h"
#include "GdiHepler.h"

namespace ui {
namespace GdiHelper {

Gdiplus::Bitmap* CreateBitmapFromHBITMAP(HBITMAP hBitmap) {
  BITMAP   bmp = { 0 };
  if (0 == GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bmp))
  {
    return   FALSE;
  }
  // Although we can get bitmap data address by bmp.bmBits member of BITMAP  
  // which is got by GetObject function sometime, 
  // we can determine the bitmap data in the HBITMAP is arranged bottom-up  
  // or top-down, so we should always use GetDIBits to get bitmap data. 
  BYTE* piexlsSrc = NULL;
  LONG   cbSize = bmp.bmWidthBytes * bmp.bmHeight;
  piexlsSrc = new   BYTE[cbSize];
  BITMAPINFO   bmpInfo = { 0 };
  // We should initialize the first six members of BITMAPINFOHEADER structure. 
  // A bottom-up DIB is specified by setting the height to a positive number,  
  // while a top-down DIB is specified by setting the height to a negative number. 
  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth = bmp.bmWidth;
  bmpInfo.bmiHeader.biHeight = bmp.bmHeight;  // 正数，说明数据从下到上，如未负数，则从上到下
  bmpInfo.bmiHeader.biPlanes = bmp.bmPlanes;
  bmpInfo.bmiHeader.biBitCount = bmp.bmBitsPixel;
  bmpInfo.bmiHeader.biCompression = BI_RGB;
  HDC   hdcScreen = CreateDC(L"DISPLAY", NULL, NULL, NULL);
  LONG   cbCopied = GetDIBits(hdcScreen, hBitmap, 0, bmp.bmHeight,
    piexlsSrc, &bmpInfo, DIB_RGB_COLORS);
  DeleteDC(hdcScreen);
  if (0 == cbCopied)
  {
    delete[]  piexlsSrc;
    return   FALSE;
  }
  // Create an GDI+ Bitmap has the same dimensions with hbitmap 
  Gdiplus::Bitmap* pBitmap = new Gdiplus::Bitmap(bmp.bmWidth, bmp.bmHeight, PixelFormat32bppPARGB);
  // Access to the Gdiplus::Bitmap's pixel data 
  Gdiplus::BitmapData  bitmapData;
  Gdiplus::Rect rect(0, 0, bmp.bmWidth, bmp.bmHeight);
  if (Gdiplus::Ok != pBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead,
    PixelFormat32bppPARGB, &bitmapData))
  {
    delete  (pBitmap);
    return   NULL;
  }
  BYTE* pixelsDest = (BYTE*)bitmapData.Scan0;
  int   nLinesize = bmp.bmWidth * sizeof(UINT);
  int   nHeight = bmp.bmHeight;
  // Copy pixel data from HBITMAP by bottom-up. 
  for (int y = 0; y < nHeight; y++)
  {
    // 从下到上复制数据，因为前面设置高度时是正数。
    memcpy_s(
      (pixelsDest + y * nLinesize),
      nLinesize,
      (piexlsSrc + (nHeight - y - 1) * nLinesize),
      nLinesize);
  }
  // Copy the data in temporary buffer to pBitmap 
  if (Gdiplus::Ok != pBitmap->UnlockBits(&bitmapData))
  {
    delete   pBitmap;
  }
  delete[]  piexlsSrc;
  return   pBitmap;
}

}

}
