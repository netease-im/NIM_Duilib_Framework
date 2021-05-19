#ifndef UI_CORE_RENDER_H_
#define UI_CORE_RENDER_H_

#pragma once

namespace ui 
{

class UILIB_API RenderContext_GdiPlus : public IRenderContext
{
public:
	RenderContext_GdiPlus();
	virtual ~RenderContext_GdiPlus();

	virtual HDC GetDC() override;
	virtual bool Resize(int width, int height, bool flipBItmap = true) override;
	virtual void Clear() override;
	virtual std::unique_ptr<IRenderContext> Clone() override;

	virtual HBITMAP DetachBitmap() override;
	virtual BYTE* GetBits() override;
	virtual HBITMAP GetBitmap() override;
	virtual int	GetWidth() override;
	virtual int GetHeight() override;
	virtual void ClearAlpha(const UiRect& rcDirty, int alpha = 0) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding = UiRect()) override;

	virtual bool IsRenderTransparent() const override;
	virtual bool SetRenderTransparent(bool bTransparent) override;

	virtual void Save() override;
	virtual void Restore() override;

	virtual CPoint OffsetWindowOrg(CPoint ptOffset) override;
	virtual CPoint SetWindowOrg(CPoint ptOffset) override;
	virtual CPoint GetWindowOrg() const override;

	virtual void SetClip(const UiRect& rc) override;
	virtual void SetRoundClip(const UiRect& rc, int width, int height) override;
	virtual void ClearClip() override;

	virtual HRESULT BitBlt(int x, int y, int cx, int cy, HDC hdcSrc, int xSrc = 0, int yScr = 0, DWORD rop = SRCCOPY) override;
	virtual bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, DWORD rop = SRCCOPY) override;
	virtual bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, BYTE uFade = 255) override;

	virtual void DrawImage(const UiRect& rcPaint, HBITMAP hBitmap, bool bAlphaChannel,
		const UiRect& rcImageDest, const UiRect& rcImageSource, UiRect rcCorners,
		bool bBitmapDpiScale = false, BYTE uFade = 255,
		bool xtiled = false, bool ytiled = false, bool fullxtiled = true, bool fullytiled = true, int nTiledMargin = 0) override;

	virtual void DrawColor(const UiRect& rc, DWORD dwColor, BYTE uFade = 255) override;
    virtual void DrawColor(const UiRect& rc, const std::wstring& colorStr, BYTE uFade = 255) override;

	virtual void DrawLine(const UiRect& rc, int nSize, DWORD dwPenColor) override;
	virtual void DrawLine(const IPen* pen, int x1, int y1, int x2, int y2) override;
	virtual void DrawBezier(const IPen* pen, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) override;
	virtual void DrawRect(const UiRect& rc, int nSize, DWORD dwPenColor) override;
	virtual void DrawRoundRect(const UiRect& rc, const CSize& roundSize, int nSize, DWORD dwPenColor) override;
	virtual void DrawText(const UiRect& rc, const std::wstring& strText, DWORD dwTextColor, const std::wstring& strFontId, UINT uStyle, BYTE uFade = 255, bool bLineLimit = false, bool bFillPath = false) override;

	virtual void DrawEllipse(const UiRect& rc, int nSize, DWORD dwColor) override;
	virtual void FillEllipse(const UiRect& rc, DWORD dwColor) override;

	virtual UiRect MeasureText(const std::wstring& strText, const std::wstring& strFontId, UINT uStyle, int width = DUI_NOSET_VALUE) override;

	virtual void DrawPath(const IPath* path, const IPen* pen) override;
	virtual void FillPath(const IPath* path, const IBrush* brush) override;

	void DrawBoxShadow(const UiRect& rc, const CSize& roundSize, const CPoint& cpOffset, int nBlurRadius, int nBlurSize, int nSpreadSize, DWORD dwColor, bool bExclude) override;

private:
	HDC			m_hDC;
	int			m_saveDC;
	HBITMAP		m_hOldBitmap;

	bool		m_bTransparent;
	GdiClip		m_clip;
	GdiBitmap	m_bitmap;
};

} // namespace ui

#endif // UI_CORE_RENDER_H_
