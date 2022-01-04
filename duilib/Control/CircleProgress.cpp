#include "stdafx.h"
#include "CircleProgress.h"
#include "shlwapi.h"

namespace ui
{

CircleProgress::CircleProgress() :
	m_bCircular(true),
	m_bClockwise(true),
	m_nCircleWidth(1),
	m_dwBackgroundColor(0),
	m_dwForegroundColor(0),
	m_dwGradientColor(0),
	m_pIndicator(nullptr)
{

}

std::wstring CircleProgress::GetType() const
{
	return DUI_CTR_CIRCLEPROGRESS;
}

void CircleProgress::SetAttribute(const std::wstring& srName, const std::wstring& strValue)
{
	if (srName == _T("circular")) SetCircular(strValue == _T("true"));
	else if (srName == _T("circlewidth")) SetCircleWidth(_ttoi(strValue.c_str()));
	else if (srName == _T("indicator")) SetIndicator(strValue);
	else if (srName == _T("clockwise")) SetClockwiseRotation(strValue == _T("true"));
	else if (srName == _T("bgcolor")) {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
		SetBackgroudColor(pValue);
	}
	else if (srName == _T("fgcolor")) {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
		SetForegroudColor(pValue);
	}
	else if (srName == _T("gradientcolor")) {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
		SetCircleGradientColor(pValue);
	}
	else Progress::SetAttribute(srName, strValue);
}

void CircleProgress::PaintStatusImage(IRenderContext* pRender)
{
	Progress::PaintStatusImage(pRender);
	if (m_bCircular)
	{
		//目前IRenderContext还有很多GDI+接口未实现，暂时直接用gdi+画图了
		//以后可能会调整：需实现1、DrawArc  2、Pen增加brush(渐变)入参 3、可以自由设置Graphics属性
		int direction = m_bClockwise ? 1 : -1;   //旋转方向
		int bordersize = 1;          //弧度宽度目前使用1像素

		Gdiplus::Graphics graphics(pRender->GetDC());
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen bgPen(m_dwBackgroundColor, m_nCircleWidth);
		// 圆形中心
		CPoint center;
		center.x = m_rcItem.left + (m_rcItem.right - m_rcItem.left) / 2;
		center.y = m_rcItem.top + (m_rcItem.bottom - m_rcItem.top) / 2;

		// 控件矩形内的最大正方形的边界
		int side = min(m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
		//UiRect rcBorder;  仍然存在UiRect 到 RectF的转换，所以直接用gdi的RectF了
		Gdiplus::RectF rcBorder;
		rcBorder.X = center.x - side / 2;
		rcBorder.Y = center.y - side / 2;
		rcBorder.Width = rcBorder.Height = side;

		Gdiplus::RectF outer = rcBorder;
		if (m_pIndicator) {
			outer.Inflate(-1.0F *m_pIndicator->GetWidth() / 2, -1.0F * m_pIndicator->GetWidth() / 2);
		}
		else
		{
			outer.Inflate(-0.5 * m_nCircleWidth, -0.5 * m_nCircleWidth);
		}
		outer.Inflate(-1, -1);


		if (m_dwGradientColor == 0)
		{
			//不使用渐变色，直接用前景色铺满
			Gdiplus::Pen fgPen(m_dwForegroundColor, m_nCircleWidth);
			graphics.DrawArc(&bgPen, outer, 270, 360);   //270从最上面开始递增，设为0的话，是最右边开始
			graphics.DrawArc(&fgPen, outer, 270, direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin));
		}
		else
		{
			Gdiplus::REAL factors[4] = { 0.0f, 0.4f, 0.6f, 1.0f };
			Gdiplus::REAL positions[4] = { 0.0f, 0.2f, 0.8f, 1.0f };

			Gdiplus::LinearGradientBrush lgbrush(rcBorder, m_dwForegroundColor, m_dwGradientColor, Gdiplus::LinearGradientModeVertical);
			lgbrush.SetBlend(factors, positions, 4);
			graphics.DrawArc(&bgPen, outer, 270, 360);
			Gdiplus::Pen fgPen(&lgbrush, m_nCircleWidth);
			graphics.DrawArc(&fgPen, outer, 270, direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin));

		}

		//画旋转指示器图标，需要用到矩阵
		if (m_pIndicator)
		{
			Gdiplus::Matrix matrix;
			matrix.RotateAt(direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin), Gdiplus::PointF(center.x, center.y), Gdiplus::MatrixOrderAppend);
			graphics.SetTransform(&matrix);
			Gdiplus::RectF rectf;
			rectf.X = center.x - m_pIndicator->GetWidth() / 2;
			rectf.Y = outer.Y + bordersize / 2 - m_pIndicator->GetHeight() / 2;
			rectf.Width = m_pIndicator->GetWidth();
			rectf.Height = m_pIndicator->GetHeight();
			graphics.DrawImage(m_pIndicator, rectf);
		}

	}
}

void CircleProgress::ClearImageCache()
{
	__super::ClearImageCache();
	if (m_pIndicator)
	{
		delete m_pIndicator;
		m_pIndicator = nullptr;
	}
}

void CircleProgress::SetCircular(bool bCircular /*= true*/)
{
	m_bCircular = bCircular;
	Invalidate();
}

void CircleProgress::SetClockwiseRotation(bool bClockwise /*= true*/)
{
	if (bClockwise != m_bClockwise)
	{
		m_bClockwise = bClockwise;
		if (m_pIndicator)
		{
			//已经旋转了图片，旋转到相反的方向
			m_pIndicator->RotateFlip(Gdiplus::Rotate180FlipNone);
		}

	}
}

void CircleProgress::SetCircleWidth(int nCircleWidth)
{
	m_nCircleWidth = nCircleWidth;
	Invalidate();
}


void CircleProgress::SetBackgroudColor(const std::wstring& strColor)
{
	m_dwBackgroundColor = GlobalManager::GetTextColor(strColor);
	ASSERT(m_dwBackgroundColor != 0);
	Invalidate();
}

void CircleProgress::SetForegroudColor(const std::wstring& strColor)
{
	m_dwForegroundColor = GlobalManager::GetTextColor(strColor);
	ASSERT(m_dwForegroundColor != 0);
	Invalidate();
}

void CircleProgress::SetIndicator(const std::wstring& sIndicatorImage)
{
	if (m_sIndicatorImage != sIndicatorImage)
	{
		m_sIndicatorImage = sIndicatorImage;
		if (m_pIndicator)
		{
			delete m_pIndicator;
			m_pIndicator = nullptr;
		}
		std::wstring imagepath = m_sIndicatorImage;
		if (!::PathFileExistsW(imagepath.c_str())) {
			imagepath = GlobalManager::GetResourcePath() + m_pWindow->GetWindowResourcePath() + imagepath;
		}
		if (!::PathFileExistsW(imagepath.c_str())) {
			return;
		}
		m_pIndicator = new Gdiplus::Image(imagepath.c_str());

		Gdiplus::Status state = m_pIndicator->GetLastStatus();
		if (Gdiplus::Ok == state)
		{
			// 假定图片指向上
			m_pIndicator->RotateFlip(m_bClockwise ? Gdiplus::Rotate90FlipNone : Gdiplus::Rotate270FlipNone);
			Invalidate();
		}
	}
}

void CircleProgress::SetCircleGradientColor(const std::wstring& strColor)
{
	m_dwGradientColor = GlobalManager::GetTextColor(strColor);
	ASSERT(m_dwGradientColor != 0);
	Invalidate();
}

}
