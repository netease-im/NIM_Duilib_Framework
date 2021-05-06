#ifndef UI_AUTOMATION_UIA_COMBO_BOX_PROVIDER_H_
#define UI_AUTOMATION_UIA_COMBO_BOX_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIAComboBoxProvider :public UIAControlProvider
{
public:
	UIAComboBoxProvider() = delete;
	UIAComboBoxProvider(Control* pControl);

	// IUnknown methods
	virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

	// IRawElementProviderSimple methods
	virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
	virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

	// IValueProvider methods
	virtual IFACEMETHODIMP SetValue(_In_ LPCWSTR val);
	virtual IFACEMETHODIMP get_Value(_Out_ BSTR* retVal);
	virtual IFACEMETHODIMP get_IsReadOnly(_Out_ BOOL* retVal);

	// ISelectionProvider methods
	virtual IFACEMETHODIMP GetSelection(__RPC__deref_out_opt SAFEARRAY** pRetVal);
	virtual IFACEMETHODIMP get_CanSelectMultiple(__RPC__out BOOL* pRetVal);
	virtual IFACEMETHODIMP get_IsSelectionRequired(__RPC__out BOOL* pRetVal);

	// IExpandCollapseProvider methods
	virtual IFACEMETHODIMP Expand(void);
	virtual IFACEMETHODIMP Collapse(void);
	virtual IFACEMETHODIMP get_ExpandCollapseState(__RPC__out enum ExpandCollapseState* pRetVal);

protected:
	virtual ~UIAComboBoxProvider() {}
};

}

#endif

#endif