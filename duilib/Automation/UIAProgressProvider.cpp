#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{



UIAProgressProvider::UIAProgressProvider(Control* pControl)
	:UIALabelProvider(pControl)
{
}

UIAProgressProvider::~UIAProgressProvider()
{
}

// IUnknown implementation.
//
IFACEMETHODIMP UIAProgressProvider::QueryInterface(REFIID riid, void** ppInterface)
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
IFACEMETHODIMP UIAProgressProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
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

IFACEMETHODIMP UIAProgressProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_ProgressBarControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

IFACEMETHODIMP UIAProgressProvider::SetValue(double val)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pProgress = dynamic_cast<Progress*>(m_pControl);
	if (!pProgress)
		return E_INVALIDARG;

	pProgress->SetValue(val);

	return S_OK;
}

IFACEMETHODIMP UIAProgressProvider::get_Value(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pProgress = dynamic_cast<Progress*>(m_pControl);
	if (!pProgress)
		return E_INVALIDARG;

	*pRetVal = pProgress->GetValue();

	return S_OK;
}

IFACEMETHODIMP UIAProgressProvider::get_IsReadOnly(BOOL* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pProgress = dynamic_cast<Progress*>(m_pControl);
	if (!pProgress)
		return E_INVALIDARG;

	*pRetVal = !pProgress->IsEnabled();

	return S_OK;
}

IFACEMETHODIMP UIAProgressProvider::get_Maximum(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pProgress = dynamic_cast<Progress*>(m_pControl);
	if (!pProgress)
		return E_INVALIDARG;

	*pRetVal = static_cast<double>(pProgress->GetMaxValue());

	return S_OK;
}

IFACEMETHODIMP UIAProgressProvider::get_Minimum(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pProgress = dynamic_cast<Progress*>(m_pControl);
	if (!pProgress)
		return E_INVALIDARG;

	*pRetVal = static_cast<double>(pProgress->GetMinValue());

	return S_OK;
}

IFACEMETHODIMP UIAProgressProvider::get_LargeChange(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pProgress = dynamic_cast<Progress*>(m_pControl);
	if (!pProgress)
		return E_INVALIDARG;

	*pRetVal = 0.0f;

	return S_OK;
}

IFACEMETHODIMP UIAProgressProvider::get_SmallChange(double* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pProgress = dynamic_cast<Progress*>(m_pControl);
	if (!pProgress)
		return E_INVALIDARG;

	*pRetVal = 0.0f;

	return S_OK;
}

}

#endif