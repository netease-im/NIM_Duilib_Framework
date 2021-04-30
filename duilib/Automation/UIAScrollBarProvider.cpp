#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{



UIAScrollBarProvider::UIAScrollBarProvider(Control* pControl)
	:UIAControlProvider(pControl)
{
}

UIAScrollBarProvider::~UIAScrollBarProvider()
{
}

// IUnknown implementation.
//
IFACEMETHODIMP UIAScrollBarProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IRangeValueProvider))
		*ppInterface = static_cast<IRangeValueProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPatternProvider.
// Gets the object that supports the specified pattern.
//
IFACEMETHODIMP UIAScrollBarProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	if (patternId == UIA_RangeValuePatternId)
		*pRetVal = static_cast<IRawElementProviderSimple*>(this);
	else
		return __super::GetPatternProvider(patternId, pRetVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_ScrollBarControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::SetValue(double val)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollBar = dynamic_cast<ScrollBar*>(m_pControl);
	if (!pScrollBar)
		return E_INVALIDARG;

	pScrollBar->SetScrollPos(val);

	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::get_Value(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollBar = dynamic_cast<ScrollBar*>(m_pControl);
	if (!pScrollBar)
		return E_INVALIDARG;

	*pRetVal = pScrollBar->GetScrollPos();

	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::get_IsReadOnly(BOOL* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollBar = dynamic_cast<ScrollBar*>(m_pControl);
	if (!pScrollBar)
		return E_INVALIDARG;

	*pRetVal = !pScrollBar->IsEnabled();

	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::get_Maximum(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollBar = dynamic_cast<ScrollBar*>(m_pControl);
	if (!pScrollBar)
		return E_INVALIDARG;

	*pRetVal = static_cast<double>(pScrollBar->GetScrollRange());

	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::get_Minimum(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollBar = dynamic_cast<ScrollBar*>(m_pControl);
	if (!pScrollBar)
		return E_INVALIDARG;

	*pRetVal = 0.0f;

	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::get_LargeChange(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollBar = dynamic_cast<ScrollBar*>(m_pControl);
	if (!pScrollBar)
		return E_INVALIDARG;

	*pRetVal = 0.0f;

	return S_OK;
}

IFACEMETHODIMP UIAScrollBarProvider::get_SmallChange(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollBar = dynamic_cast<ScrollBar*>(m_pControl);
	if (!pScrollBar)
		return E_INVALIDARG;

	*pRetVal = 0.0f;

	return S_OK;
}

}

#endif