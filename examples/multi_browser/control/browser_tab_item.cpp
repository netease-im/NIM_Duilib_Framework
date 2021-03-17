#include "browser_tab_item.h"

using namespace ui;

BrowserTabItem::BrowserTabItem()
{

}

void BrowserTabItem::InitControl(std::string browser_id)
{
	this->AttachMenu(nbase::Bind(&BrowserTabItem::OnItemMenu, this, std::placeholders::_1));
	this->AttachMouseEnter(nbase::Bind(&BrowserTabItem::OnMouseEnter, this, std::placeholders::_1));
	this->AttachMouseLeave(nbase::Bind(&BrowserTabItem::OnMouseLeave, this, std::placeholders::_1));

	icon_ = (Button*) this->FindSubControl(L"tab_item_icon");
	name_ = (Label*) this->FindSubControl(L"tab_item_name");
	button_close_ = (Button*) this->FindSubControl(L"tab_item_close");

	this->SetUTF8Name(browser_id);
}

void BrowserTabItem::SetTitle(std::wstring name)
{
	this->SetToolTipText(name);
	name_->SetText(name);
}

void BrowserTabItem::SetIcon(std::wstring icon)
{
	icon_->SetBkImage(icon);
}

bool BrowserTabItem::OnMouseEnter(ui::EventArgs* msg)
{
	button_close_->SetVisible(true);

	return true;
}

bool BrowserTabItem::OnMouseLeave(ui::EventArgs* msg)
{
	if (!m_rcItem.IsPointIn(msg->ptMouse))
	{
		button_close_->SetVisible(false);
	}
	
	return false;
}

bool BrowserTabItem::OnItemMenu(ui::EventArgs* arg)
{
	POINT point;
	::GetCursorPos(&point);
	PopupItemMenu(point);
	return true;
}

void BrowserTabItem::PopupItemMenu(POINT point)
{

}

bool BrowserTabItem::MergeItemMenuItemClick(ui::EventArgs* param)
{
	return true;
}
