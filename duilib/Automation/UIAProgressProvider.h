#ifndef UI_AUTOMATION_UIA_PROGRESS_PROVIDER_H_
#define UI_AUTOMATION_UIA_PROGRESS_PROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class UIAProgressProvider :public UIALabelProvider
{
public:
    UIAProgressProvider() = delete;
    UIAProgressProvider(Control* pControl);

    // IUnknown methods
    virtual IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    virtual IFACEMETHODIMP GetPatternProvider(_In_ PATTERNID iid, _Outptr_result_maybenull_ IUnknown** retVal);
    virtual IFACEMETHODIMP GetPropertyValue(_In_ PROPERTYID propertyId, _Out_ VARIANT* pRetVal);

    // IRangeValueProvider methods
    virtual IFACEMETHODIMP SetValue(double val);

    virtual IFACEMETHODIMP get_Value(__RPC__out double* pRetVal);

    virtual IFACEMETHODIMP get_IsReadOnly(__RPC__out BOOL* pRetVal);

    virtual IFACEMETHODIMP get_Maximum(__RPC__out double* pRetVal);

    virtual IFACEMETHODIMP get_Minimum(__RPC__out double* pRetVal);

    virtual IFACEMETHODIMP get_LargeChange(__RPC__out double* pRetVal);

    virtual IFACEMETHODIMP get_SmallChange(__RPC__out double* pRetVal);
protected:
    virtual ~UIAProgressProvider();
};

}

#endif

#endif