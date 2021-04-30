#ifndef UI_AUTOMATION_UIA_LABEL_PROVIDER_H_
#define UI_AUTOMATION_UIA_LABEL_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIALabelProvider :public UIAControlProvider
{
public:
    UIALabelProvider() = delete;
    UIALabelProvider(Control* pControl);

    // IUnknown methods
    virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
    virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID idProp, _Out_ VARIANT* retVal);

    // IValueProvider methods
    virtual IFACEMETHODIMP SetValue(_In_ LPCWSTR val);
    virtual IFACEMETHODIMP get_Value(_Out_ BSTR* retVal);
    virtual IFACEMETHODIMP get_IsReadOnly(_Out_ BOOL* retVal);

protected:
    virtual ~UIALabelProvider();
};

}

#endif

#endif