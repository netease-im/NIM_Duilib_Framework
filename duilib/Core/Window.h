#ifndef UI_CORE_WINDOW_H_
#define UI_CORE_WINDOW_H_

#pragma once

namespace ui 
{

class Box;

// Flags for Control::GetControlFlags()
#define UIFLAG_DEFAULT       0x00000000		// 默认状态
#define UIFLAG_TABSTOP       0x00000001		// 标识控件是否在收到 TAB 切换焦点时允许设置焦点

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
	HFONT hFont;
	std::wstring sFontName;
	int iSize;
	int iWeight;
	bool bBold;
	bool bUnderline;
	bool bStrikeout;
	bool bItalic;
	TEXTMETRIC tm;
} TFontInfo;

// MessageFilter interface
class IUIMessageFilter
{
public:
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
};

class ITranslateAccelerator
{
public:
	virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
};
class IControlFromPointFinder
{
public:
	virtual Control* FindControlFromPoint(const CPoint& pt) = 0;
};
/////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

/////////////////////////////////////////////////////////////////////////////////////
//

class UIAWindowProvider;

class UILIB_API Window : public virtual nbase::SupportWeakCallback
{
public:
    Window();
	~Window();

	/**
	 * @brief 获取窗口所属的 Windows 句柄
	 * @return 返回窗口关联的 Windows 窗口句柄
	 */
    HWND GetHWND() const;

	/**
	 * @brief 注册窗口类
	 * @return 返回 true 表示成功，否则表示失败
	 */
    bool RegisterWindowClass();

	/**
	 * @brief 注册控件窗口类（与窗口的过程函数不同）
	 * @return 返回 true 表示成功，否则表示失败
	 */
    bool RegisterSuperClass();

	/**
	 * @brief 获取窗口类名称
	 * @return 返回窗口类名称
	 */
	virtual std::wstring GetWindowClassName() const;

	/**
	 * @brief 获取控件窗口类
	 * @return 返回控件窗口类
	 */
	virtual std::wstring GetSuperClassName() const;

	/**
	 * @brief 获取窗口类的样式，该方法由实例化的子类实现，https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles
	 * @return 返回窗口类的样式
	 */
	virtual UINT GetClassStyle() const;

	/**
	 * @brief 子类化窗口（更改窗口过程函数）
	 * @param[in] 窗口句柄
	 * @return 新的窗口句柄
	 */
	HWND Subclass(HWND hWnd);

	/**
	 * @brief 取消子类化窗口（恢复原来的窗口过程函数）
	 * @return 无
	 */
	void Unsubclass();

	/**
	 * @brief 创建窗口
	 * @param[in] hwndParent 父窗口句柄
	 * @param[in] pstrName 窗口名称
	 * @param[in] dwStyle 窗口样式
	 * @param[in] dwExStyle 窗口拓展样式
	 * @param[in] isLayeredWindow 是否带有层窗口属性，默认为 true
	 * @param[in] rc 窗口大小
	 * @return 返回窗口句柄
	 */
	virtual HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, bool isLayeredWindow = true, const UiRect& rc = UiRect(0, 0, 0, 0));

	/**
	 * @brief 关闭窗口
	 * @param[in] 关闭消息
	 * @return 无
	 */
	virtual void Close(UINT nRet = IDOK);

	/**
	 * @brief 显示或隐藏窗口
	 * @param[in] bShow 为 true 时显示窗口，为 false 时为隐藏窗口，默认为 true
	 * @param[in] bTakeFocus 是否获得焦点（激活窗口），默认为 true
	 * @return 无
	 */
	virtual void ShowWindow(bool bShow = true, bool bTakeFocus = true);
	
	/**
	 * @brief 显示模态对话框（推荐）
	 * @param[in] parent_hwnd 父窗口句柄
	 * @return 无
	 */
	void ShowModalFake(HWND parent_hwnd);

	/**
	 * @brief 是否是模态显示
	 * @return 是否是模态显示
	 */
	bool IsFakeModal();

	/**
	 * @brief 居中窗口，支持扩展屏幕
	 * @return 无
	 */
    void CenterWindow();	

	/**
	 * @brief 设置窗口图标
	 * @param[in] nRes 窗口图标资源 ID
	 * @return 无
	 */
    void SetIcon(UINT nRes);

	/**
	 * @brief 发送消息，对 Windows SendMessage 的一层封装
	 * @param[in] uMsg 消息类型
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回窗口对消息的处理结果
	 */
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**
	 * @brief 投递一个消息到消息队列
	 * @param[in] uMsg 消息类型
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回窗口对消息的处理结果
	 */
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**
	 * @brief 监听窗口关闭事件
	 * @param[in] callback 指定关闭后的回调函数
	 * @return 无
	 */
	void AttachWindowClose(const EventCallback& callback);

protected:
	/**
	 * @brief 在窗口收到 WM_NCDESTROY 消息时会被调用
	 * @param[in] hWnd 窗口句柄
	 * @return 无
	 */
	virtual void OnFinalMessage(HWND hWnd);

	/**
	 * @brief 窗口过程函数
	 * @param[in] hWnd 窗口句柄
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回消息处理结果
	 */
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief 待补充
	 * @param[in] 待补充
	 * @return 待补充
	 */
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/**
	 * @brief 窗口接收到 WM_CREATE 消息时会被调用
	 * @param[in] hWnd 窗口句柄
	 * @return 无
	 */
	void Init(HWND hWnd);

	/**
	 * @brief 绑定窗口的顶层容器
	 * @param[in] pRoot 容器指针
	 * @return 返回 true 绑定成功，否则为失败
	 */
	bool AttachDialog(Box* pRoot);

	/**
	 * @brief 初始化控件，在容器中添加控件时会被调用（用于对控件名称做缓存）
	 * @param[in] pControl 控件指针
	 * @param[in] pParent 控件父容器
	 * @return true 为成功，否则为失败
	 */
	bool InitControls(Control* pControl, Box* pParent = NULL);

	/**
	 * @brief 回收控件
	 * @param[in] pControl 控件指针
	 * @return 无
	 */
	void ReapObjects(Control* pControl);

	/// 资源相关部分
	/**
	 * @brief 获取窗口资源路径
	 * @return 返回窗口资源路径
	 */
	std::wstring GetWindowResourcePath();

	/**
	 * @brief 设置窗口资源路径
	 * @param[in] strPath 要设置的路径
	 * @return 无
	 */
	void SetWindowResourcePath(const std::wstring& strPath);

	/**
	 * @brief 获取默认字体信息
	 * @return 返回默认字体信息结构
	 */
	TFontInfo* GetDefaultFontInfo();

	/**
	 * @brief 添加一个通用样式
	 * @param[in] strClassName 通用样式的名称
	 * @param[in] strControlAttrList 通用样式的 XML 转义格式数据
	 * @return 无
	 */
	void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList);

	/**
	 * @brief 获取所有通用样式
	 * @return 返回所有通用样式的 map 数据
	 */
	const std::map<std::wstring, std::wstring>* GetClassMap();

	/**
	 * @brief 获取指定通用样式的内容
	 * @param[in] strClassName 通用样式名称
	 * @return 返回指定名称的通用样式内容，XML 转义格式数据
	 */
	std::wstring GetClassAttributes(const std::wstring& strClassName) const;

	/**
	 * @brief 删除一个通用样式
	 * @param[in] strClassName 要删除的通用样式名称
	 * @return 返回 true 为成功，false 为失败或样式不存在
	 */
	bool RemoveClass(const std::wstring& strClassName);

	/**
	 * @brief 删除所有通用样式
	 * @return 无
	 */
	void RemoveAllClass();

	/**
	 * @brief 添加一个颜色值提供窗口内使用
	 * @param[in] strName 颜色名称（如 white）
	 * @param[in] strValue 颜色具体数值（如 #FFFFFFFF）
	 * @return 无
	 */
	void AddTextColor(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief 添加一个颜色值提供窗口内使用
	 * @param[in] strName 颜色名称（如 white）
	 * @param[in] strValue 颜色具体数值（如 #FFFFFFFF）
	 * @return 无
	 */
	void AddTextColor(const std::wstring& strName, DWORD argb);

	/**
	 * @brief 根据名称获取一个颜色的具体数值
	 * @param[in] strName 要获取的颜色名称
	 * @return 返回 DWORD 格式的颜色描述值
	 */
	DWORD GetTextColor(const std::wstring& strName);

	/**
	 * @brief 添加一个选项组
	 * @param[in] strGroupName 组名称
	 * @param[in] pControl 控件指针
	 * @return 返回 true 表示添加成功，false 可能组已经存在
	 */
	bool AddOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**
	 * @brief 获取指定选项组中控件列表
	 * @param[in] strGroupName 指定组名称
	 * @return 返回该组下的所有控件列表
	 */
	std::vector<Control*>* GetOptionGroup(const std::wstring& strGroupName);

	/**
	 * @brief 删除一个选项组
	 * @param[in] strGroupName 组名称
	 * @param[in] pControl 控件名称
	 * @return 无
	 */
	void RemoveOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**
	 * @brief 删除所有选项组
	 * @return 无
	 */
	void RemoveAllOptionGroups();

	/**
	 * @brief 清理图片缓存
	 * @return 无
	 */
	void ClearImageCache();

	/// 窗口属性
	/**
	 * @brief 获取鼠标位置
	 * @return 鼠标位置的 x 和 y 坐标
	 */
	POINT GetMousePos() const;

	/**
	 * @brief 获取窗口四边可拉伸范围的大小
	 * @return 返回窗口四边可拉伸范围的大小
	 */
	UiRect GetSizeBox();

	/**
	 * @brief 设置窗口四边可拉伸范围的大小
	 * @param[in] rcSizeBox 要设置的大小
	 * @return 无
	 */
	void SetSizeBox(const UiRect& rcSizeBox);

	/**
	 * @brief 获取窗口标题栏区域（可拖动区域），对应 XML 中 caption 属性
	 * @return 返回标题栏区域
	 */
	UiRect GetCaptionRect() const;

	/**
	 * @brief 设置窗口标题栏区域
	 * @param[in] rcCaption 要设置的区域范围
	 * @return 无
	 */
	void SetCaptionRect(UiRect& rcCaption);

	/**
	 * @brief 获取窗口圆角大小，对应 XML 中 roundcorner 属性
	 * @return 返回圆角大小
	 */
	CSize GetRoundCorner() const;

	/**
	 * @brief 设置窗口圆角大小
	 * @param[in] cx 圆角宽
	 * @param[in] cy 圆角高
	 * @return 无
	 */
	void SetRoundCorner(int cx, int cy);

	/**
	 * @brief 获取窗口最大化信息
	 * @return 返回窗口最大化信息
	 */
	UiRect GetMaximizeInfo() const;

	/**
	 * @brief 设置窗口最大化信息
	 * @param[in] rcMaximize 要设置的最大化数值
	 * @return 无
	 */
	void SetMaximizeInfo(UiRect& rcMaximize);

	/**
	 * @brief 获取透明通道修补范围的的九宫格描述，对应 XML 中 alphafixcorner 属性
	 * @return 返回修补范围
	 */
	UiRect GetAlphaFixCorner() const;

	/**
	 * @brief 设置透明通道修补范围的的九宫格描述
	 * @param[in] rc 要设置的修补范围
	 * @return 无
	 */
	void SetAlphaFixCorner(UiRect& rc);

	/**
	 * @brief 获取窗口的初始高度占屏幕高度的百分比，对应 XML 中 heightpercent 属性
	 * @return 返回指定百分比数据
	 */
	double GetHeightPercent() const;

	/**
	 * @brief 设置窗口的初始高度占屏幕高度的百分比
	 * @param[in] heightPercent 百分比
	 * @return 无
	 */
	void SetHeightPercent(double heightPercent);

	/**
	 * @brief 根据语言列表中的文本 ID 设置窗口标题栏文本
	 * @param[in] strTextId 语言 ID，该 ID 必须在语言文件中存在
	 * @return 无
	 */
	void SetTextId(const std::wstring& strTextId);

	/// 阴影相关部分
	/**
	 * @brief 设置窗口是否附加阴影效果
	 * @param[in] bShadowAttached 为 true 时附加，false 时不附加
	 * @return 无
	 */
	void SetShadowAttached(bool bShadowAttached);

	/**
	* @brief 获取是否附加阴影效果
	* @return 是否附加阴影效果
	*/
	bool IsShadowAttached();

	/**
	 * @brief 获取阴影图片
	 * @return 返回阴影图片位置
	 */
	std::wstring GetShadowImage() const;

	/**
	 * @brief 设置窗口阴影图片
	 * @param[in] strImage 图片位置
	 * @return 无
	 */
	void SetShadowImage(const std::wstring &strImage);

	/**
	 * @brief 获取阴影的九宫格描述信息
	 * @return 返回阴影的九宫格描述信息
	 */
	UiRect GetShadowCorner() const;

	/**
	 * @brief 指定阴影素材的九宫格描述
	 * @param[in] rect 九宫格描述信息
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 * @return 无
	 */
	void SetShadowCorner(const UiRect rect, bool bNeedDpiScale = true);

	/**
	 * @brief 获取窗口位置信息
	 * @param[in] bContainShadow 是否包含阴影，true 为包含，默认为 false 不包含
	 * @return 返回窗口位置信息
	 */
	UiRect GetPos(bool bContainShadow = false) const;

	/**
	 * @brief 设置窗口位置（对 SetWindowPos 的一层封装）
	 * @param[in] rc 窗口位置
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 * @param[in] uFlags 对应 SetWindowPos 的 uFlags 选项
	 * @param[in] hWndInsertAfter 对应 SetWindowPos 的 hWndInsertAfter 选项
	 * @param[in] bContainShadow 是否包含阴影范围，默认为 false
	 * @return 无
	 */
	void SetPos(const UiRect& rc, bool bNeedDpiScale, UINT uFlags, HWND hWndInsertAfter = NULL, bool bContainShadow = false);

	/**
	 * @brief 获取窗口最小范围，对应 XML 中 mininfo 属性
	 * @param[in] bContainShadow 是否包含阴影范围，默认为 false
	 * @return 返回指定大小
	 */
	CSize GetMinInfo(bool bContainShadow = false) const;

	/**
	 * @brief 获取窗口最大范围，对应 XML 中 maxinfo 属性
	 * @param[in] bContainShadow 是否包含阴影范围，默认为 false
	 * @return 返回指定大小
	 */
	CSize GetMaxInfo(bool bContainShadow = false) const;

	/**
	 * @brief 获取窗口初始大小
	 * @param[in] bContainShadow 是否包含阴影范围，默认为 false
	 * @return 返回指定大小
	 */
	CSize GetInitSize(bool bContainShadow = false) const;

	/**
	 * @brief 设置窗口最小范围
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 * @return 无
	 */
	// 
	void SetMinInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief 设置窗口最大范围
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不需要把 rc 根据 DPI 自动调整
	 * @return 无
	 */
	void SetMaxInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief 重置窗口大小
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不根据 DPI 调整
	 * @return 无
	 */
	void Resize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief 设置窗口初始大小
	 * @param[in] cx 宽度
	 * @param[in] cy 高度
	 * @param[in] bContainShadow 为 false 表示 cx cy 不包含阴影
	 * @param[in] bNeedDpiScale 为 false 表示不根据 DPI 调整
	 * @return 无
	 */
	void SetInitSize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief 添加一个消息过滤器，此时消息已经派发
	 * @param[in] pFilter 一个继承了 IUIMessageFilter 的对象实例，需要实现 MessageHandler 方法
	 * @return 始终返回 true
	 */
	bool AddMessageFilter(IUIMessageFilter* pFilter);

	/**
	 * @brief 移除一个消息过滤器
	 * @param[in] pFilter 一个继承了 IUIMessageFilter 的对象实例
	 * @return 返回 true 表示移除成功，否则可能该过滤器不存在
	 */
	bool RemoveMessageFilter(IUIMessageFilter* pFilter);

	/**
	 * @brief 查找控件时添加一个根据位置查找控件的钩子
	 * @param[in] pFinder 继承了 IControlFromPointFinder 对象指针，需实现 FindControlFromPoint 方法
	 * @return 始终返回 true
	 */
	bool AddControlFromPointFinder(IControlFromPointFinder* pFinder);

	/**
	 * @brief 移除查找控件时根据位置查找控件的钩子
	 * @param[in] pFinder 继承了 IControlFromPointFinder 对象指针
	 * @return 成功返回 true，否则该钩子可能不存在
	 */
	bool RemoveControlFromPointFinder(IControlFromPointFinder* pFinder);

	/**
	 * @brief 添加一个 TranslateMessage 之前的消息过滤器
	 * @param[in] pTranslateAccelerator 继承了 ITranslateAccelerator 对象指针，需实现 TranslateAccelerator 方法
	 * @return 始终返回 true
	 */
	bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);

	/**
	 * @brief 移除一个 TranslateMessage 之前的消息过滤器
	 * @param[in] pTranslateAccelerator 继承了 ITranslateAccelerator 对象指针
	 * @return 成功返回 true，否则返回 false 可能该过滤器不存在
	 */
	bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);

	/**
	 * @brief 执行 TranslateMessage 阶段的过滤器
	 * @param[in] pMsg 消息体
	 * @return 返回 true 成功处理消息，否则返回 false
	 */
	bool TranslateAccelerator(LPMSG pMsg);

	/**
	 * @brief 窗口消息的派发函数
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回 false 则继续派发该消息，否则不再派发该消息
	 */
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief 窗口消息的执行体，用于执行各类过滤器和处理各类消息
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @param[out] handled 消息是否已经处理，返回 true 表明已经成功处理消息，否则将消息继续传递给窗口过程
	 * @return 返回消息处理结果
	 */
	LRESULT DoHandlMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled);

	/**
	 * @brief 对 CallWindowProc API 的一层封装
	 * @param[in] uMsg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回消息处理结果
	 */
	LRESULT CallWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief 判断是否需要发送鼠标进入或离开消息
	 * @param[in] pt 鼠标当前位置
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 返回 true 需要发送鼠标进入或离开消息，返回 false 为不需要
	 */
	inline bool HandleMouseEnterLeave(const POINT &pt, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief 释放指定控件的按下状态
	 * @param[in] bClickOrPointer 单击控件还是触摸控件
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return void 无
	 */
	void ReleaseEventClick(bool bClickOrPointer, WPARAM wParam, LPARAM lParam);

	/// 焦点相关
	/**
	 * @brief 获取当前持有焦点的控件
	 * @return 返回控件的指针
	 */
	Control* GetFocus() const;

	/**
	 * @brief 设置焦点到指定控件上
	 * @param[in] pControl 控件指针
	 * @return 无
	 */
	void SetFocus(Control* pControl);

	/**
	 * @brief 设置焦点到指定控件并重绘所有控件
	 * @param[in] pControl 控件指针
	 * @return 无
	 */
	void SetFocusNeeded(Control* pControl);

	/**
	 * @brief 让窗口失去焦点
	 * @return 无
	 */
	void KillFocus();

	/**
	 * @brief 设置当要捕获的鼠标窗口句柄为当前绘制窗口
	 * @param[in]
	 * @return 无
	 */
	void SetCapture();

	/**
	 * @brief 当窗口不需要鼠标输入时释放资源
	 * @return 无
	 */
	void ReleaseCapture();

	/**
	 * @brief 判断指定控件是否被点击或触控
	 * @paran[in] pControl 控件指针
	 * @return 返回 true 表示当前控件处于被点击或触控状态
	 */
	bool IsCaptureControl(const Control* pControl);

	/**
	 * @brief 判断当前是否捕获鼠标输入
	 * @return 返回 true 为捕获，否则为不捕获
	 */
	bool IsCaptured() const;

	/**
	 * @brief 获取当前鼠标在哪个控件上
	 * @return 返回控件的指针
	 */
	Control* GetNewHover();

	/**
	 * @brief 获取鼠标最后的坐标
	 * @return 返回坐标信息
	 */
	POINT GetLastMousePos() const;

	/**
	 * @brief 设置是否处理触控消息
	 * @param[in] bHandle 设置为 true 表示处理，false 为不处理
	 * @return 无
	 */
	void SetHandlePointer(bool bHandle) { m_bHandlePointer = bHandle; };

	/**
	 * @brief 获取提示信息所属的窗口句柄
	 * @return 返回提示信息的窗口句柄
	 */
	HWND GetTooltipWindow() const;

	/**
	 * @brief 切换控件焦点到下一个（或上一个）控件
	 * @param[in] bForward true 为上一个控件，否则为 false，默认为 true
	 * @return 始终返回 true，暂无参考意义
	 */
	bool SetNextTabControl(bool bForward = true);

	/// 控件相关
	/**
	 * @brief 获取窗口最外层的容器
	 * @return 返回最外层的容器句柄
	 */
	Control* GetRoot() const;

	/**
	 * @brief 设置控件是否已经布局
	 * @param[in] bArrange true 为已经排列，否则为 false
	 * @return 无
	 */
	void SetArrange(bool bArrange);

	/**
	 * @brief 延迟销毁一个控件
	 * @param[in] pControl 控件指针
	 * @return 无
	 */
	void AddDelayedCleanup(Control* pControl);

	/**
	 * @brief 根据坐标查找指定控件
	 * @param[in] pt 指定坐标
	 * @return 返回控件指针
	 */
	Control* FindControl(POINT pt) const;

	/**
	 * @brief 根据控件名称查找控件
	 * @param[in] strName 控件名称
	 * @return 返回控件指针
	 */
	Control* FindControl(const std::wstring& strName) const;

	/**
	 * @brief 根据坐标查找子控件
	 * @param[in] pParent 要搜索的控件
	 * @param[in] pt 要查找的坐标
	 * @return 返回控件指针
	 */
	Control* FindSubControlByPoint(Control* pParent, POINT pt) const;

	/**
	 * @brief 根据名字查找子控件
	 * @param[in] pParent 要搜索的控件
	 * @param[in] strName 要查找的名称
	 * @return 返回控件指针
	 */
	Control* FindSubControlByName(Control* pParent, const std::wstring& strName) const;

	/**
	 * @brief 根据类名查找子控件
	 * @param[in] pParent 要搜索的控件
	 * @param[in] typeinfo 类型的信息
	 * @param[in] iIndex 查找到可能有多个，要获取第几个的索引
	 * @return 控件指针
	 */
	Control* FindSubControlByClass(Control* pParent, const type_info& typeinfo, int iIndex = 0);

	/**
	 * @brief 根据类名查找子控件
	 * @param[in] pParent 要搜索的控件
	 * @param[in] typeinfo 类型的信息
	 * @return 返回所有符合条件的控件列表
	 */
	std::vector<Control*>* FindSubControlsByClass(Control* pParent, const type_info& typeinfo);

	/**
	 * @brief 返回保存了控件集合的列表
	 * @return 返回列表对象
	 */
	std::vector<Control*>* GetSubControlsByClass();

	/**
	 * @brief 主动发起一个消息
	 * @param[in] eventType 转化后的消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 始终返回 true
	 */
	bool SendNotify(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0);

	/**
	 * @brief 主动发起一个消息
	 * @param[in] pControl 发送目标，要发送给哪个控件
	 * @param[in] msg 消息体
	 * @param[in] wParam 消息附加参数
	 * @param[in] lParam 消息附加参数
	 * @return 始终返回 true
	 */
	bool SendNotify(Control* pControl, EventType msg, WPARAM wParam = 0, LPARAM lParam = 0);

	/// 绘制相关
	/**
	 * @brief 获取绘制区域 DC
	 * @return 返回绘制区域 DC
	 */
	HDC GetPaintDC() const;

	/**
	 * @brief 获取绘制对象
	 * @return 返回绘制对象
	 */
	IRenderContext* GetRenderContext() const;

	/**
	 * @brief 发出重绘消息
	 * @param[in] rcItem 重绘范围
	 * @return 无
	 */
	void Invalidate(const UiRect& rcItem);

	/**
	 * @brief 绘制函数体
	 * @return 无
	 */
	void Paint();

	/**
	 * @brief 设置透明度
	 * @param[in] nAlpha 透明度数值
	 * @return 无
	 */
	void SetAlpha(int nAlpha);

	/**
	 * @brief 判断当前是否渲染透明图层
	 * @return 返回 true 为是，否则为 false
	 */
	bool IsRenderTransparent() const;

	/**
	 * @brief 设置渲染透明图层
	 * @param[in] bCanvasTransparent 设置 true 为渲染透明图层，否则为 false
	 * @return 返回上一次绘制状态
	 */
	bool SetRenderTransparent(bool bCanvasTransparent);
	bool IsLayeredWindow();

	/**
	 * @brief 初始化布局
	 * @return 无
	 */
	virtual void OnInitLayout();

	/**		
	 * @brief 是否将要关闭
	 * @return 无
	 */
	bool IsClosing(){ return m_bCloseing; };

#if defined(ENABLE_UIAUTOMATION)
	/**
     * @brief Get ui automation provider
     * @return nullptr or pointer
     */
	UIAWindowProvider* GetUIAProvider();
#endif


private:
	static Control* CALLBACK __FindControlFromNameHash(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromCount(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromPoint(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromTab(Control* pThis, LPVOID pData);
	//static Control* CALLBACK __FindControlFromShortcut(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromUpdate(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromName(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromClass(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlsFromClass(Control* pThis, LPVOID pData);

private:
	/**
	 * @brief 设置绘制偏移
	 * @param[in] renderOffset 偏移值
	 * @return 无
	 */
	void SetRenderOffset(CPoint renderOffset);

	/**
	 * @brief 设置绘制偏移 x 坐标
	 * @param[in] renderOffsetX 坐标值
	 * @return 无
	 */
	void SetRenderOffsetX(int renderOffsetX);

	/**
	 * @brief 设置绘制偏移 y 坐标
	 * @param[in] renderOffsetY 坐标值
	 * @return 无
	 */
	void SetRenderOffsetY(int renderOffsetY);

protected:
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	bool m_bSubclassed;

	Box* m_pRoot;
	EventMap OnEvent;

	UIAWindowProvider* m_pUIAProvider;

protected:
	CSize m_szMinWindow;
	CSize m_szMaxWindow;
	CSize m_szInitWindowSize;
	UiRect m_rcMaximizeInfo;
	UiRect m_rcSizeBox;
	UiRect m_rcAlphaFix;
	CSize m_szRoundCorner;
	UiRect m_rcCaption;
	double m_heightPercent;

	HDC m_hDcPaint;
	std::unique_ptr<IRenderContext> m_renderContext;
	bool m_bIsLayeredWindow;
	int m_nAlpha;
	CPoint m_renderOffset;
	bool m_bFirstLayout;

	HWND m_hwndTooltip;
	TOOLINFO m_ToolTip;

	Control* m_pFocus;
	Control* m_pNewHover;
	Control* m_pEventHover;
	Control* m_pEventClick;
	Control* m_pEventKey;
	CPoint m_ptLastMousePos;

	Control* m_pEventPointer;
	bool m_bHandlePointer;

	UINT m_uTimerID;
	bool m_bIsArranged;
	bool m_bFocusNeeded;
	bool m_bMouseTracking;
	bool m_bMouseCapture;

	std::wstring m_strWindowResourcePath; //每个窗口的资源路径,等于GetSkinFolder()
	TFontInfo m_defaultFontInfo;
	std::map<std::wstring, std::wstring> m_defaultAttrHash;
	std::map<std::wstring, DWORD> m_mapTextColor;
	std::map<std::wstring, std::vector<Control*>> m_mOptionGroup;

	std::vector<IUIMessageFilter*> m_aPreMessageFilters;
	std::vector<IUIMessageFilter*> m_aMessageFilters;
	std::vector<ITranslateAccelerator*> m_aTranslateAccelerator;
	std::vector<IControlFromPointFinder*> m_aIControlFromPointFinder;

	std::vector<Control*> m_aDelayedCleanup;
	std::vector<Control*> m_aFoundControls;
	std::map<std::wstring, Control*> m_mNameHash;

	nbase::WeakCallbackFlag m_closeFlag;
	
	Shadow m_shadow;

	bool m_bFakeModal = false;
	bool m_bCloseing = false;	//add by djj 20200428 调用Close时会延迟Post WM_CLOSE, 这个期间需要有一个标识此种'待关闭状态'
};

} // namespace ui

#endif // UI_CORE_WINDOW_H_
