#pragma once

namespace dui {
namespace render {

class Brush :public IBrush {
	DISALLOW_COPY_AND_ASSIGN(Brush);
	FRIEND_CLASS_REFCOUNTEDOBJECT(Brush);
protected:
	Brush(DWORD color);
	Brush(HBITMAP bitmap);
	~Brush() = default;

public:
	DWORD GetColor() override;

	Gdiplus::Brush* GetBrush() { return brush_ ? brush_.get() : bitmap_brush_.get(); };

private:
	DWORD color_;
	std::unique_ptr<Gdiplus::Brush> brush_;
	std::unique_ptr<Gdiplus::Brush> bitmap_brush_;
};

}
}