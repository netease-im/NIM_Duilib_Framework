#ifndef UI_RENDER_BITMAP_H_
#define UI_RENDER_BITMAP_H_

#pragma once

namespace ui
{

class GdiBitmap : public IBitmap
{
public:
	GdiBitmap();
	~GdiBitmap();

	static HBITMAP CreateDIBBitmap(HDC hdc, int width, int height, bool flipBItmap, LPVOID* pBits);

	virtual bool Init(HDC hSrcDC, int width, int height, bool flipBItmap) override;
	virtual void Clear() override;
	virtual HBITMAP DetachBitmap() override;

	virtual HBITMAP GetBitmap() override;
	virtual BYTE* GetBits() override;
	virtual int	GetWidth() override;
	virtual int GetHeight() override;

	virtual void ClearAlpha(const UiRect& rcDirty, int alpha) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding) override;
private:
	void CleanUp();

private:
	HBITMAP	m_hBitmap;
	BYTE	*m_pPiexl;

	int		m_nWidth;
	int		m_nHeight;
};
} // namespace ui

#endif // UI_CORE_RENDER_H_