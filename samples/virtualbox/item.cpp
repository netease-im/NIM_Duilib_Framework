#include "stdafx.h"
#include "item.h"


Item::Item()
:control_img_(nullptr)
{
}


Item::~Item()
{
}

void Item::InitSubControls(const std::wstring& img, const std::wstring& title)
{
	// 查找 Item 下的控件
	if (control_img_ == nullptr)
	{
		control_img_ = dynamic_cast<ui::Control*>(FindSubControl(L"control_img"));
		label_title_ = dynamic_cast<ui::Label*>(FindSubControl(L"label_title"));
		progress_ = dynamic_cast<ui::Progress*>(FindSubControl(L"progress"));
		btn_del_ = dynamic_cast<ui::Button*>(FindSubControl(L"btn_del"));
		// 模拟进度条进度
		auto timestamp = shared::tools::GenerateTimeStamp();
		int64_t timestamp_num = 0;	
		nbase::StringToInt64(timestamp, &timestamp_num);
		t_time = timestamp_num;
		progress_->SetValue((double)(timestamp_num % 100));
		// 设置图标和任务名称
		control_img_->SetBkImage(img);
		// 绑定删除任务处理函数
		btn_del_->AttachClick(nbase::Bind(&Item::OnRemove, this, std::placeholders::_1));
	}


	label_title_->SetText(nbase::StringPrintf(L"%s %d%%", title.c_str(), t_time % 100));


}


bool Item::OnRemove(ui::EventArgs* args)
{
	// 删除时，只需删除数据就可以了，不要删除界面上的元素
	return true;
}
