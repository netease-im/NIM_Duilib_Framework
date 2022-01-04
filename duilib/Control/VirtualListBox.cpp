// Copyright 2017, NetEase (Hangzhou) Network Co., Ltd. All rights reserved.
//
// zqw
// 2015/7/22
//
// VirtualListBox class
#include "stdafx.h"
#include "VirtualListBox.h"

namespace ui
{

CSize VirtualVLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	CSize sz(rc.GetWidth(), 0);

	VirtualListBox *pList = dynamic_cast<VirtualListBox*>(m_pOwner);
	ASSERT(pList);

	if (pList->UseDefaultLayout()) {
		sz = VLayout::ArrangeChild(items, rc);
	}
	else {
		sz.cy = pList->CalcElementsHeight(-1);
		pList->ReArrangeChild(false);
	}

	return sz;
}

CSize VirtualHLayout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	CSize sz(0, rc.GetHeight());

	VirtualListBox *pList = dynamic_cast<VirtualListBox*>(m_pOwner);
	ASSERT(pList);

	if (pList->UseDefaultLayout()) {
		sz = HLayout::ArrangeChild(items, rc);
	}
	else {
		sz.cx = pList->CalcElementsHeight(-1);
		pList->ReArrangeChild(false);
	}

	return sz;
}

VirtualListBox::VirtualListBox(ui::Layout* pLayout) :
ui::ListBox(pLayout),
m_pDataProvider(nullptr),
m_nElementHeight(0),
m_nMaxItemCount(0),
m_nOldYScrollPos(0),
m_bArrangedOnce(false),
m_bForceArrange(false)
{
	SetDirection(kListVertical);
}

std::wstring VirtualListBox::GetType() const
{
    return DUI_CTR_VIRTUALLISTBOX;
}

void VirtualListBox::SetDataProvider(VirtualListInterface *pProvider)
{
	ASSERT(pProvider);
	m_pDataProvider = pProvider;
}

void VirtualListBox::SetElementHeight(int nHeight)
{
	m_nElementHeight = nHeight;
}

void VirtualListBox::InitElement(int nMaxItemCount)
{
	ASSERT(m_pDataProvider);
	ASSERT(m_nElementHeight);
	m_nMaxItemCount = nMaxItemCount;

	int nCount = GetElementCount();
	if (nCount > nMaxItemCount)
		nCount = nMaxItemCount;

	for (int i = 0; i < nCount; i++) {
		Control *pControl = CreateElement();
		this->Add(pControl);
		FillElement(pControl, i);
	}
}

void VirtualListBox::SetDirection(ListDirection direction)
{
	m_eDirection = direction;
	ReSetLayout(kListVertical == direction ? dynamic_cast<Layout*>(new VirtualVLayout) : dynamic_cast<Layout*>(new VirtualHLayout));
}

void VirtualListBox::Refresh()
{
	int nElementCount = GetElementCount();
	int nItemCount = GetCount();

	if (nItemCount > nElementCount) {
		int n = nItemCount - nElementCount;
		for (int i = 0; i < n; i++)
			this->RemoveAt(0);
	}
	else if (nItemCount < nElementCount) {
		int n = 0;
		if (nElementCount <= m_nMaxItemCount) {
			n = nElementCount - nItemCount;
		}
		else {
			n = m_nMaxItemCount - nItemCount;
		}

		for (int i = 0; i < n; i++) {
			Control *pControl = CreateElement();
			this->Add(pControl);
		}
	}

	if (UseDefaultLayout()) {
		// 刚从虚拟列表转换到普通模式时，存在布局错误的情况（虚拟列表滚动条接近底部，
		// 然后数据减少，变成普通模式）
		if (nItemCount == m_nMaxItemCount) {
			this->GetLayout()->ArrangeChild(m_items, m_rcItem);
		}

		for (int i = 0; i < (int)m_items.size(); i++)
			FillElement(m_items[i], i);
	}
	else {
		ASSERT(nElementCount);
		if (nElementCount <= 0)
			return;

		ReArrangeChild(true);
		Arrange();
	}
}

void VirtualListBox::RemoveAll()
{
	__super::RemoveAll();

	if (m_pVerticalScrollBar)
		m_pVerticalScrollBar->SetScrollPos(0);
	if (m_pHorizontalScrollBar)
		m_pHorizontalScrollBar->SetScrollPos(0);

	m_nOldYScrollPos = 0;
	m_bArrangedOnce = false;
	m_bForceArrange = false;
}

void VirtualListBox::SetForceArrange(bool bForce)
{
	m_bForceArrange = bForce;
}

void VirtualListBox::GetDisplayCollection(std::vector<int>& collection)
{
	collection.clear();

	if (GetCount() == 0)
		return;

	UiRect rcThis = this->GetPos(false);

	int length = (kListVertical == m_eDirection) ? rcThis.GetWidth() : rcThis.GetHeight();
	int scroll_pos = (kListVertical == m_eDirection) ? GetScrollPos().cy : GetScrollPos().cx;
	int min = scroll_pos / m_nElementHeight;
	int max = min + (length / m_nElementHeight);
	int nCount = GetElementCount();
	if (max >= nCount)
		max = nCount - 1;

	for (auto i = min; i <= max; i++)
		collection.push_back(i);
}

void VirtualListBox::EnsureVisible(int iIndex, bool bToTop)
{
	if (iIndex < 0 || iIndex >= GetElementCount())
		return;

	int nPos = (m_eDirection == kListVertical) ? GetScrollPos().cy : GetScrollPos().cx;
	int nTopIndex = nPos / m_nElementHeight;
	int nNewPos = 0;

	if (bToTop) {
		nNewPos = CalcElementsHeight(iIndex);
		if (m_eDirection == kListVertical) {
			if (nNewPos >= GetScrollRange().cy)
				return;
		}
		else {
			if (nNewPos >= GetScrollRange().cx)
				return;
		}
	}
	else {
		if (IsElementDisplay(iIndex))
			return;

		if (iIndex > nTopIndex) {
			// 向下
			int length = CalcElementsHeight(iIndex + 1);
			nNewPos = length - ((m_eDirection == kListVertical) ? m_rcItem.GetHeight() : m_rcItem.GetWidth());
		}
		else {
			// 向上
			nNewPos = CalcElementsHeight(iIndex);
		}
	}
	CSize sz;
	if (m_eDirection == kListVertical) {
		sz = CSize(0, nNewPos);
	}
	else {
		sz = CSize(nNewPos, 0);
	}
	SetScrollPos(sz);
}

void VirtualListBox::ReArrangeChild(bool bForce)
{
	ScrollDirection direction = kScrollUp;
	if (!bForce && !m_bForceArrange) {
		if (!NeedReArrange(direction))
			return;
	}

	int nElementCount = GetElementCount();

	int nTopIndexBottom = 0;
	int nTopIndex = GetTopElementIndex(nTopIndexBottom);

	if (direction == kScrollDown) {
		// 向下滚动
		ui::UiRect rcItem = m_rcItem;
		if (m_eDirection == kListVertical) {
			rcItem.bottom = rcItem.top + nTopIndexBottom;
		}
		else {
			rcItem.right = rcItem.left + nTopIndexBottom;
		}

		for (int i = 0; i < (int)m_items.size(); i++) {
			if (m_eDirection == kListVertical) {
				rcItem.top = rcItem.bottom;
				rcItem.bottom = rcItem.top + m_nElementHeight;
			}
			else {
				rcItem.left = rcItem.right;
				rcItem.right = rcItem.left + m_nElementHeight;
			}

			m_items[i]->SetPos(rcItem);

			int nElementIndex = nTopIndex + i;
			if (nElementIndex < nElementCount)
				FillElement(m_items[i], nElementIndex);
		}
	}
	else {
		// 向上滚动
		int nDisplayCount = ((m_eDirection == kListVertical) ? m_rcItem.GetHeight() : m_rcItem.GetWidth()) / m_nElementHeight + 1;
		int nHideCount = (int)m_items.size() - nDisplayCount;
		if (nHideCount < 0)
			return;

		// 上半部分
		UiRect rcItem = m_rcItem;
		if (m_eDirection == kListVertical) {
			rcItem.top = m_rcItem.top + nTopIndexBottom;
		}
		else {
			rcItem.left = rcItem.left + nTopIndexBottom;
		}
		for (int i = nHideCount - 1; i >= 0; i--) {
			if (m_eDirection == kListVertical) {
				rcItem.bottom = rcItem.top;
				rcItem.top = rcItem.bottom - m_nElementHeight;
			}
			else {
				rcItem.right = rcItem.left;
				rcItem.left = rcItem.right - m_nElementHeight;
			}

			m_items[i]->SetPos(rcItem);

			int nElementIndex = nTopIndex - (nHideCount - i);
			if (nElementIndex >= 0)
				FillElement(m_items[i], nElementIndex);
		}

		// 下半部分
		rcItem = m_rcItem;
		if (m_eDirection == kListVertical) {
			rcItem.bottom = m_rcItem.top + nTopIndexBottom;
		}
		else {
			rcItem.right = rcItem.left + nTopIndexBottom;
		}
		for (int i = nHideCount; i < (int)m_items.size(); i++) {
			if (m_eDirection == kListVertical) {
				rcItem.top = rcItem.bottom;
				rcItem.bottom = rcItem.top + m_nElementHeight;
			}
			else {
				rcItem.left = rcItem.right;
				rcItem.right = rcItem.left + m_nElementHeight;
			}

			m_items[i]->SetPos(rcItem);

			int nElementIndex = nTopIndex + (i - nHideCount);
			if (nElementIndex < nElementCount)
				FillElement(m_items[i], nElementIndex);
		}
	}
}

void VirtualListBox::AddElement(int iIndex)
{
	int nCount = GetElementCount();
	if (nCount <= m_nMaxItemCount) {
		Control *pControl = CreateElement();
		this->AddAt(pControl, iIndex);
		FillElement(pControl, iIndex);
	}
	else {
		ReArrangeChild(true);
		Arrange();
	}
}

void VirtualListBox::RemoveElement(int iIndex)
{
	int nCount = GetElementCount();
	if (nCount < m_nMaxItemCount) {
		this->RemoveAt(iIndex);
	}
	else {
		ReArrangeChild(true);
		Arrange();
	}
}

void VirtualListBox::SetScrollPos(ui::CSize szPos)
{
	m_nOldYScrollPos = (m_eDirection == kListVertical) ? GetScrollPos().cy : GetScrollPos().cx;
	ListBox::SetScrollPos(szPos);

	if (UseDefaultLayout())
		return;

	ReArrangeChild(false);
}

void VirtualListBox::HandleMessage(ui::EventArgs& event)
{
	if (!IsMouseEnabled() && event.Type > ui::kEventMouseBegin && event.Type < ui::kEventMouseEnd) {
		if (m_pParent != NULL)
			m_pParent->HandleMessageTemplate(event);
		else
			ui::ScrollableBox::HandleMessage(event);
		return;
	}

	switch (event.Type) {
	case kEventKeyDown: {
		switch (event.chKey) {
		case VK_UP: {
			OnKeyDown(VK_UP);
			return;
		}
		case VK_DOWN: {
			OnKeyDown(VK_DOWN);
			return;
		}
		case VK_HOME:
			if (m_eDirection == kListVertical)
				SetScrollPosY(0);
			else
				SetScrollPosX(0);
			return;
		case VK_END: {
			if (m_eDirection == kListVertical) {
				SetScrollPosY(GetScrollRange().cy);
			}
			else {
				SetScrollPosX(GetScrollRange().cx);
			}
			
			return;
		}
		}
	}
	case kEventKeyUp: {
		switch (event.chKey) {
		case VK_UP: {
			OnKeyUp(VK_UP);
			return;
		}
		case VK_DOWN: {
			OnKeyUp(VK_DOWN);
			return;
		}
		}
	}
	}

	__super::HandleMessage(event);
}

void VirtualListBox::SetPos(UiRect rc)
{
	bool bChange = false;
	if (!m_rcItem.Equal(rc))
		bChange = true;

	ListBox::SetPos(rc);

	if (bChange) {
		if (UseDefaultLayout())
			return;

		ReArrangeChild(true);
	}
}

void VirtualListBox::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("vertical")) {
		SetDirection(strValue == _T("true") ? kListVertical : kListHorizontal);
	}
	else {
		__super::SetAttribute(strName, strValue);
	}
}

ui::Control* VirtualListBox::CreateElement()
{
	if (m_pDataProvider)
		return m_pDataProvider->CreateElement();

	return nullptr;
}

void VirtualListBox::FillElement(Control *pControl, int iIndex)
{
	if (m_pDataProvider)
		m_pDataProvider->FillElement(pControl, iIndex);
}

int VirtualListBox::GetElementCount()
{
	if (m_pDataProvider)
		return m_pDataProvider->GetElementCount();

	return 0;
}

bool VirtualListBox::UseDefaultLayout()
{
	return GetElementCount() <= GetCount();
}

int VirtualListBox::CalcElementsHeight(int nCount)
{
	if (nCount < 0) nCount = GetElementCount();

	return nCount * m_nElementHeight;
}

int VirtualListBox::GetTopElementIndex(int &bottom)
{
	int nPos = (m_eDirection == kListVertical) ? GetScrollPos().cy : GetScrollPos().cx;

	int iIndex = nPos / m_nElementHeight;
	bottom = iIndex * m_nElementHeight;

	return iIndex;
}

bool VirtualListBox::IsElementDisplay(int iIndex)
{
	if (iIndex < 0)
		return false;

	int nPos = (m_eDirection == kListVertical) ? GetScrollPos().cy : GetScrollPos().cx;
	int nElementPos = CalcElementsHeight(iIndex);
	if (nElementPos >= nPos) {
		int nLength = (m_eDirection == kListVertical) ? this->GetHeight() : this->GetWidth();
		if (nElementPos + m_nElementHeight <= nPos + nLength)
			return true;
	}

	return false;
}

bool VirtualListBox::NeedReArrange(ScrollDirection &direction)
{
	direction = kScrollUp;
	if (!m_bArrangedOnce) {
		m_bArrangedOnce = true;
		return true;
	}

	int nCount = GetCount();
	if (nCount <= 0)
		return false;

	if (GetElementCount() <= nCount)
		return false;

	UiRect rcThis = this->GetPos();
	if (rcThis.GetWidth() <= 0)
		return false;

	int nPos = (m_eDirection == kListVertical) ? GetScrollPos().cy : GetScrollPos().cx; 
	UiRect rcItem;

    // 补救措施
    // 情况一：通讯录列表，一开始不可见，切换后可见，如果提前布局，
    // 则Element宽度为0，因此，必须重新布局；
    // 情况二：写信窗口联系人列表，列表宽度会不断变化，因此，需要在宽度变化后
    // 重新布局，否则，导致最终Element布局时的宽度不正确
    rcItem = m_items[0]->GetPos();
    // modified by zqw, 2016/10/12
    // 针对情况二，解决方法是，列表宽度变化 或者 拖动写信窗口右侧，列表position改变，
    // 此时，在SetPos中强制重新布局
    //if (u.GetWidth() != rect.GetWidth()) {
    //    return true;
    //}

	if (nPos >= m_nOldYScrollPos) {
		// 下
		rcItem = m_items[nCount - 1]->GetPos();
		if (kListVertical == m_eDirection) {
			int nSub = (rcItem.bottom - rcThis.top) - (nPos + rcThis.GetHeight());
			if (nSub < 0) {
				direction = kScrollDown;
				return true;
			}
		}
		else {
			int nSub = (rcItem.right - rcThis.left) - (nPos + rcThis.GetWidth());
			if (nSub < 0) {
				direction = kScrollDown;
				return true;
			}
		}
	}
	else {
		// 上
		rcItem = m_items[0]->GetPos();
		if (kListVertical == m_eDirection) {
			if (nPos < (rcItem.top - rcThis.top)) {
				direction = kScrollUp;
				return true;
			}
		}
		else {
			if (nPos < (rcItem.left - rcThis.left)) {
				direction = kScrollUp;
				return true;
			}
		}
	}

	return false;
}

}