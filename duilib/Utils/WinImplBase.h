#ifndef UI_UTILS_WINIMPLBASE_H_
#define UI_UTILS_WINIMPLBASE_H_

#pragma once

namespace ui
{

#define UI_WNDSTYLE_FRAME		(WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_DIALOG		(WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_CLASSSTYLE_FRAME		(CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_DIALOG	(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

class UILIB_API WindowImplBase : public Window, public IUIMessageFilter
{
public:
	WindowImplBase();
	virtual ~WindowImplBase();

    /**
     * @brief 当接收到窗口创建消息时被调用
     * @return 无
     */
	virtual void InitWindow(){}

    /**
     * @brief 当接收到窗口销毁消息时被调用
     * @param[in] hWnd 要销毁的窗口句柄
     * @return 无
     */
	virtual void OnFinalMessage( HWND hWnd ) override;

    /**
     * @brief 获取窗口样式
     * @return 返回窗口样式
     */
	virtual LONG GetStyle();

    /**
     * @brief 获取窗口类样式
     * @return 返回窗口类样式
     */
	virtual UINT GetClassStyle() const;

    /**
     * @brief 当要创建的控件不是标准的控件名称时会调用该函数
     * @param[in] pstrClass 控件名称
     * @return 返回一个自定义控件指针，一般情况下根据 pstrClass 参数创建自定义的控件
     */
	virtual Control* CreateControl(const std::wstring& pstrClass);

    /**
     * @brief 接收所有消息
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当收到窗口关闭消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当收到窗口被销毁消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当收到活动或非活动状态消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当收到要计算客户区域大小消息时（WM_NCCALCSIZE）被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 窗口位置或 Z 次序发生改变时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当接收到绘制标题栏区域消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当接收到 WM_NCHITTEST 消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当接收到 WM_GETMINMAXINFO 消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当接收到鼠标悬浮消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当接收到窗口大小改变消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当接收到窗口大小改变消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 当接收到字符按键消息时（WM_CHAR）被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 接收窗口控制命令消息时（WM_SYSCOMMAND）被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 接收键盘按键按下消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 接收失去焦点消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 接收获取焦点消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 接收到鼠标左键按下消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 接收到鼠标左键弹起消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 接收到鼠标移动消息时被调用
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 返回 false 则继续派发该消息，否则不再派发该消息
     * @return 返回消息处理结果
     */
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 窗口消息的派发函数
     * @param[in] uMsg 消息内容
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @return 返回消息处理结果
     */
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

public:
    /**
    * @brief 激活窗口
    * @return void 无返回值
    */
	virtual void ActiveWindow();

    /**
    * @brief 设置窗口标题
    * @param[in] title 窗口标题
    * @return void 无返回值
    */
	virtual void SetTaskbarTitle(const std::wstring &title);

    /**
    * @brief 置顶窗口
    * @param[in] forever 是否一直置顶
    * @return void 无返回值
    */
	void ToTopMost(bool forever);
		
protected:
    /**
     * @brief 创建窗口时被调用，由子类实现用以获取窗口皮肤目录
     * @return 子类需实现并返回窗口皮肤目录
     */
	virtual std::wstring GetSkinFolder() = 0;

    /**
     * @brief 创建窗口时被调用，由子类实现用以获取窗口皮肤 XML 描述文件
     * @return 子类需实现并返回窗口皮肤 XML 描述文件
     */
	virtual std::wstring GetSkinFile() = 0;

    /**
     * @brief 创建窗口时被调用，由子类实现用以获取窗口唯一的类名称
     * @return 子类需实现并返回窗口唯一的类名称
     */
	virtual std::wstring GetWindowClassName(void) const = 0 ;

private:
    /**
     * @brief 收到窗口创建消息时被调用，请使用 InitWindow 接口来实现自定义需求
     * @param[in] uMsg 消息ID
     * @param[in] wParam 消息附加参数
     * @param[in] lParam 消息附加参数
     * @param[out] bHandled 消息是否已经被处理
     * @return 返回消息处理结果
     */
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /**
     * @brief 窗口功能按钮被点击时调用
     * @param[in] param 携带的参数
     * @return 始终返回 true
     */
	bool OnButtonClick(EventArgs* param);
};

}

#endif // UI_UTILS_WINIMPLBASE_H_
