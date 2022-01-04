#include "StdAfx.h"

namespace ui 
{

ListBox::ListBox(Layout* pLayout) : 
	ScrollableBox(pLayout),
	m_bScrollSelect(false),
	m_iCurSel(-1),
	m_pCompareFunc(nullptr),
	m_pCompareData(NULL),
	m_bSelNextWhenRemoveActive(true)
{

}

std::wstring ListBox::GetType() const
{
	return DUI_CTR_LISTBOX;
}

UIAControlProvider* ListBox::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIAListBoxProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

void ListBox::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if( strName == _T("scrollselect") ) {
		SetScrollSelect(strValue == _T("true"));
	}
	else {
		ScrollableBox::SetAttribute(strName, strValue);
	}
}

void ListBox::HandleMessage(EventArgs& event)
{
	if (!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) {
		if (m_pParent != NULL) m_pParent->HandleMessageTemplate(event);
		else ScrollableBox::HandleMessage(event);
		return;
	}

	switch (event.Type) {
	case kEventMouseButtonDown:
	case kEventMouseButtonUp:
		return;
	case kEventKeyDown:
		switch (event.chKey) {
		case VK_UP:
			SelectItem(FindSelectable(m_iCurSel - 1, false), true);
			return;
		case VK_DOWN:
			SelectItem(FindSelectable(m_iCurSel + 1, true), true);
			return;
		case VK_HOME:
			SelectItem(FindSelectable(0, false), true);
			return;
		case VK_END:
			SelectItem(FindSelectable(GetCount() - 1, true), true);
			return;
		}
		break;
	case kEventMouseScrollWheel:
	{
		int detaValue = event.wParam;
		if (detaValue > 0) {
			if (m_bScrollSelect) {
				SelectItem(FindSelectable(m_iCurSel - 1, false), true);
				return;
			}
			break;
		}
		else {
			if (m_bScrollSelect) {
				SelectItem(FindSelectable(m_iCurSel + 1, true), true);
				return;
			}
			break;
		}
	}
	break;
	}

	ScrollableBox::HandleMessage(event);
}

void ListBox::HandleMessageTemplate(EventArgs& event)
{
	ScrollableBox::HandleMessageTemplate(event);
}

int ListBox::GetCurSel() const
{
	return m_iCurSel;
}
void ListBox::SelectNextWhenActiveRemoved(bool bSelectNextItem)
{
	m_bSelNextWhenRemoveActive = bSelectNextItem;
}
bool ListBox::SelectItem(int iIndex, bool bTakeFocus, bool bTrigger)
{
	//if( iIndex == m_iCurSel ) return true;
	int iOldSel = m_iCurSel;
	// We should first unselect the currently selected item
	if (m_iCurSel >= 0) {
		Control* pControl = GetItemAt(m_iCurSel);
		if (pControl != NULL) {
			ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(pControl);
			if (pListItem != NULL) pListItem->OptionTemplate<Box>::Selected(false, bTrigger);
		}

		m_iCurSel = -1;
	}
	if (iIndex < 0) return false;

	Control* pControl = GetItemAt(iIndex);
	if (pControl == NULL) return false;
	if (!pControl->IsVisible()) return false;
	if (!pControl->IsEnabled()) return false;

	ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(pControl);
	if (pListItem == NULL) return false;
	m_iCurSel = iIndex;
	pListItem->OptionTemplate<Box>::Selected(true, bTrigger);

	if (GetItemAt(m_iCurSel)) {
		UiRect rcItem = GetItemAt(m_iCurSel)->GetPos();
		EnsureVisible(rcItem);
	}

	if (bTakeFocus) pControl->SetFocus();
	if (m_pWindow != NULL && bTrigger) {
		m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, iOldSel);
	}

	return true;
}

void ListBox::EnsureVisible(const UiRect& rcItem)
{
	UiRect rcNewItem = rcItem;
	rcNewItem.Offset(-GetScrollPos().cx, -GetScrollPos().cy);
	UiRect rcList = GetPos();
	UiRect rcListInset = m_pLayout->GetPadding();

	rcList.left += rcListInset.left;
	rcList.top += rcListInset.top;
	rcList.right -= rcListInset.right;
	rcList.bottom -= rcListInset.bottom;

	ScrollBar* pHorizontalScrollBar = GetHorizontalScrollBar();
	if (pHorizontalScrollBar && pHorizontalScrollBar->IsVisible()) rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();

	if (rcNewItem.left >= rcList.left && rcNewItem.top >= rcList.top
		&& rcNewItem.right <= rcList.right && rcNewItem.bottom <= rcList.bottom) {
		if (m_pParent && dynamic_cast<ListContainerElement*>(m_pParent) != NULL) {
			dynamic_cast<ListContainerElement*>(m_pParent)->GetOwner()->EnsureVisible(rcNewItem);
		}
		return;
	}

	int dx = 0;
	if (rcNewItem.left < rcList.left) dx = rcNewItem.left - rcList.left;
	if (rcNewItem.right > rcList.right) dx = rcNewItem.right - rcList.right;
	int dy = 0;
	if (rcNewItem.top < rcList.top) dy = rcNewItem.top - rcList.top;
	if (rcNewItem.bottom > rcList.bottom) dy = rcNewItem.bottom - rcList.bottom;

	CSize sz = GetScrollPos();
	SetScrollPos(CSize(sz.cx + dx, sz.cy + dy));
}

void ListBox::StopScroll()
{
	m_scrollAnimation.Reset();
}

bool ListBox::ButtonDown(EventArgs& msg)
{
	bool ret = __super::ButtonDown(msg);
	StopScroll();

	return ret;
}

bool ListBox::ScrollItemToTop(const std::wstring& strItemName)
{
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if ((*it)->GetName() == strItemName) {
			if (GetScrollRange().cy != 0) {
				CSize scrollPos = GetScrollPos();
				scrollPos.cy = (*it)->GetPos().top - m_pLayout->GetInternalPos().top;
				if (scrollPos.cy >= 0) {
					SetScrollPos(scrollPos);
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}

	return false;
}

Control* ListBox::GetTopItem()
{
	int listTop = GetPos().top + m_pLayout->GetPadding().top + GetScrollPos().cy;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if ((*it)->IsVisible() && !(*it)->IsFloat() && (*it)->GetPos().bottom >= listTop) {
			return (*it);
		}
	}

	return nullptr;
}

bool ListBox::SetItemIndex(Control* pControl, std::size_t iIndex)
{
	int iOrginIndex = GetItemIndex(pControl);
	if( iOrginIndex == -1 ) return false;
	if( iOrginIndex == (int)iIndex ) return true;

	ListContainerElement* pSelectedListItem = NULL;
	if( m_iCurSel >= 0 ) pSelectedListItem = dynamic_cast<ListContainerElement*>(GetItemAt(m_iCurSel));
	if( !ScrollableBox::SetItemIndex(pControl, iIndex) ) return false;
	std::size_t iMinIndex = min((std::size_t)iOrginIndex, iIndex);
	std::size_t iMaxIndex = max((std::size_t)iOrginIndex, iIndex);
	for(std::size_t i = iMinIndex; i < iMaxIndex + 1; ++i) {
		Control* pControl = GetItemAt(i);
		ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(pControl);
		if( pListItem != NULL ) {
			pListItem->SetIndex((int)i);
		}
	}
	if( m_iCurSel >= 0 && pSelectedListItem != NULL ) m_iCurSel = pSelectedListItem->GetIndex();
	return true;
}

void ListBox::Previous()
{
	if (m_iCurSel > 0) {
		SelectItem(m_iCurSel - 1);
	}
}

void ListBox::Next()
{
	int count = GetCount();
	if (m_iCurSel < count - 1) {
		SelectItem(m_iCurSel + 1);
	}
}

void ListBox::ActiveItem()
{
	if (m_iCurSel >= 0)	{
		ListContainerElement* item = dynamic_cast<ListContainerElement*>( GetItemAt(m_iCurSel) );
		item->InvokeDoubleClickEvent();
	}
}

bool ListBox::Add(Control* pControl)
{
	// Override the Add() method so we can add items specifically to
	// the intended widgets. Headers are assumed to be
	// answer the correct interface so we can add multiple list headers.
	// The list items should know about us
	ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(pControl);
	if( pListItem != NULL ) {
		pListItem->SetOwner(this);
		pListItem->SetIndex(GetCount());
	}
	return ScrollableBox::Add(pControl);
}

bool ListBox::AddAt(Control* pControl, std::size_t iIndex)
{
	// Override the AddAt() method so we can add items specifically to
	// the intended widgets. Headers and are assumed to be
	// answer the correct interface so we can add multiple list headers.

	if (!ScrollableBox::AddAt(pControl, iIndex)) return false;

	// The list items should know about us
	ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(pControl);
	if( pListItem != NULL ) {
		pListItem->SetOwner(this);
		pListItem->SetIndex(iIndex);
	}

	for(int i = iIndex + 1; i < GetCount(); ++i) {
		Control* p = GetItemAt(i);
		pListItem = dynamic_cast<ListContainerElement*>(p);
		if( pListItem != NULL ) {
			pListItem->SetIndex(i);
		}
	}
	if( m_iCurSel >= iIndex ) m_iCurSel += 1;
	return true;
}

bool ListBox::Remove(Control* pControl)
{
	int iIndex = GetItemIndex(pControl);
	if (iIndex == -1) return false;

	return RemoveAt(iIndex);
}

bool ListBox::RemoveAt(std::size_t iIndex)
{
	if (!ScrollableBox::RemoveAt(iIndex)) return false;

	for(int i = iIndex; i < GetCount(); ++i) {
		Control* p = GetItemAt(i);
		ListContainerElement* pListItem = dynamic_cast<ListContainerElement*>(p);
		if( pListItem != NULL ) pListItem->SetIndex(i);
	}

	if( iIndex == m_iCurSel && m_iCurSel >= 0 ) {
		if (m_bSelNextWhenRemoveActive)
			SelectItem(FindSelectable(m_iCurSel--, false));
		else
			m_iCurSel = -1;
	}
	else if( iIndex < m_iCurSel ) m_iCurSel -= 1;
	return true;
}

void ListBox::RemoveAll()
{
	m_iCurSel = -1;
	ScrollableBox::RemoveAll();
}

bool ListBox::SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData)
{
	if (!pfnCompare)
		return false;
		
	if (m_items.size() == 0)
	{
		return true;
	}

	m_pCompareFunc = pfnCompare;
	m_pCompareData = dwData;
	qsort_s(&(*m_items.begin()), m_items.size(), sizeof(Control*), ListBox::ItemComareFunc, this);	
	ListContainerElement *pItem = NULL;
	for (int i = 0; i < (int)m_items.size(); ++i)
	{
		pItem = dynamic_cast<ListContainerElement*>(static_cast<Control*>(m_items[i]));
		if (pItem) {
			pItem->SetIndex(i);
			pItem->Selected(false, true);
		}
	}
	SelectItem(-1);
	SetPos(GetPos());
	Invalidate();

	return true;
}

int __cdecl ListBox::ItemComareFunc(void *pvlocale, const void *item1, const void *item2)
{
	ListBox *pThis = (ListBox*)pvlocale;
	if (!pThis || !item1 || !item2)
		return 0;
	return pThis->ItemComareFunc(item1, item2);
}

int __cdecl ListBox::ItemComareFunc(const void *item1, const void *item2)
{
	Control *pControl1 = *(Control**)item1;
	Control *pControl2 = *(Control**)item2;
	return m_pCompareFunc((UINT_PTR)pControl1, (UINT_PTR)pControl2, m_pCompareData);
}

bool ListBox::GetScrollSelect()
{
	return m_bScrollSelect;
}

void ListBox::SetScrollSelect(bool bScrollSelect)
{
	m_bScrollSelect = bScrollSelect;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

ListContainerElement::ListContainerElement() :
	m_iIndex(-1),
	m_pOwner(nullptr)
{
	m_uTextStyle = DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_NOCLIP | DT_SINGLELINE;
	// 列表项不处理WM_POINTER消息，可以顺利收到右键菜单消息
	SetReceivePointerMsg(false);
}

std::wstring ListContainerElement::GetType() const
{
	return DUI_CTR_LISTCONTAINERELEMENT;
}

UIAControlProvider* ListContainerElement::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIAListBoxItemProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

void ListContainerElement::SetVisible(bool bVisible)
{
	__super::SetVisible(bVisible);
	if (!IsVisible() && m_bSelected) {
		m_bSelected = false;
		if (m_pOwner != NULL) m_pOwner->SelectItem(-1);
	}
}

void ListContainerElement::Selected(bool bSelected, bool trigger)
{
	if (!IsEnabled()) return;
	if (bSelected && m_pOwner != NULL) m_pOwner->SelectItem(m_iIndex, false, trigger);
}

void ListContainerElement::HandleMessage(EventArgs& event)
{
	if (!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) {
		if (m_pOwner != NULL) m_pOwner->HandleMessageTemplate(event);
		else Box::HandleMessage(event);
		return;
	}
	else if (event.Type == kEventInternalDoubleClick) {
		if (IsActivatable()) {
			InvokeDoubleClickEvent();
		}
		return;
	}
	else if (event.Type == kEventKeyDown && IsEnabled()) {
		if (event.chKey == VK_RETURN) {
			if (IsActivatable()) {
				if (m_pWindow != NULL) m_pWindow->SendNotify(this, kEventReturn);
			}
			return;
		}
	}
	else if (event.Type == kEventInternalMenu && IsEnabled()) {
		Selected(true, true);
		m_pWindow->SendNotify(this, kEventMouseMenu);
		Invalidate();

		return;
	}

	__super::HandleMessage(event);

	// An important twist: The list-item will send the event not to its immediate
	// parent but to the "attached" list. A list may actually embed several components
	// in its path to the item, but key-presses etc. needs to go to the actual list.
	//if( m_pOwner != NULL ) m_pOwner->HandleMessage(event); else Control::HandleMessage(event);
}

IListOwner* ListContainerElement::GetOwner()
{
    return m_pOwner;
}

void ListContainerElement::SetOwner(IListOwner* pOwner)
{
    m_pOwner = pOwner;
}

int ListContainerElement::GetIndex() const
{
    return m_iIndex;
}

void ListContainerElement::SetIndex(int iIndex)
{
    m_iIndex = iIndex;
}
void ListContainerElement::InvokeDoubleClickEvent()
{
	if( m_pWindow != NULL ) m_pWindow->SendNotify(this, kEventMouseDoubleClick);
}

} // namespace ui
