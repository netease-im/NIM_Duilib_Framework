#include "StdAfx.h"

namespace ui {

using namespace Gdiplus;

void Matrix_Gdiplus::Translate(int offsetX, int offsetY)
{
	matrix_.Translate(offsetX, offsetY);
}

void Matrix_Gdiplus::Scale(int scaleX, int scaleY)
{
	matrix_.Scale(scaleX, scaleY);
}

void Matrix_Gdiplus::Rotate(float angle)
{
	matrix_.Rotate(angle);
}

void Matrix_Gdiplus::RotateAt(float angle, const CPoint& center)
{
	matrix_.RotateAt(angle, PointF(center.x, center.y));
}

} // namespace ui
