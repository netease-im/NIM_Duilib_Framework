#pragma once
#include "duilib/UIlib.h"

namespace nim_comp
{
/** @class WindowEx
 * @brief 所有窗体的基类
 * @copyright (c) 2015, NetEase Inc. All rights reserved
 * @date 2015/9/16
 */
class WindowEx : public ui::WindowImplBase
{
public:
	WindowEx();
	virtual ~WindowEx();

	/**
	 * 创建窗口
	 * @param[in] hwndParent 父窗口句柄
	 * @param[in] pstrName 窗口名称
	 * @param[in] dwStyle 窗口样式
	 * @param[in] dwExStyle 窗口扩展样式
	 * @param[in] isLayeredWindow 是否创建分层窗口
	 * @param[in] rc 窗口位置
	 * @return HWND 窗口句柄
	 */
	virtual HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle,
		bool isLayeredWindow = true, const ui::UiRect& rc = ui::UiRect(0, 0, 0, 0)) override;

	/**
	 * 处理窗口被销毁的消息
	 * @param[in] uMsg 消息
	 * @param[in] wParam 参数
	 * @param[in] lParam 参数
	 * @param[out] bHandled 消息是否被处理
	 * @return LRESULT 处理结果
	 */
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * 处理ESC键单击的消息
	 * @param[out] bHandled 消息是否被处理
	 * @return void 无返回值
	 */
	virtual void OnEsc(BOOL &bHandled);

	/**
	 * 获取窗口类名的接口
	 * @return wstring 窗口类名
	 */
	virtual std::wstring GetWindowClassName(void) const = 0;

	/**
	 * 获取窗口id的接口
	 * @return wstring 窗口id
	 */
	virtual std::wstring GetWindowId(void) const = 0;

	/**
	 * 处理窗口消息
	 * @param[in] uMsg 消息
	 * @param[in] wParam 参数
	 * @param[in] lParam 参数
	 * @return LRESULT 处理结果
	 */
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	/**
	 * 从WindowManager中注册自己
	 * @return bool true 注册成功，false 注册失败
	 */
	bool RegisterWnd();

	/**
	 * 从WindowManager中反注册自己
	 * @return void 无返回值
	 */
	void UnRegisterWnd();
};

/**
* 获取弹出窗口可以显示在右下角位置的坐标
* @return POINT 窗口坐标
*/
POINT GetPopupWindowPos(WindowEx* window);

}