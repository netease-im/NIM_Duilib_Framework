#ifndef UI_CONTROL_CHECKBOX_H_
#define UI_CONTROL_CHECKBOX_H_

#pragma once

namespace ui {

template<typename InheritType = Control>
class UILIB_API CheckBoxTemplate : public ButtonTemplate<InheritType>
{
public:
    CheckBoxTemplate();

    /// ��д���෽�����ṩ���Ի����ܣ���ο���������
    virtual std::wstring GetType() const override;
    virtual UIAControlProvider* GetUIAProvider() override;
    virtual void Activate() override;
    virtual Image* GetEstimateImage() override;
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual void PaintStatusColor(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
    virtual void PaintStatusImage(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
    virtual void PaintText(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
    virtual bool HasHotState();

    /**
     * @brief ѡ��״̬�£�û�����ñ���ɫ�򱳾�ͼʱ���Ƿ��÷�ѡ��״̬�Ķ�Ӧ����������
     * @return ���� true Ϊѡ��״̬������Ϊ false
     */
    bool IsPaintNormalFirst() const { return m_bPaintNormalFirst; };

    /**
     * @brief ���ÿؼ�ѡ��״̬�£�û�����ñ���ɫ�򱳾�ͼʱ���÷�ѡ��״̬�Ķ�Ӧ����������
     * @param[in] bFirst Ϊ true ���Ʒ�ѡ��״̬���ԣ�false ������
     * @return ��
     */
    void SetPaintNormalFirst(bool bFirst) { m_bPaintNormalFirst = bFirst; };

    /**
     * @brief �жϵ�ǰ�Ƿ���ѡ��״̬
     * @return ���� true Ϊѡ��״̬������Ϊ false
     */
    bool IsSelected() const { return m_bSelected; }

    /**
     * @brief ���ÿؼ��Ƿ�ѡ��״̬
     * @param[in] bSelected Ϊ true ʱΪѡ��״̬��false ʱΪȡ��ѡ��״̬
     * @param[in] bTriggerEvent �Ƿ���״̬�ı��¼���true Ϊ���ͣ�����Ϊ false��Ĭ��Ϊ false
     * @return ��
     */
    virtual void Selected(bool bSelected, bool bTriggerEvent = false);

    /**
     * @brief ��ȡ��ѡ��ʱ��ͼƬ
     * @param[in] stateType Ҫ��ȡ����״̬�µ�ͼƬ���ο� ControlStateType ö��
     * @return ����ͼƬλ��
     */
    std::wstring GetSelectedStateImage(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱ��ͼƬ
     * @param[in] stateType Ҫ��������״̬�µ�ͼƬ
     * @param[in] strImage ͼƬ��ַ
     * @return ��
     */
    void SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage);

    /**
     * @brief ��ȡ��ѡ��ʱ���ı���ɫ
     * @return ���ر�ѡ��ʱ���ı���ɫ
     */
    std::wstring GetSelectedTextColor();

    /**
     * @brief ���ñ�ѡ��ʱ���ı���ɫ
     * @param[in] dwTextColor Ҫ���õ���ɫ�ַ���������ɫ������ global.xml �д���
     * @return ��
     */
    void SetSelectedTextColor(const std::wstring& dwTextColor);

    /**
     * @brief ��ȡ��ѡ��ʱָ��״̬�µ��ı���ɫ
     * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetSelectedStateTextColor(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱָ��״̬�µ��ı���ɫ
     * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
     * @param[in] stateColor Ҫ���õ���ɫ
     * @return ��
     */
    void SetSelectedStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor);

    /**
     * @brief ��ȡ��ѡ��ʱָ��״̬�µ�ʵ�ʱ���Ⱦ�ı���ɫ
     * @param[in] buttonStateType Ҫ��ȡ����״̬�µ���ɫ
     * @param[out] stateType ʵ�ʱ���Ⱦ��״̬
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetPaintSelectedStateTextColor(ControlStateType buttonStateType, ControlStateType& stateType);

    /**
     * @brief ��ȡ��ѡ��ʱ�Ŀؼ���ɫ
     * @param[in] stateType Ҫ��ȡ����״̬�µ���ɫ
     * @return ������ɫ�ַ�������ֵ�� global.xml �ж���
     */
    std::wstring GetSelectStateColor(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱ�Ŀؼ���ɫ
     * @param[in] stateType Ҫ���ú���״̬�µ���ɫ
     * @param[in] stateColor Ҫ���õ���ɫ
     * @return ��
     */
    void SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor);

    /**
     * @brief ��ȡ��ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ��ȡ����״̬�µ�ǰ��ͼƬ
     * @return ����ͼƬλ��
     */
    std::wstring GetSelectedForeStateImage(ControlStateType stateType);

    /**
     * @brief ���ñ�ѡ��ʱ��ǰ��ͼƬ
     * @param[in] stateType Ҫ���ú���״̬�µ�ǰ��ͼƬ
     * @param[in] pStrImage ͼƬλ��
     * @return ��
     */
    void SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage);

    /**
     * @brief ������ѡ��ʱ���¼�
     * @param[in] callback ��ѡ��ʱ�����Ļص�����
     * @return ��
     */
    void AttachSelect(const EventCallback& callback) { this->OnEvent[kEventSelect] += callback; }

    /**
     * @brief ����ȡ��ѡ��ʱ���¼�
     * @param[in] callback ȡ��ѡ��ʱ�����Ļص�����
     * @return ��
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
void CheckBoxTemplate<InheritType>::PaintStatusColor(dui::common::dui_refptr<dui::render::IRenderContext> pRender)
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
void CheckBoxTemplate<InheritType>::PaintStatusImage(dui::common::dui_refptr<dui::render::IRenderContext> pRender)
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
void CheckBoxTemplate<InheritType>::PaintText(dui::common::dui_refptr<dui::render::IRenderContext> pRender)
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
                pRender->DrawText(rc, 
                    this->GetText(), 
                    dwClrColor, 
                    GlobalManager::GetFont(this->m_sFontId), 
                    this->m_uTextStyle,
                    255, 
                    this->m_bLineLimit);
            }

            if (this->m_nHotAlpha > 0) {
                std::wstring clrColor = GetSelectedStateTextColor(kControlStateHot);
                if (!clrColor.empty()) {
                    DWORD dwClrColor = this->GetWindowColor(clrColor);
                    pRender->DrawText(rc, 
                        this->GetText(), 
                        dwClrColor, 
                        GlobalManager::GetFont(this->m_sFontId), 
                        this->m_uTextStyle,
                        (BYTE)this->m_nHotAlpha,
                        this->m_bLineLimit);
                }
            }

            return;
        }
    }

    pRender->DrawText(rc, 
        this->GetText(), 
        dwClrColor, 
        GlobalManager::GetFont(this->m_sFontId),
        this->m_uTextStyle, 
        255, 
        this->m_bLineLimit);
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