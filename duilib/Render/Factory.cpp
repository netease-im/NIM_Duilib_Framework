#include "StdAfx.h"

namespace ui {

ui::IPen* RenderFactory_GdiPlus::CreatePen(DWORD color, int width /*= 1*/)
{
	return new Pen_GdiPlus(color, width);
}

ui::IBrush* RenderFactory_GdiPlus::CreateBrush(DWORD color)
{
	return new Brush_Gdiplus(color);
}

ui::IBrush* RenderFactory_GdiPlus::CreateBrush(HBITMAP bitmap)
{
	return new Brush_Gdiplus(bitmap);
}

ui::IMatrix* RenderFactory_GdiPlus::CreateMatrix()
{
	return new Matrix_Gdiplus();
}

ui::IPath* RenderFactory_GdiPlus::CreatePath()
{
	return new Path_Gdiplus();
}

ui::IBitmap* RenderFactory_GdiPlus::CreateBitmap()
{
	return new GdiBitmap();
}

ui::IRenderContext* RenderFactory_GdiPlus::CreateRenderContext()
{
	return new RenderContext_GdiPlus();
}

} // namespace ui
