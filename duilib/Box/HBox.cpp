#include "stdafx.h"
#include "HBox.h"

namespace ui
{

HLayout::HLayout()
{

}

CSize HLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	// Determine the width of elements that are sizeable
	CSize szAvailable(rc.right - rc.left, rc.bottom - rc.top);

	int nAdjustables = 0;
	int cxFixed = 0;
	int nEstimateNum = 0;

	for( auto it = items.begin(); it != items.end(); it++ ) {
		auto pControl = *it;
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;
		CSize sz = pControl->EstimateSize(szAvailable);
		if( sz.cx == DUI_LENGTH_STRETCH ) {
			nAdjustables++;
			cxFixed += pControl->GetMargin().left + pControl->GetMargin().right;
		}
		else {
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( pControl->GetMaxWidth() >= 0 && sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			cxFixed += sz.cx +  pControl->GetMargin().left + pControl->GetMargin().right;
		}
			
		nEstimateNum++;
	}
	cxFixed += (nEstimateNum - 1) * m_iChildMargin;

	int cxExpand = 0;
	int cxNeeded = 0;
	if( nAdjustables > 0 ) cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
	int deviation = szAvailable.cx - cxFixed - cxExpand * nAdjustables;
	// Position the elements
	CSize szRemaining = szAvailable;
	int iPosX = rc.left;
	int iPosTop = rc.top;
	int iPosBottom = rc.bottom;
	int iAdjustable = 0;
	int max_height = 0;

	for( auto it = items.begin(); it != items.end(); it++ ) {
		auto pControl = *it;
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(pControl, rc);
			continue;
		}

		UiRect rcMargin = pControl->GetMargin();
		szRemaining.cx -= rcMargin.left;
		CSize sz = pControl->EstimateSize(szRemaining);
		if( sz.cx == DUI_LENGTH_STRETCH ) {
			iAdjustable++;
			sz.cx = cxExpand;
			if (deviation > 0) {
				sz.cx += 1;
				deviation--;
			}
		}
		if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
		if( pControl->GetMaxWidth() >= 0 && sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

		if( sz.cy == DUI_LENGTH_STRETCH ) sz.cy = szAvailable.cy - rcMargin.top - rcMargin.bottom;
		if( sz.cy < 0 ) sz.cy = 0;
		if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
		if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();

		int childTop = 0;
		int childBottm = 0;
		VerAlignType verAlignType = pControl->GetVerAlignType();
		if (verAlignType == kVerAlignTop) {
			childTop = iPosTop + rcMargin.top;
			childBottm = childTop + sz.cy;
		}
		else if (verAlignType == kVerAlignBottom) {
			childBottm = iPosBottom - rcMargin.bottom;
			childTop = childBottm - sz.cy;
		}
		else if (verAlignType == kVerAlignCenter) {
			childTop = iPosTop + (iPosBottom - iPosTop + rcMargin.top - rcMargin.bottom - sz.cy) / 2;
			childBottm = childTop + sz.cy;
		}

		UiRect rcChildPos(iPosX + rcMargin.left, childTop, iPosX + rcMargin.left + sz.cx, childBottm);
		max_height = MAX(max_height, rcChildPos.GetHeight());
		pControl->SetPos(rcChildPos);
		iPosX += sz.cx + m_iChildMargin + rcMargin.left + rcMargin.right;
		cxNeeded += sz.cx + rcMargin.left + rcMargin.right;
		szRemaining.cx -= sz.cx + m_iChildMargin + rcMargin.right;
	}
	cxNeeded += (nEstimateNum - 1) * m_iChildMargin;

	CSize size(cxNeeded, max_height);
	return size;
}

CSize HLayout::AjustSizeByChild(const std::vector<Control*>& items, CSize szAvailable)
{
	CSize totalSize;
	CSize itemSize;
	Control* pChildControl = NULL;
	int count = (int)items.size();
	int estimateChildCount = 0;

	for(int it = 0; it < count; it++) 
	{
		pChildControl = (Control*)items[it];
		if (!pChildControl->IsVisible() || pChildControl->IsFloat()){
			continue;
		}

		estimateChildCount++;
		itemSize = pChildControl->EstimateSize(szAvailable);
		if( itemSize.cx < pChildControl->GetMinWidth() ) itemSize.cx = pChildControl->GetMinWidth();
		if( pChildControl->GetMaxWidth() >= 0 && itemSize.cx > pChildControl->GetMaxWidth() ) itemSize.cx = pChildControl->GetMaxWidth();
		if( itemSize.cy < pChildControl->GetMinHeight() ) itemSize.cy = pChildControl->GetMinHeight();
		if( itemSize.cy > pChildControl->GetMaxHeight() ) itemSize.cy = pChildControl->GetMaxHeight();
		totalSize.cx += itemSize.cx + pChildControl->GetMargin().left + pChildControl->GetMargin().right;
		totalSize.cy = MAX(itemSize.cy + pChildControl->GetMargin().top + pChildControl->GetMargin().bottom, totalSize.cy);
	}

	if (estimateChildCount - 1 > 0) {
		totalSize.cx += (estimateChildCount - 1) * m_iChildMargin;
	}
	totalSize.cx += m_rcPadding.left + m_rcPadding.right;
	totalSize.cy += m_rcPadding.top + m_rcPadding.bottom;
	return totalSize;
}

HBox::HBox() : Box(new HLayout())
{

}

std::wstring HBox::GetType() const
{
	return DUI_CTR_HBOX;
}

}
