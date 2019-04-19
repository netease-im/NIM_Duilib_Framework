#ifndef UI_CORE_RENDER_MATRIX_H_
#define UI_CORE_RENDER_MATRIX_H_

#pragma once
#include <GdiPlus.h>

namespace ui 
{

class UILIB_API Matrix_Gdiplus : public IMatrix
{
public:
	virtual void Translate(int offsetX, int offsetY) override;
	virtual void Scale(int scaleX, int scaleY) override;
	virtual void Rotate(float angle) override;
	virtual void RotateAt(float angle, const CPoint& center) override;

	Gdiplus::Matrix* GetMatrix() { return &matrix_; };
private:
	Gdiplus::Matrix matrix_;
};

} // namespace ui

#endif // UI_CORE_RENDER_MATRIX_H_
