#pragma once

namespace dui {
namespace render {

class Matrix :public IMatrix {
	DISALLOW_COPY_AND_ASSIGN(Matrix);
	FRIEND_CLASS_REFCOUNTEDOBJECT(Matrix);
protected:
	Matrix();
	~Matrix() = default;

public:
	void Translate(int offset_x, int offset_y) override;
	void Scale(int scale_x, int scale_y) override;
	void Rotate(float angle) override;
	void RotateAt(float angle, const CPoint& center) override;

	Gdiplus::Matrix* GetMatrix() { return matrix_.get(); }

private:
	std::unique_ptr<Gdiplus::Matrix> matrix_;
};

}
}