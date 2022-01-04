/** @file cef_manager.h
 * @brief 管理Cef组件的初始化、销毁、消息循环
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/7/19
 */
#pragma once
#include "include/cef_app.h"
#include "cef_control/handler/drag/osr_dragdrop_win.h"

namespace nim_comp
{ 
/** @class CefMessageLoopDispatcher
 * @brief 当multi_threaded_message_loop为false时，需要我们在自己的消息循环中主动调用Cef的消息循环接口
 *		 项目底层组件使用base库的UI消息循环，从而无法使用cef自带的CefRunMessageLoop阻塞消息循环函数
 *		 CefMessageLoopDispatcher继承base库的消息调度接口，规律的调用cef的CefDoMessageLoopWork非阻塞函数
 *		 这样可以让base库兼容cef的消息循环
 *		 当multi_threaded_message_loop为true时，不需要使用本类
 */
class CefMessageLoopDispatcher : public nbase::Dispatcher
{
public:
	virtual bool Dispatch(const MSG &message) override;

private:
	// 判断当前要调度的消息是否为空闲消息，CefDoMessageLoopWork函数在空闲时被调用
	static BOOL IsIdleMessage(const MSG* pMsg);
};


/** @class CefManager
 * @brief Cef组件初始化和销毁
 */
class CefManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(CefManager);
public:
	CefManager();
	~CefManager();

	/**
	* 把cef dll文件的位置添加到程序的"path"环境变量中,这样可以把dll文件放到bin以外的目录，并且不需要手动频繁切换dll文件
	* @return void	无返回值
	*/
	void AddCefDllToPath();

	/**
	* 初始化cef组件
	* @param[in] app_data_dir 应用路径名称
	* @param[in] settings Cef全部配置
	* @param[in] is_enable_offset_render 是否开启离屏渲染
	* @return bool true 继续运行，false 应该结束程序
	*/
	bool Initialize(const std::wstring& app_data_dir, CefSettings &settings, bool is_enable_offset_render = true);

	/**
	* 清理cef组件
	* @return void	无返回值
	*/
	void UnInitialize();

	/**
	* 是否开启离屏渲染
	* @return bool true 开启，false 不开启
	*/
	bool IsEnableOffsetRender() const;

	/**
	* 获取兼容Cef的消息循环分派器
	* @return nbase::Dispatcher* 消息循环分派器
	*/
	nbase::Dispatcher* GetMessageDispatcher();

	// 记录浏览器对象的数量
	void AddBrowserCount();
	void SubBrowserCount();
	int	GetBrowserCount();

	// 在Cef浏览器对象销毁后发送WM_QUIT消息
	void PostQuitMessage(int nExitCode);

	// 获取某个窗口对应的DropTarget，用于浏览器控件的拖动功能
	client::DropTargetHandle GetDropTarget(HWND hwnd);
private:
	/**
	* 设置cef配置信息
	* @param[in] app_data_dir 应用路径名称，见nim::Client::Init的说明
	* @param[out] settings cef配置类
	* @return void	无返回值
	*/
	void GetCefSetting(const std::wstring& app_data_dir, CefSettings &settings);

private:
	CefMessageLoopDispatcher message_dispatcher_;
	int browser_count_;
	bool is_enable_offset_render_;

	std::map<HWND, std::weak_ptr<client::DropTargetWin>> map_drag_target_reference_; // 各个DropTarget的弱引用，一个窗口对应一个DropTarget，这个DropTarget可以给多个BorwserHandler使用
};
}