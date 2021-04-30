#ifndef UI_AUTOMATION_UIA_LISTBOX_PROVIDER_H_
#define UI_AUTOMATION_UIA_LISTBOX_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIAListBoxProvider :public UIAScrollableBoxProvider
{
public:
    UIAListBoxProvider() = delete;
    UIAListBoxProvider(Control* pControl);

    // IUnknown methods
    virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
    virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

    // ISelectionProvider methods
    virtual IFACEMETHODIMP GetSelection(__RPC__deref_out_opt SAFEARRAY** pRetVal);
    virtual IFACEMETHODIMP get_CanSelectMultiple(__RPC__out BOOL* pRetVal);
    virtual IFACEMETHODIMP get_IsSelectionRequired(__RPC__out BOOL* pRetVal);

protected:
    virtual ~UIAListBoxProvider() {}
};

}

#endif

#endif