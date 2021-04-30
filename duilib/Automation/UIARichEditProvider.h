#ifndef UI_AUTOMATION_UIA_RICHEDIT_PROVIDER_H_
#define UI_AUTOMATION_UIA_RICHEDIT_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIARichEditProvider :public UIAControlProvider
{
public:
    UIARichEditProvider() = delete;
    UIARichEditProvider(Control* pControl);

    // IUnknown methods
    virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
    virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

    // IValueProvider methods
    virtual IFACEMETHODIMP SetValue(_In_ LPCWSTR val);
    virtual IFACEMETHODIMP get_Value(_Out_ BSTR* retVal);
    virtual IFACEMETHODIMP get_IsReadOnly(_Out_ BOOL* retVal);

protected:
    virtual ~UIARichEditProvider();
};

}

#endif

#endif