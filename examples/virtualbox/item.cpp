#include "stdafx.h"
#include "item.h"
#include "VirtualTileBox.h"
#include "provider.h"

Item::Item()
:control_img_(nullptr)
{
}


Item::~Item()
= default;

void Item::InitSubControls(const std::wstring& img, const std::wstring& title, int nDataIndex)
{
	// 查找 Item 下的控件
	if (control_img_ == nullptr)
	{
		control_img_ = dynamic_cast<ui::Control*>(FindSubControl(L"control_img"));
		label_title_ = dynamic_cast<ui::Label*>(FindSubControl(L"label_title"));
		progress_ = dynamic_cast<ui::Progress*>(FindSubControl(L"progress"));
		btn_del_ = dynamic_cast<ui::Button*>(FindSubControl(L"btn_del"));
		// 模拟进度条进度
		nbase::TimeDelta time_delta = nbase::TimeDelta::FromMicroseconds(nbase::Time::Now().ToInternalValue());
		t_time = time_delta.ToMilliseconds();
		progress_->SetValue((double)(time_delta.ToMilliseconds() % 100));
		// 设置图标和任务名称
		control_img_->SetBkImage(img);
		// 绑定删除任务处理函数
		btn_del_->AttachClick(nbase::Bind(&Item::OnRemove, this, std::placeholders::_1));
	}


	label_title_->SetText(nbase::StringPrintf(L"%s %d%%", title.c_str(), t_time % 100));
	m_nDataIndex = nDataIndex;

}


bool Item::OnRemove(ui::EventArgs* args)
{
	// 删除时，只需删除数据就可以了，不要删除界面上的元素
	VirtualTileBox* pTileBox = dynamic_cast<VirtualTileBox*>(m_pOwner);
	Provider* pProvider = dynamic_cast<Provider*>(pTileBox->GetDataProvider());
	pProvider->RemoveTask(m_nDataIndex);
	return true;
}
