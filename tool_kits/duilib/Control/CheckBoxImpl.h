template<typename InheritType>
CheckBoxTemplate<InheritType>::CheckBoxTemplate() :	m_bSelected(false),	m_dwSelectedTextColor(), m_selectedColorMap()
{
	m_selectedColorMap.SetControl(this);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Activate()
{
	if (!IsActivatable())
		return;
	Selected(!m_bSelected, true);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::Selected(bool bSelected, bool bTriggerEvent)
{
	if( m_bSelected == bSelected ) return;
	m_bSelected = bSelected;

	if( m_pWindow != NULL ) {
		if (bTriggerEvent) {
			if (m_bSelected) {
				m_pWindow->SendNotify(this, kEventSelect);
			}
			else {
				m_pWindow->SendNotify(this, kEventUnSelect);
			}
		}
	}

	Invalidate();
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
		estimateImage = m_imageMap.GetEstimateImage(kStateImageSelectedBk);
	}

	return estimateImage;
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if( strName == _T("selected") ) Selected(strValue == _T("true"), true);
	else if( strName == _T("selectednormalimage") ) SetSelectedStateImage(kControlStateNormal, strValue);
	else if( strName == _T("selectedhotimage") ) SetSelectedStateImage(kControlStateHot, strValue);
	else if( strName == _T("selectedpushedimage") ) SetSelectedStateImage(kControlStatePushed, strValue);
	else if( strName == _T("selecteddisabledimage") ) SetSelectedStateImage(kControlStateDisabled, strValue);
	else if( strName == _T("selectedtextcolor") ) SetSelectedTextColor(strValue);
	else if( strName == _T("selectednormalcolor") ) SetSelectedStateColor(kControlStateNormal, strValue);
	else if( strName == _T("selectedhotcolor") ) SetSelectedStateColor(kControlStateHot, strValue);
	else if( strName == _T("selectedpushedcolor") ) SetSelectedStateColor(kControlStatePushed, strValue);
	else if( strName == _T("selecteddisabledcolor") ) SetSelectedStateColor(kControlStateDisabled, strValue);
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

	m_selectedColorMap.PaintStatusColor(pRender, m_rcPaint, m_uButtonState);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintStatusImage(IRenderContext* pRender)
{
	if (!IsSelected()) {
		__super::PaintStatusImage(pRender);
		return;
	}

	m_imageMap.PaintStatusImage(pRender, kStateImageSelectedBk, m_uButtonState);
	m_imageMap.PaintStatusImage(pRender, kStateImageSelectedFore, m_uButtonState);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::PaintText(IRenderContext* pRender)
{
	if (!IsSelected()) {
		__super::PaintText(pRender);
		return;
	}

	if( GetText().empty() ) return;
	UiRect rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

	std::wstring newTextColor = m_dwSelectedTextColor.empty() ? m_textColorMap[kControlStateNormal] : m_dwSelectedTextColor;
	DWORD dwTextColor = GlobalManager::GetTextColor(newTextColor);
	DWORD dwDisabledTextColor = GlobalManager::GetTextColor(m_textColorMap[kControlStateDisabled]);
	pRender->DrawText(rc, GetText(), IsEnabled() ? dwTextColor : dwDisabledTextColor, m_iFont, m_uTextStyle);
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedStateImage(ControlStateType stateType)
{
	return m_imageMap.GetImagePath(kStateImageSelectedBk, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_imageMap.SetImage(kStateImageSelectedBk, stateType, pStrImage);
	if (GetFixedWidth() == DUI_LENGTH_AUTO || GetFixedHeight() == DUI_LENGTH_AUTO) {
		ArrangeAncestor();
	}
	else {
		Invalidate();
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
	Invalidate();
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
	Invalidate();
}

template<typename InheritType>
std::wstring CheckBoxTemplate<InheritType>::GetSelectedForeStateImage(ControlStateType stateType)
{
	return m_imageMap.GetImagePath(kStateImageSelectedFore, stateType);
}

template<typename InheritType>
void CheckBoxTemplate<InheritType>::SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage)
{
	m_imageMap.SetImage(kStateImageSelectedFore, stateType, pStrImage);
	if (GetFixedWidth() == DUI_LENGTH_AUTO || GetFixedHeight() == DUI_LENGTH_AUTO) {
		ArrangeAncestor();
	}
	else {
		Invalidate();
	}
}