# Window（窗口）

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| size | 0,0 | SIZE | 窗口的初始化大小,如(800,600) |
| mininfo | 0,0 | SIZE | 窗口最小大小,如(320,240) |
| maxinfo | 0,0 | SIZE | 窗口最大大小,如(1600,1200) |
| heightpercent | 0.0 | DOUBLE | 窗口的初始高度占屏幕高度的百分比,应该写在size、mininfo、maxinfo属性后面 |
| sizebox | 0,0,0,0 | RECT | 窗口可拖动改变窗口大小的边距,如(4,4,6,6) |
| caption | 0,0,0,0 | RECT | 窗口可拖动的标题栏大小的边距,最后一个参数是指离上边框的距离,如(0,0,0,28) |
| textid |  | STRING | 窗体标题字符串的ID,ID在多语言文件中指定,如(STRID_MIANWINDOW_TITLE) |
| roundcorner | 0,0 | SIZE | 窗口圆角大小,如(4,4) |
| shadowattached | true | BOOL | 窗口是否附加阴影效果,如(true) |
| shadowimage |  | STRING | 使用自定义的阴影素材去代替默认的阴影效果，设置的路径要注意相对路径以及九宫格属性，如(file='../public/bk/bk_shadow.png' corner='30,30,30,30') |
| shadowcorner | 0,0,0,0 | RECT | 设置了shadowimage属性后，设置此属性来指定阴影素材的九宫格描述，这个属性一定要写在size属性前面 |
| alphafixcorner | 14,14,14,14 | RECT | 透明通道修补范围的的九宫格描述 |
| custom_shadow | 14,14,14,14 | RECT | 与alphafixcorner完全一样，为了兼容老版本xml所以没有去掉这个属性 |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [GetHWND](#GetHWND) | 获取窗口所属的 Windows 句柄 |
| [RegisterWindowClass](#RegisterWindowClass) | 注册窗口类 |
| [RegisterSuperClass](#RegisterSuperClass) | 注册控件窗口类（与窗口的过程函数不同） |
| [GetWindowClassName](#GetWindowClassName) | 获取窗口类名称 |
| [GetSuperClassName](#GetSuperClassName) | 获取控件窗口类 |
| [GetClassStyle](#GetClassStyle) | 获取窗口类的样式，该方法由实例化的子类实现，https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles |
| [Subclass](#Subclass) | 子类化窗口（更改窗口过程函数） |
| [Unsubclass](#Unsubclass) | 取消子类化窗口（恢复原来的窗口过程函数） |
| [Create](#Create) | 创建窗口 |
| [Close](#Close) | 关闭窗口 |
| [ShowWindow](#ShowWindow) | 显示或隐藏窗口 |
| [ShowModalFake](#ShowModalFake) | 显示模态对话框（推荐） |
| [CenterWindow](#CenterWindow) | 居中窗口，支持扩展屏幕 |
| [SetIcon](#SetIcon) | 设置窗口图标 |
| [SendMessage](#SendMessage) | 发送消息，对 Windows SendMessage 的一层封装 |
| [PostMessage](#PostMessage) | 投递一个消息到消息队列 |
| [AttachWindowClose](#AttachWindowClose) | 监听窗口关闭事件 |
| [OnFinalMessage](#OnFinalMessage) | 在窗口收到 WM_NCDESTROY 消息时会被调用 |
| [__WndProc](#__WndProc) | 窗口过程函数 |
| [__ControlProc](#__ControlProc) | 待补充 |
| [Init](#Init) | 窗口接收到 WM_CREATE 消息时会被调用，一般用于初始化 |
| [AttachDialog](#AttachDialog) | 绑定窗口的顶层容器 |
| [InitControls](#InitControls) | 初始化控件，在容器中添加控件时会被调用（用于对控件名称做缓存） |
| [ReapObjects](#ReapObjects) | 回收控件 |
| [GetWindowResourcePath](#GetWindowResourcePath) | 获取窗口资源路径 |
| [SetWindowResourcePath](#SetWindowResourcePath) | 设置窗口资源路径 |
| [GetDefaultFontInfo](#GetDefaultFontInfo) | 获取默认字体信息 |
| [AddClass](#AddClass) | 添加一个通用样式 |
| [GetClassMap](#GetClassMap) | 获取所有通用样式 |
| [GetClassAttributes](#GetClassAttributes) | 获取指定通用样式的内容 |
| [RemoveClass](#RemoveClass) | 删除一个通用样式 |
| [RemoveAllClass](#RemoveAllClass) | 删除所有通用样式 |
| [AddOptionGroup](#AddOptionGroup) | 添加一个选项组 |
| [GetOptionGroup](#GetOptionGroup) | 获取指定选项组中控件列表 |
| [RemoveOptionGroup](#RemoveOptionGroup) | 删除一个选项组 |
| [RemoveAllOptionGroups](#RemoveAllOptionGroups) | 删除所有选项组 |
| [ClearImageCache](#ClearImageCache) | 清理图片缓存 |
| [GetMousePos](#GetMousePos) | 获取鼠标位置 |
| [GetSizeBox](#GetSizeBox) | 获取窗口四边可拉伸范围的大小 |
| [SetSizeBox](#SetSizeBox) | 设置窗口四边可拉伸范围的大小 |
| [GetCaptionRect](#GetCaptionRect) | 获取窗口标题栏区域（可拖动区域），对应 XML 中 caption 属性 |
| [SetCaptionRect](#SetCaptionRect) | 设置窗口标题栏区域 |
| [GetRoundCorner](#GetRoundCorner) | 获取窗口圆角大小，对应 XML 中 roundcorner 属性 |
| [SetRoundCorner](#SetRoundCorner) | 设置窗口圆角大小 |
| [GetMaximizeInfo](#GetMaximizeInfo) | 获取窗口最大化信息 |
| [SetMaximizeInfo](#SetMaximizeInfo) | 设置窗口最大化信息 |
| [GetAlphaFixCorner](#GetAlphaFixCorner) | 获取透明通道修补范围的的九宫格描述，对应 XML 中 alphafixcorner 属性 |
| [SetAlphaFixCorner](#SetAlphaFixCorner) | 设置透明通道修补范围的的九宫格描述 |
| [GetHeightPercent](#GetHeightPercent) | 获取窗口的初始高度占屏幕高度的百分比，对应 XML 中 heightpercent 属性 |
| [SetHeightPercent](#SetHeightPercent) | 设置窗口的初始高度占屏幕高度的百分比 |
| [SetTextId](#SetTextId) | 根据语言列表中的文本 ID 设置窗口标题栏文本 |
| [SetShadowAttached](#SetShadowAttached) | 设置窗口是否附加阴影效果 |
| [GetShadowImage](#GetShadowImage) | 获取阴影图片 |
| [SetShadowImage](#SetShadowImage) | 设置窗口阴影图片 |
| [GetShadowCorner](#GetShadowCorner) | 获取阴影的九宫格描述信息 |
| [SetShadowCorner](#SetShadowCorner) | 指定阴影素材的九宫格描述 |
| [GetPos](#GetPos) | 获取窗口位置信息 |
| [SetPos](#SetPos) | 设置窗口位置（对 SetWindowPos 的一层封装） |
| [GetMinInfo](#GetMinInfo) | 获取窗口最小范围，对应 XML 中 mininfo 属性 |
| [GetMaxInfo](#GetMaxInfo) | 获取窗口最大范围，对应 XML 中 maxinfo 属性 |
| [GetInitSize](#GetInitSize) | 获取窗口初始大小 |
| [SetMinInfo](#SetMinInfo) | 设置窗口最小范围 |
| [SetMaxInfo](#SetMaxInfo) | 设置窗口最大范围 |
| [SetInitSize](#SetInitSize) | 设置窗口初始大小 |
| [AddMessageFilter](#AddMessageFilter) | 添加一个消息过滤器，此时消息已经派发 |
| [RemoveMessageFilter](#RemoveMessageFilter) | 移除一个消息过滤器 |
| [AddControlFromPointFinder](#AddControlFromPointFinder) | 查找控件时添加一个根据位置查找控件的钩子 |
| [RemoveControlFromPointFinder](#RemoveControlFromPointFinder) | 移除查找控件时根据位置查找控件的钩子 |
| [AddTranslateAccelerator](#AddTranslateAccelerator) | 添加一个 TranslateMessage 之前的消息过滤器 |
| [RemoveTranslateAccelerator](#RemoveTranslateAccelerator) | 移除一个 TranslateMessage 之前的消息过滤器 |
| [TranslateAccelerator](#TranslateAccelerator) | 执行 TranslateMessage 阶段的过滤器 |
| [HandleMessage](#HandleMessage) | 窗口消息的派发函数 |
| [DoHandlMessage](#DoHandlMessage) | 窗口消息的执行体，用于执行各类过滤器和处理各类消息 |
| [CallWindowProc](#CallWindowProc) | 对 CallWindowProc API 的一层封装 |
| [HandleMouseEnterLeave](#HandleMouseEnterLeave) | 判断是否需要发送鼠标进入或离开消息 |
| [GetFocus](#GetFocus) | 获取当前持有焦点的控件 |
| [SetFocus](#SetFocus) | 设置焦点到指定控件上 |
| [SetFocusNeeded](#SetFocusNeeded) | 设置焦点到指定控件并重绘所有控件 |
| [KillFocus](#KillFocus) | 让窗口失去焦点 |
| [SetCapture](#SetCapture) | 设置当要捕获的鼠标窗口句柄为当前绘制窗口 |
| [ReleaseCapture](#ReleaseCapture) | 当窗口不需要鼠标输入时释放资源 |
| [IsCaptureControl](#IsCaptureControl) | 判断指定控件是否被点击或触控 |
| [IsCaptured](#IsCaptured) | 判断当前是否捕获鼠标输入 |
| [GetNewHover](#GetNewHover) | 获取当前鼠标在哪个控件上 |
| [GetLastMousePos](#GetLastMousePos) | 获取鼠标最后的坐标 |
| [SetHandlePointer](#SetHandlePointer) | 设置是否处理触控消息 |
| [GetTooltipWindow](#GetTooltipWindow) | 获取提示信息所属的窗口句柄 |
| [SetNextTabControl](#SetNextTabControl) | 切换控件焦点到下一个（或上一个）控件 |
| [GetRoot](#GetRoot) | 获取窗口最外层的容器 |
| [SetArrange](#SetArrange) | 设置控件是否已经布局 |
| [AddDelayedCleanup](#AddDelayedCleanup) | 延迟销毁一个控件 |
| [FindControl](#FindControl) | 根据坐标查找指定控件 |
| [FindControl](#FindControl) | 根据控件名称查找控件 |
| [FindSubControlByPoint](#FindSubControlByPoint) | 根据坐标查找子控件 |
| [FindSubControlByName](#FindSubControlByName) | 根据名字查找子控件 |
| [FindSubControlByClass](#FindSubControlByClass) | 根据类名查找子控件 |
| [FindSubControlsByClass](#FindSubControlsByClass) | 根据类名查找子控件 |
| [GetSubControlsByClass](#GetSubControlsByClass) | 返回保存了控件集合的列表 |
| [SendNotify](#SendNotify) | 主动发起一个消息 |
| [SendNotify](#SendNotify) | 主动发起一个消息 |
| [GetPaintDC](#GetPaintDC) | 获取绘制区域 DC |
| [GetRenderContext](#GetRenderContext) | 获取绘制对象 |
| [Invalidate](#Invalidate) | 发出重绘消息 |
| [Paint](#Paint) | 绘制函数体 |
| [SetAlpha](#SetAlpha) | 设置透明度 |
| [IsRenderTransparent](#IsRenderTransparent) | 判断当前是否渲染透明图层 |
| [SetRenderTransparent](#SetRenderTransparent) | 设置渲染透明图层 |
| [OnInitLayout](#OnInitLayout) | 初始化布局 |
| [SetRenderOffset](#SetRenderOffset) | 设置绘制偏移 |
| [SetRenderOffsetX](#SetRenderOffsetX) | 设置绘制偏移 x 坐标 |
| [SetRenderOffsetY](#SetRenderOffsetY) | 设置绘制偏移 y 坐标 |


### GetHWND

获取窗口所属的 Windows 句柄

```cpp
HWND GetHWND()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口关联的 Windows 窗口句柄

### RegisterWindowClass

注册窗口类

```cpp
bool RegisterWindowClass()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示成功，否则表示失败

### RegisterSuperClass

注册控件窗口类（与窗口的过程函数不同）

```cpp
bool RegisterSuperClass()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示成功，否则表示失败

### GetWindowClassName

获取窗口类名称

```cpp
virtual std::wstring GetWindowClassName()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口类名称

### GetSuperClassName

获取控件窗口类

```cpp
virtual std::wstring GetSuperClassName()
```

 - 参&emsp;数：无  
 - 返回值：返回控件窗口类

### GetClassStyle

获取窗口类的样式，该方法由实例化的子类实现，https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles

```cpp
virtual UINT GetClassStyle()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口类的样式

### Subclass

子类化窗口（更改窗口过程函数）

```cpp
HWND Subclass(HWND hWnd)
```

 - 参&emsp;数：  
    - `窗口句柄
 - 返回值：新的窗口句柄

### Unsubclass

取消子类化窗口（恢复原来的窗口过程函数）

```cpp
void Unsubclass()
```

 - 参&emsp;数：无  
 - 返回值：无

### Create

创建窗口

```cpp
virtual HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, bool isLayeredWindow = true, const UiRect& rc = UiRect(0, 0, 0, 0)
```

 - 参&emsp;数：  
    - `hwndParent` 父窗口句柄
    - `pstrName` 窗口名称
    - `dwStyle` 窗口样式
    - `dwExStyle` 窗口拓展样式
    - `isLayeredWindow` 是否带有层窗口属性，默认为 true
    - `rc` 窗口大小
 - 返回值：返回窗口句柄

### Close

关闭窗口

```cpp
virtual void Close(UINT nRet = IDOK)
```

 - 参&emsp;数：  
    - `关闭消息
 - 返回值：无

### ShowWindow

显示或隐藏窗口

```cpp
virtual void ShowWindow(bool bShow = true, bool bTakeFocus = true)
```

 - 参&emsp;数：  
    - `bShow` 为 true 时显示窗口，为 false 时为隐藏窗口，默认为 true
    - `bTakeFocus` 是否获得焦点（激活窗口），默认为 true
 - 返回值：无

### ShowModalFake

显示模态对话框（推荐）

```cpp
void ShowModalFake(HWND parent_hwnd)
```

 - 参&emsp;数：  
    - `parent_hwnd` 父窗口句柄
 - 返回值：无

### CenterWindow

居中窗口，支持扩展屏幕

```cpp
void CenterWindow()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetIcon

设置窗口图标

```cpp
void SetIcon(UINT nRes)
```

 - 参&emsp;数：  
    - `nRes` 窗口图标资源 ID
 - 返回值：无

### SendMessage

发送消息，对 Windows SendMessage 的一层封装

```cpp
LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L)
```

 - 参&emsp;数：  
    - `uMsg` 消息类型
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：返回窗口对消息的处理结果

### PostMessage

投递一个消息到消息队列

```cpp
LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L)
```

 - 参&emsp;数：  
    - `uMsg` 消息类型
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：返回窗口对消息的处理结果

### AttachWindowClose

监听窗口关闭事件

```cpp
void AttachWindowClose(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 指定关闭后的回调函数
 - 返回值：无

### OnFinalMessage

在窗口收到 WM_NCDESTROY 消息时会被调用

```cpp
virtual void OnFinalMessage(HWND hWnd)
```

 - 参&emsp;数：  
    - `hWnd` 窗口句柄
 - 返回值：无

### __WndProc

窗口过程函数

```cpp
static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
```

 - 参&emsp;数：  
    - `hWnd` 窗口句柄
    - `uMsg` 消息体
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：返回消息处理结果

### __ControlProc

待补充

```cpp
static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### Init

窗口接收到 WM_CREATE 消息时会被调用，一般用于初始化

```cpp
void Init(HWND hWnd)
```

 - 参&emsp;数：  
    - `hWnd` 窗口句柄
 - 返回值：无

### AttachDialog

绑定窗口的顶层容器

```cpp
bool AttachDialog(Box* pRoot)
```

 - 参&emsp;数：  
    - `pRoot` 容器指针
 - 返回值：返回 true 绑定成功，否则为失败

### InitControls

初始化控件，在容器中添加控件时会被调用（用于对控件名称做缓存）

```cpp
bool InitControls(Control* pControl, Box* pParent = NULL)
```

 - 参&emsp;数：  
    - `pControl` 控件指针
    - `pParent` 控件父容器
 - 返回值：true 为成功，否则为失败

### ReapObjects

回收控件

```cpp
void ReapObjects(Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 控件指针
 - 返回值：无

### GetWindowResourcePath

获取窗口资源路径

```cpp
std::wstring GetWindowResourcePath()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口资源路径

### SetWindowResourcePath

设置窗口资源路径

```cpp
void SetWindowResourcePath(const std::wstring& strPath)
```

 - 参&emsp;数：  
    - `strPath` 要设置的路径
 - 返回值：无

### GetDefaultFontInfo

获取默认字体信息

```cpp
TFontInfo* GetDefaultFontInfo()
```

 - 参&emsp;数：无  
 - 返回值：返回默认字体信息结构

### AddClass

添加一个通用样式

```cpp
void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList)
```

 - 参&emsp;数：  
    - `strClassName` 通用样式的名称
    - `strControlAttrList` 通用样式的 XML 转义格式数据
 - 返回值：无

### GetClassMap

获取所有通用样式

```cpp
const std::map<std::wstring, std::wstring>* GetClassMap()
```

 - 参&emsp;数：无  
 - 返回值：返回所有通用样式的 map 数据

### GetClassAttributes

获取指定通用样式的内容

```cpp
std::wstring GetClassAttributes(const std::wstring& strClassName)
```

 - 参&emsp;数：  
    - `strClassName` 通用样式名称
 - 返回值：返回指定名称的通用样式内容，XML 转义格式数据

### RemoveClass

删除一个通用样式

```cpp
bool RemoveClass(const std::wstring& strClassName)
```

 - 参&emsp;数：  
    - `strClassName` 要删除的通用样式名称
 - 返回值：返回 true 为成功，false 为失败或样式不存在

### RemoveAllClass

删除所有通用样式

```cpp
void RemoveAllClass()
```

 - 参&emsp;数：无  
 - 返回值：无

### AddOptionGroup

添加一个选项组

```cpp
bool AddOptionGroup(const std::wstring& strGroupName, Control* pControl)
```

 - 参&emsp;数：  
    - `strGroupName` 组名称
    - `pControl` 控件指针
 - 返回值：返回 true 表示添加成功，false 可能组已经存在

### GetOptionGroup

获取指定选项组中控件列表

```cpp
std::vector<Control*>* GetOptionGroup(const std::wstring& strGroupName)
```

 - 参&emsp;数：  
    - `strGroupName` 指定组名称
 - 返回值：返回该组下的所有控件列表

### RemoveOptionGroup

删除一个选项组

```cpp
void RemoveOptionGroup(const std::wstring& strGroupName, Control* pControl)
```

 - 参&emsp;数：  
    - `strGroupName` 组名称
    - `pControl` 控件名称
 - 返回值：无

### RemoveAllOptionGroups

删除所有选项组

```cpp
void RemoveAllOptionGroups()
```

 - 参&emsp;数：无  
 - 返回值：无

### ClearImageCache

清理图片缓存

```cpp
void ClearImageCache()
```

 - 参&emsp;数：无  
 - 返回值：无

### GetMousePos

获取鼠标位置

```cpp
POINT GetMousePos()
```

 - 参&emsp;数：无  
 - 返回值：鼠标位置的 x 和 y 坐标

### GetSizeBox

获取窗口四边可拉伸范围的大小

```cpp
UiRect GetSizeBox()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口四边可拉伸范围的大小

### SetSizeBox

设置窗口四边可拉伸范围的大小

```cpp
void SetSizeBox(const UiRect& rcSizeBox)
```

 - 参&emsp;数：  
    - `rcSizeBox` 要设置的大小
 - 返回值：无

### GetCaptionRect

获取窗口标题栏区域（可拖动区域），对应 XML 中 caption 属性

```cpp
UiRect GetCaptionRect()
```

 - 参&emsp;数：无  
 - 返回值：返回标题栏区域

### SetCaptionRect

设置窗口标题栏区域

```cpp
void SetCaptionRect(UiRect& rcCaption)
```

 - 参&emsp;数：  
    - `rcCaption` 要设置的区域范围
 - 返回值：无

### GetRoundCorner

获取窗口圆角大小，对应 XML 中 roundcorner 属性

```cpp
CSize GetRoundCorner()
```

 - 参&emsp;数：无  
 - 返回值：返回圆角大小

### SetRoundCorner

设置窗口圆角大小

```cpp
void SetRoundCorner(int cx, int cy)
```

 - 参&emsp;数：  
    - `cx` 圆角宽
    - `cy` 圆角高
 - 返回值：无

### GetMaximizeInfo

获取窗口最大化信息

```cpp
UiRect GetMaximizeInfo()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口最大化信息

### SetMaximizeInfo

设置窗口最大化信息

```cpp
void SetMaximizeInfo(UiRect& rcMaximize)
```

 - 参&emsp;数：  
    - `rcMaximize` 要设置的最大化数值
 - 返回值：无

### GetAlphaFixCorner

获取透明通道修补范围的的九宫格描述，对应 XML 中 alphafixcorner 属性

```cpp
UiRect GetAlphaFixCorner()
```

 - 参&emsp;数：无  
 - 返回值：返回修补范围

### SetAlphaFixCorner

设置透明通道修补范围的的九宫格描述

```cpp
void SetAlphaFixCorner(UiRect& rc)
```

 - 参&emsp;数：  
    - `rc` 要设置的修补范围
 - 返回值：无

### GetHeightPercent

获取窗口的初始高度占屏幕高度的百分比，对应 XML 中 heightpercent 属性

```cpp
double GetHeightPercent()
```

 - 参&emsp;数：无  
 - 返回值：返回指定百分比数据

### SetHeightPercent

设置窗口的初始高度占屏幕高度的百分比

```cpp
void SetHeightPercent(double heightPercent)
```

 - 参&emsp;数：  
    - `heightPercent` 百分比
 - 返回值：无

### SetTextId

根据语言列表中的文本 ID 设置窗口标题栏文本

```cpp
void SetTextId(const std::wstring& strTextId)
```

 - 参&emsp;数：  
    - `strTextId` 语言 ID，该 ID 必须在语言文件中存在
 - 返回值：无

### SetShadowAttached

设置窗口是否附加阴影效果

```cpp
void SetShadowAttached(bool bShadowAttached)
```

 - 参&emsp;数：  
    - `bShadowAttached` 为 true 时附加，false 时不附加
 - 返回值：无

### GetShadowImage

获取阴影图片

```cpp
std::wstring GetShadowImage()
```

 - 参&emsp;数：无  
 - 返回值：返回阴影图片位置

### SetShadowImage

设置窗口阴影图片

```cpp
void SetShadowImage(const std::wstring &strImage)
```

 - 参&emsp;数：  
    - `strImage` 图片位置
 - 返回值：无

### GetShadowCorner

获取阴影的九宫格描述信息

```cpp
UiRect GetShadowCorner()
```

 - 参&emsp;数：无  
 - 返回值：返回阴影的九宫格描述信息

### SetShadowCorner

指定阴影素材的九宫格描述

```cpp
void SetShadowCorner(const UiRect rect)
```

 - 参&emsp;数：  
    - `rect` 九宫格描述信息
 - 返回值：无

### GetPos

获取窗口位置信息

```cpp
UiRect GetPos(bool bContainShadow = false)
```

 - 参&emsp;数：  
    - `bContainShadow` 是否包含阴影，true 为包含，默认为 false 不包含
 - 返回值：返回窗口位置信息

### SetPos

设置窗口位置（对 SetWindowPos 的一层封装）

```cpp
void SetPos(const UiRect& rc, bool bNeedDpiScale, UINT uFlags, HWND hWndInsertAfter = NULL, bool bContainShadow = false)
```

 - 参&emsp;数：  
    - `rc` 窗口位置
    - `bNeedDpiScale` 为 false 表示不需要把 rc 根据 DPI 自动调整
    - `uFlags` 对应 SetWindowPos 的 选项
    - `hWndInsertAfter` 对应 SetWindowPos 的 选项
    - `bContainShadow` 是否包含阴影范围，默认为 false
 - 返回值：无

### GetMinInfo

获取窗口最小范围，对应 XML 中 mininfo 属性

```cpp
CSize GetMinInfo(bool bContainShadow = false)
```

 - 参&emsp;数：  
    - `bContainShadow` 是否包含阴影范围，默认为 false
 - 返回值：返回指定大小

### GetMaxInfo

获取窗口最大范围，对应 XML 中 maxinfo 属性

```cpp
CSize GetMaxInfo(bool bContainShadow = false)
```

 - 参&emsp;数：  
    - `bContainShadow` 是否包含阴影范围，默认为 false
 - 返回值：返回指定大小

### GetInitSize

获取窗口初始大小

```cpp
CSize GetInitSize(bool bContainShadow = false)
```

 - 参&emsp;数：  
    - `bContainShadow` 是否包含阴影范围，默认为 false
 - 返回值：返回指定大小

### SetMinInfo

设置窗口最小范围

```cpp
void SetMinInfo(int cx, int cy, bool bContainShadow = false)
```

 - 参&emsp;数：  
    - `cx` 宽度
    - `cy` 高度
    - `bContainShadow` 为 false 表示 cx cy 不包含阴影
 - 返回值：无

### SetMaxInfo

设置窗口最大范围

```cpp
void SetMaxInfo(int cx, int cy, bool bContainShadow = false)
```

 - 参&emsp;数：  
    - `cx` 宽度
    - `cy` 高度
    - `bContainShadow` 为 false 表示 cx cy 不包含阴影
 - 返回值：无

### SetInitSize

设置窗口初始大小

```cpp
void SetInitSize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true)
```

 - 参&emsp;数：  
    - `cx` 宽度
    - `cy` 高度
    - `bContainShadow` 为 false 表示 cx cy 不包含阴影
    - `bNeedDpiScale` 为 false 表示不根据 DPI 调整
 - 返回值：无

### AddMessageFilter

添加一个消息过滤器，此时消息已经派发

```cpp
bool AddMessageFilter(IUIMessageFilter* pFilter)
```

 - 参&emsp;数：  
    - `pFilter` 一个继承了 IUIMessageFilter 的对象实例，需要实现 MessageHandler 方法
 - 返回值：始终返回 true

### RemoveMessageFilter

移除一个消息过滤器

```cpp
bool RemoveMessageFilter(IUIMessageFilter* pFilter)
```

 - 参&emsp;数：  
    - `pFilter` 一个继承了 IUIMessageFilter 的对象实例
 - 返回值：返回 true 表示移除成功，否则可能该过滤器不存在

### AddControlFromPointFinder

查找控件时添加一个根据位置查找控件的钩子

```cpp
bool AddControlFromPointFinder(IControlFromPointFinder* pFinder)
```

 - 参&emsp;数：  
    - `pFinder` 继承了 IControlFromPointFinder 对象指针，需实现 FindControlFromPoint 方法
 - 返回值：始终返回 true

### RemoveControlFromPointFinder

移除查找控件时根据位置查找控件的钩子

```cpp
bool RemoveControlFromPointFinder(IControlFromPointFinder* pFinder)
```

 - 参&emsp;数：  
    - `pFinder` 继承了 IControlFromPointFinder 对象指针
 - 返回值：成功返回 true，否则该钩子可能不存在

### AddTranslateAccelerator

添加一个 TranslateMessage 之前的消息过滤器

```cpp
bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
```

 - 参&emsp;数：  
    - `pTranslateAccelerator` 继承了 ITranslateAccelerator 对象指针，需实现 TranslateAccelerator 方法
 - 返回值：始终返回 true

### RemoveTranslateAccelerator

移除一个 TranslateMessage 之前的消息过滤器

```cpp
bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
```

 - 参&emsp;数：  
    - `pTranslateAccelerator` 继承了 ITranslateAccelerator 对象指针
 - 返回值：成功返回 true，否则返回 false 可能该过滤器不存在

### TranslateAccelerator

执行 TranslateMessage 阶段的过滤器

```cpp
bool TranslateAccelerator(LPMSG pMsg)
```

 - 参&emsp;数：  
    - `pMsg` 消息体
 - 返回值：返回 true 成功处理消息，否则返回 false

### HandleMessage

窗口消息的派发函数

```cpp
virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
```

 - 参&emsp;数：  
    - `uMsg` 消息体
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：返回 true 则继续派发该消息，否则不再派发该消息

### DoHandlMessage

窗口消息的执行体，用于执行各类过滤器和处理各类消息

```cpp
LRESULT DoHandlMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled)
```

 - 参&emsp;数：  
    - `uMsg` 消息体
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
    - `handled` 消息是否已经处理，返回 true 表明已经成功处理消息，否则将消息继续传递给窗口过程
 - 返回值：返回消息处理结果

### CallWindowProc

对 CallWindowProc API 的一层封装

```cpp
LRESULT CallWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
```

 - 参&emsp;数：  
    - `uMsg` 消息体
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：返回消息处理结果

### HandleMouseEnterLeave

判断是否需要发送鼠标进入或离开消息

```cpp
inline bool HandleMouseEnterLeave(const POINT &pt, WPARAM wParam, LPARAM lParam)
```

 - 参&emsp;数：  
    - `pt` 鼠标当前位置
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：返回 true 需要发送鼠标进入或离开消息，返回 false 为不需要

### GetFocus

获取当前持有焦点的控件

```cpp
Control* GetFocus()
```

 - 参&emsp;数：无  
 - 返回值：返回控件的指针

### SetFocus

设置焦点到指定控件上

```cpp
void SetFocus(Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 控件指针
 - 返回值：无

### SetFocusNeeded

设置焦点到指定控件并重绘所有控件

```cpp
void SetFocusNeeded(Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 控件指针
 - 返回值：无

### KillFocus

让窗口失去焦点

```cpp
void KillFocus()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetCapture

设置当要捕获的鼠标窗口句柄为当前绘制窗口

```cpp
void SetCapture()
```

 - 参&emsp;数：无  
 - 返回值：无

### ReleaseCapture

当窗口不需要鼠标输入时释放资源

```cpp
void ReleaseCapture()
```

 - 参&emsp;数：无  
 - 返回值：无

### IsCaptureControl

判断指定控件是否被点击或触控

```cpp
bool IsCaptureControl(const Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 控件指针
 - 返回值：返回 true 表示当前控件处于被点击或触控状态

### IsCaptured

判断当前是否捕获鼠标输入

```cpp
bool IsCaptured()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为捕获，否则为不捕获

### GetNewHover

获取当前鼠标在哪个控件上

```cpp
Control* GetNewHover()
```

 - 参&emsp;数：无  
 - 返回值：返回控件的指针

### GetLastMousePos

获取鼠标最后的坐标

```cpp
POINT GetLastMousePos()
```

 - 参&emsp;数：无  
 - 返回值：返回坐标信息

### SetHandlePointer

设置是否处理触控消息

```cpp
void SetHandlePointer(bool bHandle)
```

 - 参&emsp;数：  
    - `bHandle` 设置为 true 表示处理，false 为不处理
 - 返回值：无

### GetTooltipWindow

获取提示信息所属的窗口句柄

```cpp
HWND GetTooltipWindow()
```

 - 参&emsp;数：无  
 - 返回值：返回提示信息的窗口句柄

### SetNextTabControl

切换控件焦点到下一个（或上一个）控件

```cpp
bool SetNextTabControl(bool bForward = true)
```

 - 参&emsp;数：  
    - `bForward` true 为上一个控件，否则为 false，默认为 true
 - 返回值：始终返回 true，暂无参考意义

### GetRoot

获取窗口最外层的容器

```cpp
Control* GetRoot()
```

 - 参&emsp;数：无  
 - 返回值：返回最外层的容器句柄

### SetArrange

设置控件是否已经布局

```cpp
void SetArrange(bool bArrange)
```

 - 参&emsp;数：  
    - `bArrange` true 为已经排列，否则为 false
 - 返回值：无

### AddDelayedCleanup

延迟销毁一个控件

```cpp
void AddDelayedCleanup(Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 控件指针
 - 返回值：无

### FindControl

根据坐标查找指定控件

```cpp
Control* FindControl(POINT pt)
```

 - 参&emsp;数：  
    - `pt` 指定坐标
 - 返回值：返回控件指针

### FindControl

根据控件名称查找控件

```cpp
Control* FindControl(const std::wstring& strName)
```

 - 参&emsp;数：  
    - `strName` 控件名称
 - 返回值：返回控件指针

### FindSubControlByPoint

根据坐标查找子控件

```cpp
Control* FindSubControlByPoint(Control* pParent, POINT pt)
```

 - 参&emsp;数：  
    - `pParent` 要搜索的控件
    - `pt` 要查找的坐标
 - 返回值：返回控件指针

### FindSubControlByName

根据名字查找子控件

```cpp
Control* FindSubControlByName(Control* pParent, const std::wstring& strName)
```

 - 参&emsp;数：  
    - `pParent` 要搜索的控件
    - `strName` 要查找的名称
 - 返回值：返回控件指针

### FindSubControlByClass

根据类名查找子控件

```cpp
Control* FindSubControlByClass(Control* pParent, const type_info& typeinfo, int iIndex = 0)
```

 - 参&emsp;数：  
    - `pParent` 要搜索的控件
    - `typeinfo` 类型的信息
    - `iIndex` 查找到可能有多个，要获取第几个的索引
 - 返回值：控件指针

### FindSubControlsByClass

根据类名查找子控件

```cpp
std::vector<Control*>* FindSubControlsByClass(Control* pParent, const type_info& typeinfo)
```

 - 参&emsp;数：  
    - `pParent` 要搜索的控件
    - `typeinfo` 类型的信息
 - 返回值：返回所有符合条件的控件列表

### GetSubControlsByClass

返回保存了控件集合的列表

```cpp
std::vector<Control*>* GetSubControlsByClass()
```

 - 参&emsp;数：无  
 - 返回值：返回列表对象

### SendNotify

主动发起一个消息

```cpp
bool SendNotify(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0)
```

 - 参&emsp;数：  
    - `eventType` 转化后的消息体
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：始终返回 true

### SendNotify

主动发起一个消息

```cpp
bool SendNotify(Control* pControl, EventType msg, WPARAM wParam = 0, LPARAM lParam = 0)
```

 - 参&emsp;数：  
    - `pControl` 发送目标，要发送给哪个控件
    - `msg` 消息体
    - `wParam` 消息附加参数
    - `lParam` 消息附加参数
 - 返回值：始终返回 true

### GetPaintDC

获取绘制区域 DC

```cpp
HDC GetPaintDC()
```

 - 参&emsp;数：无  
 - 返回值：返回绘制区域 DC

### GetRenderContext

获取绘制对象

```cpp
IRenderContext* GetRenderContext()
```

 - 参&emsp;数：无  
 - 返回值：返回绘制对象

### Invalidate

发出重绘消息

```cpp
void Invalidate(const UiRect& rcItem)
```

 - 参&emsp;数：  
    - `rcItem` 重绘范围
 - 返回值：无

### Paint

绘制函数体

```cpp
void Paint()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetAlpha

设置透明度

```cpp
void SetAlpha(int nAlpha)
```

 - 参&emsp;数：  
    - `nAlpha` 透明度数值
 - 返回值：无

### IsRenderTransparent

判断当前是否渲染透明图层

```cpp
bool IsRenderTransparent()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为是，否则为 false

### SetRenderTransparent

设置渲染透明图层

```cpp
bool SetRenderTransparent(bool bCanvasTransparent)
```

 - 参&emsp;数：  
    - `bCanvasTransparent` 设置 true 为渲染透明图层，否则为 false
 - 返回值：返回上一次绘制状态

### OnInitLayout

初始化布局

```cpp
virtual void OnInitLayout()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetRenderOffset

设置绘制偏移

```cpp
void SetRenderOffset(CPoint renderOffset)
```

 - 参&emsp;数：  
    - `renderOffset` 偏移值
 - 返回值：无

### SetRenderOffsetX

设置绘制偏移 x 坐标

```cpp
void SetRenderOffsetX(int renderOffsetX)
```

 - 参&emsp;数：  
    - `renderOffsetX` 坐标值
 - 返回值：无

### SetRenderOffsetY

设置绘制偏移 y 坐标

```cpp
void SetRenderOffsetY(int renderOffsetY)
```

 - 参&emsp;数：  
    - `renderOffsetY` 坐标值
 - 返回值：无

