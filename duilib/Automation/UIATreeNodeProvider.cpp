#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

UIATreeNodeProvider::UIATreeNodeProvider(Control* pControl)
	:UIAListBoxItemProvider(pControl)
{
}

IFACEMETHODIMP UIATreeNodeProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IExpandCollapseProvider))
		*ppInterface = static_cast<IExpandCollapseProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

IFACEMETHODIMP UIATreeNodeProvider::GetPatternProvider(PATTERNID iid, IUnknown** retVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*retVal = nullptr;

	if (iid == UIA_ExpandCollapsePatternId)
		*retVal = static_cast<IExpandCollapseProvider*>(this);
	else
		return __super::GetPatternProvider(iid, retVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIATreeNodeProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_TreeControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

IFACEMETHODIMP UIATreeNodeProvider::Expand(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pNode = dynamic_cast<TreeNode*>(m_pControl);
	if (pNode) {
		pNode->SetExpand(true);
	}

	return S_OK;
}

IFACEMETHODIMP UIATreeNodeProvider::Collapse(void)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pNode = dynamic_cast<TreeNode*>(m_pControl);
	if (pNode) {
		pNode->SetExpand(false);
	}

	return S_OK;
}

IFACEMETHODIMP UIATreeNodeProvider::get_ExpandCollapseState(ExpandCollapseState* pRetVal)
{
	*pRetVal = ExpandCollapseState_LeafNode;
	UIA_CHECK_ELEMENT(m_pControl);

	auto pNode = dynamic_cast<TreeNode*>(m_pControl);
	if (pNode && pNode->GetChildNodeCount()) {
		*pRetVal = pNode->IsExpand() ? ExpandCollapseState_Expanded : ExpandCollapseState_Collapsed;
	}

	return S_OK;
}




}

#endif