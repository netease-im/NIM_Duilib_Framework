#ifndef UI_AUTOMATION_UIA_SCROLLABLEBOX_PROVIDER_H_
#define UI_AUTOMATION_UIA_SCROLLABLEBOX_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIAScrollableBoxProvider :public UIABoxProvider
{
public:
    UIAScrollableBoxProvider() = delete;
    UIAScrollableBoxProvider(Control* pControl);

    // IUnknown methods
    virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
    virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

    // IScrollProvider
    virtual IFACEMETHODIMP Scroll(enum ScrollAmount horizontalAmount, enum ScrollAmount verticalAmount);
    virtual IFACEMETHODIMP SetScrollPercent(double horizontalPercent, double verticalPercent);
    virtual IFACEMETHODIMP get_HorizontalScrollPercent(__RPC__out double* pRetVal);
    virtual IFACEMETHODIMP get_VerticalScrollPercent(__RPC__out double* pRetVal);
    virtual IFACEMETHODIMP get_HorizontalViewSize(__RPC__out double* pRetVal);
    virtual IFACEMETHODIMP get_VerticalViewSize(__RPC__out double* pRetVal);
    virtual IFACEMETHODIMP get_HorizontallyScrollable(__RPC__out BOOL* pRetVal);
    virtual IFACEMETHODIMP get_VerticallyScrollable(__RPC__out BOOL* pRetVal);
protected:
    virtual ~UIAScrollableBoxProvider() {}
};

}

#endif

#endif