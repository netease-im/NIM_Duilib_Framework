# WindowImplBase

| 方法名称 | 用途 |
| :--- | :--- |
| [InitWindow](#InitWindow) | 当接收到窗口创建消息时被调用 |
| [OnFinalMessage](#OnFinalMessage) | 当接收到窗口销毁消息时被调用 |
| [GetStyle](#GetStyle) | 获取窗口样式 |
| [GetClassStyle](#GetClassStyle) | 获取窗口类样式 |
| [GetResourceType](#GetResourceType) | 待补充 |
| [GetResourceID](#GetResourceID) | 待补充 |
| [CreateControl](#CreateControl) | 当要创建的控件不是标准的控件名称时会调用该函数 |
| [MessageHandler](#MessageHandler) | 接收所有消息 |
| [OnClose](#OnClose) | 当收到窗口关闭消息时被调用 |
| [OnDestroy](#OnDestroy) | 当收到窗口被销毁消息时被调用 |
| [OnNcActivate](#OnNcActivate) | 当收到活动或非活动状态消息时被调用 |
| [OnNcCalcSize](#OnNcCalcSize) | 当收到要计算客户区域大小消息时（WM_NCCALCSIZE）被调用 |
| [OnWindowPosChanging](#OnWindowPosChanging) | 窗口位置或 Z 次序发生改变时被调用 |
| [OnNcPaint](#OnNcPaint) | 当接收到绘制标题栏区域消息时被调用 |
| [OnNcLButtonDbClick](#OnNcLButtonDbClick) | 当接收到标题栏区域双击消息时被调用 |
| [OnNcHitTest](#OnNcHitTest) | 当接收到 WM_NCHITTEST 消息时被调用 |
| [OnGetMinMaxInfo](#OnGetMinMaxInfo) | 当接收到 WM_GETMINMAXINFO 消息时被调用 |
| [OnMouseWheel](#OnMouseWheel) | 当接收到鼠标悬浮消息时被调用 |
| [OnMouseHover](#OnMouseHover) | 当接收到窗口大小改变消息时被调用 |
| [OnSize](#OnSize) | 当接收到窗口大小改变消息时被调用 |
| [OnChar](#OnChar) | 当接收到字符按键消息时（WM_CHAR）被调用 |
| [OnSysCommand](#OnSysCommand) | 接收窗口控制命令消息时（WM_SYSCOMMAND）被调用 |
| [OnKeyDown](#OnKeyDown) | 接收键盘按键按下消息时被调用 |
| [OnKillFocus](#OnKillFocus) | 接收失去焦点消息时被调用 |
| [OnSetFocus](#OnSetFocus) | 接收获取焦点消息时被调用 |
| [OnLButtonDown](#OnLButtonDown) | 接收到鼠标左键按下消息时被调用 |
| [OnLButtonUp](#OnLButtonUp) | 接收到鼠标左键弹起消息时被调用 |
| [OnMouseMove](#OnMouseMove) | 接收到鼠标移动消息时被调用 |
| [OnDpiChanged](#OnDpiChanged) | 当接收到DPI改变消息时被调用（只有在 Startup 中指定启用 DPI 适配后才会触发） |
| [HandleMessage](#HandleMessage) | 窗口消息的派发函数 |
| [ActiveWindow](#ActiveWindow) | 激活窗口 |
| [SetTaskbarTitle](#SetTaskbarTitle) | 设置窗口标题 |
| [ToTopMost](#ToTopMost) | 置顶窗口 |
| [GetSkinFolder](#GetSkinFolder) | 创建窗口时被调用，由子类实现用以获取窗口皮肤目录 |
| [GetSkinFile](#GetSkinFile) | 创建窗口时被调用，由子类实现用以获取窗口皮肤 XML 描述文件 |
| [GetWindowClassName](#GetWindowClassName) | 创建窗口时被调用，由子类实现用以获取窗口唯一的类名称 |
| [OnCreate](#OnCreate) | 收到窗口创建消息时被调用，请使用 InitWindow 接口来实现自定义需求 |
| [OnButtonClick](#OnButtonClick) | 窗口功能按钮被点击时调用 |


### InitWindow

当接收到窗口创建消息时被调用

```cpp
virtual void InitWindow()
```

 - 参&emsp;数：无  
 - 返回值：无

### OnFinalMessage

当接收到窗口销毁消息时被调用

```cpp
virtual void OnFinalMessage( HWND hWnd )
```

 - 参&emsp;数：  
    - `hWnd` 要销毁的窗口句柄
 - 返回值：无

### GetStyle

获取窗口样式

```cpp
virtual LONG GetStyle()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口样式

### GetClassStyle

获取窗口类样式

```cpp
virtual UINT GetClassStyle()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口类样式

### GetResourceType

待补充

```cpp
virtual UILIB_RESOURCETYPE GetResourceType()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### GetResourceID

待补充

```cpp
virtual std::wstring GetResourceID()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### CreateControl

当要创建的控件不是标准的控件名称时会调用该函数

```cpp
virtual Control* CreateControl(const std::wstring& pstrClass)
```

 - 参&emsp;数：  
    - `pstrClass` 控件名称
 - 返回值：返回一个自定义控件指针，一般情况下根据 pstrClass 参数创建自定义的控件

### MessageHandler

接收所有消息

```cpp
virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
	- `bHandled` 返回 false 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnClose

当收到窗口关闭消息时被调用

```cpp
virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 false 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnDestroy

当收到窗口被销毁消息时被调用

```cpp
virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnNcActivate

当收到活动或非活动状态消息时被调用

```cpp
virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnNcCalcSize

当收到要计算客户区域大小消息时（WM_NCCALCSIZE）被调用

```cpp
virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnWindowPosChanging

窗口位置或 Z 次序发生改变时被调用

```cpp
virtual LRESULT OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnNcPaint

当接收到绘制标题栏区域消息时被调用

```cpp
virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnNcLButtonDbClick

当接收到标题栏区域双击消息时被调用

```cpp
virtual LRESULT OnNcLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnNcHitTest

当接收到 WM_NCHITTEST 消息时被调用

```cpp
virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnGetMinMaxInfo

当接收到 WM_GETMINMAXINFO 消息时被调用

```cpp
virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnMouseWheel

当接收到鼠标悬浮消息时被调用

```cpp
virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnMouseHover

当接收到窗口大小改变消息时被调用

```cpp
virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnSize

当接收到窗口大小改变消息时被调用

```cpp
virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnChar

当接收到字符按键消息时（WM_CHAR）被调用

```cpp
virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnSysCommand

接收窗口控制命令消息时（WM_SYSCOMMAND）被调用

```cpp
virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnKeyDown

接收键盘按键按下消息时被调用

```cpp
virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnKillFocus

接收失去焦点消息时被调用

```cpp
virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnSetFocus

接收获取焦点消息时被调用

```cpp
virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnLButtonDown

接收到鼠标左键按下消息时被调用

```cpp
virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnLButtonUp

接收到鼠标左键弹起消息时被调用

```cpp
virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### OnMouseMove

接收到鼠标移动消息时被调用

```cpp
virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果
 
### OnDpiChanged

当接收到DPI改变消息时被调用（只有在 Startup 中指定启用 DPI 适配后才会触发）

```cpp
virtual LRESULT OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### HandleMessage

窗口消息的派发函数

```cpp
virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
```

 - 参&emsp;数：  
    - `uMsg` 消息内容
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 返回 true 则继续派发该消息，否则不再派发该消息
 - 返回值：返回消息处理结果

### ActiveWindow

激活窗口

```cpp
virtual void ActiveWindow()
```

 - 参&emsp;数：无  
 - 返回值：void 无返回值

### SetTaskbarTitle

设置窗口标题

```cpp
virtual void SetTaskbarTitle(const std::wstring &title)
```

 - 参&emsp;数：  
    - `title` 窗口标题
 - 返回值：void 无返回值

### ToTopMost

置顶窗口

```cpp
void ToTopMost(bool forever)
```

 - 参&emsp;数：  
    - `forever` 是否一直置顶
 - 返回值：void 无返回值

### GetSkinFolder

创建窗口时被调用，由子类实现用以获取窗口皮肤目录

```cpp
virtual std::wstring GetSkinFolder()
```

 - 参&emsp;数：无  
 - 返回值：子类需实现并返回窗口皮肤目录

### GetSkinFile

创建窗口时被调用，由子类实现用以获取窗口皮肤 XML 描述文件

```cpp
virtual std::wstring GetSkinFile()
```

 - 参&emsp;数：无  
 - 返回值：子类需实现并返回窗口皮肤 XML 描述文件

### GetWindowClassName

创建窗口时被调用，由子类实现用以获取窗口唯一的类名称

```cpp
virtual std::wstring GetWindowClassName(void)
```

 - 参&emsp;数：无  
 - 返回值：子类需实现并返回窗口唯一的类名称

### OnCreate

收到窗口创建消息时被调用，请使用 InitWindow 接口来实现自定义需求

```cpp
LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
```

 - 参&emsp;数：  
    - `uMsg` 消息ID
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `bHandled` 消息是否已经被处理
 - 返回值：返回消息处理结果

### OnButtonClick

窗口功能按钮被点击时调用

```cpp
bool OnButtonClick(EventArgs* param)
```

 - 参&emsp;数：  
    - `param` 携带的参数
 - 返回值：始终返回 true

