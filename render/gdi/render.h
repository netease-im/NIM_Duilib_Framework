#pragma once

namespace dui {
namespace render {

class RenderContext :public IRenderContext {
	DISALLOW_COPY_AND_ASSIGN(RenderContext);
	FRIEND_CLASS_REFCOUNTEDOBJECT(RenderContext);
protected:
	RenderContext();
	~RenderContext();
public:
	HDC GetDC() override;
	bool Resize(int width, int height, bool flipBItmap = true) override;
	void Clear() override;

	HBITMAP DetachBitmap() override;
	BYTE* GetBits() override;
	HBITMAP GetBitmap() override;
	int	GetWidth() override;
	int GetHeight() override;
	void ClearAlpha(const UiRect& rcDirty, int alpha = 0) override;
	void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) override;
	void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding = UiRect()) override;

	bool IsRenderTransparent() const override;
	bool SetRenderTransparent(bool bTransparent) override;

	void Save() override;
	void Restore() override;

	CPoint OffsetWindowOrg(CPoint ptOffset) override;
	CPoint SetWindowOrg(CPoint ptOffset) override;
	CPoint GetWindowOrg() const override;

	void SetClip(const UiRect& rc) override;
	void SetRoundClip(const UiRect& rc, int width, int height) override;
	void ClearClip() override;

	bool BitBlt(int x, int y, int cx, int cy, HDC hdcSrc, int xSrc = 0, int yScr = 0, DWORD rop = SRCCOPY) override;
	bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, DWORD rop = SRCCOPY) override;
	bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, HDC hdcSrc, int xSrc, int yScr, int widthSrc, int heightSrc, BYTE uFade = 255) override;

	void DrawImage(const UiRect& rcPaint, 
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
		int nTiledMargin = 0) override;

	void DrawColor(const UiRect& rc, DWORD dwColor, BYTE uFade = 255) override;

	void DrawLine(const UiRect& rc, int nSize, DWORD dwPenColor) override;
	void DrawLine(common::dui_refptr<IPen> pen, int x1, int y1, int x2, int y2) override;
	void DrawBezier(common::dui_refptr<IPen> pen, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) override;
	void DrawRect(const UiRect& rc, int nSize, DWORD dwPenColor) override;
	void DrawRoundRect(const UiRect& rc, const CSize& roundSize, int nSize, DWORD dwPenColor) override;
	void DrawText(const UiRect& rc, 
		const std::wstring& strText,
		DWORD dwTextColor, 
		const HFONT hfont,
		UINT uStyle, 
		BYTE uFade = 255, 
		bool bLineLimit = false, 
		bool bFillPath = false) override;

	void DrawEllipse(const UiRect& rc, int nSize, DWORD dwColor) override;
	void FillEllipse(const UiRect& rc, DWORD dwColor) override;

	UiRect MeasureText(const std::wstring& strText, const HFONT hfont, UINT uStyle, int width = -1) override;

	void DrawPath(common::dui_refptr<IPath> path, common::dui_refptr<IPen> pen) override;
	void FillPath(common::dui_refptr<IPath> path, common::dui_refptr<IBrush> brush) override;

	void DrawBoxShadow(const UiRect& rc, const CSize& roundSize, const CPoint& cpOffset, int nBlurRadius, int nBlurSize, int nSpreadSize, DWORD dwColor, bool bExclude) override;
private:
	HDC			m_hDC;
	int			m_saveDC;
	HBITMAP		m_hOldBitmap;

	bool		m_bTransparent;
	common::dui_refptr<Clicp> clip_;
	common::dui_refptr<Bitmap> bitmap_;
};

}
}