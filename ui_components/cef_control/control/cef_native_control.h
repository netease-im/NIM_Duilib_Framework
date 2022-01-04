/** @file cef_control.h
 * @brief 封装Cef浏览器对象为duilib控件
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/7/19
 */
#pragma once
#include "cef_control_base.h"

namespace nim_comp {

using namespace ui;

class CefNativeControl : public CefControlBase
{
public:
	CefNativeControl(void);
	~CefNativeControl(void);	

	virtual void Init() override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual void SetVisible(bool bVisible = true) override;
	virtual void SetInternVisible(bool bVisible = true) override;
	virtual void SetWindow(ui::Window* pManager, ui::Box* pParent, bool bInit) override;

	/**
	* @brief 打开开发者工具
	* @param[in] view 一个 CefControl 控件实例(仅在CefControl类里需要传入)
	* @return 成功返回 true，失败返回 false
	*/
	virtual bool AttachDevTools(Control* view) override;

protected:
	virtual void ReCreateBrowser() override;
};
}