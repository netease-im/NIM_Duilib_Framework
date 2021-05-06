#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

UIAComboBoxProvider::UIAComboBoxProvider(Control* pControl)
	:UIAControlProvider(pControl)
{
}

IFACEMETHODIMP UIAComboBoxProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IValueProvider))
		*ppInterface = static_cast<IValueProvider*>(this);
	else if (riid == __uuidof(ISelectionProvider))
		*ppInterface = static_cast<ISelectionProvider*>(this);
	else if (riid == __uuidof(IExpandCollapseProvider))
		*ppInterface = static_cast<IExpandCollapseProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

IFACEMETHODIMP UIAComboBoxProvider::GetPatternProvider(PATTERNID iid, IUnknown** retVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*retVal = nullptr;

	if (iid == UIA_ValuePatternId)
		*retVal = static_cast<IValueProvider*>(this);
	else if (iid == UIA_SelectionPatternId)
		*retVal = static_cast<ISelectionProvider*>(this);
	else if (iid == UIA_ExpandCollapsePatternId)
		*retVal = static_cast<IExpandCollapseProvider*>(this);
	else
		return __super::GetPatternProvider(iid, retVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIAComboBoxProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_ComboBoxControlTypeId;
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

IFACEMETHODIMP UIAComboBoxProvider::SetValue(_In_ LPCWSTR val)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pComboBox = dynamic_cast<Combo*>(m_pControl);

	int index = _wtoi(val);

	if (pComboBox && pComboBox->IsActivatable() &&
		pComboBox->GetCurSel() != index && pComboBox->SelectItem(index))
		return S_OK;
	else
		return E_INVALIDARG;
}

IFACEMETHODIMP UIAComboBoxProvider::get_Value(_Out_ BSTR* retVal)
{
	*retVal = nullptr;
	UIA_CHECK_ELEMENT(m_pControl);

	auto pComboBox = dynamic_cast<Combo*>(m_pControl);

	std::wstring strValue;
	if (pComboBox)
		strValue = pComboBox->GetText();
	else
		return E_INVALIDARG;

	*retVal = SysAllocString(strValue.c_str());


	return S_OK;
}

IFACEMETHODIMP UIAComboBoxProvider::get_IsReadOnly(_Out_ BOOL* retVal)
{
	*retVal = TRUE;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pComboBox = dynamic_cast<Combo*>(m_pControl);
	if (pComboBox)
		*retVal = !pComboBox->IsEnabled();

	return S_OK;
}

// ISelectionProvider methods
//
IFACEMETHODIMP UIAComboBoxProvider::GetSelection(SAFEARRAY** pRetVal)
{
	*pRetVal = nullptr;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pComboBox = dynamic_cast<Combo*>(m_pControl);
	if (pComboBox && pComboBox->GetCurSel() >= 0) {
		auto control = pComboBox->GetItemAt(pComboBox->GetCurSel());
		if (control) {
			auto pProvider = control->GetUIAProvider();
			if (pProvider)
				pProvider->AddRef();

			LONG index = 0;
			*pRetVal = SafeArrayCreateVector(VT_UNKNOWN, 0, 1);
			SafeArrayPutElement(*pRetVal, &index, pProvider);
		}
	}

	return S_OK;
}

IFACEMETHODIMP UIAComboBoxProvider::get_CanSelectMultiple(BOOL* pRetVal)
{
	*pRetVal = FALSE;

	UIA_CHECK_ELEMENT(m_pControl);

	return S_OK;
}

IFACEMETHODIMP UIAComboBoxProvider::get_IsSelectionRequired(BOOL* pRetVal)
{
	*pRetVal = FALSE;

	UIA_CHECK_ELEMENT(m_pControl);

	return S_OK;
}

// IExpandCollapseProvider methods
//
IFACEMETHODIMP UIAComboBoxProvider::Expand(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pComboBox = dynamic_cast<Combo*>(m_pControl);
	if (pComboBox) {
		pComboBox->Activate();
	}

	return S_OK;
}

IFACEMETHODIMP UIAComboBoxProvider::Collapse(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pComboBox = dynamic_cast<Combo*>(m_pControl);
	if (pComboBox) {
		pComboBox->Deactivate();
	}

	return S_OK;
}

IFACEMETHODIMP UIAComboBoxProvider::get_ExpandCollapseState(ExpandCollapseState* pRetVal)
{
	*pRetVal = ExpandCollapseState_LeafNode;
	UIA_CHECK_ELEMENT(m_pControl);

	auto pComboBox = dynamic_cast<Combo*>(m_pControl);
	if (pComboBox && pComboBox->IsActivatable()) {
		*pRetVal = pComboBox->IsActivated() ? ExpandCollapseState_Expanded : ExpandCollapseState_Collapsed;
	}

	return S_OK;
}




}

#endif