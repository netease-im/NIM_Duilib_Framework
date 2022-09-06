#include "StdAfx.h"

namespace ui
{
Layout::Layout() :
	m_rcPadding(0, 0, 0, 0),
	m_iChildMargin(0),
	m_pOwner(nullptr)
{

}

void Layout::SetOwner(Box* pOwner)
{
	m_pOwner = pOwner;
}

CSize Layout::SetFloatPos(Control* pControl, UiRect rcContainer)
{
	if (!pControl->IsVisible()) return CSize();

	int childLeft = 0;
	int childRight = 0;
	int childTop = 0;
	int childBottm = 0;
	UiRect rcMargin = pControl->GetMargin();
	int iPosLeft = rcContainer.left + rcMargin.left;
	int iPosRight = rcContainer.right - rcMargin.right;
	int iPosTop = rcContainer.top + rcMargin.top;
	int iPosBottom = rcContainer.bottom - rcMargin.bottom;
	CSize szAvailable(iPosRight - iPosLeft, iPosBottom - iPosTop);
	CSize childSize = pControl->EstimateSize(szAvailable);
	if (pControl->GetFixedWidth() == DUI_LENGTH_AUTO && pControl->GetFixedHeight() == DUI_LENGTH_AUTO
		&& pControl->GetMaxWidth() == DUI_LENGTH_STRETCH) {
		int maxwidth = MAX(0, szAvailable.cx);
		if (childSize.cx > maxwidth) {
			pControl->SetFixedWidth(maxwidth, false);
			childSize = pControl->EstimateSize(szAvailable);
			pControl->SetFixedWidth(DUI_LENGTH_AUTO, false);
		}
	}
	if (childSize.cx == DUI_LENGTH_STRETCH) {
		childSize.cx = MAX(0, szAvailable.cx);
	}
	if (childSize.cx < pControl->GetMinWidth()) childSize.cx = pControl->GetMinWidth();
	if (pControl->GetMaxWidth() >= 0 && childSize.cx > pControl->GetMaxWidth()) childSize.cx = pControl->GetMaxWidth();

	if (childSize.cy == DUI_LENGTH_STRETCH) {
		childSize.cy = MAX(0, szAvailable.cy);
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

bool Layout::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	bool hasAttribute = true;
	if( strName == _T("padding") ) {
		UiRect rcPadding;
		LPTSTR pstr = NULL;
		rcPadding.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
		rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		SetPadding(rcPadding);
	}
	else if( strName == _T("childmargin") ) {
		SetChildMargin(_ttoi(strValue.c_str()));
	}
	else {
		hasAttribute = false;
	}

	return hasAttribute;
}

CSize Layout::ArrangeChild(const std::vector<Control*>& items, UiRect rc)
{
	CSize size;
	for (auto it = items.begin(); it != items.end(); it++) 
	{
		Control* pControl = *it;
		if( !pControl->IsVisible() ) continue;
		CSize new_size = SetFloatPos(pControl, rc);
		size.cx = MAX(size.cx, new_size.cx);
		size.cy = MAX(size.cy, new_size.cy);
	}

	return size;
}

CSize Layout::AjustSizeByChild(const std::vector<Control*>& items, CSize szAvailable)
{
	CSize maxSize(-9999, -9999);
	CSize itemSize;
	for (auto it = items.begin(); it != items.end(); it++) 
	{
		if (!(*it)->IsVisible())
			continue;

		itemSize = (*it)->EstimateSize(szAvailable);
		if( itemSize.cx < (*it)->GetMinWidth() ) itemSize.cx = (*it)->GetMinWidth();
		if( (*it)->GetMaxWidth() >= 0 && itemSize.cx > (*it)->GetMaxWidth() ) itemSize.cx = (*it)->GetMaxWidth();
		if( itemSize.cy < (*it)->GetMinHeight() ) itemSize.cy = (*it)->GetMinHeight();
		if( itemSize.cy > (*it)->GetMaxHeight() ) itemSize.cy = (*it)->GetMaxHeight();
		maxSize.cx = MAX(itemSize.cx + (*it)->GetMargin().left + (*it)->GetMargin().right, maxSize.cx);
		maxSize.cy = MAX(itemSize.cy + (*it)->GetMargin().top + (*it)->GetMargin().bottom, maxSize.cy);
	}
	maxSize.cx += m_rcPadding.left + m_rcPadding.right;
	maxSize.cy += m_rcPadding.top + m_rcPadding.bottom;
	return maxSize;
}

UiRect Layout::GetPadding() const
{
	return m_rcPadding;
}

void Layout::SetPadding(UiRect rcPadding, bool bNeedDpiScale /*= true*/)
{
	if (bNeedDpiScale)
		DpiManager::GetInstance()->ScaleRect(rcPadding);

	m_rcPadding = rcPadding;
	m_pOwner->Arrange();
}

int Layout::GetChildMargin() const
{
	return m_iChildMargin;
}

void Layout::SetChildMargin(int iMargin)
{
	DpiManager::GetInstance()->ScaleInt(iMargin);
	m_iChildMargin = iMargin;
	m_pOwner->Arrange();
}

UiRect Layout::GetInternalPos() const
{
	UiRect internalPos = m_pOwner->GetPos();
	internalPos.Deflate(m_rcPadding);
	return internalPos;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

Box::Box(Layout* pLayout) :
	m_pLayout(pLayout),
	m_bAutoDestroy(true),
	m_bDelayedDestroy(true),
	m_bMouseChildEnabled(true),
	m_items(),
	OnBubbledEvent()
{
	m_pLayout->SetOwner(this);
}

Box::Box(const Box& r) :
	Control(r),
	m_bAutoDestroy(r.m_bAutoDestroy),
	m_bDelayedDestroy(r.m_bDelayedDestroy),
	m_bMouseChildEnabled(r.m_bMouseChildEnabled),
	m_items(),
	OnBubbledEvent()
{
	m_pLayout.reset(new Layout(*r.m_pLayout)),
	m_pLayout->SetOwner(this);
}

Box::~Box()
{
	m_bDelayedDestroy = false;
	RemoveAll();
}

std::wstring Box::GetType() const
{
	return DUI_CTR_BOX;
}

UIAControlProvider* Box::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIABoxProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

void Box::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->SetWindow(pManager, this, bInit);
	}

	Control::SetWindow(pManager, pParent, bInit);
}

void Box::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (m_pLayout->SetAttribute(strName, strValue))	{

	}
	else if( strName == _T("mousechild") ) SetMouseChildEnabled(strValue == _T("true"));
	else Control::SetAttribute(strName, strValue);
}

void Box::SetPos(UiRect rc)
{
	Control::SetPos(rc);
	rc.left += m_pLayout->GetPadding().left;
	rc.top += m_pLayout->GetPadding().top;
	rc.right -= m_pLayout->GetPadding().right;
	rc.bottom -= m_pLayout->GetPadding().bottom;

	CSize requiredSize;
	if( m_items.size() == 0) {
		requiredSize.cx = 0;
		requiredSize.cy = 0;
	}
	else {
		requiredSize = m_pLayout->ArrangeChild(m_items, rc);
	}
}

void Box::HandleMessageTemplate(EventArgs& msg)
{
	if (msg.Type == kEventInternalDoubleClick || msg.Type == kEventInternalMenu
		|| msg.Type == kEventInternalSetFocus || msg.Type == kEventInternalKillFocus) {
		HandleMessage(msg);
		return;
	}

	bool bRet = true;
	std::weak_ptr<nbase::WeakFlag> weakflag = GetWeakFlag();
	if (this == msg.pSender) {
		auto callback = OnEvent.find(msg.Type);
		if (callback != OnEvent.end()) {
			bRet = callback->second(&msg);
		}
		if (weakflag.expired()) {
			return;
		}

		callback = OnEvent.find(kEventAll);
		if (callback != OnEvent.end()) {
			bRet = callback->second(&msg);
		}
		if (weakflag.expired()) {
			return;
		}

		if (bRet) {
			auto callback = OnXmlEvent.find(msg.Type);
			if (callback != OnXmlEvent.end()) {
				bRet = callback->second(&msg);
			}
			if (weakflag.expired()) {
				return;
			}

			callback = OnXmlEvent.find(kEventAll);
			if (callback != OnXmlEvent.end()) {
				bRet = callback->second(&msg);
			}
			if (weakflag.expired()) {
				return;
			}
		}
	}

	auto callback = OnBubbledEvent.find(msg.Type);
	if (callback != OnBubbledEvent.end()) {
		bRet = callback->second(&msg);
	}
	if (weakflag.expired()) {
		return;
	}

	callback = OnBubbledEvent.find(kEventAll);
	if (callback != OnBubbledEvent.end()) {
		bRet = callback->second(&msg);
	}
	if (weakflag.expired()) {
		return;
	}

	if (bRet) {
		auto callback = OnXmlBubbledEvent.find(msg.Type);
		if (callback != OnXmlBubbledEvent.end()) {
			bRet = callback->second(&msg);
		}
		if (weakflag.expired()) {
			return;
		}

		callback = OnXmlBubbledEvent.find(kEventAll);
		if (callback != OnXmlBubbledEvent.end()) {
			bRet = callback->second(&msg);
		}
		if (weakflag.expired()) {
			return;
		}
	}

	if (bRet) {
		HandleMessage(msg);
	}
}

void Box::SetReceivePointerMsg(bool bRecv)
{
	__super::SetReceivePointerMsg(bRecv);
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->SetReceivePointerMsg(bRecv);
	}
}

void Box::PaintChild(IRenderContext* pRender, const UiRect& rcPaint)
{
	UiRect rcTemp;
	if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		Control* pControl = *it;
		if( !pControl->IsVisible() ) continue;
		pControl->AlphaPaint(pRender, rcPaint);
	}
}

void Box::SetVisible_(bool bVisible)
{
	__super::SetVisible_(bVisible);
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->SetInternVisible(IsVisible());
	}
}

// 逻辑上，对于Container控件不公开此方法
// 调用此方法的结果是，内部子控件隐藏，控件本身依然显示，背景等效果存在
void Box::SetInternVisible(bool bVisible)
{
	Control::SetInternVisible(bVisible);
	if (m_items.empty()) return;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		// 控制子控件显示状态
		// InternVisible状态应由子控件自己控制
		(*it)->SetInternVisible(IsVisible());
	}
}

void Box::SetEnabled(bool bEnabled)
{
	if (m_bEnabled == bEnabled) return;

	Control::SetEnabled(bEnabled);
	if (m_items.empty()) return;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->SetEnabled(bEnabled);
	}

	Invalidate();
}

CSize Box::EstimateSize(CSize szAvailable)
{
	CSize fixedSize = m_cxyFixed;
	if (GetFixedWidth() == DUI_LENGTH_AUTO || GetFixedHeight() == DUI_LENGTH_AUTO) {
		if (!m_bReEstimateSize) {
			return m_szEstimateSize;
		}

		szAvailable.cx -= m_pLayout->GetPadding().left + m_pLayout->GetPadding().right;
		szAvailable.cy -= m_pLayout->GetPadding().top + m_pLayout->GetPadding().bottom;
		CSize sizeByChild = m_pLayout->AjustSizeByChild(m_items, szAvailable);
		if (GetFixedWidth() == DUI_LENGTH_AUTO) {
			fixedSize.cx = sizeByChild.cx;
		}
		if (GetFixedHeight() == DUI_LENGTH_AUTO) {
			fixedSize.cy = sizeByChild.cy;
		}

		m_bReEstimateSize = false;
		for (auto it = m_items.begin(); it != m_items.end(); it++) {
			if (!(*it)->IsVisible()) {
				continue;
			}
			if ((*it)->GetFixedWidth() == DUI_LENGTH_AUTO || (*it)->GetFixedHeight() == DUI_LENGTH_AUTO) {
				if ((*it)->IsReEstimateSize()) {
					m_bReEstimateSize = true;
					break;
				}
			}
		}

		m_szEstimateSize = fixedSize;
	}

	return fixedSize;
}

Control* Box::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos)
{
	// Check if this guy is valid
	if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) return NULL;
	if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) return NULL;
	if ((uFlags & UIFIND_HITTEST) != 0) {
		if (!::PtInRect(&m_rcItem, *(static_cast<LPPOINT>(pData)))) return NULL;
		if (!m_bMouseChildEnabled) {
			Control* pResult = NULL;
			if (pResult == NULL) pResult = Control::FindControl(Proc, pData, uFlags);
			return pResult;
		}
	}

	Control* pResult = NULL;
	if (pResult != NULL) return pResult;

	if ((uFlags & UIFIND_ME_FIRST) != 0) {
		Control* pControl = Control::FindControl(Proc, pData, uFlags);
		if (pControl != NULL) return pControl;
	}
	UiRect rc = m_rcItem;
	rc.left += m_pLayout->GetPadding().left;
	rc.top += m_pLayout->GetPadding().top;
	rc.right -= m_pLayout->GetPadding().right;
	rc.bottom -= m_pLayout->GetPadding().bottom;

	if ((uFlags & UIFIND_TOP_FIRST) != 0) {
		for (int it = (int)m_items.size() - 1; it >= 0; it--) {
			Control* pControl;
			if ((uFlags & UIFIND_HITTEST) != 0) {
				CPoint newPoint = *(static_cast<LPPOINT>(pData));
				newPoint.Offset(scrollPos);
				pControl = m_items[it]->FindControl(Proc, &newPoint, uFlags);
			}
			else {
				pControl = m_items[it]->FindControl(Proc, pData, uFlags);
			}
			if (pControl != NULL) {
				if ((uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
					continue;
				else
					return pControl;
			}
		}
	}
	else {
		for (auto it = m_items.begin(); it != m_items.end(); it++) {
			Control* pControl;
			if ((uFlags & UIFIND_HITTEST) != 0) {
				CPoint newPoint = *(static_cast<LPPOINT>(pData));
				newPoint.Offset(scrollPos);
				pControl = (*it)->FindControl(Proc, &newPoint, uFlags);
			}
			else {
				pControl = (*it)->FindControl(Proc, pData, uFlags);
			}
			if (pControl != NULL) {
				if ((uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))))
					continue;
				else
					return pControl;
			}
		}
	}

	if (pResult == NULL && (uFlags & UIFIND_ME_FIRST) == 0) pResult = Control::FindControl(Proc, pData, uFlags);
	return pResult;
}

Control* Box::FindSubControl(const std::wstring& pstrSubControlName)
{
	Control* pSubControl = NULL;
	pSubControl = static_cast<Control*>(GetWindow()->FindSubControlByName(this, pstrSubControlName));
	return pSubControl;
}

int Box::FindSelectable(int iIndex, bool bForward /*= true*/) const
{
	// NOTE: This is actually a helper-function for the list/combo/ect controls
	//       that allow them to find the next enabled/available selectable item
	if (GetCount() == 0) return -1;
	iIndex = CLAMP(iIndex, 0, GetCount() - 1);
	if (bForward) {
		for (int i = iIndex; i < GetCount(); i++) {
			if (dynamic_cast<ListContainerElement*>(GetItemAt(i)) != NULL
				&& GetItemAt(i)->IsVisible()
				&& GetItemAt(i)->IsEnabled()) return i;
		}
		return -1;
	}
	else {
		for (int i = iIndex; i >= 0; --i) {
			if (dynamic_cast<ListContainerElement*>(GetItemAt(i)) != NULL
				&& GetItemAt(i)->IsVisible()
				&& GetItemAt(i)->IsEnabled()) return i;
		}
		return FindSelectable(0, true);
	}
}

Control* Box::GetItemAt(std::size_t iIndex) const
{
	if( iIndex < 0 || iIndex >= m_items.size() ) return NULL;
	return static_cast<Control*>(m_items[iIndex]);
}

int Box::GetItemIndex(Control* pControl) const
{
	auto it = std::find(m_items.begin(), m_items.end(), pControl);
	if (it == m_items.end()) {
		return -1;
	}
	return it - m_items.begin();
}

bool Box::SetItemIndex(Control* pControl, std::size_t iIndex)
{
	if( iIndex < 0 || iIndex >= m_items.size() ) return false;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if( *it == pControl ) {
			Arrange();            
			m_items.erase(it);
			m_items.insert(m_items.begin() + iIndex, pControl);
			return true;
		}
	}

	return false;
}

int Box::GetCount() const
{
	return (int)m_items.size();
}

bool Box::Add(Control* pControl)
{
	if( pControl == NULL) return false;

	if( m_pWindow != NULL ) m_pWindow->InitControls(pControl, this);
	if( IsVisible() ) 
		Arrange();
	else
		pControl->SetInternVisible(false);
	m_items.push_back(pControl);   
	return true;
}

bool Box::AddAt(Control* pControl, std::size_t iIndex)
{
	if( pControl == NULL) return false;
	if( iIndex < 0 || iIndex > m_items.size() ) {
		ASSERT(FALSE);
		return false;
	}
	if( m_pWindow != NULL ) m_pWindow->InitControls(pControl, this);
	if( IsVisible() ) 
		Arrange();
	else 
		pControl->SetInternVisible(false);
	m_items.insert(m_items.begin() + iIndex, pControl);
	return true;
}

bool Box::Remove(Control* pControl)
{
	if( pControl == NULL) return false;

	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if( *it == pControl ) {
			Arrange();
			if( m_bAutoDestroy ) {
				if( m_bDelayedDestroy && m_pWindow ) m_pWindow->AddDelayedCleanup(pControl);             
				else delete pControl;
			}
			m_items.erase(it);
			return true;
		}
	}
	return false;
}

bool Box::RemoveAt(std::size_t iIndex)
{
	Control* pControl = GetItemAt(iIndex);
	if (pControl != NULL) {
		return Box::Remove(pControl);
	}

	return false;
}

void Box::RemoveAll()
{
	if (m_bAutoDestroy) {
		for (auto it = m_items.begin(); it != m_items.end(); it++) {
			if( m_bDelayedDestroy && m_pWindow ) m_pWindow->AddDelayedCleanup((*it));             
			else delete (*it);
		}
	}

	m_items.clear();
	Arrange();
}

bool Box::HasItem(Control* pControl) const
{
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if (*it == pControl) {
			return true;
		}
	}
	return false;
}

void Box::SwapChild(Control* pChild1, Control* pChild2)
{
	ASSERT(std::find(m_items.begin(), m_items.end(), pChild1) != m_items.end());
	ASSERT(std::find(m_items.begin(), m_items.end(), pChild2) != m_items.end());

	std::vector<Control*>::iterator it1, it2;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if (*it == pChild1 || *it == pChild2) {
			Control* child = (*it == pChild1) ? pChild2 : pChild1;
			it = m_items.erase(it);
			it = m_items.insert(it, child);
		}
	}
}

void Box::ResetChildIndex(Control* pChild, std::size_t iIndex)
{
	ASSERT(std::find(m_items.begin(), m_items.end(), pChild) != m_items.end());

	std::size_t oldIndex = 0;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		if (*it == pChild) {
			m_items.erase(it);
			if (oldIndex >= iIndex) {
				AddAt(pChild, iIndex);
			}
			else {
				AddAt(pChild, iIndex - 1);
			}

			break;
		}
		oldIndex++;
	}
}

bool Box::IsAutoDestroy() const
{
	return m_bAutoDestroy;
}

void Box::SetAutoDestroyChild(bool bAuto)
{
	m_bAutoDestroy = bAuto;
}

bool Box::IsDelayedDestroy() const
{
	return m_bDelayedDestroy;
}

void Box::SetDelayedDestroy(bool bDelayed)
{
	m_bDelayedDestroy = bDelayed;
}

bool Box::IsMouseChildEnabled() const
{
	return m_bMouseChildEnabled;
}

void Box::SetMouseChildEnabled(bool bEnable)
{
	m_bMouseChildEnabled = bEnable;
}

Layout* Box::GetLayout() const
{
	return m_pLayout.get();
}

void Box::ReSetLayout(Layout* pLayout)
{
	m_pLayout.reset(pLayout);
	m_pLayout->SetOwner(this);
}

UiRect Box::GetPaddingPos() const
{
	UiRect pos = GetPos();
	UiRect padding = m_pLayout->GetPadding();
	pos.left += padding.left;
	pos.top += padding.top;
	pos.right -= padding.right;
	pos.bottom -= padding.bottom;

	return pos;
}

void Box::InvokeLoadImageCache()
{
	__super::InvokeLoadImageCache();
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->InvokeLoadImageCache();
	}
}

void Box::UnLoadImageCache()
{
	__super::UnLoadImageCache();
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->UnLoadImageCache();
	}
}

void Box::ClearImageCache()
{
	__super::ClearImageCache();
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->ClearImageCache();
	}
}

UINT Box::GetControlFlags() const
{
	return UIFLAG_DEFAULT; // Box 默认不支持 TAB 切换焦点
}

void Box::DetachBubbledEvent(EventType eventType)
{
    auto event = OnBubbledEvent.find(eventType);
    if (event != OnBubbledEvent.end())
    {
        OnBubbledEvent.erase(eventType);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
//
//

ScrollableBox::ScrollableBox(Layout* pLayout) :
	Box(pLayout),
	m_pVerticalScrollBar(),
	m_pHorizontalScrollBar(),
	m_nVerScrollUnitPixels(30),
    m_nHerScrollUnitPixels(30),
	m_bScrollProcess(false),
	m_bScrollBarFloat(true),
	m_bVScrollBarLeftPos(false),
	m_bHoldEnd(false),
	m_bDefaultDisplayScrollbar(true),
	m_rcScrollBarPadding(),
	m_ptLastTouchPos(-1, -1),
	m_scrollAnimation(),
	m_renderOffsetYAnimation()
{
	m_rcScrollBarPadding.left = m_rcScrollBarPadding.top = m_rcScrollBarPadding.right = m_rcScrollBarPadding.bottom = 0;
}

ScrollableBox::ScrollableBox(const ScrollableBox& r):
	Box(r),
	m_nVerScrollUnitPixels(r.m_nVerScrollUnitPixels),
	m_nHerScrollUnitPixels(r.m_nHerScrollUnitPixels),
	m_bScrollProcess(r.m_bScrollProcess),
	m_bScrollBarFloat(r.m_bScrollBarFloat),
	m_bVScrollBarLeftPos(r.m_bVScrollBarLeftPos),
	m_bHoldEnd(r.m_bHoldEnd),
	m_bDefaultDisplayScrollbar(r.m_bDefaultDisplayScrollbar),
	m_rcScrollBarPadding(r.m_rcScrollBarPadding),
	m_ptLastTouchPos(r.m_ptLastTouchPos),
	m_scrollAnimation(),
	m_renderOffsetYAnimation()
{
	m_pVerticalScrollBar.reset(new ScrollBar(*m_pVerticalScrollBar.get()));
	m_pVerticalScrollBar->SetOwner(this);
	m_pHorizontalScrollBar.reset(new ScrollBar(*m_pHorizontalScrollBar.get()));
	m_pHorizontalScrollBar->SetOwner(this);
}

std::wstring ScrollableBox::GetType() const
{
	return std::wstring(_T("Scrollable")) + DUI_CTR_BOX;
}


UIAControlProvider* ScrollableBox::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIAScrollableBoxProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

void ScrollableBox::SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue)
{
	if( pstrName == _T("vscrollbar") ) {
		EnableScrollBar(pstrValue == _T("true"), GetHorizontalScrollBar() != NULL);
	}
	else if( pstrName == _T("vscrollbarstyle") ) {
		EnableScrollBar(true, GetHorizontalScrollBar() != NULL);
		if( GetVerticalScrollBar() ) GetVerticalScrollBar()->ApplyAttributeList(pstrValue);
	}
	else if( pstrName == _T("hscrollbar") ) {
		EnableScrollBar(GetVerticalScrollBar() != NULL, pstrValue == _T("true"));
	}
	else if( pstrName == _T("hscrollbarstyle") ) {
		EnableScrollBar(GetVerticalScrollBar() != NULL, true);
		if( GetHorizontalScrollBar() ) GetHorizontalScrollBar()->ApplyAttributeList(pstrValue);
	}
	else if( pstrName == _T("scrollbarpadding") ) {
		UiRect rcScrollbarPadding;
		LPTSTR pstr = NULL;
		rcScrollbarPadding.left = _tcstol(pstrValue.c_str(), &pstr, 10);  ASSERT(pstr);    
		rcScrollbarPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcScrollbarPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcScrollbarPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		SetScrollBarPadding(rcScrollbarPadding);
	}
	else if( pstrName == _T("vscrollunit") ) SetVerScrollUnitPixels(_ttoi(pstrValue.c_str()));
	else if (pstrName == _T("hscrollunit")) SetHorScrollUnitPixels(_ttoi(pstrValue.c_str()));
	else if (pstrName == _T("scrollbarfloat")) SetScrollBarFloat(pstrValue == _T("true"));
	else if (pstrName == _T("vscrollbarleft")) SetVScrollBarLeftPos(pstrValue == _T("true"));
	else if( pstrName == _T("defaultdisplayscrollbar") ) SetDefaultDisplayScrollbar(pstrValue == _T("true"));
	else if( pstrName == _T("holdend") ) SetHoldEnd(pstrValue == _T("true"));
	else Box::SetAttribute(pstrName, pstrValue);
}

void ScrollableBox::SetPos(UiRect rc)
{
	bool bEndDown = false;
	if (IsHoldEnd() && IsVScrollBarValid() && GetScrollRange().cy - GetScrollPos().cy == 0) {
		bEndDown = true;
	}
	SetPosInternally(rc);
	if (bEndDown && IsVScrollBarValid()) {
		EndDown(false, false);
	}
}

void ScrollableBox::SetPosInternally(UiRect rc)
{
	Control::SetPos(rc);
	UiRect rcRaw = rc;
	rc.left += m_pLayout->GetPadding().left;
	rc.top += m_pLayout->GetPadding().top;
	rc.right -= m_pLayout->GetPadding().right;
	rc.bottom -= m_pLayout->GetPadding().bottom;

	CSize requiredSize = CalcRequiredSize(rc);
	ProcessVScrollBar(rcRaw, requiredSize.cy);
	ProcessHScrollBar(rcRaw, requiredSize.cx);
}

CSize ScrollableBox::CalcRequiredSize(const UiRect& rc)
{
	CSize requiredSize;
	if (m_items.size() == 0) {
		requiredSize.cx = 0;
		requiredSize.cy = 0;
	}
	else {
		UiRect childSize = rc;
		if (!m_bScrollBarFloat && m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid()) {
			if (m_bVScrollBarLeftPos)
				childSize.left += m_pVerticalScrollBar->GetFixedWidth();
			else
				childSize.right -= m_pVerticalScrollBar->GetFixedWidth();
		}
		if (!m_bScrollBarFloat && m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid()) {
			childSize.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
		}
		requiredSize = m_pLayout->ArrangeChild(m_items, childSize);
	}
	return requiredSize;
}

void ScrollableBox::HandleMessage(EventArgs& event)
{
	if( (!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) || 
		event.Type == kEventLast) {
		if( m_pParent != NULL ) m_pParent->HandleMessageTemplate(event);
		else Box::HandleMessage(event);
		return;
	}
		
	if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsValid() && m_pVerticalScrollBar->IsEnabled() ) {
		if( event.Type == kEventKeyDown ) {
			switch( event.chKey ) {
			case VK_DOWN:
				LineDown();
				return;
			case VK_UP:
				LineUp();
				return;
			case VK_NEXT:
				PageDown();
				return;
			case VK_PRIOR:
				PageUp();
				return;
			case VK_HOME:
				HomeUp();
				return;
			case VK_END:
				EndDown();
				return;
			}
		}
		else if( event.Type == kEventMouseScrollWheel ) {
			int deltaValue = event.wParam;
			if (deltaValue > 0 ) {
				LineUp(abs(deltaValue));
			}
			else {
				LineDown(abs(deltaValue));
			}

			return;
		}
		else if (event.Type == kEventTouchDown || event.Type == kEventPointDown) {
			if (IsEnabled()) {
				SetMouseFocused(true);
				m_ptLastTouchPos = event.ptMouse;
				return;
			}
		}
		else if (event.Type == kEventTouchMove || event.Type == kEventPointMove) {
			if (IsMouseFocused()) {
				int detaValue = event.ptMouse.y - m_ptLastTouchPos.y;
				if (detaValue == 0)
					return;

				m_ptLastTouchPos = event.ptMouse;

				if (detaValue > 0) {
					TouchUp(abs(detaValue));
				}
				else {
					TouchDown(abs(detaValue));
				}
				return;
			}
		}
		else if (event.Type == kEventTouchUp || event.Type == kEventPointUp) {
			if (IsMouseFocused()) {
				SetMouseFocused(false);
				m_ptLastTouchPos = event.ptMouse;
				return;
			}
		}
	}
	else if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsValid() && m_pHorizontalScrollBar->IsEnabled() ) {
		if( event.Type == kEventKeyDown ) {
			switch( event.chKey ) {
			case VK_DOWN:
				LineRight();
				return;
			case VK_UP:
				LineLeft();
				return;
			case VK_NEXT:
				PageRight();
				return;
			case VK_PRIOR:
				PageLeft();
				return;
			case VK_HOME:
				HomeLeft();
				return;
			case VK_END:
				EndRight();
				return;
			}
		}
		else if( event.Type == kEventMouseScrollWheel )	{
			int deltaValue = event.wParam;
			if (deltaValue > 0 ) {
				LineLeft();
				return;
			}
			else {
				LineRight();
				return;
			}
		}
	}
		
	Box::HandleMessage(event);
}

bool ScrollableBox::MouseEnter(EventArgs& msg)
{
	bool bRet = __super::MouseEnter(msg);
	if (bRet && m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsValid() && m_pVerticalScrollBar->IsEnabled()) {
		if (m_pVerticalScrollBar->IsAutoHideScroll()) {
			m_pVerticalScrollBar->SetVisible(true);
		}
	}
	if (bRet && m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsValid() && m_pHorizontalScrollBar->IsEnabled()) {
		if (m_pHorizontalScrollBar->IsAutoHideScroll()) {
			m_pHorizontalScrollBar->SetVisible(true);
		}
	}

	return bRet;
}

bool ScrollableBox::MouseLeave(EventArgs& msg)
{
	bool bRet = __super::MouseLeave(msg);
	if (bRet && m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsValid() && m_pVerticalScrollBar->IsEnabled()) {
		if (m_pVerticalScrollBar->GetThumbState() == kControlStateNormal
			&& m_pVerticalScrollBar->IsAutoHideScroll()) {
			m_pVerticalScrollBar->SetVisible(false);
		}
	}
	if (bRet && m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsValid() && m_pHorizontalScrollBar->IsEnabled()) {
		if (m_pHorizontalScrollBar->GetThumbState() == kControlStateNormal
			&& m_pHorizontalScrollBar->IsAutoHideScroll()) {
			m_pHorizontalScrollBar->SetVisible(false);
		}
	}

	return bRet;
}

void ScrollableBox::PaintChild(IRenderContext* pRender, const UiRect& rcPaint)
{
	UiRect rcTemp;
	if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		Control* pControl = *it;
		if( !pControl->IsVisible() ) continue;
		if (pControl->IsFloat()) {
			pControl->AlphaPaint(pRender, rcPaint);	
		}
		else {
			CSize scrollPos = GetScrollPos();
			UiRect rcNewPaint = GetPaddingPos();
			AutoClip alphaClip(pRender, rcNewPaint, IsClip());
			rcNewPaint.Offset(scrollPos.cx, scrollPos.cy);
			rcNewPaint.Offset(GetRenderOffset().x, GetRenderOffset().y);

			CPoint ptOffset(scrollPos.cx, scrollPos.cy);
			CPoint ptOldOrg = pRender->OffsetWindowOrg(ptOffset);
			pControl->AlphaPaint(pRender, rcNewPaint);
			pRender->SetWindowOrg(ptOldOrg);
		}
	}

	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
		m_pHorizontalScrollBar->AlphaPaint(pRender, rcPaint);
	}
		
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
		m_pVerticalScrollBar->AlphaPaint(pRender, rcPaint);
	}
		
	static bool bFirstPaint = true;
	if (bFirstPaint) {
		bFirstPaint = false;
		LoadImageCache(true);
	}
}

void ScrollableBox::SetMouseEnabled(bool bEnabled)
{
	if (m_pVerticalScrollBar != NULL) m_pVerticalScrollBar->SetMouseEnabled(bEnabled);
	if (m_pHorizontalScrollBar != NULL) m_pHorizontalScrollBar->SetMouseEnabled(bEnabled);
	Box::SetMouseEnabled(bEnabled);
}

void ScrollableBox::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	if (m_pVerticalScrollBar != NULL) m_pVerticalScrollBar->SetWindow(pManager, this, bInit);
	if (m_pHorizontalScrollBar != NULL) m_pHorizontalScrollBar->SetWindow(pManager, this, bInit);
	Box::SetWindow(pManager, pParent, bInit);
}

Control* ScrollableBox::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos)
{
	// Check if this guy is valid
	if ((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) return NULL;
	if ((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) return NULL;
	if ((uFlags & UIFIND_HITTEST) != 0) {
		if (!::PtInRect(&m_rcItem, *(static_cast<LPPOINT>(pData)))) return NULL;
		if (!m_bMouseChildEnabled) {
			Control* pResult = NULL;
			if (m_pVerticalScrollBar != NULL) pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
			if (pResult == NULL && m_pHorizontalScrollBar != NULL) pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
			if (pResult == NULL) pResult = Control::FindControl(Proc, pData, uFlags);
			return pResult;
		}
	}

	Control* pResult = NULL;
	if (m_pVerticalScrollBar != NULL) pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
	if (pResult == NULL && m_pHorizontalScrollBar != NULL) pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
	if (pResult != NULL) return pResult;

	CPoint ptNewScrollPos(GetScrollPos().cx, GetScrollPos().cy);
	return Box::FindControl(Proc, pData, uFlags, ptNewScrollPos);
}

CSize ScrollableBox::GetScrollPos() const
{
	CSize sz;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) sz.cy = m_pVerticalScrollBar->GetScrollPos();
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) sz.cx = m_pHorizontalScrollBar->GetScrollPos();
	return sz;
}

CSize ScrollableBox::GetScrollRange() const
{
	CSize sz;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) sz.cy = m_pVerticalScrollBar->GetScrollRange();
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) sz.cx = m_pHorizontalScrollBar->GetScrollRange();
	return sz;
}

void ScrollableBox::SetScrollPos(CSize szPos)
{
	if (szPos.cy < 0) {
		szPos.cy = 0;
		m_scrollAnimation.Reset();
	}
	else if (szPos.cy > GetScrollRange().cy) {
		szPos.cy = GetScrollRange().cy;
		m_scrollAnimation.Reset();
	}

	int cx = 0;
	int cy = 0;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) {
		int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
		m_pVerticalScrollBar->SetScrollPos(szPos.cy);
		cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) {
		int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
		m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
		cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( cx == 0 && cy == 0 ) return;
	LoadImageCache(cy > 0);
	Invalidate();
	if( m_pWindow != NULL )	{
		m_pWindow->SendNotify(this, kEventScrollChange, (cy == 0) ? 0 : 1, (cx == 0) ? 0 : 1);
	}
}

void ScrollableBox::LoadImageCache(bool bFromTopLeft)
{
	CSize scrollPos = GetScrollPos();
	UiRect rcImageCachePos = GetPos();
	rcImageCachePos.Offset(scrollPos.cx, scrollPos.cy);
	rcImageCachePos.Offset(GetRenderOffset().x, GetRenderOffset().y);
	rcImageCachePos.Inflate(UiRect(0, 730, 0, 730));

	auto forEach = [this, scrollPos, rcImageCachePos](ui::Control* pControl) {
		if (!pControl->IsVisible()) return;
		if (pControl->IsFloat()) return;
		UiRect rcTemp;
		UiRect controlPos = pControl->GetPos();
		if (!::IntersectRect(&rcTemp, &rcImageCachePos, &controlPos)) {
			pControl->UnLoadImageCache();
		}
		else {
			pControl->InvokeLoadImageCache();
		}
	};

	if (!bFromTopLeft) {
		std::for_each(m_items.rbegin(), m_items.rend(), forEach);
	}
	else {
		std::for_each(m_items.begin(), m_items.end(), forEach);
	}
}

void ScrollableBox::SetScrollPosY(int y)
{
	CSize scrollPos = GetScrollPos();
	scrollPos.cy = y;
	SetScrollPos(scrollPos);
}

void ScrollableBox::SetScrollPosX(int x)
{
    CSize scrollPos = GetScrollPos();
    scrollPos.cx = x;
    SetScrollPos(scrollPos);
}

void ScrollableBox::LineUp(int deltaValue, bool withAnimation)
{
	int cyLine = GetVerScrollUnitPixels();
	if (cyLine == 0) {
		cyLine = 20;
	}
	if (deltaValue != DUI_NOSET_VALUE) {
		cyLine = min(cyLine, deltaValue);
	}

	CSize scrollPos = GetScrollPos();
	if (scrollPos.cy <= 0) {
		return;
	}

	if (!withAnimation) {
		scrollPos.cy -= cyLine;
		if (scrollPos.cy < 0)
		{
			scrollPos.cy = 0;
		}
		SetScrollPos(scrollPos);
	}
	else {
		m_scrollAnimation.SetStartValue(scrollPos.cy);
		if (m_scrollAnimation.IsPlaying()) {
			if (m_scrollAnimation.GetEndValue() > m_scrollAnimation.GetStartValue()) {
				m_scrollAnimation.SetEndValue(scrollPos.cy - cyLine);
			}
			else {
				m_scrollAnimation.SetEndValue(m_scrollAnimation.GetEndValue() - cyLine);
			}
		}
		else {
			m_scrollAnimation.SetEndValue(scrollPos.cy - cyLine);
		}
		m_scrollAnimation.SetSpeedUpRatio(0);
		m_scrollAnimation.SetSpeedDownfactorA(-0.012);
		m_scrollAnimation.SetSpeedDownRatio(0.5);
		m_scrollAnimation.SetTotalMillSeconds(DUI_NOSET_VALUE);
		m_scrollAnimation.SetCallback(nbase::Bind(&ScrollableBox::SetScrollPosY, this, std::placeholders::_1));
		m_scrollAnimation.Start();
	}
}

void ScrollableBox::LineDown(int deltaValue, bool withAnimation)
{
	int cyLine = GetVerScrollUnitPixels();
	if (cyLine == 0) {
		cyLine = 20;
	}
	if (deltaValue != DUI_NOSET_VALUE) {
		cyLine = min(cyLine, deltaValue);
	}

	CSize scrollPos = GetScrollPos();
	if (scrollPos.cy >= GetScrollRange().cy) {
		return;
	}

	if (!withAnimation) {
		scrollPos.cy += cyLine;
		if (scrollPos.cy < 0)
		{
			scrollPos.cy = 0;
		}
		SetScrollPos(scrollPos);
	}
	else {
		m_scrollAnimation.SetStartValue(scrollPos.cy);
		if (m_scrollAnimation.IsPlaying()) {
			if (m_scrollAnimation.GetEndValue() < m_scrollAnimation.GetStartValue()) {
				m_scrollAnimation.SetEndValue(scrollPos.cy + cyLine);
			}
			else {
				m_scrollAnimation.SetEndValue(m_scrollAnimation.GetEndValue() + cyLine);
			}
		}
		else {
			m_scrollAnimation.SetEndValue(scrollPos.cy + cyLine);
		}
		m_scrollAnimation.SetSpeedUpRatio(0);
		m_scrollAnimation.SetSpeedDownfactorA(-0.012);
		m_scrollAnimation.SetSpeedDownRatio(0.5);
		m_scrollAnimation.SetTotalMillSeconds(DUI_NOSET_VALUE);
		m_scrollAnimation.SetCallback(nbase::Bind(&ScrollableBox::SetScrollPosY, this, std::placeholders::_1));
		m_scrollAnimation.Start();
	}
}
void ScrollableBox::LineLeft(int detaValue)
{
    int cxLine = GetHorScrollUnitPixels();
    if (cxLine == 0) {
        cxLine = 20;
    }
    if (detaValue != DUI_NOSET_VALUE) {
        cxLine = min(cxLine, detaValue);
    }

    CSize scrollPos = GetScrollPos();
    if (scrollPos.cx <= 0) {
        return;
    }
    scrollPos.cx -= cxLine;
    if (scrollPos.cx < 0)
    {
        scrollPos.cx = 0;
    }
    SetScrollPos(scrollPos);
    /*m_scrollAnimation.SetStartValue(scrollPos.cx);
    if (m_scrollAnimation.IsPlaying()) {
        if (m_scrollAnimation.GetEndValue() > m_scrollAnimation.GetStartValue()) {
            m_scrollAnimation.SetEndValue(scrollPos.cx - cxLine);
        }
        else {
            m_scrollAnimation.SetEndValue(m_scrollAnimation.GetEndValue() - cxLine);
        }
    }
    else {
        m_scrollAnimation.SetEndValue(scrollPos.cx - cxLine);
    }
    m_scrollAnimation.SetSpeedUpRatio(0);
    m_scrollAnimation.SetSpeedDownfactorA(-0.012);
    m_scrollAnimation.SetSpeedDownRatio(0.5);
    m_scrollAnimation.SetTotalMillSeconds(DUI_NOSET_VALUE);
    m_scrollAnimation.SetCallback(nbase::Bind(&ScrollableBox::SetScrollPosX, this, std::placeholders::_1));
    m_scrollAnimation.Start();*/

}

void ScrollableBox::LineRight(int detaValue)
{
    int cxLine = GetHorScrollUnitPixels();
    if (cxLine == 0) {
        cxLine = 20;
    }
    if (detaValue != DUI_NOSET_VALUE) {
        cxLine = min(cxLine, detaValue);
    }

    CSize scrollPos = GetScrollPos();
    if (scrollPos.cx >= GetScrollRange().cx) {
        return;
    }
    scrollPos.cx += cxLine;
    if (scrollPos.cx > GetScrollRange().cx)
    {
        scrollPos.cx = GetScrollRange().cx;
    }
    SetScrollPos(scrollPos);
    //m_scrollAnimation.SetStartValue(scrollPos.cx);
    //if (m_scrollAnimation.IsPlaying()) {
    //    if (m_scrollAnimation.GetEndValue() < m_scrollAnimation.GetStartValue()) {
    //        m_scrollAnimation.SetEndValue(scrollPos.cx + cxLine);
    //    }
    //    else {
    //        m_scrollAnimation.SetEndValue(m_scrollAnimation.GetEndValue() + cxLine);
    //    }
    //}
    //else {
    //    m_scrollAnimation.SetEndValue(scrollPos.cx + cxLine);
    //}
    //m_scrollAnimation.SetSpeedUpRatio(0);
    //m_scrollAnimation.SetSpeedDownfactorA(-0.012);
    //m_scrollAnimation.SetSpeedDownRatio(0.5);
    //m_scrollAnimation.SetTotalMillSeconds(DUI_NOSET_VALUE);
    //m_scrollAnimation.SetCallback(nbase::Bind(&ScrollableBox::SetScrollPosX, this, std::placeholders::_1));
    //m_scrollAnimation.Start();
}
void ScrollableBox::PageUp()
{
	CSize sz = GetScrollPos();
	int iOffset = m_rcItem.bottom - m_rcItem.top - m_pLayout->GetPadding().top - m_pLayout->GetPadding().bottom;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
	sz.cy -= iOffset;
	SetScrollPos(sz);
}

void ScrollableBox::PageDown()
{
	CSize sz = GetScrollPos();
	int iOffset = m_rcItem.bottom - m_rcItem.top - m_pLayout->GetPadding().top - m_pLayout->GetPadding().bottom;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
	sz.cy += iOffset;
	SetScrollPos(sz);
}

void ScrollableBox::HomeUp()
{
	CSize sz = GetScrollPos();
	sz.cy = 0;
	SetScrollPos(sz);
}

void ScrollableBox::EndDown(bool arrange, bool withAnimation)
{
	if (arrange) {
		SetPosInternally(GetPos());
	}
	
	int renderOffsetY = GetScrollRange().cy - GetScrollPos().cy + (m_renderOffsetYAnimation.GetEndValue() - GetRenderOffset().y);
	if (withAnimation == true && IsVScrollBarValid() && renderOffsetY > 0) {
		PlayRenderOffsetYAnimation(-renderOffsetY);
	}

	CSize sz = GetScrollPos();
	sz.cy = GetScrollRange().cy;
	SetScrollPos(sz);
}

void ScrollableBox::PageLeft()
{
	CSize sz = GetScrollPos();
	int iOffset = m_rcItem.right - m_rcItem.left - m_pLayout->GetPadding().left - m_pLayout->GetPadding().right;
	//if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) iOffset -= m_pVerticalScrollBar->GetFixedWidth();
	sz.cx -= iOffset;
	SetScrollPos(sz);
}

void ScrollableBox::PageRight()
{
	CSize sz = GetScrollPos();
	int iOffset = m_rcItem.right - m_rcItem.left - m_pLayout->GetPadding().left - m_pLayout->GetPadding().right;
	//if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) iOffset -= m_pVerticalScrollBar->GetFixedWidth();
	sz.cx += iOffset;
	SetScrollPos(sz);
}

void ScrollableBox::HomeLeft()
{
	CSize sz = GetScrollPos();
	sz.cx = 0;
	SetScrollPos(sz);
}

void ScrollableBox::EndRight()
{
	CSize sz = GetScrollPos();
	sz.cx = GetScrollRange().cx;
	SetScrollPos(sz);
}

void ScrollableBox::TouchUp(int deltaValue)
{
	CSize scrollPos = GetScrollPos();
	if (scrollPos.cy <= 0) {
		return;
	}

	scrollPos.cy = scrollPos.cy - deltaValue;
	SetScrollPos(scrollPos);
}

void ScrollableBox::TouchDown(int deltaValue)
{
	CSize scrollPos = GetScrollPos();
	if (scrollPos.cy >= GetScrollRange().cy) {
		return;
	}

	scrollPos.cy = scrollPos.cy + deltaValue;
	SetScrollPos(scrollPos);
}

void ScrollableBox::EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal)
{
	if( bEnableVertical && !m_pVerticalScrollBar ) {
		m_pVerticalScrollBar.reset(new ScrollBar);
		m_pVerticalScrollBar->SetVisible_(false);
		m_pVerticalScrollBar->SetScrollRange(0);
		m_pVerticalScrollBar->SetOwner(this);
		m_pVerticalScrollBar->SetWindow(m_pWindow, NULL, false);
		m_pVerticalScrollBar->SetClass(_T("vscrollbar"));
	}
	else if( !bEnableVertical && m_pVerticalScrollBar ) {
		m_pVerticalScrollBar.reset();
	}

	if( bEnableHorizontal && !m_pHorizontalScrollBar ) {
		m_pHorizontalScrollBar.reset(new ScrollBar);
		m_pHorizontalScrollBar->SetVisible_(false);
		m_pHorizontalScrollBar->SetScrollRange(0);
		m_pHorizontalScrollBar->SetHorizontal(true);
		m_pHorizontalScrollBar->SetOwner(this);
		m_pHorizontalScrollBar->SetWindow(m_pWindow, NULL, false);
		m_pHorizontalScrollBar->SetClass(_T("hscrollbar"));
	}
	else if( !bEnableHorizontal && m_pHorizontalScrollBar ) {
		m_pHorizontalScrollBar.reset();
	}

	Arrange();
}

ScrollBar* ScrollableBox::GetVerticalScrollBar() const
{
	return m_pVerticalScrollBar.get();
}

ScrollBar* ScrollableBox::GetHorizontalScrollBar() const
{
	return m_pHorizontalScrollBar.get();
}

void ScrollableBox::ProcessVScrollBar(UiRect rc, int cyRequired)
{
	UiRect rcScrollBarPos = rc;
	rcScrollBarPos.left += m_rcScrollBarPadding.left;
	rcScrollBarPos.top += m_rcScrollBarPadding.top;
	rcScrollBarPos.right -= m_rcScrollBarPadding.right;
	rcScrollBarPos.bottom -= m_rcScrollBarPadding.bottom;

	if( m_pVerticalScrollBar == NULL ) return;

	rc.left += m_pLayout->GetPadding().left;
	rc.top += m_pLayout->GetPadding().top;
	rc.right -= m_pLayout->GetPadding().right;
	rc.bottom -= m_pLayout->GetPadding().bottom;
	int nHeight = rc.bottom - rc.top;
	if (cyRequired > nHeight && !m_pVerticalScrollBar->IsValid()) {
		m_pVerticalScrollBar->SetScrollRange(cyRequired - nHeight);
		m_pVerticalScrollBar->SetScrollPos(0);
		m_bScrollProcess = true;
		SetPos(m_rcItem);
		m_bScrollProcess = false;

		return;
	}
	// No scrollbar required
	if( !m_pVerticalScrollBar->IsValid() ) return;

	// Scroll not needed anymore?
	int cyScroll = cyRequired - nHeight;
	if( cyScroll <= 0 && !m_bScrollProcess) {
		m_pVerticalScrollBar->SetScrollPos(0);
		m_pVerticalScrollBar->SetScrollRange(0);
		SetPos(m_rcItem);
	}
	else {
		if (m_bVScrollBarLeftPos) {
			UiRect rcVerScrollBarPos(rcScrollBarPos.left, rcScrollBarPos.top, rcScrollBarPos.left + m_pVerticalScrollBar->GetFixedWidth(), rcScrollBarPos.bottom);
			m_pVerticalScrollBar->SetPos(rcVerScrollBarPos);
		}
		else {
			UiRect rcVerScrollBarPos(rcScrollBarPos.right - m_pVerticalScrollBar->GetFixedWidth(), rcScrollBarPos.top, rcScrollBarPos.right, rcScrollBarPos.bottom);
			m_pVerticalScrollBar->SetPos(rcVerScrollBarPos);
		}

		if( m_pVerticalScrollBar->GetScrollRange() != cyScroll ) {
			int iScrollPos = m_pVerticalScrollBar->GetScrollPos();
			m_pVerticalScrollBar->SetScrollRange(::abs(cyScroll));
			if( !m_pVerticalScrollBar->IsValid() ) {
				m_pVerticalScrollBar->SetScrollPos(0);
			}

			if( iScrollPos > m_pVerticalScrollBar->GetScrollPos() ) {
				SetPos(m_rcItem);
			}
		}
	}
}

void ScrollableBox::ProcessHScrollBar(UiRect rc, int cxRequired)
{
	UiRect rcScrollBarPos = rc;
	rcScrollBarPos.left += m_rcScrollBarPadding.left;
	rcScrollBarPos.top += m_rcScrollBarPadding.top;
	rcScrollBarPos.right -= m_rcScrollBarPadding.right;
	rcScrollBarPos.bottom -= m_rcScrollBarPadding.bottom;

	if (m_pHorizontalScrollBar == NULL) return;

	rc.left += m_pLayout->GetPadding().left;
	rc.top += m_pLayout->GetPadding().top;
	rc.right -= m_pLayout->GetPadding().right;
	rc.bottom -= m_pLayout->GetPadding().bottom;
	int nWidth = rc.right - rc.left;
	if (cxRequired > nWidth && !m_pHorizontalScrollBar->IsValid()) {
		m_pHorizontalScrollBar->SetScrollRange(cxRequired - nWidth);
		m_pHorizontalScrollBar->SetScrollPos(0);
		m_bScrollProcess = true;
		SetPos(m_rcItem);
		m_bScrollProcess = false;

		return;
	}
	// No scrollbar required
	if (!m_pHorizontalScrollBar->IsValid()) return;

	// Scroll not needed anymore?
	int cxScroll = cxRequired - nWidth;
	if (cxScroll <= 0 && !m_bScrollProcess) {
		m_pHorizontalScrollBar->SetScrollPos(0);
		m_pHorizontalScrollBar->SetScrollRange(0);
		SetPos(m_rcItem);
	}
	else {
		UiRect rcVerScrollBarPos(rcScrollBarPos.left, rcScrollBarPos.bottom - m_pHorizontalScrollBar->GetFixedHeight(), rcScrollBarPos.right, rcScrollBarPos.bottom);
		m_pHorizontalScrollBar->SetPos(rcVerScrollBarPos);

		if (m_pHorizontalScrollBar->GetScrollRange() != cxScroll) {
			int iScrollPos = m_pHorizontalScrollBar->GetScrollPos();
			m_pHorizontalScrollBar->SetScrollRange(::abs(cxScroll));
			if (!m_pHorizontalScrollBar->IsValid()) {
				m_pHorizontalScrollBar->SetScrollPos(0);
			}

			if (iScrollPos > m_pHorizontalScrollBar->GetScrollPos()) {
				SetPos(m_rcItem);
			}
		}
	}
}

bool ScrollableBox::IsVScrollBarValid() const
{
	if (m_pVerticalScrollBar) {
		return m_pVerticalScrollBar->IsValid();
	}
	return false;
}

bool ScrollableBox::IsHScrollBarValid() const
{
	if (m_pHorizontalScrollBar) {
		return m_pHorizontalScrollBar->IsValid();
	}
	return false;
}

void ScrollableBox::ReomveLastItemAnimation()
{
	int nStartRang = GetScrollRange().cy;
	SetPosInternally(GetPos());
	int nEndRang = GetScrollRange().cy;

	int nRenderOffset = nEndRang - nStartRang + (m_renderOffsetYAnimation.GetEndValue() - GetRenderOffset().y);
	if (nRenderOffset < 0) {
		PlayRenderOffsetYAnimation(-nRenderOffset);
	}
}

void ScrollableBox::PlayRenderOffsetYAnimation(int nRenderY)
{
	m_renderOffsetYAnimation.SetStartValue(nRenderY);
	m_renderOffsetYAnimation.SetEndValue(0);
	m_renderOffsetYAnimation.SetSpeedUpRatio(0.3);
	m_renderOffsetYAnimation.SetSpeedUpfactorA(0.003);
	m_renderOffsetYAnimation.SetSpeedDownRatio(0.7);
	m_renderOffsetYAnimation.SetTotalMillSeconds(DUI_NOSET_VALUE);
	m_renderOffsetYAnimation.SetMaxTotalMillSeconds(650);
	std::function<void(int)> playCallback = nbase::Bind(&ScrollableBox::SetRenderOffsetY, this, std::placeholders::_1);
	m_renderOffsetYAnimation.SetCallback(playCallback);
	m_renderOffsetYAnimation.Start();
}

bool ScrollableBox::IsAtEnd() const
{
	return GetScrollRange().cy <= GetScrollPos().cy;
}

bool ScrollableBox::IsHoldEnd() const
{
	return m_bHoldEnd;
}

void ScrollableBox::SetHoldEnd(bool bHoldEnd)
{
	m_bHoldEnd = bHoldEnd;
}

int ScrollableBox::GetVerScrollUnitPixels() const
{
	return m_nVerScrollUnitPixels;
}

void ScrollableBox::SetVerScrollUnitPixels(int nUnitPixels)
{
	DpiManager::GetInstance()->ScaleInt(nUnitPixels);
	m_nVerScrollUnitPixels = nUnitPixels;
}

int ScrollableBox::GetHorScrollUnitPixels() const
{
    return m_nHerScrollUnitPixels;
}

void ScrollableBox::SetHorScrollUnitPixels(int nUnitPixels)
{
    DpiManager::GetInstance()->ScaleInt(nUnitPixels);
    m_nHerScrollUnitPixels = nUnitPixels;
}

bool ScrollableBox::GetScrollBarFloat() const
{
	return m_bScrollBarFloat;
}

void ScrollableBox::SetScrollBarFloat(bool bScrollBarFloat)
{
	m_bScrollBarFloat = bScrollBarFloat;
}

bool ScrollableBox::GetVScrollBarLeftPos() const
{
	return m_bVScrollBarLeftPos;
}

void ScrollableBox::SetVScrollBarLeftPos(bool bLeftPos)
{
	m_bVScrollBarLeftPos = bLeftPos;
}

ui::UiRect ScrollableBox::GetScrollBarPadding() const
{
	return m_rcScrollBarPadding;
}

void ScrollableBox::SetScrollBarPadding(UiRect rcScrollBarPadding)
{
	DpiManager::GetInstance()->ScaleRect(rcScrollBarPadding);
	m_rcScrollBarPadding = rcScrollBarPadding;
}

bool ScrollableBox::GetDefaultDisplayScrollbar() const
{
	return m_bDefaultDisplayScrollbar;
}

void ScrollableBox::SetDefaultDisplayScrollbar(bool bDefaultDisplay)
{
	m_bDefaultDisplayScrollbar = bDefaultDisplay;
}

void ScrollableBox::ClearImageCache()
{
	__super::ClearImageCache();

	if (m_pHorizontalScrollBar)
		m_pHorizontalScrollBar->ClearImageCache();
	if (m_pVerticalScrollBar)
		m_pVerticalScrollBar->ClearImageCache();
}

} // namespace ui
