#ifndef UI_AUTOMATION_UIA_BUTTON_PROVIDER_H_
#define UI_AUTOMATION_UIA_BUTTON_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIAButtonProvider :public UIALabelProvider
{
public:
    UIAButtonProvider() = delete;
    UIAButtonProvider(Control* pControl);

    // IUnknown methods
    virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
    virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

    // IInvokeProvider methods
    virtual IFACEMETHODIMP Invoke(void);
protected:
    virtual ~UIAButtonProvider();
};

}

#endif

#endif