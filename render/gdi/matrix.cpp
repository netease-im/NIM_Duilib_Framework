#include "framework.h"

namespace dui {
namespace render {

Matrix::Matrix()
{
	matrix_ = std::make_unique<Gdiplus::Matrix>();
}

void Matrix::Translate(int offset_x, int offset_y)
{
	matrix_->Translate(offset_x, offset_y);
}

void Matrix::Scale(int scale_x, int scale_y)
{
	matrix_->Scale(scale_x, scale_y);
}

void Matrix::Rotate(float angle)
{
	matrix_->Rotate(angle);
}

void Matrix::RotateAt(float angle, const CPoint& center)
{
	matrix_->RotateAt(angle, Gdiplus::PointF(center.x, center.y));
}

}
}