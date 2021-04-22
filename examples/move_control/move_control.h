#pragma once
#include "app_db.h"

enum ThreadId
{
	kThreadUI
};

class MoveControlForm : public ui::WindowImplBase
{
public:
	MoveControlForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml);
	~MoveControlForm();

	/**
	 * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	 * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	 * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	 * GetWindowClassName	接口设置窗口唯一的类名称
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
	 */
	virtual void InitWindow() override;

	/**
	 * 收到 WM_CLOSE 消息时该函数会被调用
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/**
	* @brief 接收到鼠标左键弹起消息时被调用
	* @param[in] uMsg 消息内容
	* @param[in] wParam 消息附加参数
	* @param[in] lParam 消息附加参数
	* @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
	* @return 返回消息处理结果
	*/
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


public:
	static void ShowCustomWindow(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml);

private:
	//drag-drop相关
	bool OnProcessAppItemDrag(ui::EventArgs* param);
	void DoDrag(ui::Control* pAppItem, POINT pt_offset);
	void DoBeforeDrag();
	void DoDraging(POINT pt_offset);
	bool DoAfterDrag(ui::Box* check);

private:
	std::wstring class_name_;
	std::wstring theme_directory_;
	std::wstring layout_xml_;

	ui::Box* frequent_app_=nullptr;
	ui::Box* my_app_ = nullptr;

	bool is_drag_state_=false;
	POINT old_drag_point_;
	AppItemUi* current_item_ = nullptr;
	
};

