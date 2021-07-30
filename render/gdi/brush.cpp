#include "framework.h"

namespace dui {
namespace render {

Brush::Brush(DWORD color)
	: color_(color)
{
	brush_ = std::make_unique<Gdiplus::SolidBrush>(color_);
}

Brush::Brush(HBITMAP bitmap)
	: color_(0)
{
	Gdiplus::Bitmap image(bitmap, nullptr);
	bitmap_brush_ = std::make_unique<Gdiplus::TextureBrush>(&image);
}

DWORD Brush::GetColor()
{
	return color_;
}

}
}