#ifndef UI_CONTROL_LABEL_H_
#define UI_CONTROL_LABEL_H_

#pragma once

namespace ui
{

template<typename InheritType = Control>
class UILIB_API LabelTemplate : public InheritType
{
public:
	LabelTemplate();

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetText() const;
	virtual std::string GetUTF8Text() const;
	virtual void SetText(const std::wstring& strText);
	virtual void SetUTF8Text(const std::string& strText);
	virtual void SetTextId(const std::wstring& strTextId);
	virtual void SetUTF8TextId(const std::string& strTextId);
	virtual bool HasHotState();
	virtual CSize EstimateText(CSize szAvailable, bool& bReEstimateSize) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintText(IRenderContext* pRender) override;

	/**
	 * @brief 设置文本样式
	 * @param[in] uStyle 要设置的样式
	 * @return 无
	 */
	void SetTextStyle(UINT uStyle);

	/**
	 * @brief 获取文本样式
	 * @return 返回文本样式
	 */
	UINT GetTextStyle() const;

	/**
	 * @brief 获取指定状态下的文本颜色
	 * @param[in] stateType 要获取的状态标志
	 * @return 返回指定状态下的文本颜色
	 */
	std::wstring GetStateTextColor(ControlStateType stateType);

	/**
	 * @brief 设置指定状态下的文本颜色
	 * @param[in] stateType 要设置的状态标志
	 * @param[in] dwTextColor 要设置的状态颜色字符串，该值必须在 global.xml 中存在
	 * @return 无
	 */
	void SetStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor);

	/**
	 * @brief 获取指定状态下的实际被渲染文本颜色
	 * @param[in] buttonStateType 要获取何种状态下的颜色
	 * @param[out] stateType 实际被渲染的状态
	 * @return 返回颜色字符串，该值在 global.xml 中定义
	 */
	std::wstring GetPaintStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType);

	/**
	 * @brief 获取当前字体编号
	 * @return 返回字体编号，该编号在 global.xml 中标识
	 */
	std::wstring GetFont() const;

	/**
	 * @brief 设置当前字体
	 * @param[in] index 要设置的字体编号，该编号必须在 global.xml 中存在
	 * @return 无
	 */
	void SetFont(const std::wstring& strFontId);

	/**
	 * @brief 获取文字边距
	 * @return 返回文字的边距信息
	 */
	UiRect GetTextPadding() const;

	/**
	 * @brief 设置文字边距信息
	 * @param[in] rc 边距信息
	 * @return 无
	 */
	void SetTextPadding(UiRect rc);

	/**
	 * @brief 判断是否是单行模式
	 * @return 返回 true 表示单行模式，否则为 false
	 */
	bool IsSingleLine();

	/**
	 * @brief 设置为单行输入模式
	 * @param[in] bSingleLine 为 true 时为单行模式，否则为 false
	 * @return 无
	 */
	void SetSingleLine(bool bSingleLine);

	/**
	 * @brief 是否限制整行输出
	 * @return 返回 true 为限制，false 为不限制
	 */
	bool IsLineLimit();

	/**
	 * @brief 限制整行输出
	 * @param[in] bLineLimit 设置 true 为限制，false 为不限制
	 * @return 无
	 */
	void SetLineLimit(bool bLineLimit);

protected:
	std::wstring m_sFontId;
	UINT	m_uTextStyle;
	bool    m_bSingleLine;
	bool    m_bLineLimit;
	int		m_hAlign;
	int		m_vAlign;
	UiRect	m_rcTextPadding;
	std::wstring	m_sText;
	std::wstring	m_sTextId;
	StateColorMap	m_textColorMap;
};

#include "LabelImpl.h"

typedef LabelTemplate<Control> Label;
typedef LabelTemplate<Box> LabelBox;

}

#endif // UI_CONTROL_LABEL_H_