#include "StdAfx.h"

#if defined(ENABLE_UIAUTOMATION)

namespace ui
{

UIAScrollableBoxProvider::UIAScrollableBoxProvider(Control* pControl)
	:UIABoxProvider(pControl)
{
}

IFACEMETHODIMP UIAScrollableBoxProvider::QueryInterface(REFIID riid, void** ppInterface)
{
	*ppInterface = nullptr;

	if (riid == __uuidof(IScrollProvider))
		*ppInterface = static_cast<IScrollProvider*>(this);
	else
		return __super::QueryInterface(riid, ppInterface);

	AddRef();

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::GetPatternProvider(PATTERNID iid, IUnknown** retVal)
{
	UIA_CHECK_ELEMENT(m_pControl);

	*retVal = nullptr;

	if (iid == UIA_ScrollPatternId)
		*retVal = static_cast<IScrollProvider*>(this);
	else
		return __super::GetPatternProvider(iid, retVal);

	AddRef();


	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
	pRetVal->vt = VT_EMPTY;
	UIA_CHECK_ELEMENT(m_pControl);

	switch (propertyId)
	{
	case UIA_ControlTypePropertyId:
		pRetVal->vt = VT_I4;
		pRetVal->lVal = UIA_ListControlTypeId;
		break;
	default:
		return __super::GetPropertyValue(propertyId, pRetVal);
		break;
	}

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::Scroll(ScrollAmount horizontalAmount, ScrollAmount verticalAmount)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox) {

		auto cyLine = pScrollableBox->GetVerScrollUnitPixels() / 2;
		auto cxLine = pScrollableBox->GetHorScrollUnitPixels() / 2;

		switch (horizontalAmount)
		{
		case ScrollAmount_LargeDecrement:
		case ScrollAmount_LargeIncrement:
			cxLine *= 2;
			break;
		case ScrollAmount_SmallDecrement:
		case ScrollAmount_SmallIncrement:
			break;
		case ScrollAmount_NoAmount:
			break;
		default:
			break;
		}

		switch (verticalAmount)
		{
		case ScrollAmount_LargeDecrement:
		case ScrollAmount_LargeIncrement:
			cyLine *= 2;
			break;
		case ScrollAmount_SmallDecrement:
		case ScrollAmount_SmallIncrement:
			break;
		case ScrollAmount_NoAmount:
			break;
		default:
			break;
		}

		// decrement to up default
		if (verticalAmount == ScrollAmount_LargeDecrement || verticalAmount == ScrollAmount_SmallDecrement)
			pScrollableBox->LineUp(cyLine);
		else if(verticalAmount == ScrollAmount_LargeIncrement || verticalAmount == ScrollAmount_SmallIncrement)
			pScrollableBox->LineDown(cyLine);

		// decrement to left default
		if (horizontalAmount == ScrollAmount_LargeDecrement || horizontalAmount == ScrollAmount_SmallDecrement)
			pScrollableBox->LineLeft(cxLine);
		else if (horizontalAmount == ScrollAmount_LargeIncrement || horizontalAmount == ScrollAmount_SmallIncrement)
			pScrollableBox->LineRight(cxLine);
	}

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::SetScrollPercent(double horizontalPercent, double verticalPercent)
{
	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox) {
		auto scrollRange = pScrollableBox->GetScrollRange();
		pScrollableBox->SetScrollPos({
			(int)(static_cast<double>(scrollRange.cx) * horizontalPercent),
			(int)(static_cast<double>(scrollRange.cy) * verticalPercent)
			});
	}

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::get_HorizontalScrollPercent(double* pRetVal)
{
	*pRetVal = 0.0f;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox) {
		*pRetVal = static_cast<double>(pScrollableBox->GetScrollPos().cx) /
			static_cast<double>(pScrollableBox->GetScrollRange().cx);
	}

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::get_VerticalScrollPercent(double* pRetVal)
{
	*pRetVal = 0.0f;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox) {
		*pRetVal = static_cast<double>(pScrollableBox->GetScrollPos().cy) /
			static_cast<double>(pScrollableBox->GetScrollRange().cy);
	}

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::get_HorizontalViewSize(double* pRetVal)
{
	*pRetVal = 0.0f;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox)
		*pRetVal = pScrollableBox->GetScrollRange().cx;

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::get_VerticalViewSize(double* pRetVal)
{
	*pRetVal = 0.0f;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox)
		*pRetVal = pScrollableBox->GetScrollRange().cy;

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::get_HorizontallyScrollable(BOOL* pRetVal)
{
	*pRetVal = FALSE;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox)
		*pRetVal = pScrollableBox->IsHScrollBarValid();

	return S_OK;
}

IFACEMETHODIMP UIAScrollableBoxProvider::get_VerticallyScrollable(BOOL* pRetVal)
{
	*pRetVal = FALSE;

	UIA_CHECK_ELEMENT(m_pControl);

	auto pScrollableBox = dynamic_cast<ScrollableBox*>(m_pControl);
	if (pScrollableBox)
		*pRetVal = pScrollableBox->IsVScrollBarValid();

	return S_OK;
}

}

#endif