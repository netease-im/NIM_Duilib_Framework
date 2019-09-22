#pragma once
#include "browser_box.h"

enum ThreadId
{
	kThreadUI
};

/** @class MultiBrowserManager
  * @brief 多标签浏览器管理类，用于统一管理多标签浏览器打开、合并、拆分、拖拽功能
  * @date 2019/3/20
  */
class SdkDataObject;
class MultiBrowserManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(MultiBrowserManager);
public:
	MultiBrowserManager();
	~MultiBrowserManager();

	/**
	* 创建一个浏览器盒子
	* @param[in] browser_form 浏览器窗口，如果为NULL则创建一个新的窗口，否则在这个窗口里新建浏览器盒子
	* @param[in] id 某个浏览器盒子的唯一标识
	* @param[in] url 初始化URL
	* @return BorwserBox*	浏览器盒子指针
	*/
	BrowserBox* CreateBorwserBox(MultiBrowserForm *browser_form, const std::string &id, const std::wstring &url);

	/**
	* 判断某个浏览器盒子是否处于激活状态
	* @param[in] id 某个浏览器盒子的唯一标识
	* @return bool true 激活，false 非激活
	*/
	bool IsBorwserBoxActive(const std::string& id);

	/**
	* 根据会话id，找回浏览器盒子
	* @param[in] id 某个浏览器盒子的唯一标识
	* @return BorwserBox* 返回浏览器盒子的指针
	*/
	BrowserBox* FindBorwserBox(const std::string &id);

	/**
	* 根据会话id，移除本类中浏览器盒子信息
	* @param[in] id 某个浏览器盒子的唯一标识
	* @param[in] box id对应的浏览器盒子指针
	* @return void 无返回值
	*/
	void RemoveBorwserBox(std::string id, const BrowserBox* box = NULL);

public:
	/**
	* 设置是否开启标签合并功能
	* @param[in] enable true:开启合并功能,自动合并当前所有浏览器盒子
	*					false:关闭合并功能,自动拆分当前所有浏览器盒子
	* @return 无返回值
	*/
	void SetEnableMerge(bool enable);

	/**
	* 是否开启标签合并功能
	* @return bool true 是，false 否
	*/
	bool IsEnableMerge() const;

	/**
	* 设置是否使用自定义的拖拽缩略图
	* @param[in] use true:使用自定义拖拽缩略图，会使用低级键盘钩子，一些安全软件会拦截
	*				 false:使用系统的拖拽缩略图，效果上不如自定义的好
	* @return 无返回值
	*/
	void SetUseCustomDragImage(bool use);

	/**
	* 是否使用自定义的拖拽缩略图
	* @return bool true 是，false 否
	*/
	bool IsUseCustomDragImage() const;

	/**
	* 当前是否正在拖拽浏览器盒子
	* @return bool true 是，false 否
	*/
	bool IsDragingBorwserBox() const;

	/**
	* 设置被放入浏览器盒子的浏览器窗体
	* @param[in] browser_form 浏览器窗口
	* @return void	无返回值
	*/
	void SetDropForm(MultiBrowserForm *browser_form);

	/**
	* 执行拖拽浏览器盒子的操作
	* @param[in] browser_box 浏览器盒子
	* @param[in] bitmap 浏览器盒子生成的位图
	* @param[in] pt_offset 拖拽图像相对于鼠标的偏移
	* @return bool true 成功，false 失败
	*/
	bool DoDragBorwserBox(BrowserBox *browser_box, HBITMAP bitmap, POINT pt_offset);

private:

	/**
	* 创建一个用于拖拽的IDataObject对象
	* @param[in] bitmap 用于生成拖拽效果的位图
	* @param[in] pt_offset 拖拽图像相对于鼠标的偏移
	* @return SdkDataObject* IDataObject对象
	*/
	SdkDataObject* CreateDragDataObject(HBITMAP bitmap, POINT pt_offset);

	/**
	* 执行拖拽操作前被调用
	* @return void	无返回值
	*/
	void OnBeforeDragBorwserBox(BrowserBox *browser_box, HBITMAP bitmap, POINT pt_offset);

	/**
	* 执行拖拽操作后被调用
	* @return void	无返回值
	*/
	void OnAfterDragBorwserBox();

private:
	bool				enable_merge_;
	bool				use_custom_drag_image_;
	BrowserBox			*draging_box_;	// 当下正在被拖拽的浏览器盒子
	MultiBrowserForm	*drop_browser_form_;	// 拖入浏览器盒子的浏览器窗口，用于附加拖拽来的浏览器盒子

	std::map<std::string, BrowserBox*>	box_map_;
};