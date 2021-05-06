#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{



UIACheckBoxProvider::UIACheckBoxProvider(Control* pControl)
	:UIAButtonProvider(pControl)
{
}

UIACheckBoxProvider::~UIACheckBoxProvider()
{
}

// IUnknown implementation.
//
IFACEMETHODIMP UIACheckBoxProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IToggleProvider))
		*ppInterface = static_cast<IToggleProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPatternProvider.
// Gets the object that supports the specified pattern.
//
IFACEMETHODIMP UIACheckBoxProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	if (patternId == UIA_TogglePatternId)
		*pRetVal = static_cast<IRawElementProviderSimple*>(this);
	else
		return __super::GetPatternProvider(patternId, pRetVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIACheckBoxProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_CheckBoxControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

IFACEMETHODIMP UIACheckBoxProvider::Toggle(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pCheckBox = dynamic_cast<CheckBox*>(m_pControl);
	auto pCheckBoxBox = dynamic_cast<CheckBoxBox*>(m_pControl);

	if (pCheckBox)
		pCheckBox->Selected(!pCheckBox->IsSelected(), true);
	else if (pCheckBoxBox)
		pCheckBoxBox->Selected(!pCheckBoxBox->IsSelected(), true);
	else
		return UIA_E_ELEMENTNOTAVAILABLE;

	return S_OK;
}

IFACEMETHODIMP UIACheckBoxProvider::get_ToggleState(ToggleState* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pCheckBox = dynamic_cast<CheckBox*>(m_pControl);
	auto pCheckBoxBox = dynamic_cast<CheckBoxBox*>(m_pControl);

	bool bIsSelected = false;
	*pRetVal = ToggleState_Indeterminate;

	if (pCheckBox)
		bIsSelected = pCheckBox->IsSelected();
	else if (pCheckBoxBox)
		bIsSelected = pCheckBoxBox->IsSelected();
	else
		return UIA_E_ELEMENTNOTAVAILABLE;

	*pRetVal = bIsSelected ? ToggleState_On : ToggleState_Off;

	return S_OK;
}

}

#endif