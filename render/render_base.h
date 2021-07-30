#ifndef RENDER_BASE_H_
#define RENDER_BASE_H_

#pragma once

#include <Windows.h>

namespace dui {
namespace render {

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CPoint : public tagPOINT
{
public:
	CPoint()
	{
		x = y = 0;
	}

	CPoint(const POINT& src)
	{
		x = src.x;
		y = src.y;
	}

	CPoint(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	CPoint(LPARAM lParam)
	{
		x = ((int)(short)LOWORD(lParam));
		y = ((int)(short)HIWORD(lParam));
	}

	void Offset(int offsetX, int offsetY)
	{
		x += offsetX;
		y += offsetY;
	}

	void Offset(CPoint offsetPoint)
	{
		x += offsetPoint.x;
		y += offsetPoint.y;
	}

	bool operator==(const CPoint& dst) {
		return x == dst.x && y == dst.y;
	}

	bool operator!=(const CPoint& dst) {
		return x != dst.x || y != dst.y;
	}
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CSize : public tagSIZE
{
public:
	CSize()
	{
		cx = cy = 0;
	}

	CSize(const SIZE& src)
	{
		cx = src.cx;
		cy = src.cy;
	}

	CSize(const CSize& src)
	{
		cx = src.cx;
		cy = src.cy;
	}

	CSize(int _cx, int _cy)
	{
		cx = _cx;
		cy = _cy;
	}

	void Offset(int offsetCX, int offsetCY)
	{
		cx += offsetCX;
		cy += offsetCY;
	}

	void Offset(CSize offsetPoint)
	{
		cx += offsetPoint.cx;
		cy += offsetPoint.cy;
	}
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API UiRect : public tagRECT
{
public:
	UiRect()
	{
		left = top = right = bottom = 0;
	}

	UiRect(const RECT& src)
	{
		left = src.left;
		top = src.top;
		right = src.right;
		bottom = src.bottom;
	}

	UiRect(int iLeft, int iTop, int iRight, int iBottom)
	{
		left = iLeft;
		top = iTop;
		right = iRight;
		bottom = iBottom;
	}

	int GetWidth() const
	{
		return right - left;
	}

	int GetHeight() const
	{
		return bottom - top;
	}

	void Clear()
	{
		left = top = right = bottom = 0;
	}

	bool IsRectEmpty() const
	{
		return ::IsRectEmpty(this) == TRUE;
	}

	void ResetOffset()
	{
		::OffsetRect(this, -left, -top);
	}

	void Normalize()
	{
		if (left > right) { int iTemp = left; left = right; right = iTemp; }
		if (top > bottom) { int iTemp = top; top = bottom; bottom = iTemp; }
	}

	void Offset(int cx, int cy)
	{
		::OffsetRect(this, cx, cy);
	}

	void Offset(const CPoint& offset)
	{
		::OffsetRect(this, offset.x, offset.y);
	}

	void Inflate(int cx, int cy)
	{
		::InflateRect(this, cx, cy);
	}

	void Inflate(const UiRect& rect)
	{
		this->left -= rect.left;
		this->top -= rect.top;
		this->right += rect.right;
		this->bottom += rect.bottom;
	}

	void Deflate(int cx, int cy)
	{
		::InflateRect(this, -cx, -cy);
	}

	void Deflate(const UiRect& rect)
	{
		this->left += rect.left;
		this->top += rect.top;
		this->right -= rect.right;
		this->bottom -= rect.bottom;
	}

	void Union(const UiRect& rc)
	{
		::UnionRect(this, this, &rc);
	}

	void Intersect(const UiRect& rc)
	{
		::IntersectRect(this, this, &rc);
	}

	void Subtract(const UiRect& rc)
	{
		::SubtractRect(this, this, &rc);
	}

	bool IsPointIn(const CPoint& point) const
	{
		return ::PtInRect(this, point) == TRUE;
	}

	bool Equal(const UiRect& rect) const
	{
		return this->left == rect.left && this->top == rect.top
			&& this->right == rect.right && this->bottom == rect.bottom;
	}
};

} // namespace render
} // namespace dui

#endif // !RENDER_BASE_H_
