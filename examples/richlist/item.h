#pragma once

// 从 ui::ListContainerElement 中继承所有可用功能
class Item : public ui::ListContainerElement
{
public:
	Item();
	~Item();

	// 提供外部调用来初始化 item 数据
	void InitSubControls(const std::wstring& img, const std::wstring& title);
	
private:
	bool OnRemove(ui::EventArgs* args);

private:
	ui::ListBox*	list_box_;

	ui::Control*	control_img_;
	ui::Label*		label_title_;
	ui::Progress*	progress_;
	ui::Button*		btn_del_;
};

