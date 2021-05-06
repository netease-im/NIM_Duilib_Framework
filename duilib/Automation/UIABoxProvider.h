#ifndef UI_AUTOMATION_UIA_BOX_PROVIDER_H_
#define UI_AUTOMATION_UIA_BOX_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIABoxProvider :public UIAControlProvider
{
public:
    UIABoxProvider() = delete;
    UIABoxProvider(Control* pControl);

    // IUnknown methods
    virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
    virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

    // IItemContainerProvider
    virtual IFACEMETHODIMP FindItemByProperty(__RPC__in_opt IRawElementProviderSimple* pStartAfter,
        PROPERTYID propertyId, VARIANT value,
        __RPC__deref_out_opt IRawElementProviderSimple** pFound);

protected:
    virtual ~UIABoxProvider() {}
};

}

#endif

#endif