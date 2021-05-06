#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

UIAListBoxItemProvider::UIAListBoxItemProvider(Control* pControl)
	:UIACheckBoxProvider(pControl)
{
}

IFACEMETHODIMP UIAListBoxItemProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(ISelectionItemProvider))
		*ppInterface = static_cast<ISelectionItemProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

IFACEMETHODIMP UIAListBoxItemProvider::GetPatternProvider(PATTERNID iid, IUnknown** retVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*retVal = nullptr;

	if (iid == UIA_SelectionItemPatternId)
		*retVal = static_cast<ISelectionItemProvider*>(this);
	else
		return __super::GetPatternProvider(iid, retVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIAListBoxItemProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_ListItemControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

IFACEMETHODIMP UIAListBoxItemProvider::Select(void)
{
	UIA_CHECK_ELEMENT(m_pControl);
	
	auto pItem = dynamic_cast<ListContainerElement*>(m_pControl);
	if (pItem && !pItem->IsSelected())
		pItem->Selected(true, true);

	return S_OK;
}

IFACEMETHODIMP UIAListBoxItemProvider::AddToSelection(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pItem = dynamic_cast<ListContainerElement*>(m_pControl);
	if (pItem && !pItem->IsSelected())
		pItem->Selected(true, true);

	return S_OK;
}

IFACEMETHODIMP UIAListBoxItemProvider::RemoveFromSelection(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pItem = dynamic_cast<ListContainerElement*>(m_pControl);
	if (pItem && pItem->IsSelected())
		pItem->Selected(false, true);
		

	return S_OK;
}

IFACEMETHODIMP UIAListBoxItemProvider::get_IsSelected(BOOL* pRetVal)
{
	*pRetVal = FALSE;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pItem = dynamic_cast<ListContainerElement*>(m_pControl);
	if (pItem)
		*pRetVal = pItem->IsSelected();

	return S_OK;
}

IFACEMETHODIMP UIAListBoxItemProvider::get_SelectionContainer(IRawElementProviderSimple** pRetVal)
{
	*pRetVal = nullptr;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pListBox = dynamic_cast<Box*>(m_pControl->GetParent());
	if (pListBox) {
		*pRetVal = static_cast<IRawElementProviderSimple*>(pListBox->GetUIAProvider());

		(*pRetVal)->AddRef();
	}

	return S_OK;
}



}

#endif