#ifndef UI_AUTOMATION_UIAWINDOWPROVIDER_H_
#define UI_AUTOMATION_UIAWINDOWPROVIDER_H_

#pragma once

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

class Window;

class UIAWindowProvider : public IRawElementProviderSimple,
	public IRawElementProviderFragment,
	public IRawElementProviderFragmentRoot,
	public IWindowProvider,
	public ITransformProvider
{
public:
	// Constructor/destructor.
	UIAWindowProvider(Window* pWindow);

	void ResetWindow(Window* pWindow = nullptr);

	// IUnknown methods
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

	// IRawElementProviderSimple methods
	IFACEMETHODIMP get_ProviderOptions(ProviderOptions* pRetVal);
	IFACEMETHODIMP GetPatternProvider(PATTERNID iid, IUnknown** pRetVal);
	IFACEMETHODIMP GetPropertyValue(PROPERTYID idProp, VARIANT* pRetVal);
	IFACEMETHODIMP get_HostRawElementProvider(IRawElementProviderSimple** pRetVal);

	// IRawElementProviderFragment methods
	IFACEMETHODIMP Navigate(NavigateDirection direction, IRawElementProviderFragment** pRetVal);
	IFACEMETHODIMP GetRuntimeId(SAFEARRAY** pRetVal);
	IFACEMETHODIMP get_BoundingRectangle(UiaRect* pRetVal);
	IFACEMETHODIMP GetEmbeddedFragmentRoots(SAFEARRAY** pRetVal);
	IFACEMETHODIMP SetFocus();
	IFACEMETHODIMP get_FragmentRoot(IRawElementProviderFragmentRoot** pRetVal);

	// IRawElementProviderFragmenRoot methods
	IFACEMETHODIMP ElementProviderFromPoint(double x, double y, IRawElementProviderFragment** pRetVal);
	IFACEMETHODIMP GetFocus(IRawElementProviderFragment** pRetVal);

	// IWindowProvider
	IFACEMETHODIMP SetVisualState(enum WindowVisualState state);
	IFACEMETHODIMP Close(void);
	IFACEMETHODIMP WaitForInputIdle(int milliseconds,__RPC__out BOOL* pRetVal);
	IFACEMETHODIMP get_CanMaximize(__RPC__out BOOL* pRetVal);
	IFACEMETHODIMP get_CanMinimize(__RPC__out BOOL* pRetVal);
	IFACEMETHODIMP get_IsModal(__RPC__out BOOL* pRetVal);
	IFACEMETHODIMP get_WindowVisualState(__RPC__out enum WindowVisualState* pRetVal);
	IFACEMETHODIMP get_WindowInteractionState(__RPC__out enum WindowInteractionState* pRetVal);
	IFACEMETHODIMP get_IsTopmost(__RPC__out BOOL* pRetVal);


	// ITransformProvider
	IFACEMETHODIMP Move(double x,double y);
	IFACEMETHODIMP Resize(double width,double height);
	IFACEMETHODIMP Rotate(double degrees);
	IFACEMETHODIMP get_CanMove(__RPC__out BOOL* pRetVal);
	IFACEMETHODIMP get_CanResize(__RPC__out BOOL* pRetVal);
	IFACEMETHODIMP get_CanRotate(__RPC__out BOOL* pRetVal);

private:
	virtual ~UIAWindowProvider();

private:
	// Ref counter for this COM object.
	ULONG m_refCount;

	HWND m_hWnd;
	Window* m_pWnd;
};

}

#endif


#endif