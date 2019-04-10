#ifndef UI_CONTROL_CHECKBOX_H_
#define UI_CONTROL_CHECKBOX_H_

#pragma once

namespace ui
{

template<typename InheritType = Control>
class UILIB_API CheckBoxTemplate : public ButtonTemplate<InheritType>
{
public:
	CheckBoxTemplate();

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual void Activate() override;
	virtual Image* GetEstimateImage() override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	virtual void PaintStatusColor(IRenderContext* pRender) override;
	virtual void PaintStatusImage(IRenderContext* pRender) override;
	virtual void PaintText(IRenderContext* pRender) override;
	virtual bool HasHotState();

	/**
	 * @brief 判断当前是否是选择状态
	 * @return 返回 true 为选择状态，否则为 false
	 */
	bool IsSelected() const { return m_bSelected; }

	/**
	 * @brief 设置控件是否选择状态
	 * @param[in] bSelected 为 true 时为选择状态，false 时为取消选择状态
	 * @param[in] bTriggerEvent 是否发送状态改变事件，true 为发送，否则为 false。默认为 false
	 * @return 无
	 */
	virtual void Selected(bool bSelected, bool bTriggerEvent = false);

	/**
	 * @brief 获取被选择时的图片
	 * @param[in] stateType 要获取何种状态下的图片，参考 ControlStateType 枚举
	 * @return 返回图片位置
	 */
	std::wstring GetSelectedStateImage(ControlStateType stateType);

	/**
	 * @brief 设置被选择时的图片
	 * @param[in] stateType 要设置哪中状态下的图片
	 * @param[in] strImage 图片地址
	 * @return 无
	 */
	void SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage);

	/**
	 * @brief 获取被选择时的文本颜色
	 * @return 返回被选择时的文本颜色
	 */
	std::wstring GetSelectedTextColor();

	/**
	 * @brief 设置被选择时的文本颜色
	 * @param[in] dwTextColor 要设置的颜色字符串，该颜色必须在 global.xml 中存在
	 * @return 无
	 */
	void SetSelectedTextColor(const std::wstring& dwTextColor);

	/**
	 * @brief 获取被选择时的控件颜色 
	 * @param[in] stateType 要获取何种状态下的颜色
	 * @return 返回颜色字符串，该值在 global.xml 中定义
	 */
	std::wstring GetSelectStateColor(ControlStateType stateType);

	/**
	 * @brief 设置被选择时的控件颜色
	 * @param[in] stateType 要设置何种状态下的颜色
	 * @param[in] stateColor 要设置的颜色
	 * @return 无
	 */
	void SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor);

	/**
	 * @brief 获取被选择时的前景图片
	 * @param[in] stateType 要获取何种状态下的前景图片
	 * @return 返回图片位置
	 */
	std::wstring GetSelectedForeStateImage(ControlStateType stateType);

	/**
	 * @brief 设置被选择时的前景图片
	 * @param[in] stateType 要设置何种状态下的前景图片
	 * @param[in] pStrImage 图片位置
	 * @return 无
	 */
	void SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage);

	/**
	 * @brief 监听被选择时的事件
	 * @param[in] callback 被选择时触发的回调函数
	 * @return 无
	 */
	void AttachSelect(const EventCallback& callback) { OnEvent[kEventSelect] += callback; }

	/**
	 * @brief 监听取消选择时的事件
	 * @param[in] callback 取消选择时触发的回调函数
	 * @return 无
	 */
	void AttachUnSelect(const EventCallback& callback) { OnEvent[kEventUnSelect] += callback; }

protected:
	bool			m_bSelected;
	std::wstring	m_dwSelectedTextColor;
	StateColorMap	m_selectedColorMap;
};

#include "CheckBoxImpl.h"

typedef CheckBoxTemplate<Control> CheckBox;
typedef CheckBoxTemplate<Box> CheckBoxBox;

} // namespace ui

#endif // UI_CONTROL_CHECKBOX_H_#ifndef UI_CONTROL_CHECKBOX_H_