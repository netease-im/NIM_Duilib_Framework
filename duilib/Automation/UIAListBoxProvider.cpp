#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

UIAListBoxProvider::UIAListBoxProvider(Control* pControl)
	:UIAScrollableBoxProvider(pControl)
{
}

IFACEMETHODIMP UIAListBoxProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(ISelectionProvider))
		*ppInterface = static_cast<ISelectionProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

IFACEMETHODIMP UIAListBoxProvider::GetPatternProvider(PATTERNID iid, IUnknown** retVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*retVal = nullptr;

	if (iid == UIA_SelectionPatternId)
		*retVal = static_cast<ISelectionProvider*>(this);
	else
		return __super::GetPatternProvider(iid, retVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIAListBoxProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_ListControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

// ISelectionProvider methods
//
IFACEMETHODIMP UIAListBoxProvider::GetSelection(SAFEARRAY** pRetVal)
{
	*pRetVal = nullptr;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pListBox = dynamic_cast<ListBox*>(m_pControl);
	if (pListBox && pListBox->GetCurSel() >= 0) {
		auto control = pListBox->GetItemAt(pListBox->GetCurSel());
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

IFACEMETHODIMP UIAListBoxProvider::get_CanSelectMultiple(BOOL* pRetVal)
{
	*pRetVal = FALSE;

	UIA_CHECK_ELEMENT(m_pControl);

	return S_OK;
}

IFACEMETHODIMP UIAListBoxProvider::get_IsSelectionRequired(BOOL* pRetVal)
{
	*pRetVal = FALSE;

	UIA_CHECK_ELEMENT(m_pControl);

	return S_OK;
}

}

#endif