#ifndef __SHADOW_WND_H__
#define __SHADOW_WND_H__

#pragma once

#include "windows_manager/window_ex.h"

namespace nim_comp {

using namespace ui;

/** @class ShadowWnd
 * @brief 一个附加到基础窗口周围的窗口，带有 WS_EX_LAYERED 属性来实现阴影
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @date 2019-03-22
 */
class ShadowWnd : public WindowImplBase
{
public:
	ShadowWnd();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	
	virtual HWND Create(Window* window);
private:
	Window*	window_ = nullptr;
};

/** @class ShadowWnd
 * @brief 使用附加阴影窗口的基类，实现了创建窗口并附加的功能，提供没有 WS_EX_LAYERED 属性的窗口继承
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @date 2019-03-22
 */
class ShadowWndBase : public WindowImplBase
{
public:
	ShadowWndBase();

	virtual HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, 
		bool isLayeredWindow = false, const ui::UiRect& rc = ui::UiRect(0, 0, 0, 0)) override;

private:
	ShadowWnd* shadow_wnd_;
};
} // namespace ui

#endif // __SHADOW_WND_H__
