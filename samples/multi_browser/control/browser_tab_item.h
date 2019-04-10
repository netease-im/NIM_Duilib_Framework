#pragma once

/** @class BrowserTabItem
  * @brief 多标签浏览器窗口顶部标签控件
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2019/3/20
  */
class BrowserTabItem : public ui::ListContainerElement
{
public:
	BrowserTabItem();

	/** 
	* 初始化控件内部指针	
	* @param[in] browser_id 浏览器id
	* @return void 无返回值
	*/
	void InitControl(std::string browser_id);

	/**
	* 设置显示的浏览器标题
	* @param[in] name 标题
	* @return void	无返回值
	*/
	void SetTitle(std::wstring name);

	/**
	* 设置显示的浏览器头像
	* @param[in] icon 头像路径
	* @return void	无返回值
	*/
	void SetIcon(std::wstring icon);

private:

	/**
	* 处理鼠标移入消息
	* @param[in] msg 消息的相关信息
	* @return bool true 继续传递控件消息，false 停止传递控件消息
	*/
	bool OnMouseEnter(ui::EventArgs* msg);

	/**
	* 处理鼠标移出消息
	* @param[in] msg 消息的相关信息
	* @return bool true 继续传递控件消息，false 停止传递控件消息
	*/
	bool OnMouseLeave(ui::EventArgs* msg);

	/**
	* 处理控件的菜单消息
	* @param[in] msg 消息的相关信息
	* @return bool true 继续传递控件消息，false 停止传递控件消息
	*/
	bool OnItemMenu(ui::EventArgs* msg);

	/**
	* 弹出菜单
	* @param[in] point 弹出坐标
	* @return void	无返回值
	*/
	void PopupItemMenu(POINT point);

	/**
	* 处理菜单项被单击的消息
	* @param[in] msg 消息的相关信息
	* @return bool true 继续传递控件消息，false 停止传递控件消息
	*/
	bool MergeItemMenuItemClick(ui::EventArgs* msg);

private:
	ui::Control		*icon_;
	ui::Label		*name_;
	ui::Button		*button_close_;

	std::wstring	session_id_;
};