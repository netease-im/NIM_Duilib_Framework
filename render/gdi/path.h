#pragma once

namespace dui {
namespace render {

class Path :public IPath {
	DISALLOW_COPY_AND_ASSIGN(Path);
	FRIEND_CLASS_REFCOUNTEDOBJECT(Path);
protected:
	Path();
	~Path() = default;

public:
	void Reset() override;

	void SetFillMode(FillMode mode) override;
	FillMode GetFillMode() override;

	void StartFigure() override;
	void CloseFigure() override;

	void AddLine(int x1, int y1, int x2, int y2) override;
	void AddLines(const CPoint* points, int count) override;
	void AddBezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) override;
	void AddCurve(const CPoint* points, int count) override;
	void AddRect(int left, int top, int right, int bottom) override;
	void AddRect(const UiRect& rect) override;
	void AddEllipse(int left, int top, int right, int bottom) override;
	void AddEllipse(const UiRect& rect) override;
	void AddArc(int x, int y, int width, int height, float start_angle, float sweep_angle) override;
	void AddPie(int x, int y, int width, int height, float start_angle, float sweep_angle) override;
	void AddPolygon(const CPoint* points, int count) override;

	UiRect GetBound(common::dui_refptr<IPen> pen) override;
	bool IsContainsPoint(int x, int y) override;
	bool IsStrokeContainsPoint(int x, int y, common::dui_refptr<IPen> pen) override;

	void Transform(common::dui_refptr<IMatrix> matrix) override;

	Gdiplus::GraphicsPath* GetPath() { return path_.get(); };

private:
	std::unique_ptr<Gdiplus::GraphicsPath> path_;
};

}
}