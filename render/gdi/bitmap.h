#pragma once

namespace dui {
namespace render {

class Bitmap :public IBitmap {
	DISALLOW_COPY_AND_ASSIGN(Bitmap);
	FRIEND_CLASS_REFCOUNTEDOBJECT(Bitmap);
protected:
	Bitmap();
	~Bitmap();

public:
	bool Init(HDC hdc, int width, int height, bool flip) override;
	void Clear() override;
	HBITMAP DetachBitmap() override;

	HBITMAP GetBitmap() override;
	BYTE* GetBits() override;
	int	GetWidth() override;
	int GetHeight() override;

	void ClearAlpha(const UiRect& rc_dirty, int alpha) override;
	void RestoreAlpha(const UiRect& rc_dirty, const UiRect& rc_shadow_padding, int alpha) override;
	void RestoreAlpha(const UiRect& rc_dirty, const UiRect& rc_shadow_padding) override;

private:
	HBITMAP CreateDIBBitmap(HDC hdc, int width, int height, bool flip, LPVOID* pBits);
	void CleanUp();

private:
	HBITMAP	bitmap_;
	BYTE* pixel_;

	int width_;
	int	height_;
};

}
}