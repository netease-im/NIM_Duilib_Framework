#ifndef UI_AUTOMATION_UIA_BUTTON_PROVIDER_H_
#define UI_AUTOMATION_UIA_BUTTON_PROVIDER_H_

#pragma once

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

    // IInvokeProvider methods
    virtual IFACEMETHODIMP Invoke(void);
protected:
    virtual ~UIAButtonProvider();
};

}

#endif