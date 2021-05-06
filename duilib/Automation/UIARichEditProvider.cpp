#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{



UIARichEditProvider::UIARichEditProvider(Control* pControl)
	:UIAControlProvider(pControl)
{
}

UIARichEditProvider::~UIARichEditProvider()
{
}
// IUnknown implementation.
//
IFACEMETHODIMP UIARichEditProvider::QueryInterface(REFIID riid, void** ppInterface)
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
IFACEMETHODIMP UIARichEditProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	if (patternId == UIA_ValuePatternId)
		*pRetVal = static_cast<IRawElementProviderSimple*>(this);
	else
		return __super::GetPatternProvider(patternId, pRetVal);

	AddRef();


	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPropertyValue.
// Gets custom properties. 
//
IFACEMETHODIMP UIARichEditProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
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
		pRetVal->lVal = UIA_EditControlTypeId;
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

IFACEMETHODIMP UIARichEditProvider::SetValue(_In_ LPCWSTR val)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pRichEdit = dynamic_cast<RichEdit*>(m_pControl);
	if (!pRichEdit)
		return UIA_E_ELEMENTNOTAVAILABLE;

	pRichEdit->SetText(val);

	return S_OK;
}

IFACEMETHODIMP UIARichEditProvider::get_Value(_Out_ BSTR* retVal)
{
	*retVal = nullptr;
	UIA_CHECK_ELEMENT(m_pControl);

	auto pRichEdit = dynamic_cast<RichEdit*>(m_pControl);
	if (!pRichEdit)
		return UIA_E_ELEMENTNOTAVAILABLE;

	*retVal = SysAllocString(pRichEdit->GetText().c_str());


	return S_OK;
}

IFACEMETHODIMP UIARichEditProvider::get_IsReadOnly(_Out_ BOOL* retVal)
{
	*retVal = TRUE;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pRichEdit = dynamic_cast<RichEdit*>(m_pControl);
	if (!pRichEdit)
		return UIA_E_ELEMENTNOTAVAILABLE;

	*retVal = pRichEdit->IsReadOnly();

	return S_OK;
}

}

#endif