#include "stdafx.h"
#include "TabBox.h"

namespace ui
{

TabBox::TabBox(Layout* pLayout) 
: Box(pLayout)
, m_iCurSel(-1)
, m_iInitSel(-1)
, m_bIsInit(false)
, m_bFadeSwith(false)
{

}

void TabBox::DoInit()
{
	m_bIsInit = true;
	if (m_iInitSel != -1)  SelectItem(m_iInitSel);
}

std::wstring TabBox::GetType() const
{
	return DUI_CTR_TABBOX;
}

bool TabBox::Add(Control* pControl)
{
	bool ret = Box::Add(pControl);
	if( !ret ) return ret;

	if(m_iCurSel == -1 && pControl->IsVisible()) {
		m_iCurSel = GetItemIndex(pControl);
	} 

	if (m_iCurSel != GetItemIndex(pControl) || !pControl->IsVisible())
	{
		if (!IsFadeSwitch()) {
			pControl->SetVisible(false);
		}
		pControl->SetMouseEnabled(false);
		if (Box* box = dynamic_cast<Box*>(pControl)) {
			box->SetMouseChildEnabled(false);
		}
		pControl->SetAlpha(0);
	}
	return ret;
}

bool TabBox::AddAt(Control* pControl, std::size_t iIndex)
{
	bool ret = Box::AddAt(pControl, iIndex);
	if(!ret)
		return ret;

	if(m_iCurSel == -1 && pControl->IsVisible()) {
		m_iCurSel = GetItemIndex(pControl);
	}
	else if( m_iCurSel != -1 && iIndex <= (std::size_t)m_iCurSel ) {
		m_iCurSel += 1;
	}
	else {
		if (!IsFadeSwitch()) {
			pControl->SetVisible(false);
		}
		pControl->SetMouseEnabled(false);
		if (Box* box = dynamic_cast<Box*>(pControl)) {
			box->SetMouseChildEnabled(false);
		}
		pControl->SetAlpha(0);
	}

	return ret;
}

bool TabBox::Remove(Control* pControl)
{
	if( pControl == NULL) return false;

	int index = GetItemIndex(pControl);
	bool ret = Box::Remove(pControl);
	if( !ret ) return false;

	if( m_iCurSel == index)	{
		if( GetCount() > 0 ) {
			m_iCurSel=0;
			if (!IsFadeSwitch()) {
				GetItemAt(m_iCurSel)->SetVisible(true);
			}

			pControl->SetMouseEnabled(true);
			if (Box* box = dynamic_cast<Box*>(pControl)) {
				box->SetMouseChildEnabled(true);
			}
			pControl->SetAlpha(255);
		}
		else
			m_iCurSel=-1;

		ArrangeAncestor();
	}
	else if( m_iCurSel > index ) {
		m_iCurSel -= 1;
	}

	return ret;
}

bool TabBox::RemoveAt(std::size_t iIndex)
{
	Control* pControl = GetItemAt(iIndex);
	if (pControl == NULL) return false;
	return Remove(pControl);
}

void TabBox::RemoveAll()
{
	m_iCurSel = -1;
	Box::RemoveAll();
	ArrangeAncestor();
}

int TabBox::GetCurSel() const
{
	return m_iCurSel;
}
	
bool TabBox::SelectItem(int iIndex)
{
	if( iIndex < 0 || (std::size_t)iIndex >= m_items.size() ) return false;
	if( iIndex == m_iCurSel ) return true;

	int iOldSel = m_iCurSel;
	m_iCurSel = iIndex;
	for( std::size_t it = 0; it < m_items.size(); it++ ){
		if( (int)it == iIndex ) {
			ShowTabItem(it);

			if (!IsFadeSwitch()) {
				m_items[it]->SetVisible();
			}
			else {
				int startValue = 0;
				int endValue = 0;
				if (m_iCurSel < iOldSel) {
					startValue = GetPos().GetWidth();
					endValue = 0;
				}
				else {
					startValue = -GetPos().GetWidth();
					endValue = 0;
				}

				auto player = m_items[it]->GetAnimationManager().SetFadeInOutX(true, true);
				player->SetStartValue(startValue);
				player->SetEndValue(endValue);
				player->SetSpeedUpfactorA(0.015);
				player->SetCompleteCallback(CompleteCallback());
				player->Start();
			}
		}
		else {
			if ((int)it != iOldSel) {
				HideTabItem(it);
				if (!IsFadeSwitch()) {
					m_items[it]->SetVisible(false);
				}
			}
			else {
				if (!IsFadeSwitch()) {
					HideTabItem(it);
					m_items[it]->SetVisible(false);
				}
				else {
					int startValue = 0;
					int endValue = 0;
					if (m_iCurSel < iOldSel) {
						startValue = 0;
						endValue = -GetPos().GetWidth();
					}
					else {
						startValue = 0;
						endValue = GetPos().GetWidth();
					}

					auto player = m_items[it]->GetAnimationManager().SetFadeInOutX(true, true);
					player->SetStartValue(startValue);
					player->SetEndValue(endValue);
					player->SetSpeedUpfactorA(0.015);
					CompleteCallback compelteCallback = nbase::Bind(&TabBox::HideTabItem, this, it);
					player->SetCompleteCallback(compelteCallback);
					player->Start();
				}
			}
		}
	}		

	if( m_pWindow != NULL ) {
		m_pWindow->SetNextTabControl();
		m_pWindow->SendNotify(this, kEventSelect, m_iCurSel, iOldSel);
	}
	return true;
}

void TabBox::HideTabItem(std::size_t it)
{
	m_items[it]->SetMouseEnabled(false);
	if (Box* box = dynamic_cast<Box*>(this->m_items[it])) {
		box->SetMouseChildEnabled(false);
	}
	m_items[it]->SetAlpha(0);
}

void TabBox::ShowTabItem(std::size_t it)
{
	m_items[it]->SetMouseEnabled(true);
	if (Box* box = dynamic_cast<Box*>(this->m_items[it])) {
		box->SetMouseChildEnabled(true);
	}
	m_items[it]->SetAlpha(255);
}

bool TabBox::SelectItem( Control* pControl )
{
	int iIndex = GetItemIndex(pControl);
	if (iIndex==-1)
		return false;
	else
		return SelectItem(iIndex);
}

bool TabBox::SelectItem(const std::wstring& pControlName)
{
	Control* pControl = FindSubControl(pControlName);
	ASSERT(pControl);
	return SelectItem(pControl);
}

void TabBox::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("selectedid"))
	{
		int iSel = _ttoi(strValue.c_str());
		m_bIsInit ? SelectItem(iSel) : m_iInitSel = iSel;
	}
	else if (strName == _T("fadeswitch")) SetFadeSwitch(strValue == _T("true"));
	else Box::SetAttribute(strName, strValue);
}

void TabBox::SetFadeSwitch(bool bFadeSwitch)
{
	m_bFadeSwith = bFadeSwitch;
	for (auto &it : m_items) {
		int index = GetItemIndex(it);
		if (index != m_iCurSel) {
			it->SetVisible(IsFadeSwitch());
		}
	}
}

}
