#include "StdAfx.h"

namespace ui {

using namespace Gdiplus;

Brush_Gdiplus::Brush_Gdiplus(DWORD color)
	: IBrush(color)
{
	brush_.reset(new SolidBrush(color));
}

Brush_Gdiplus::Brush_Gdiplus(HBITMAP bitmap)
	: IBrush(bitmap)
{
	Gdiplus::Bitmap image(bitmap, NULL);
	bitmap_brush_.reset(new TextureBrush(&image));
}

Brush_Gdiplus::Brush_Gdiplus(const Brush_Gdiplus& r)
	: IBrush(r)
{
	if (r.brush_)
		brush_.reset(r.brush_->Clone());

	if (r.bitmap_brush_)
		bitmap_brush_.reset(r.bitmap_brush_->Clone());
}

ui::IBrush* Brush_Gdiplus::Clone()
{
	return new Brush_Gdiplus(*this);
}

} // namespace ui
