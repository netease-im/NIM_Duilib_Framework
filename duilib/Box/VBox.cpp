#include "stdafx.h"
#include "VBox.h"

namespace ui
{

VLayout::VLayout()
{

}

CSize VLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	// Determine the minimum size
	CSize szAvailable(rc.right - rc.left, rc.bottom - rc.top);

	int nAdjustables = 0;
	int cyFixed = 0;
	int nEstimateNum = 0;
	for( auto it = items.begin(); it != items.end(); it++ ) {
		auto pControl = *it;
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;
		CSize sz = pControl->EstimateSize(szAvailable);
		if( sz.cy == DUI_LENGTH_STRETCH ) {
			nAdjustables++;
			cyFixed += pControl->GetMargin().top + pControl->GetMargin().bottom;
		}
		else {
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			cyFixed += sz.cy + pControl->GetMargin().top + pControl->GetMargin().bottom;
		}
			
		nEstimateNum++;
	}
	cyFixed += (nEstimateNum - 1) * m_iChildMargin;

	// Place elements
	int cyNeeded = 0;
	int cyExpand = 0;
	if( nAdjustables > 0 ) cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
	int deviation = szAvailable.cy - cyFixed - cyExpand * nAdjustables;
	// Position the elements
	CSize szRemaining = szAvailable;
	int iPosLeft = rc.left;
	int iPosRight = rc.right;
	int iPosY = rc.top;
	int iAdjustable = 0;
	int max_width = 0;

	for( auto it = items.begin(); it != items.end(); it++ ) {
		auto pControl = *it;
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(pControl, rc);
			continue;
		}

		UiRect rcMargin = pControl->GetMargin();
		szRemaining.cy -= rcMargin.top;
		CSize sz = pControl->EstimateSize(szRemaining);
		if( sz.cy == DUI_LENGTH_STRETCH ) {
			iAdjustable++;
			sz.cy = cyExpand;
			if (deviation > 0) {
				sz.cy += 1;
				deviation--;
			}
		}
		if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();
		if (sz.cy > pControl->GetMaxHeight()) sz.cy = pControl->GetMaxHeight();
			
		if( sz.cx == DUI_LENGTH_STRETCH ) sz.cx = szAvailable.cx - rcMargin.left - rcMargin.right;
		if( sz.cx < 0 ) sz.cx = 0;
		if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
		if( pControl->GetMaxWidth() >= 0 && sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

		int childLeft = 0;
		int childRight = 0;
		HorAlignType horAlignType = pControl->GetHorAlignType();
		if (horAlignType == kHorAlignLeft) {
			childLeft = iPosLeft + rcMargin.left;
			childRight = childLeft + sz.cx;
		}
		else if (horAlignType == kHorAlignRight) {
			childRight = iPosRight - rcMargin.right;
			childLeft = childRight - sz.cx;
		}
		else if (horAlignType == kHorAlignCenter) {
			childLeft = iPosLeft + (iPosRight - iPosLeft + rcMargin.left - rcMargin.right - sz.cx) / 2;
			childRight = childLeft + sz.cx;
		}

		UiRect rcCtrl(childLeft, iPosY + rcMargin.top, childRight, iPosY + rcMargin.top + sz.cy);
		max_width = MAX(max_width, rcCtrl.GetWidth());
		pControl->SetPos(rcCtrl);

		iPosY += sz.cy + m_iChildMargin + rcMargin.top + rcMargin.bottom;
		cyNeeded += sz.cy + rcMargin.top + rcMargin.bottom;
		szRemaining.cy -= sz.cy + m_iChildMargin + rcMargin.bottom;
	}
	cyNeeded += (nEstimateNum - 1) * m_iChildMargin;

	CSize size(max_width, cyNeeded);
	return size;
}

CSize VLayout::AjustSizeByChild(const std::vector<Control*>& items, CSize szAvailable)
{
	CSize totalSize;
	CSize itemSize;
	Control* pChildControl = NULL;
	int count = (int)items.size();
	int estimateChildCount = 0;
	for(int it = 0; it < count; it++) 
	{
		pChildControl = (Control*)items[it];
		if (!pChildControl->IsVisible() || pChildControl->IsFloat()) {
			continue;
		}

		estimateChildCount++;
		itemSize = pChildControl->EstimateSize(szAvailable);
		if( itemSize.cx < pChildControl->GetMinWidth() ) itemSize.cx = pChildControl->GetMinWidth();
		if( pChildControl->GetMaxWidth() >= 0 && itemSize.cx > pChildControl->GetMaxWidth() ) itemSize.cx = pChildControl->GetMaxWidth();
		if( itemSize.cy < pChildControl->GetMinHeight() ) itemSize.cy = pChildControl->GetMinHeight();
		if( itemSize.cy > pChildControl->GetMaxHeight() ) itemSize.cy = pChildControl->GetMaxHeight();
		totalSize.cx = MAX(itemSize.cx + pChildControl->GetMargin().left + pChildControl->GetMargin().right, totalSize.cx);
		totalSize.cy += itemSize.cy + pChildControl->GetMargin().top + pChildControl->GetMargin().bottom;
	}

	if (estimateChildCount - 1 > 0) {
		totalSize.cy += (estimateChildCount - 1) * m_iChildMargin;
	}
	totalSize.cx += m_rcPadding.left + m_rcPadding.right;
	totalSize.cy += m_rcPadding.top + m_rcPadding.bottom;
	return totalSize;
}

VBox::VBox() : Box(new VLayout())
{

}

std::wstring VBox::GetType() const
{
	return DUI_CTR_VBOX;
}

}
