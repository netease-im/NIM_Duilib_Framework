#ifndef RENDER_INTERFACE_H_
#define RENDER_INTERFACE_H_

#pragma once

#include <string>

#include "common/dui_api.h"
#include "common/dui_refptr.h"
#include "render_base.h"

namespace dui {
namespace render
{

class IClip :public common::RefCountInterface
{
protected:
	virtual ~IClip() = default;
public:
	virtual void CreateClip(HDC hdc, UiRect rc) = 0;
	virtual void CreateRoundClip(HDC hdc, UiRect rc, int width, int height) = 0;
	virtual void ClearClip(HDC hdc) = 0;
};

class IBitmap : public common::RefCountInterface
{
protected:
	virtual ~IBitmap() = default;
public:
	virtual bool Init(HDC hdc, int width, int height, bool flip) = 0;
	virtual void Clear() = 0;
	virtual HBITMAP DetachBitmap() = 0;

	virtual HBITMAP GetBitmap() = 0;
	virtual BYTE* GetBits() = 0;
	virtual int	GetWidth() = 0;
	virtual int GetHeight() = 0;

	virtual void ClearAlpha(const UiRect& rc_dirty, int alpha) = 0;
	virtual void RestoreAlpha(const UiRect& rc_dirty, const UiRect& rc_shadow_padding, int alpha) = 0;
	virtual void RestoreAlpha(const UiRect& rc_dirty, const UiRect& rc_shadow_padding) = 0;
};

class IPen :public common::RefCountInterface
{
protected:
	virtual ~IPen() = default;
public:
	enum LineCap
	{
		LineCapFlat = 0,
		LineCapSquare = 1,
		LineCapRound = 2,
		LineCapTriangle = 3
	};

	enum LineJoin
	{
		LineJoinMiter = 0,
		LineJoinBevel = 1,
		LineJoinRound = 2,
		LineJoinMiterClipped = 3
	};

	enum DashStyle
	{
		DashStyleSolid,          // 0
		DashStyleDash,           // 1
		DashStyleDot,            // 2
		DashStyleDashDot,        // 3
		DashStyleDashDotDot,     // 4
		DashStyleCustom          // 5
	};

public:
	virtual void SetWidth(int width) = 0;
	virtual int GetWidth() = 0;
	virtual void SetColor(DWORD color) = 0;
	virtual DWORD GetColor() = 0;


	virtual void SetStartCap(LineCap cap) = 0;
	virtual void SetEndCap(LineCap cap) = 0;
	virtual void SetDashCap(LineCap cap) = 0;
	virtual LineCap GetStartCap() = 0;
	virtual LineCap GetEndCap() = 0;
	virtual LineCap GetDashCap() = 0;

	virtual void SetLineJoin(LineJoin join) = 0;
	virtual LineJoin GetLineJoin() = 0;

	virtual void SetDashStyle(DashStyle style) = 0;
	virtual DashStyle GetDashStyle() = 0;
};

class IBrush :public common::RefCountInterface
{
protected:
	virtual ~IBrush() = default;
public:
	virtual DWORD GetColor() = 0;
};

class IMatrix :public common::RefCountInterface
{
protected:
	virtual ~IMatrix() = default;
public:
	virtual void Translate(int offset_x, int offset_y) = 0;
	virtual void Scale(int scale_x, int scale_y) = 0;
	virtual void Rotate(float angle) = 0;
	virtual void RotateAt(float angle, const CPoint& center) = 0;
};

class IPath :public common::RefCountInterface
{
public:
	enum FillMode
	{
		FillModeAlternate,        // 0
		FillModeWinding           // 1
	};

protected:
	virtual ~IPath() = default;

public:
	virtual void Reset() = 0;

	virtual void SetFillMode(FillMode mode) = 0;
	virtual FillMode GetFillMode() = 0;

	virtual void StartFigure() = 0;
	virtual void CloseFigure() = 0;

	virtual void AddLine(int x1, int y1, int x2, int y2) = 0;
	virtual void AddLines(const CPoint* points, int count) = 0;
	virtual void AddBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) = 0;
	virtual void AddCurve(const CPoint* points, int count) = 0;
	virtual void AddRect(int left, int top, int right, int bottom) = 0;
	virtual void AddRect(const UiRect& rect) = 0;
	virtual void AddEllipse(int left, int top, int right, int bottom) = 0;
	virtual void AddEllipse(const UiRect& rect) = 0;
	virtual void AddArc(int x, int y, int width, int height, float start_angle, float sweep_angle) = 0;
	virtual void AddPie(int x, int y, int width, int height, float start_angle, float sweep_angle) = 0;
	virtual void AddPolygon(const CPoint* points, int count) = 0;

	virtual UiRect GetBound(common::dui_refptr<IPen> pen) = 0;
	virtual bool IsContainsPoint(int x, int y) = 0;
	virtual bool IsStrokeContainsPoint(int x, int y, common::dui_refptr<IPen> pen) = 0;

	virtual void Transform(common::dui_refptr<IMatrix> matrix) = 0;
};

class IRenderContext :public common::RefCountInterface
{
protected:
	virtual ~IRenderContext() = default;
public:
	virtual HDC GetDC() = 0;
	virtual bool Resize(int width, int height, bool flipBItmap = true) = 0;
	virtual void Clear() = 0;

	virtual HBITMAP DetachBitmap() = 0;
	virtual BYTE* GetBits() = 0;
	virtual HBITMAP GetBitmap() = 0;
	virtual int	GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual void ClearAlpha(const UiRect& rcDirty, int alpha = 0) = 0;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) = 0;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding = UiRect()) = 0;

	virtual bool IsRenderTransparent() const = 0;
	virtual bool SetRenderTransparent(bool bTransparent) = 0;

	virtual void Save() = 0;
	virtual void Restore() = 0;
	virtual CPoint OffsetWindowOrg(CPoint ptOffset) = 0;
	virtual CPoint SetWindowOrg(CPoint ptOffset) = 0;
	virtual CPoint GetWindowOrg() const = 0;

	virtual void SetClip(const UiRect& rc) = 0;
	virtual void SetRoundClip(const UiRect& rcItem, int width, int height) = 0;
	virtual void ClearClip() = 0;

	virtual bool BitBlt(int x, int y, int cx, int cy, HDC hdcSrc, int xSrc = 0, int yScr = 0, DWORD rop = SRCCOPY) = 0;
	virtual bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, DWORD rop = SRCCOPY) = 0;
	virtual bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, BYTE uFade = 255) = 0;

	virtual void DrawImage(const UiRect& rcPaint,
		const CSize& scrollPos,
		HBITMAP hBitmap,
		bool bAlphaChannel,
		const UiRect& rcImageDest,
		const UiRect& rcImageSource,
		UiRect rcCorners,
		UiRect rcDestCorners,
		bool bBitmapDpiScale = false,
		BYTE uFade = 255,
		bool xtiled = false,
		bool ytiled = false,
		bool fullxtiled = true,
		bool fullytiled = true,
		int nTiledMargin = 0) = 0;

	virtual void DrawColor(const UiRect& rc, DWORD dwColor, BYTE uFade = 255) = 0;
	virtual void DrawLine(const UiRect& rc, int nSize, DWORD dwPenColor) = 0;
	virtual void DrawLine(common::dui_refptr<IPen> pen, int x1, int y1, int x2, int y2) = 0;
	virtual void DrawBezier(common::dui_refptr<IPen> pen, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) = 0;
	virtual void DrawRect(const UiRect& rc, int nSize, DWORD dwPenColor) = 0;
	virtual void DrawRoundRect(const UiRect& rc, const CSize& roundSize, int nSize, DWORD dwPenColor) = 0;

	virtual void DrawText(const UiRect& rc,
		const std::wstring& strText,
		DWORD dwTextColor,
		const HFONT hfont,
		UINT uStyle,
		BYTE uFade = 255,
		bool bLineLimit = false,
		bool bFillPath = false) = 0;

	virtual void DrawEllipse(const UiRect& rc, int nSize, DWORD dwColor) = 0;
	virtual void FillEllipse(const UiRect& rc, DWORD dwColor) = 0;

	virtual UiRect MeasureText(const std::wstring& strText, const HFONT hfont, UINT uStyle, int width = -1) = 0;

	virtual void DrawPath(common::dui_refptr<IPath> path, common::dui_refptr<IPen> pen) = 0;
	virtual void FillPath(common::dui_refptr<IPath> path, common::dui_refptr<IBrush> brush) = 0;

	virtual void DrawBoxShadow(const UiRect& rc,
		const CSize& roundSize,
		const CPoint& cpOffset,
		int nBlurRadius,
		int nBlurSize,
		int nSpreadSize,
		DWORD dwColor,
		bool bExclude) = 0;
};

class IRenderFactory
{
protected:
	virtual ~IRenderFactory() = default;
public:
	virtual common::dui_refptr<IPen> CreatePen(DWORD color, int width = 1) = 0;
	virtual common::dui_refptr<IBrush> CreateBrush(DWORD color) = 0;
	virtual common::dui_refptr<IBrush> CreateBrush(HBITMAP bitmap) = 0;
	virtual common::dui_refptr<IMatrix> CreateMatrix() = 0;
	virtual common::dui_refptr<IPath> CreatePath() = 0;
	virtual common::dui_refptr<IBitmap> CreateBitmap() = 0;
	virtual common::dui_refptr<IRenderContext> CreateRenderContext() = 0;
};

UILIB_C UILIB_API IRenderFactory* UILIB_CALL CreateFactory();

} // namespace render
} // namespace dui

namespace dui {
namespace render {

class AutoClip
{
public:
	AutoClip(common::dui_refptr<IRenderContext> render, const UiRect& rc, bool is_clip = true) {
		render_ = NULL;
		is_clip_ = false;

		if (is_clip)
		{
			is_clip_ = is_clip;
			render_ = render;
			render_->SetClip(rc);
		}
	}
	AutoClip(common::dui_refptr<IRenderContext> render, const UiRect& rc_round, int width, int height, bool is_clip = true) {
		render_ = NULL;
		is_clip_ = false;

		if (is_clip)
		{
			is_clip_ = is_clip;
			render_ = render;
			render_->SetRoundClip(rc_round, width, height);
		}
	}
	~AutoClip() {
		if (is_clip_ && render_)
			render_->ClearClip();
	}

private:
	common::dui_refptr<IRenderContext> render_;
	bool is_clip_;
};

} // namespace render
} // namespace dui

#endif // RENDER_INTERFACE_H_
