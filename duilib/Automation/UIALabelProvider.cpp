#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui 
{



UIALabelProvider::UIALabelProvider(Control* pControl)
	:UIAControlProvider(pControl)
{
}

UIALabelProvider::~UIALabelProvider()
{
}
// IUnknown implementation.
//
IFACEMETHODIMP UIALabelProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IValueProvider))
		*ppInterface = static_cast<IValueProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPatternProvider.
// Gets the object that supports the specified pattern.
//
IFACEMETHODIMP UIALabelProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	if (patternId == UIA_ValuePatternId)
		*pRetVal = static_cast<IValueProvider*>(this);
	else 
		return __super::GetPatternProvider(patternId, pRetVal);

	AddRef();


	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPropertyValue.
// Gets custom properties. 
//
IFACEMETHODIMP UIALabelProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_IsContentElementPropertyId:
		pRetVal->boolVal = VARIANT_TRUE;
		pRetVal->vt = VT_BOOL;
		break;
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_TextControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

//
// IValueProvider Implementation
//

IFACEMETHODIMP UIALabelProvider::SetValue(_In_ LPCWSTR val)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pLabelControlBase = dynamic_cast<LabelTemplate<Control>*>(m_pControl);
	auto pLabelBoxBase = dynamic_cast<LabelTemplate<Box>*>(m_pControl);

	if (pLabelControlBase)
		pLabelControlBase->SetText(val);
	else if (pLabelBoxBase)
		pLabelBoxBase->SetText(val);
	else
		return E_INVALIDARG;

	return S_OK;
}

IFACEMETHODIMP UIALabelProvider::get_Value(_Out_ BSTR* retVal)
{
	*retVal = nullptr;
	UIA_CHECK_ELEMENT(m_pControl);

	auto pLabelControlBase = dynamic_cast<LabelTemplate<Control>*>(m_pControl);
	auto pLabelBoxBase = dynamic_cast<LabelTemplate<Box>*>(m_pControl);

	std::wstring strValue;
	if (pLabelControlBase)
		strValue = pLabelControlBase->GetText();
	else if (pLabelBoxBase)
		strValue = pLabelBoxBase->GetText();
	else
		return E_INVALIDARG;

	*retVal = SysAllocString(strValue.c_str());


	return S_OK;
}

IFACEMETHODIMP UIALabelProvider::get_IsReadOnly(_Out_ BOOL* retVal)
{
	*retVal = TRUE;

	UIA_CHECK_ELEMENT(m_pControl);

	return S_OK;
}

}

#endif