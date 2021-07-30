#include "framework.h"

#include <vector>


namespace dui {
namespace render {

Path::Path()
{
	path_ = std::make_unique<Gdiplus::GraphicsPath>();
}

void Path::Reset()
{
	path_->Reset();
}

void Path::SetFillMode(FillMode mode)
{
	path_->SetFillMode(static_cast<Gdiplus::FillMode>(mode));
}

Path::FillMode Path::GetFillMode()
{
	return static_cast<IPath::FillMode>(path_->GetFillMode());
}

void Path::StartFigure()
{
	path_->StartFigure();
}

void Path::CloseFigure()
{
	path_->CloseFigure();
}

void Path::AddLine(int x1, int y1, int x2, int y2)
{
	path_->AddLine(x1, y1, x2, y2);
}

void Path::AddLines(const CPoint* points, int count)
{
	std::vector<Gdiplus::Point> p;
	for (int i = 0; i < count; i++)
	{
		p.emplace_back(points[i].x, points[i].y);
	}
	path_->AddLines(&p[0], p.size());
}

void Path::AddBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	path_->AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);
}

void Path::AddCurve(const CPoint* points, int count)
{
	std::vector<Gdiplus::Point> p;
	for (int i = 0; i < count; i++)
	{
		p.emplace_back(points[i].x, points[i].y);
	}
	path_->AddCurve(&p[0], p.size());
}

void Path::AddRect(int left, int top, int right, int bottom)
{
	path_->AddRectangle(Gdiplus::Rect(left, top, right - left, bottom - top));
}

void Path::AddRect(const UiRect& rect)
{
	path_->AddRectangle(Gdiplus::Rect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()));
}

void Path::AddEllipse(int left, int top, int right, int bottom)
{
	path_->AddEllipse(Gdiplus::Rect(left, top, right - left, bottom - top));
}

void Path::AddEllipse(const UiRect& rect)
{
	path_->AddEllipse(Gdiplus::Rect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()));
}

void Path::AddArc(int x, int y, int width, int height, float start_angle, float sweep_angle)
{
	path_->AddArc(x, y, width, height, start_angle, sweep_angle);
}

void Path::AddPie(int x, int y, int width, int height, float start_angle, float sweep_angle)
{
	path_->AddPie(x, y, width, height, start_angle, sweep_angle);
}

void Path::AddPolygon(const CPoint* points, int count)
{
	std::vector<Gdiplus::Point> p;
	for (int i = 0; i < count; i++)
	{
		p.emplace_back(points[i].x, points[i].y);
	}
	path_->AddPolygon(&p[0], p.size());
}

UiRect Path::GetBound(common::dui_refptr<IPen> pen)
{
	auto p = reinterpret_cast<Pen*>(pen.get());
	Gdiplus::Rect rc;
	path_->GetBounds(&rc, NULL, p ? p->GetPen() : NULL);
	return UiRect(rc.X, rc.Y, rc.GetRight(), rc.GetBottom());
}

bool Path::IsContainsPoint(int x, int y)
{
	return path_->IsVisible(x, y) == TRUE;
}

bool Path::IsStrokeContainsPoint(int x, int y, common::dui_refptr<IPen> pen)
{
	auto p = reinterpret_cast<Pen*>(pen.get());
	
	return path_->IsOutlineVisible(x, y, p->GetPen()) == TRUE;
}

void Path::Transform(common::dui_refptr<IMatrix> matrix)
{
	auto p = reinterpret_cast<Matrix*>(matrix.get());

	path_->Transform(p->GetMatrix());
}

}
}