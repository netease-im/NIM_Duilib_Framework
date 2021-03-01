#include "StdAfx.h"

namespace ui {

using namespace Gdiplus;
Pen_GdiPlus::Pen_GdiPlus(DWORD color, int width /*= 1*/)
	: IPen(color, width)
{
	pen_.reset(new Pen(color_, width));
}

Pen_GdiPlus::Pen_GdiPlus(const Pen_GdiPlus& r)
	: IPen(r)
{
	pen_.reset(r.pen_->Clone());
}

ui::IPen* Pen_GdiPlus::Clone()
{
	return new Pen_GdiPlus(*this);
}

void Pen_GdiPlus::SetWidth(int width)
{
	pen_->SetWidth(width);	
}

int Pen_GdiPlus::GetWidth()
{
	return (int)pen_->GetWidth();
}

void Pen_GdiPlus::SetColor(DWORD color)
{
	color_ = color;
	pen_->SetColor(color);
}

void Pen_GdiPlus::SetStartCap(LineCap cap)
{
	pen_->SetStartCap((Gdiplus::LineCap)cap);
}

void Pen_GdiPlus::SetEndCap(LineCap cap)
{
	pen_->SetEndCap((Gdiplus::LineCap)cap);
}

void Pen_GdiPlus::SetDashCap(LineCap cap)
{
	pen_->SetDashCap((Gdiplus::DashCap)cap);
}

IPen::LineCap Pen_GdiPlus::GetStartCap()
{
	return (IPen::LineCap)pen_->GetStartCap();
}

IPen::LineCap Pen_GdiPlus::GetEndCap()
{
	return (IPen::LineCap)pen_->GetEndCap();
}

IPen::LineCap Pen_GdiPlus::GetDashCap()
{
	return (IPen::LineCap)pen_->GetDashCap();
}

void Pen_GdiPlus::SetLineJoin(LineJoin join)
{
	pen_->SetLineJoin((Gdiplus::LineJoin)join);
}

IPen::LineJoin Pen_GdiPlus::GetLineJoin()
{
	return (IPen::LineJoin)pen_->GetLineJoin();
}

void Pen_GdiPlus::SetDashStyle(DashStyle style)
{
	pen_->SetDashStyle((Gdiplus::DashStyle)style);
}

IPen::DashStyle Pen_GdiPlus::GetDashStyle()
{
	return (IPen::DashStyle)pen_->GetDashStyle();
}

} // namespace ui
