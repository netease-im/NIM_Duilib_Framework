#include "StdAfx.h"

#include <typeinfo>

#if defined(ENABLE_UIAUTOMATION)

namespace ui {

UIAControlProvider::UIAControlProvider(Control* pControl) :m_refCount(1)
{
	m_pControl = pControl;
}

void UIAControlProvider::ResetControl(Control* pControl)
{
	m_pControl = pControl;
}

Control* UIAControlProvider::GetControl() const
{
	return m_pControl;
}


// IUnknown implementation.

IFACEMETHODIMP_(ULONG) UIAControlProvider::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}

IFACEMETHODIMP_(ULONG) UIAControlProvider::Release()
{
	long val = InterlockedDecrement(&m_refCount);
	if (val == 0)
	{
		delete this;
	}
	return val;
}

IFACEMETHODIMP UIAControlProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IUnknown))
		*ppInterface = static_cast<IRawElementProviderSimple*>(this);
	else if (riid == __uuidof(IRawElementProviderSimple))
		*ppInterface = static_cast<IRawElementProviderSimple*>(this);
	else if (riid == __uuidof(IRawElementProviderFragment))
		*ppInterface = static_cast<IRawElementProviderFragment*>(this);
	else
		return E_NOINTERFACE;

	AddRef();

	return S_OK;
}


// IRawElementProviderSimple implementation
//
// Implementation of IRawElementProviderSimple::GetProviderOptions.
//
IFACEMETHODIMP UIAControlProvider::get_ProviderOptions(ProviderOptions* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	// ProviderOptions_UseClientCoordinates constant is new for Windows 8 and 
	// instructs UIA that this provider works in terms of client coordinates rather than screen coordinates.
	// Constant may not be available yet in the SDK, depending on when the SDK was constructed.
	*pRetVal = ProviderOptions_ServerSideProvider |
		ProviderOptions_UseComThreading |
		(ProviderOptions)0x100 /*ProviderOptions_UseClientCoordinates*/;

	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPatternProvider.
// Gets the object that supports the specified pattern.
// ALL pattern types listed here:
// https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-controlpatternmapping
IFACEMETHODIMP UIAControlProvider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	return S_OK;
}

// Implementation of IRawElementProviderSimple::GetPropertyValue.
// Gets custom properties.
// https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-entry-propids
// https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-automation-element-propids
IFACEMETHODIMP UIAControlProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_CulturePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = 0x0409; // en-US
		break;
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_CustomControlTypeId;
		break;
	case UIA_LocalizedControlTypePropertyId: // only lower string
		pRetVal->bstrVal = SysAllocString(StringHelper::MakeLowerString(m_pControl->GetType()).c_str());
		if (pRetVal->bstrVal != nullptr)
		{
			pRetVal->vt = VT_BSTR;
		}
		break;
	case UIA_NamePropertyId:
		if (!m_pControl->GetName().empty()) {
			pRetVal->bstrVal = SysAllocString(m_pControl->GetName().c_str());
			if (pRetVal->bstrVal != nullptr)
			{
				pRetVal->vt = VT_BSTR;
			}
		}
		break;
	case UIA_AutomationIdPropertyId:
		if (!m_pControl->GetName().empty()) {
			pRetVal->bstrVal = SysAllocString(m_pControl->GetName().c_str());
			if (pRetVal->bstrVal != nullptr)
			{
				pRetVal->vt = VT_BSTR;
			}
		}
		break;
	case UIA_HelpTextPropertyId:
		if (!m_pControl->GetToolTipText().empty()) {
			pRetVal->bstrVal = SysAllocString(m_pControl->GetToolTipText().c_str());
			if (pRetVal->bstrVal != nullptr)
			{
				pRetVal->vt = VT_BSTR;
			}
		}
		break;
	case UIA_IsEnabledPropertyId:
		pRetVal->boolVal = m_pControl->IsEnabled() ? VARIANT_TRUE : VARIANT_FALSE;
		pRetVal->vt = VT_BOOL;
		break;
	case UIA_IsKeyboardFocusablePropertyId:
		pRetVal->boolVal = m_pControl->IsKeyboardEnabled() ? VARIANT_TRUE : VARIANT_FALSE;
		pRetVal->vt = VT_BOOL;
		break;
	case UIA_HasKeyboardFocusPropertyId:
		pRetVal->boolVal = m_pControl->IsFocused() && !m_pControl->IsMouseFocused() ?
			VARIANT_TRUE : VARIANT_FALSE;
		pRetVal->vt = VT_BOOL;
		break;

		// custom properties
	case UIA_ItemStatusPropertyId:
		pRetVal->vt = VT_EMPTY; // should be busy or connected with VT_BSTR type
		break;
		// view types
		// https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-treeoverview
	case UIA_IsContentElementPropertyId:
	case UIA_IsControlElementPropertyId:
		pRetVal->boolVal = VARIANT_TRUE;
		pRetVal->vt = VT_BOOL;
		break;
	case UIA_IsPasswordPropertyId:
		pRetVal->boolVal = VARIANT_FALSE;
		pRetVal->vt = VT_BOOL;
		break;
	case UIA_IsOffscreenPropertyId:
		pRetVal->boolVal = m_pControl->IsVisible() ? VARIANT_FALSE : VARIANT_TRUE;
		pRetVal->vt = VT_BOOL;
		break;
	case 30162://UIA_FillTypePropertyId: // uch as none, color, gradient, picture, pattern, and so on.
		pRetVal->vt = VT_I4;
		pRetVal->lVal = 0;
		break;
	case 30160://UIA_FillColorPropertyId:
	case 30161://UIA_OutlineColorPropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = 0x000000;
		break;
	default:
		break;
	}

	return S_OK;
}

// Implementation of IRawElementProviderSimple::get_HostRawElementProvider.
// Gets the UI Automation provider for the host window. 
// Return nullptr.
//
IFACEMETHODIMP UIAControlProvider::get_HostRawElementProvider(IRawElementProviderSimple** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	return S_OK;
}


// IRawElementProviderFragment implementation.
//
// Implementation of IRawElementProviderFragment::Navigate.
// Enables UI Automation to locate the element in the tree.
//
IFACEMETHODIMP UIAControlProvider::Navigate(NavigateDirection direction, IRawElementProviderFragment** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pParent = m_pControl->GetParent();
	auto pBox = dynamic_cast<Box*>(m_pControl);
	*pRetVal = nullptr;

	switch (direction)
	{
	case NavigateDirection_Parent:
	{
		if (pParent)
			*pRetVal = static_cast<IRawElementProviderFragment*>(pParent->GetUIAProvider());
		else {
			Window* pRootWindow = m_pControl->GetWindow();
			if (pRootWindow) {
				*pRetVal = static_cast<IRawElementProviderFragment*>(pRootWindow->GetUIAProvider());
			}
		}
	}
	break;

	case NavigateDirection_NextSibling:
	{
		if (pParent) {
			auto index = pParent->GetItemIndex(m_pControl);
			auto nextSiblingControl = pParent->GetItemAt(index + 1);

			if (nextSiblingControl)
				*pRetVal = static_cast<IRawElementProviderFragment*>(nextSiblingControl->GetUIAProvider());
		}
	}
	break;

	case NavigateDirection_PreviousSibling:
	{
		if (pParent) {
			auto index = pParent->GetItemIndex(m_pControl);
			auto preSiblingControl = pParent->GetItemAt(index - 1);
			if (preSiblingControl)
				*pRetVal = static_cast<IRawElementProviderFragment*>(preSiblingControl->GetUIAProvider());
		}
	}
	break;

	case NavigateDirection_FirstChild:
	{
		if (pBox && pBox->GetItemAt(0))
			*pRetVal = static_cast<IRawElementProviderFragment*>(pBox->GetItemAt(0)->GetUIAProvider());
	}
	break;

	case NavigateDirection_LastChild:
		if (pBox && pBox->GetItemAt(pBox->GetCount() - 1))
			*pRetVal = static_cast<IRawElementProviderFragment*>(pBox->GetItemAt(pBox->GetCount() - 1)->GetUIAProvider());
		break;

	default:
		break;
	}

	if (*pRetVal)
		(*pRetVal)->AddRef();


	return S_OK;
}

// Implementation of IRawElementProviderFragment::GetRuntimeId.
// Gets the runtime identifier. This is an array consisting of UiaAppendRuntimeId, 
// which makes the ID unique among instances of the control, and the Automation Id.
//
IFACEMETHODIMP UIAControlProvider::GetRuntimeId(SAFEARRAY** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	int nIndex = 0;
	int nParentIndex = 0;

	std::vector<int> indexs;

	auto pParent = m_pControl->GetParent();
	if (pParent) {
		nIndex = pParent->GetItemIndex(m_pControl);
		indexs.emplace_back(nIndex);

		while (pParent->GetParent()) {
			nParentIndex = pParent->GetParent()->GetItemIndex(pParent);
			indexs.emplace_back(nParentIndex);

			pParent = pParent->GetParent();
		}
	}

	//push root box index, always 1
	indexs.emplace_back(1);

	//push uia id
	indexs.emplace_back(UiaAppendRuntimeId);

	*pRetVal = SafeArrayCreateVector(VT_I4, 0, indexs.size());
	if (*pRetVal != nullptr)
	{
		for (long n = 0; n < indexs.size(); n++)
		{
			SafeArrayPutElement(*pRetVal, &n, &indexs.at(n));
		}
	}
	else
		return E_OUTOFMEMORY;

	return S_OK;
}

// Implementation of IRawElementProviderFragment::get_BoundingRectangle.
// Gets the bounding rectangle of the item, in screen coordinates.
//
IFACEMETHODIMP UIAControlProvider::get_BoundingRectangle(UiaRect* pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	UiRect rc = m_pControl->GetPosWithScrollOffset();

	Window* pRootWindow = m_pControl->GetWindow();
	if (pRootWindow) {
		UiRect rc_window = pRootWindow->GetPos();
		UiRect rc_shadow = pRootWindow->GetShadowCorner();

		rc.Offset(rc_window.left, rc_window.top);
		rc.Offset(-rc_shadow.left, -rc_shadow.top);
	}

	pRetVal->left = rc.left;
	pRetVal->top = rc.top;
	pRetVal->width = rc.GetWidth();
	pRetVal->height = rc.GetHeight();

	return S_OK;
}


// Implementation of IRawElementProviderFragment::GetEmbeddedFragmentRoots.
// Retrieves any fragment roots that may be hosted in this element.
//
IFACEMETHODIMP UIAControlProvider::GetEmbeddedFragmentRoots(SAFEARRAY** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	return S_OK;
}

// Implementation of IRawElementProviderFragment::SetFocus.
// Responds to the control receiving focus through a UI Automation request.
//
IFACEMETHODIMP UIAControlProvider::SetFocus()
{
	UIA_CHECK_ELEMENT(m_pControl);

	m_pControl->SetFocus();

	return S_OK;
}

// Implementation of IRawElementProviderFragment::get_FragmentRoot.
// Retrieves the root element of this fragment.
//
IFACEMETHODIMP UIAControlProvider::get_FragmentRoot(IRawElementProviderFragmentRoot** pRetVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*pRetVal = nullptr;

	Window* pRootWindow = m_pControl->GetWindow();
	if (pRootWindow) {
		*pRetVal = static_cast<IRawElementProviderFragmentRoot*>(pRootWindow->GetUIAProvider());
		(*pRetVal)->AddRef();
	}
	else
		return E_FAIL;

	return S_OK;
}
}

#endif