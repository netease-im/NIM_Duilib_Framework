#ifndef UI_AUTOMATION_UIA_TREE_NODE_PROVIDER_H_
#define UI_AUTOMATION_UIA_TREE_NODE_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIATreeNodeProvider :public UIAListBoxItemProvider
{
public:
	UIATreeNodeProvider() = delete;
	UIATreeNodeProvider(Control* pControl);

	// IUnknown methods
	virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

	// IRawElementProviderSimple methods
	virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
	virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

	// IExpandCollapseProvider METHODS
	virtual IFACEMETHODIMP Expand(void);
	virtual IFACEMETHODIMP Collapse(void);
	virtual IFACEMETHODIMP get_ExpandCollapseState(__RPC__out enum ExpandCollapseState* pRetVal);

protected:
	virtual ~UIATreeNodeProvider() {}
};

}

#endif

#endif