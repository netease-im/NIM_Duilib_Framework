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
	void AttachSelect(const EventCallback& callback) { this->OnEvent[kEventSelect] += callback; }

	/**
	 * @brief 监听取消选择时的事件
	 * @param[in] callback 取消选择时触发的回调函数
	 * @return 无
	 */
	void AttachUnSelect(const EventCallback& callback) { this->OnEvent[kEventUnSelect] += callback; }

protected:
	bool			m_bSelected;
	std::wstring	m_dwSelectedTextColor;
	StateColorMap	m_selectedColorMap;
};

template<typename InheritType>
CheckBoxTemplate<InheritType>::CheckBoxTemplate() : m_bSelected(false), m_dwSelectedTextColor(), m_selectedColorMap()
{
	m_selectedColorMap.SetControl(this);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Activate()
{
	if (!this->IsActivatable())
		return;
	Selected(!m_bSelected, true);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Selected(bool bSelected, bool bTriggerEvent)
{
	if (m_bSelected == bSelected) return;
	m_bSelected = bSelected;

	if (this->m_pWindow != NULL) {
		if (bTriggerEvent) {
			if (m_bSelected) {
				this->m_pWindow->SendNotify(this, kEventSelect);
			}
			else {
				this->m_pWindow->SendNotify(this, kEventUnSelect);
			}
		}
	}

	this->Invalidate();
}

template<typename InheritType>
bool CheckBoxTemplate<InheritType>::HasHotState()
{
	return m_selectedColorMap.HasHotColor() || __super::HasHotState();
}

template<typename InheritType>
Image* CheckBoxTemplate<InheritType>::GetEstimateImage()
{
	Image* estimateImage = __super::GetEstimateImage();
	if (!estimateImage) {
		estimateImage = this->m_imageMap.GetEstimateImage(kStateImageSelectedBk);
	}

	return estimateImage;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("selected")) Selected(strValue == _T("true"), true);
	else if (strName == _T("selectednormalimage")) SetSelectedStateImage(kControlStateNormal, strValue);
	else if (strName == _T("selectedhotimage")) SetSelectedStateImage(kControlStateHot, strValue);
	else if (strName == _T("selectedpushedimage")) SetSelectedStateImage(kControlStatePushed, strValue);
	else if (strName == _T("selecteddisabledimage")) SetSelectedStateImage(kControlStateDisabled, strValue);
	else if (strName == _T("selectedtextcolor")) SetSelectedTextColor(strValue);
	else if (strName == _T("selectednormalcolor")) SetSelectedStateColor(kControlStateNormal, strValue);
	else if (strName == _T("selectedhotcolor")) SetSelectedStateColor(kControlStateHot, strValue);
	else if (strName == _T("selectedpushedcolor")) SetSelectedStateColor(kControlStatePushed, strValue);
	else if (strName == _T("selecteddisabledcolor")) SetSelectedStateColor(kControlStateDisabled, strValue);
	else if (strName == _T("selectedforenormalimage")) SetSelectedForeStateImage(kControlStateNormal, strValue);
	else if (strName == _T("selectedforehotimage")) SetSelectedForeStateImage(kControlStateHot, strValue);
	else if (strName == _T("selectedforepushedimage")) SetSelectedForeStateImage(kControlStatePushed, strValue);
	else if (strName == _T("selectedforedisabledimage")) SetSelectedForeStateImage(kControlStateDisabled, strValue);
	else __super::SetAttribute(strName, strValue);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusColor(IRenderContext* pRender)
{
	if (!IsSelected()) {
		__super::PaintStatusColor(pRender);
		return;
	}

	m_selectedColorMap.PaintStatusColor(pRender, this->m_rcPaint, this->m_uButtonState);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusImage(IRenderContext* pRender)
{
	if (!IsSelected()) {
		__super::PaintStatusImage(pRender);
		return;
	}

	this->m_imageMap.PaintStatusImage(pRender, kStateImageSelectedBk, this->m_uButtonState);
	this->m_imageMap.PaintStatusImage(pRender, kStateImageSelectedFore, this->m_uButtonState);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintText(IRenderContext* pRender)
{
	if (!IsSelected()) {
		__super::PaintText(pRender);
		return;
	}

	if (this->GetText().empty()) return;
	UiRect rc = this->m_rcItem;
	rc.left += this->m_rcTextPadding.left;
	rc.right -= this->m_rcTextPadding.right;
	rc.top += this->m_rcTextPadding.top;
	rc.bottom -= this->m_rcTextPadding.bottom;

	std::wstring newTextColor = m_dwSelectedTextColor.empty() ? this->m_textColorMap[kControlStateNormal] : m_dwSelectedTextColor;
	DWORD dwTextColor = GlobalManager::GetTextColor(newTextColor);
	DWORD dwDisabledTextColor = GlobalManager::GetTextColor(this->m_textColorMap[kControlStateDisabled]);
	pRender->DrawText(rc, this->GetText(), this->IsEnabled() ? dwTextColor : dwDisabledTextColor, this->m_sFontId, this->m_uTextStyle);
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedStateImage(ControlStateType stateType)
{
	return this->m_imageMap.GetImagePath(kStateImageSelectedBk, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	this->m_imageMap.SetImage(kStateImageSelectedBk, stateType, pStrImage);
	if (this->GetFixedWidth() == DUI_LENGTH_AUTO || this->GetFixedHeight() == DUI_LENGTH_AUTO) {
		this->ArrangeAncestor();
	}
	else {
		this->Invalidate();
	}
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedTextColor()
{
	return m_dwSelectedTextColor;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedTextColor(const std::wstring& dwTextColor)
{
	m_dwSelectedTextColor = dwTextColor;
	this->Invalidate();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectStateColor(ControlStateType stateType)
{
	return m_selectedColorMap[stateType];
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor)
{
	m_selectedColorMap[stateType] = stateColor;
	this->Invalidate();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedForeStateImage(ControlStateType stateType)
{
	return this->m_imageMap.GetImagePath(kStateImageSelectedFore, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	this->m_imageMap.SetImage(kStateImageSelectedFore, stateType, pStrImage);
	if (this->GetFixedWidth() == DUI_LENGTH_AUTO || this->GetFixedHeight() == DUI_LENGTH_AUTO) {
		this->ArrangeAncestor();
	}
	else {
		this->Invalidate();
	}
}

typedef CheckBoxTemplate<Control> CheckBox;
typedef CheckBoxTemplate<Box> CheckBoxBox;

} // namespace ui

#endif // UI_CONTROL_CHECKBOX_H_#ifndef UI_CONTROL_CHECKBOX_H_