#include "custom_layout.h"

using namespace ui;

ui::CSize CustomLayout::ArrangeChild(const std::vector<ui::Control*>& m_items, ui::UiRect rc)
{
	CSize size;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		Control* pControl = *it;

		// 即使子控件internVisible属性为false，这里也暂时强行设置为true，排版完成后恢复属性。
		// 否则无法排版被隐藏的SessionBox，在任务栏生成缩略图和预览图时需要排版后的SessionBox
		bool visible = pControl->IsInternVisible();
		pControl->SetInternVisible(true);
		CSize new_size = this->SetCustomPos(pControl, rc);
		pControl->SetInternVisible(visible);

		size.cx = max(size.cx, new_size.cx);
		size.cy = max(size.cy, new_size.cy);
	}

	return size;
}

CSize CustomLayout::SetCustomPos(Control* pControl, UiRect containerRect)
{
	int childLeft = 0;
	int childRight = 0;
	int childTop = 0;
	int childBottm = 0;
	UiRect rcMargin = pControl->GetMargin();
	int iPosLeft = containerRect.left + rcMargin.left;
	int iPosRight = containerRect.right - rcMargin.right;
	int iPosTop = containerRect.top + rcMargin.top;
	int iPosBottom = containerRect.bottom - rcMargin.bottom;
	CSize szAvailable(iPosRight - iPosLeft, iPosBottom - iPosTop);
	CSize childSize = pControl->EstimateSize(szAvailable);
	if (pControl->GetFixedWidth() == DUI_LENGTH_AUTO && pControl->GetFixedHeight() == DUI_LENGTH_AUTO
		&& pControl->GetMaxWidth() == DUI_LENGTH_STRETCH) {
		int maxwidth = max(0, szAvailable.cx);
		if (childSize.cx > maxwidth) {
			pControl->SetFixedWidth(maxwidth, false);
			childSize = pControl->EstimateSize(szAvailable);
			pControl->SetFixedWidth(DUI_LENGTH_AUTO, false);
		}
	}
	if (childSize.cx == DUI_LENGTH_STRETCH) {
		childSize.cx = max(0, szAvailable.cx);
	}
	if (childSize.cx < pControl->GetMinWidth()) childSize.cx = pControl->GetMinWidth();
	if (pControl->GetMaxWidth() >= 0 && childSize.cx > pControl->GetMaxWidth()) childSize.cx = pControl->GetMaxWidth();

	if (childSize.cy == DUI_LENGTH_STRETCH) {
		childSize.cy = max(0, szAvailable.cy);
	}
	if (childSize.cy < pControl->GetMinHeight()) childSize.cy = pControl->GetMinHeight();
	if (childSize.cy > pControl->GetMaxHeight()) childSize.cy = pControl->GetMaxHeight();


	int childWidth = childSize.cx;
	int childHeight = childSize.cy;
	HorAlignType horAlignType = pControl->GetHorAlignType();
	VerAlignType verAlignType = pControl->GetVerAlignType();

	if (horAlignType == kHorAlignLeft) {
		childLeft = iPosLeft;
		childRight = childLeft + childWidth;
	}
	else if (horAlignType == kHorAlignRight) {
		childRight = iPosRight;
		childLeft = childRight - childWidth;
	}
	else if (horAlignType == kHorAlignCenter) {
		childLeft = iPosLeft + (iPosRight - iPosLeft - childWidth) / 2;
		childRight = childLeft + childWidth;
	}

	if (verAlignType == kVerAlignTop) {
		childTop = iPosTop;
		childBottm = childTop + childHeight;
	}
	else if (verAlignType == kVerAlignBottom) {
		childBottm = iPosBottom;
		childTop = childBottm - childHeight;
	}
	else if (verAlignType == kVerAlignCenter) {
		childTop = iPosTop + (iPosBottom - iPosTop - childHeight) / 2;
		childBottm = childTop + childHeight;
	}

	UiRect childPos(childLeft, childTop, childRight, childBottm);
	pControl->SetPos(childPos);
	return CSize(childPos.GetWidth(), childPos.GetHeight());
}