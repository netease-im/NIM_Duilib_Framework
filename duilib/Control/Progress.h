#ifndef UI_CONTROL_PROGRESS_H_
#define UI_CONTROL_PROGRESS_H_

#pragma once

namespace ui
{

class UILIB_API Progress : public LabelTemplate<Control>
{
public:
	Progress();

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStatusImage(IRenderContext* pRender) override;
	virtual void ClearImageCache() override;

	/**
	 * @brief 判断是否是水平进度条
	 * @return 返回 true 是水平进度条，false 为垂直滚动条
	 */
	bool IsHorizontal();

	/**
	 * @brief 设置水平或垂直滚动条
	 * @param[in] bHorizontal 为 true 时设置为水平滚动条，false 时设置为垂直滚动条，默认为 true
	 * @return 无
	 */
	void SetHorizontal(bool bHorizontal = true);

	/**
	 * @brief 获取进度条最小值
	 * @return 返回进度条最小值
	 */
	int GetMinValue() const;

	/**
	 * @brief 设置进度条最小值
	 * @param[in] nMin 最小值数值
	 * @return 无
	 */
	void SetMinValue(int nMin);

	/**
	 * @brief 获取进度条最大值
	 * @return 返回进度条最大值
	 */
	int GetMaxValue() const;

	/**
	 * @brief 设置进度条最大值
	 * @param[in] nMax 要设置的最大值
	 * @return 无
	 */
	void SetMaxValue(int nMax);

	/**
	 * @brief 获取当前进度百分比
	 * @return 返回当前进度百分比
	 */
	double GetValue() const;

	/**
	 * @brief 设置当前进度百分比
	 * @param[in] nValue 要设置的百分比数值
	 * @return 无
	 */
	void SetValue(double nValue);

	/**
	 * @brief 进度条前景图片是否缩放显示
	 * @return 返回 true 为缩放显示，false 为不缩放显示
	 */
	bool IsStretchForeImage();

	/**
	 * @brief 设置进度条前景图片是否缩放显示
	 * @param[in] bStretchForeImage true 为缩放显示，false 为不缩放显示
	 * @return 无
	 */
	void SetStretchForeImage(bool bStretchForeImage = true);

	/**
	 * @brief 获取滚动条背景图片
	 * @return 返回背景图片位置
	 */
	std::wstring GetProgressImage() const;

	/**
	 * @brief 设置进度条背景图片
	 * @param[in] strImage 图片地址
	 * @return 无
	 */
	void SetProgressImage(const std::wstring& strImage);

	/**
	 * @brief 获取进度条背景颜色
	 * @return 返回背景颜色的字符串值，对应 global.xml 中的指定色值
	 */
	std::wstring GetProgressColor() const;

	/**
	 * @brief 设置进度条背景颜色
	 * @param[in] 要设置的背景颜色字符串，该字符串必须在 global.xml 中存在
	 * @return 无
	 */
	void SetProgressColor(const std::wstring& strProgressColor);

	/**
	 * @brief 获取进度条位置
	 * @return 返回进度条当前位置
	 */
	virtual UiRect GetProgressPos();

	/**
	 * @brief 播放Marquee
	 * @return 无
	 */
	virtual void Play();

	/**
	 * @brief 绘制Marquee
	 * @return 无
	 */
	void PaintMarquee(IRenderContext* pRender);

	/**
	 * @brief 是否是Marquee
	 * @return 无
	 */
	bool IsMarquee();

	/**
	 * @brief 设置Marquee
	 * @return 无
	 */
	void SetMarquee(bool bMarquee);

	/**
	 * @brief 获取Marquee宽度
	 * @return 无
	 */
	int GetMarqueeWidth();

	/**
	 * @brief 设置Marquee宽度
	 * @return 无
	 */
	void SetMarqueeWidth(int nMarqueeWidth);

	/**
	 * @brief 获取Marquee步长
	 * @return 无
	 */
	int GetMarqueeStep();

	/**
	 * @brief 设置Marquee步长
	 * @return 无
	 */
	void SetMarqueeStep(int nMarqueeStep);

	/**
	 * @brief 获取Marquee持续时间
	 * @return 无
	 */
	int GetMarqueeElapsed();

	/**
	 * @brief 设置Marquee持续时间
	 * @return 无
	 */
	void SetMarqueeElapsed(int nMarqueeElapsed);

  /**
  * @brief 设置是否反向填充进度条
  * @return 无
  */
  void SetReverse(bool bReverse);

protected:
	bool m_bHorizontal;
	bool m_bStretchForeImage;
	int m_nMax;
	int m_nMin;
	double m_nValue;
	Image m_progressImage;
	std::wstring m_sProgressColor;
	std::wstring m_sProgressImageModify;

	bool m_bMarquee;
	int m_nMarqueeWidth;
	int m_nMarqueeStep;
	int m_nMarqueeElapsed;
	int m_nMarqueePos;

  bool m_bReverse;

	nbase::WeakCallbackFlag m_timer;
};

} // namespace ui

#endif // UI_CONTROL_PROGRESS_H_
