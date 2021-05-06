#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

UIAButtonProvider::UIAButtonProvider(Control* pControl)
	:UIALabelProvider(pControl)
{
}

UIAButtonProvider::~UIAButtonProvider()
{
}

// IUnknown implementation.
//
IFACEMETHODIMP UIAButtonProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IInvokeProvider))
		*ppInterface = static_cast<IInvokeProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPatternProvider.
// Gets the object that supports the specified pattern.
//
IFACEMETHODIMP UIAButtonProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	if (patternId == UIA_InvokePatternId)
		*pRetVal = static_cast<IRawElementProviderSimple*>(this);
	else
		return __super::GetPatternProvider(patternId, pRetVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIAButtonProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_ButtonControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}


// IInvokeProvider methods
IFACEMETHODIMP UIAButtonProvider::Invoke(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pButtonControl = dynamic_cast<Button*>(m_pControl);
	auto pButtonBox = dynamic_cast<ButtonBox*>(m_pControl);

	if (pButtonBox)
		pButtonBox->Activate();
	else if (pButtonControl)
		pButtonControl->Activate();

	return S_OK;
}

}

#endif