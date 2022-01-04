/** @file CircleProgress.h
* @brief 环型进度条控件，圆环中间可以有文本（如85%）
* @copyright (c) 2019-2022, NetEase Inc. All rights reserved
* @author Xuhuajie
* @date 2019/8/14
*/

#ifndef UI_CONTROL_CIRCLEPROGRESS_H_
#define UI_CONTROL_CIRCLEPROGRESS_H_

#pragma once

namespace ui
{

class UILIB_API CircleProgress : public Progress
{
public:
	CircleProgress();

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStatusImage(IRenderContext* pRender) override;
	virtual void ClearImageCache() override;

	/**
	* @brief 设置环型进度条，默认为普通进度条
	* @param[in] bCircular 为 true 时设置为环型进度条，false 时设置为父级进度条样式，默认为 true
	* @return 无
	*/
	void SetCircular(bool bCircular = true);

	/**
	* @brief 设置递增方向
	* @param[in] bClockwise 为 true 时设置为顺时针，false 时设置为逆时针，默认为 true
	* @return 无
	*/
	void SetClockwiseRotation(bool bClockwise = true);

	/**
	* @brief 设置圆环宽度
	* @param[in] nCircleWidth 宽度数值
	* @return 无
	*/
	void SetCircleWidth(int nCircleWidth);

	/**
	* @brief 设置进度条背景颜色
	* @param[in] strColor要设置的背景颜色字符串，该字符串必须在 global.xml 中存在
	* @return 无
	*/
	void SetBackgroudColor(const std::wstring& strColor);

	/**
	* @brief 设置进度条前景颜色
	* @param[in] strColor要设置的前景颜色字符串，该字符串必须在 global.xml 中存在
	* @return 无
	*/
	void SetForegroudColor(const std::wstring& strColor);

	/**
	* @brief 设置进度条前景渐变颜色，与 SetForegroudColor 同时使用，可以不设置,则无渐变效果
	* @param[in] strColor要设置的前景渐变颜色字符串，该字符串必须在 global.xml 中存在
	* @return 无
	*/
	void SetCircleGradientColor(const std::wstring& strColor);

	/**
	* @brief 设置进度指示移动图标
	* @param[in] sIndicatorImage 要设置的图片
	* @return 无
	*/
	void SetIndicator(const std::wstring& sIndicatorImage);

protected:
	bool			m_bCircular;
	bool			m_bClockwise;
	int				m_nCircleWidth;
	DWORD			m_dwBackgroundColor;
	DWORD			m_dwForegroundColor;
	DWORD			m_dwGradientColor;
	Gdiplus::Image*	m_pIndicator;   //此类目前维护资源管理
	std::wstring	m_sIndicatorImage;

};

} // namespace ui

#endif // UI_CONTROL_CIRCLEPROGRESS_H_
