#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

UIABoxProvider::UIABoxProvider(Control* pControl) 
	:UIAControlProvider(pControl)
{
}

IFACEMETHODIMP UIABoxProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IItemContainerProvider))
		*ppInterface = static_cast<IItemContainerProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

IFACEMETHODIMP UIABoxProvider::GetPatternProvider(PATTERNID iid, IUnknown** retVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*retVal = nullptr;

	if (iid == UIA_ItemContainerPatternId)
		*retVal = static_cast<IItemContainerProvider*>(this);
	else
		return __super::GetPatternProvider(iid, retVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIABoxProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
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

// IItemContainerProvider
// https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-implementingitemcontainer
// https://docs.microsoft.com/en-us/windows/win32/api/uiautomationcore/nf-uiautomationcore-iitemcontainerprovider-finditembyproperty
// 
IFACEMETHODIMP UIABoxProvider::FindItemByProperty(IRawElementProviderSimple* pStartAfter, 
	PROPERTYID propertyId, VARIANT value, IRawElementProviderSimple** pFound)
{
	*pFound = nullptr;

	UIA_CHECK_ELEMENT(m_pControl);

	Box* pBox = dynamic_cast<Box*>(m_pControl);
	if (!pBox)
		return UIA_E_ELEMENTNOTAVAILABLE;

	
	// When the propertyId parameter is 0, the provider should return the next item after pStartAfter.
	// If the pStartAfter parameter is NULL and propertyId is 0, the provider should return the first item in the container.
	if (propertyId == 0) {
		if (pStartAfter == nullptr) {
			Control* pFirst = pBox->GetItemAt(0);
			if (pFirst)
				*pFound = static_cast<IRawElementProviderSimple*>(pFirst->GetUIAProvider());
		}
		else {
			auto pStarterProvider = dynamic_cast<UIAControlProvider*>(pStartAfter);
			if (pStarterProvider && pStarterProvider->GetControl()) {
				auto pStarterControl = pStarterProvider->GetControl();
				auto pNextControl = pBox->GetItemAt(pBox->GetItemIndex(pStarterControl) + 1);
				if(pNextControl)
					*pFound = static_cast<IRawElementProviderSimple*>(pNextControl->GetUIAProvider());
			}
		}
	}
	// While not required, Microsoft highly recommends that FindItemByProperty support 
	// the Name, AutomationId, and IsSelected properties.
	else {
		switch (propertyId)
		{
		case UIA_NamePropertyId:
		case UIA_AutomationIdPropertyId:
		{
			if (!value.bstrVal)
				return E_INVALIDARG;

			auto pFoundControl = pBox->FindSubControl(value.bstrVal);
			if (pFoundControl)
				*pFound = static_cast<IRawElementProviderSimple*>(pFoundControl->GetUIAProvider());
		}
		break;

		case UIA_SelectionItemIsSelectedPropertyId:
		{
			ISelectionProvider* pSelectionProvider = nullptr;
			if (QueryInterface(__uuidof(ISelectionProvider), (void**)&pSelectionProvider) == S_OK 
				&& pSelectionProvider) {

				SAFEARRAY* pSelectedItemArray = nullptr;
				if (pSelectionProvider->GetSelection(&pSelectedItemArray) == S_OK &&
					pSelectedItemArray && SafeArrayGetElemsize(pSelectedItemArray)) {

					long index = 0;
					SafeArrayGetElement(pSelectedItemArray, &index, *pFound);
				}

				if (pSelectedItemArray)
					SafeArrayDestroy(pSelectedItemArray);
				
				pSelectionProvider->Release();
			}
		}
		break;

		default:
			return E_INVALIDARG;
			break;
		}
	}

	if (*pFound)
		(*pFound)->AddRef();

	return S_OK;
}


}

#endif