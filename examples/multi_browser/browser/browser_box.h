/** @file browser_box.h
* @brief 标签页盒子
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2019/3/20
*/
#pragma once

class TaskbarTabItem;
class MultiBrowserForm;
class BrowserBox : public ui::VBox
{
public:
	friend class MultiBrowserForm;

	/**
	* 构造函数
	* @param[in] id	浏览器盒子的唯一标识，用于区分不同的标签页
	*/
	BrowserBox(std::string id);
	~BrowserBox() { };

	/**
	* 获取id
	* @return string 标识id
	*/
	std::string GetId() const { return browser_id_; }

	/**
	* 获取本浏览器盒子所属的浏览器窗口指针
	* @return MultiBrowserForm* 浏览器窗口
	*/
	MultiBrowserForm* GetBrowserForm() const;

	/**
	* 获取Cef控件指针
	* @return ui::CefControlBase* Cef控件
	*/
	nim_comp::CefControlBase* GetCefControl();

	/**
	* 获取网页标题
	* @return std::wstring& 网页标题
	*/
	std::wstring& GetTitle();

	/**
	* 初始化浏览器盒子
	* @param[in] url 初始化URL
	* @return void	无返回值
	*/
	virtual void InitBrowserBox(const std::wstring &url);

	/**
	* 反初始化浏览器盒子
	* @return void	无返回值
	*/
	virtual void UninitBrowserBox();

	/**
	* 根据控件类名创建自定义控件
	* @param[in] pstrClass 控件类名
	* @return Control* 创建的控件的指针
	*/
	ui::Control* CreateControl(const std::wstring& pstrClass);

	//////////////////////////////////////////////////////////////////////////
	//窗口合并功能相关的操作
public:
	/**
	* 获取与本浏览器盒子绑定的Tab指针
	* @return TaskbarItem*	Tab指针
	*/
	TaskbarTabItem* GetTaskbarItem();

	/**
	* 覆盖父类虚函数，用于指定本控件所属窗体及父控件信息
	* @param[in] pManager 所属窗口指针
	* @param[in] pParent 父控件指针
	* @param[in] bInit 是否要初始化本控件
	* @return void	无返回值
	*/
	virtual void SetWindow(ui::Window* pManager, ui::Box* pParent, bool bInit) override;

	/**
	* 覆盖基类虚函数，为了在任务栏显示缩略图和预览图，重写了设置子控件SetInternVisible属性的逻辑
	* @param[in] bVisible 是否显示
	* @return void	无返回值
	*/
	virtual void SetInternVisible(bool bVisible = true) override;

	/**
	* 覆盖基类虚函数，为了在重绘任务栏显示的缩略图
	* @return void	无返回值
	*/
	virtual void Invalidate() override;

	/**
	* 覆盖基类虚函数，为了在重绘任务栏显示的缩略图
	* @param[in] rc 要设置的控件的位置
	* @return void	无返回值
	*/
	virtual void SetPos(ui::UiRect rc) override;

private:
	void OnBeforeMenu(CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model);
	bool OnMenuCommand(CefRefPtr<CefContextMenuParams> params, int command_id, CefContextMenuHandler::EventFlags event_flags);
	void OnTitleChange(const std::wstring& title);
	void OnUrlChange(const std::wstring& url);
	bool OnLinkClick(const std::wstring& url);
	CefRequestHandler::ReturnValue OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect);
	void OnLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward);
	void OnLoadStart();
	void OnLoadEnd(int httpStatusCode);
	void OnLoadError(CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl);

private:
	nim_comp::CefControlBase	*cef_control_;

	// 任务栏缩略图管理
	TaskbarTabItem		*taskbar_item_;
	MultiBrowserForm	*browser_form_;
	std::string			browser_id_;
	std::wstring		url_;
	std::wstring		title_;
};