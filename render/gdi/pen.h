#pragma once

namespace dui {
namespace render {

class Pen : public IPen {
	DISALLOW_COPY_AND_ASSIGN(Pen);
	FRIEND_CLASS_REFCOUNTEDOBJECT(Pen);
protected:
	Pen(DWORD color, int width = 1);
	~Pen() = default;

public:
	void SetWidth(int width) override;
	int GetWidth() override;
	void SetColor(DWORD color) override;
	DWORD GetColor() override;

	void SetStartCap(LineCap cap) override;
	void SetEndCap(LineCap cap) override;
	void SetDashCap(LineCap cap) override;
	LineCap GetStartCap() override;
	LineCap GetEndCap() override;
	LineCap GetDashCap() override;

	void SetLineJoin(LineJoin join) override;
	LineJoin GetLineJoin() override;

	void SetDashStyle(DashStyle style) override;
	DashStyle GetDashStyle() override;

	const Gdiplus::Pen* GetPen() { return pen_.get(); };

private:
	std::unique_ptr<Gdiplus::Pen> pen_;

	DWORD color_;
	int width_;
};

}
}