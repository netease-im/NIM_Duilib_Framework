/** @file cef_control.h
 * @brief 封装Cef浏览器对象为duilib控件
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/7/19
 */
#pragma once
#include "cef_control_base.h"
#include "cef_control/util/memory_dc.h"

namespace nim_comp {

class CefControl :public CefControlBase, public IUIMessageFilter
{	
public:
	CefControl(void);
	~CefControl(void);	

	/// 重写父类接口，提供个性化功能
	virtual void Init() override;
	virtual void SetPos(UiRect rc) override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual void SetVisible(bool bVisible = true) override;
	virtual void SetInternVisible(bool bVisible = true) override;
	virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint) override;
	virtual void SetWindow(ui::Window* pManager, ui::Box* pParent, bool bInit) override;

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override; // 处理窗体消息，转发到Cef浏览器对象

	/**
	* @brief 打开开发者工具
	* @param[in] view 一个 CefControl 控件实例(仅在CefControl类里需要传入)
	* @return 成功返回 true，失败返回 false
	*/
	virtual bool AttachDevTools(Control* view) override;

protected:
	virtual void ReCreateBrowser() override;

protected:
	/**
	 * @brief 转发鼠标按下消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendButtonDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 转发鼠标双击消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendButtonDoubleDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 转发鼠标弹起消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendButtonUpEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 转发鼠标移动消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendMouseMoveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 转发鼠标滚动消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendMouseWheelEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 转发鼠标离开消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendMouseLeaveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 转发键盘相关消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * @brief 转发捕获焦点消息到 BrowserHost
	 * @param[in] uMsg 消息
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] bHandled 是否继续传递消息
	 * @return 返回消息处理结果
	 */
	LRESULT SendCaptureLostEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	/**
	 * @brief 判断是否有按键按下
	 * @param[in] wparam 消息附加参数
	 * @return 返回 true 表示有按键按下，false 表示没有按键按下
	 */
	static bool IsKeyDown(WPARAM wparam);

	/**
	 * @brief 转换普通键盘消息到 CEF 可识别的键盘消息
	 * @param[in] wparam 消息附加参数
	 * @param[in] lparam 消息附加参数
	 * @return 返回转换后的结果
	 */
	static int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam);

	/**
	 * @brief 转换普通鼠标消息到 CEF 可识别的鼠标消息
	 * @param[in] wparam 消息附加参数
	 * @return 返回转换后的结果
	 */
	static int GetCefMouseModifiers(WPARAM wparam);

private:
	// 当浏览器渲染数据变化时，会触发此接口，此时把渲染数据保存到内存dc
	// 并且通知窗体刷新控件，在控件的Paint函数里把内存dc的位图画到窗体上
	// 由此实现离屏渲染数据画到窗体上
	virtual void OnPaint(CefRefPtr<CefBrowser> browser,
		CefRenderHandler::PaintElementType type,
		const CefRenderHandler::RectList& dirtyRects,
		const std::string* buffer,
		int width,
		int height) OVERRIDE;

	virtual void ClientToControl(POINT &pt) OVERRIDE;

	virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;

	virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE;

private:
	MemoryDC			dc_cef_;		// 内存dc,把cef离屏渲染的数据保存到dc中
	MemoryDC			dc_cef_popup_;	// 内存dc,把cef的popup窗口的离屏渲染数据保存到dc中
	CefRect				rect_popup_;	// 当网页的组合框一类的控件弹出时，记录弹出的位置
};

}