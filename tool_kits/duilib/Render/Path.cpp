#include "StdAfx.h"

namespace ui {

using namespace Gdiplus;
Path_Gdiplus::Path_Gdiplus()
{
	path_.reset(new GraphicsPath());
}

Path_Gdiplus::Path_Gdiplus(const Path_Gdiplus& r)
	: IPath(r)
{
	path_.reset(r.path_->Clone());
}

ui::IPath* Path_Gdiplus::Clone()
{
	return new Path_Gdiplus(*this);
}

void Path_Gdiplus::Reset()
{
	path_->Reset();
}

void Path_Gdiplus::SetFillMode(FillMode mode)
{
	path_->SetFillMode((Gdiplus::FillMode)mode);
}

IPath::FillMode Path_Gdiplus::GetFillMode()
{
	return (IPath::FillMode)path_->GetFillMode();
}

void Path_Gdiplus::StartFigure()
{
	path_->StartFigure();
}

void Path_Gdiplus::CloseFigure()
{
	path_->CloseFigure();
}

void Path_Gdiplus::AddLine(int x1, int y1, int x2, int y2)
{
	path_->AddLine(x1, y1, x2, y2);
}

void Path_Gdiplus::AddLines(const CPoint* points, int count)
{
	std::vector<Point> p;
	for (int i = 0; i < count; i++)
	{
		p.emplace_back(points[i].x, points[i].y);
	}
	path_->AddLines(&p[0], p.size());
}

void Path_Gdiplus::AddBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	path_->AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);
}

void Path_Gdiplus::AddCurve(const CPoint* points, int count)
{
	std::vector<Point> p;
	for (int i = 0; i < count; i++)
	{
		p.emplace_back(points[i].x, points[i].y);
	}
	path_->AddCurve(&p[0], p.size());
}

void Path_Gdiplus::AddRect(int left, int top, int right, int bottom)
{
	path_->AddRectangle(Rect(left, top, right -left, bottom - top));
}

void Path_Gdiplus::AddRect(const UiRect& rect)
{
	path_->AddRectangle(Rect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()));
}

void Path_Gdiplus::AddEllipse(int left, int top, int right, int bottom)
{
	path_->AddEllipse(Rect(left, top, right - left, bottom - top));
}

void Path_Gdiplus::AddEllipse(const UiRect& rect)
{
	path_->AddEllipse(Rect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()));
}

void Path_Gdiplus::AddArc(int x, int y, int width, int height, float startAngle, float sweepAngle)
{
	path_->AddArc(x, y, width, height, startAngle, sweepAngle);
}

void Path_Gdiplus::AddPie(int x, int y, int width, int height, float startAngle, float sweepAngle)
{
	path_->AddPie(x, y, width, height, startAngle, sweepAngle);
}

void Path_Gdiplus::AddPolygon(const CPoint* points, int count)
{
	std::vector<Point> p;
	for (int i = 0; i < count; i++)
	{
		p.emplace_back(points[i].x, points[i].y);
	}
	path_->AddPolygon(&p[0], p.size());
}

ui::UiRect Path_Gdiplus::GetBound(const IPen* pen)
{
	auto p = (Pen_GdiPlus*)(pen);
	Rect rc;
	path_->GetBounds(&rc, NULL, p ? p->GetPen() : NULL);
	return UiRect(rc.X, rc.Y, rc.GetRight(), rc.GetBottom());
}

bool Path_Gdiplus::IsContainsPoint(int x, int y)
{
	return path_->IsVisible(x, y) == TRUE;
}

bool Path_Gdiplus::IsStrokeContainsPoint(int x, int y, const IPen* pen)
{
	return path_->IsOutlineVisible(x, y, ((Pen_GdiPlus*)pen)->GetPen()) == TRUE;
}

void Path_Gdiplus::Transform(const IMatrix* matrix)
{
	path_->Transform(((Matrix_Gdiplus*)matrix)->GetMatrix());
}

} // namespace ui
