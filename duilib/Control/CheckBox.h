#ifndef UI_CONTROL_CHECKBOX_H_
#define UI_CONTROL_CHECKBOX_H_

#pragma once

namespace ui {

template<typename InheritType = Control>
class UILIB_API CheckBoxTemplate : public ButtonTemplate<InheritType>
{
public:
    CheckBoxTemplate();

    /// 重写父类方法，提供个性化功能，请参考父类声明
    virtual std::wstring GetType() const override;
    virtual UIAControlProvider* GetUIAProvider() override;
    virtual void Activate() override;
    virtual Image* GetEstimateImage() override;
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual void PaintStatusColor(IRenderContext* pRender) override;
    virtual void PaintStatusImage(IRenderContext* pRender) override;
    virtual void PaintText(IRenderContext* pRender) override;
    virtual bool HasHotState();

    /**
     * @brief 选择状态下，没有设置背景色或背景图时，是否用非选择状态的对应属性来绘制
     * @return 返回 true 为选择状态，否则为 false
     */
    bool IsPaintNormalFirst() const { return m_bPaintNormalFirst; };

    /**
     * @brief 设置控件选择状态下，没有设置背景色或背景图时，用非选择状态的对应属性来绘制
     * @param[in] bFirst 为 true 绘制非选择状态属性，false 不绘制
     * @return 无
     */
    void SetPaintNormalFirst(bool bFirst) { m_bPaintNormalFirst = bFirst; };

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
     * @brief 获取被选择时指定状态下的文本颜色
     * @param[in] stateType 要获取何种状态下的颜色
     * @return 返回颜色字符串，该值在 global.xml 中定义
     */
    std::wstring GetSelectedStateTextColor(ControlStateType stateType);

    /**
     * @brief 设置被选择时指定状态下的文本颜色
     * @param[in] stateType 要设置何种状态下的颜色
     * @param[in] stateColor 要设置的颜色
     * @return 无
     */
    void SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor);

    /**
     * @brief 获取被选择时指定状态下的实际被渲染文本颜色
     * @param[in] buttonStateType 要获取何种状态下的颜色
     * @param[out] stateType 实际被渲染的状态
     * @return 返回颜色字符串，该值在 global.xml 中定义
     */
    std::wstring GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType);

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
    bool			m_bPaintNormalFirst;
    std::wstring	m_dwSelectedTextColor;
    StateColorMap	m_selectedTextColorMap;
    StateColorMap	m_selectedColorMap;
};

template<typename InheritType>
CheckBoxTemplate<InheritType>::CheckBoxTemplate() : m_bSelected(false), m_bPaintNormalFirst(false), m_dwSelectedTextColor(), m_selectedColorMap()
{
    m_selectedColorMap.SetControl(this);
    m_selectedTextColorMap.SetControl(this);
}

template<typename InheritType>
inline std::wstring CheckBoxTemplate<InheritType>::GetType() const
{
    return DUI_CTR_CHECKBOX;
}

template<typename InheritType>
inline UIAControlProvider* CheckBoxTemplate<InheritType>::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
    if (this->m_pUIAProvider == nullptr)
    {
        this->m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIACheckBoxProvider(this));
    }
    return this->m_pUIAProvider;
#else
  return nullptr;
#endif
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

#if defined(ENABLE_UIAUTOMATION)
    if (this->m_pUIAProvider != nullptr && UiaClientsAreListening()) {
        VARIANT vtOld = { 0 }, vtNew = { 0 };
        vtOld.vt = vtNew.vt = VT_I4;
        vtOld.lVal = m_bSelected ? ToggleState_Off : ToggleState_On;
        vtNew.lVal = m_bSelected ? ToggleState_On : ToggleState_Off;

        UiaRaiseAutomationPropertyChangedEvent(this->m_pUIAProvider, UIA_ToggleToggleStatePropertyId, vtOld, vtNew);

    }
#endif

    this->Invalidate();
}

template<typename InheritType>
bool CheckBoxTemplate<InheritType>::HasHotState()
{
    return m_selectedColorMap.HasHotColor() || m_selectedTextColorMap.HasHotColor() || __super::HasHotState();
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
    else if (strName == _T("normalfirst")) SetPaintNormalFirst(strValue == _T("true"));
    else if (strName == _T("selectednormalimage")) SetSelectedStateImage(kControlStateNormal, strValue);
    else if (strName == _T("selectedhotimage")) SetSelectedStateImage(kControlStateHot, strValue);
    else if (strName == _T("selectedpushedimage")) SetSelectedStateImage(kControlStatePushed, strValue);
    else if (strName == _T("selecteddisabledimage")) SetSelectedStateImage(kControlStateDisabled, strValue);
    else if (strName == _T("selectedtextcolor")) SetSelectedTextColor(strValue);
    else if (strName == _T("selectednormaltextcolor")) SetSelectedStateTextColor(kControlStateNormal, strValue);
    else if (strName == _T("selectedhottextcolor"))	SetSelectedStateTextColor(kControlStateHot, strValue);
    else if (strName == _T("selectedpushedtextcolor")) SetSelectedStateTextColor(kControlStatePushed, strValue);
    else if (strName == _T("selecteddisabledtextcolor")) SetSelectedStateTextColor(kControlStateDisabled, strValue);
    else if (strName == _T("selectednormalcolor")) SetSelectedStateColor(kControlStateNormal, strValue);
    else if (strName == _T("selectedhotcolor")) SetSelectedStateColor(kControlStateHot, strValue);
    else if (strName == _T("selectedpushedcolor")) SetSelectedStateColor(kControlStatePushed, strValue);
    else if (strName == _T("selecteddisabledcolor")) SetSelectedStateColor(kControlStateDisabled, strValue);
    else if (strName == _T("selectedforenormalimage")) SetSelectedForeStateImage(kControlStateNormal, strValue);
    else if (strName == _T("selectedforehotimage")) SetSelectedForeStateImage(kControlStateHot, strValue);
    else if (strName == _T("selectedforepushedimage")) SetSelectedForeStateImage(kControlStatePushed, strValue);
    else if (strName == _T("selectedforedisabledimage")) SetSelectedForeStateImage(kControlStateDisabled, strValue);
    else if (strName == _T("switchselect")) Selected(!IsSelected());
    else __super::SetAttribute(strName, strValue);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusColor(IRenderContext* pRender)
{
    if (!IsSelected()) {
        __super::PaintStatusColor(pRender);
        return;
    }

    if (IsPaintNormalFirst() && !m_selectedColorMap.HasColor())
        this->m_colorMap.PaintStatusColor(pRender, this->m_rcPaint, this->m_uButtonState);
    else
        m_selectedColorMap.PaintStatusColor(pRender, this->m_rcPaint, this->m_uButtonState);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusImage(IRenderContext* pRender)
{
    if (!IsSelected()) {
        __super::PaintStatusImage(pRender);
        return;
    }

    if (IsPaintNormalFirst() && !this->m_imageMap.HasImageType(kStateImageSelectedBk))
        this->m_imageMap.PaintStatusImage(pRender, kStateImageBk, this->m_uButtonState);
    else
        this->m_imageMap.PaintStatusImage(pRender, kStateImageSelectedBk, this->m_uButtonState);

    if (IsPaintNormalFirst() && !this->m_imageMap.HasImageType(kStateImageSelectedFore))
        this->m_imageMap.PaintStatusImage(pRender, kStateImageFore, this->m_uButtonState);
    else
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

    auto stateType = this->m_uButtonState;
    std::wstring clrColor = GetPaintSelectedStateTextColor(this->m_uButtonState, stateType);
    if (clrColor.empty())
        clrColor = m_dwSelectedTextColor.empty() ? this->GetPaintStateTextColor(this->m_uButtonState, stateType) : m_dwSelectedTextColor;
    DWORD dwClrColor = this->GetWindowColor(clrColor);

    if (this->m_bSingleLine)
        this->m_uTextStyle |= DT_SINGLELINE;
    else
        this->m_uTextStyle &= ~~DT_SINGLELINE;

    if (this->m_animationManager.GetAnimationPlayer(kAnimationHot)) {
        if ((stateType == kControlStateNormal || stateType == kControlStateHot)
            && !GetSelectedStateTextColor(kControlStateHot).empty()) {
            std::wstring clrColor = GetSelectedStateTextColor(kControlStateNormal);
            if (!clrColor.empty()) {
                DWORD dwClrColor = this->GetWindowColor(clrColor);
                pRender->DrawText(rc, this->GetText(), dwClrColor, this->m_sFontId, this->m_uTextStyle, 255, this->m_bLineLimit);
            }

            if (this->m_nHotAlpha > 0) {
                std::wstring clrColor = GetSelectedStateTextColor(kControlStateHot);
                if (!clrColor.empty()) {
                    DWORD dwClrColor = this->GetWindowColor(clrColor);
                    pRender->DrawText(rc, this->GetText(), dwClrColor, this->m_sFontId, this->m_uTextStyle, (BYTE)this->m_nHotAlpha, this->m_bLineLimit);
                }
            }

            return;
        }
    }

    pRender->DrawText(rc, this->GetText(), dwClrColor, this->m_sFontId, this->m_uTextStyle, 255, this->m_bLineLimit);
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

template<typename InheritType /*= Control*/>
std::wstring ui::CheckBoxTemplate<InheritType>::GetSelectedStateTextColor(ControlStateType stateType)
{
    return m_selectedTextColorMap[stateType];
}

template<typename InheritType /*= Control*/>
void ui::CheckBoxTemplate<InheritType>::SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor)
{
    m_selectedTextColorMap[stateType] = dwTextColor;
    this->Invalidate();
}

template<typename InheritType /*= Control*/>
std::wstring ui::CheckBoxTemplate<InheritType>::GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType)
{
    stateType = buttonStateType;
    if (stateType == kControlStatePushed && GetSelectedStateTextColor(kControlStatePushed).empty()) {
        stateType = kControlStateHot;
    }
    if (stateType == kControlStateHot && GetSelectedStateTextColor(kControlStateHot).empty()) {
        stateType = kControlStateNormal;
    }
    if (stateType == kControlStateDisabled && GetSelectedStateTextColor(kControlStateDisabled).empty()) {
        stateType = kControlStateNormal;
    }
    return GetSelectedStateTextColor(stateType);
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

#endif // UI_CONTROL_CHECKBOX_H_