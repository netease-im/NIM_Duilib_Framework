#include "framework.h"

namespace dui {
namespace render {

Pen::Pen(DWORD color, int width)
	: color_(color), width_(width)
{
	pen_ = std::make_unique<Gdiplus::Pen>(color, width);
}

void Pen::SetWidth(int width)
{
	pen_->SetWidth(width);
}

int Pen::GetWidth()
{
	return (int)pen_->GetWidth();
}

void Pen::SetColor(DWORD color)
{
	color_ = color;
	pen_->SetColor(color);
}

DWORD Pen::GetColor()
{
	return color_;
}

void Pen::SetStartCap(LineCap cap)
{
	pen_->SetStartCap((Gdiplus::LineCap)cap);
}

void Pen::SetEndCap(LineCap cap)
{
	pen_->SetEndCap((Gdiplus::LineCap)cap);
}

void Pen::SetDashCap(LineCap cap)
{
	pen_->SetDashCap((Gdiplus::DashCap)cap);
}

IPen::LineCap Pen::GetStartCap()
{
	return (IPen::LineCap)pen_->GetStartCap();
}

IPen::LineCap Pen::GetEndCap()
{
	return (IPen::LineCap)pen_->GetEndCap();
}

IPen::LineCap Pen::GetDashCap()
{
	return (IPen::LineCap)pen_->GetDashCap();
}

void Pen::SetLineJoin(LineJoin join)
{
	pen_->SetLineJoin((Gdiplus::LineJoin)join);
}

IPen::LineJoin Pen::GetLineJoin()
{
	return (IPen::LineJoin)pen_->GetLineJoin();
}

void Pen::SetDashStyle(DashStyle style)
{
	pen_->SetDashStyle((Gdiplus::DashStyle)style);
}

IPen::DashStyle Pen::GetDashStyle()
{
	return (IPen::DashStyle)pen_->GetDashStyle();
}




}
}