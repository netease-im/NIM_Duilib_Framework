# Control（基础控件）

`Control` 控件为所有可用控件的基类，包含了控件常用的属性，如宽度、高度、边距等属性。一般情况下不作为具体可用控件使用，但常用于一些占位符，请参考示例。

### 可用属性

`Control` 是所有控件的基类，包含了所有控件的可用通用属性。

```xml
<!-- 一个根据父容器宽度自动拉伸的 Control 控件，一般用于作为占位符使用 -->
<Control width="stretch"/>
```

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| class |  | STRING | 控件样式,用样式里面的属性值来设置本控件的属性,如(btn_default),可以同时指定多个样式,各个样式之间用空格分隔,需要写在第一个属性位置, |
| name |  | STRING | 控件名字,同一窗口内必须唯一,如(testbtn) |
| halign | left | STRING | 控件的横向位置,如(center),支持left、center、right三种位置 |
| valign | top | STRING | 控件的纵向位置,如(center),支持top、center、bottom三种位置 |
| margin | 0,0,0,0 | RECT | 外边距,如(2,2,2,2) |
| bkcolor |  | STRING | 背景颜色字符串常量,如(white) |
| bkcolor1 |  | STRING | 背景渐变色1,和bkcolor配合使用,如(blue) |
| normalcolor |  | STRING | 普通状态颜色,如(white) |
| hotcolor |  | STRING | 悬浮状态颜色,如(white) |
| pushedcolor |  | STRING | 按下状态颜色,如(white) |
| disabledcolor |  | STRING | 禁用状态颜色,如(white) |
| bordercolor |  | STRING | 边框颜色,如(blue) |
| bordersize | 0 | INT / RECT | 可以设置INT或RECT类型的值。当值为ING时则左、上、右、下都用该值作为宽。值为RECT类型时则分别设置左、上、右、下的边框 |
| leftbordersize | 0 | INT | 左边边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置 |
| topbordersize | 0 | INT | 顶部边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置 |
| rightbordersize | 0 | INT | 右边边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置 |
| bottombordersize | 0 | INT | 底部边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置 |
| borderround | 0,0 | SIZE | 边框圆角半径,如(2,2) |
| width | stretch | INT / STRING | 可以设置INT或STRING类型的值.当值为INT是则设置控件的最小宽度,如(100);当值为STRING时,stretch代表由父容器计算控件宽度,auto代表根据内容情况自动计算宽度 |
| height | stretch | INT / STRING | 可以设置INT或STRING类型的值.当值为INT是则设置控件的最小高度,如(100);当值为STRING时,stretch代表由父容器计算控件高度,auto代表根据内容情况自动计算高度 |
| minwidth | -1 | INT | 控件的最小宽度,如(30) |
| minheight | -1 | INT | 控件的最小高度,如(30) |
| maxwidth | 9999999 | INT / STRING | 控件的最大宽度,描述同width |
| maxheight | 9999999 | INT / STRING | 控件的最大高度,描述同height |
| bkimage |  | STRING | 背景图片,如(bk.bmp或file='aaa.jpg' res='' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' fade='255' xtiled='false' ytiled='false') |
| tooltiptext |  | STRING | 鼠标悬浮提示,如(请在这里输入你的密码) |
| tooltiptextid |  | STRING | 鼠标悬浮提示,指定多语言模块的ID,当tooltiptext为空时则显示此属性,如(TOOL_TIP_ID) |
| dataid |  | STRING | 自定义字符串数据,辅助函数，供用户使用 |
| enabled | true | BOOL | 是否可以响应用户操作,如(true) |
| mouse | true | BOOL | 本控件是否可以响应鼠标操作,如(true) |
| receivepointer | true | BOOL | 是否响应触控消息,如(true) |
| keyboard | true | BOOL | 非CButtonUI类忽略该值,为false时不支持TAB_STOP,且该对象不处理键盘信息 |
| visible | true | BOOL | 是否可见,如(true) |
| fadevisible | true | BOOL | 是否可见,如(true),此属性会触发控件动画效果 |
| float | false | BOOL | 是否使用绝对定位,如(true) |
| menu | false | BOOL | 是否需要右键菜单,如(true) |
| cache | false | BOOL | 是否启用控件绘制缓存,如(true) |
| nofocus | false | BOOL | 是否可以获取焦点,如(true) |
| alpha | 255 | INT | 控件的整体透明度,如(128) |
| state | normal | STRING | 控件的当前状态,支持normal、hot、pushed、disabled状态 |
| cursortype | arrow | STRING | 鼠标移动到控件上时的鼠标光标,arrow、hand、ibeam |
| normalimage |  | STRING | 普通状态图片 |
| hotimage |  | STRING | 鼠标悬浮的状态图片 |
| pushedimage |  | STRING | 鼠标按下的状态图片 |
| disabledimage |  | STRING | 禁用的状态图片 |
| forenormalimage |  | STRING | 普通状态前景图片 |
| forehotimage |  | STRING | 鼠标悬浮的状态前景图片 |
| forepushedimage |  | STRING | 鼠标按下的状态前景图片 |
| foredisabledimage |  | STRING | 禁用的状态前景图片 |
| renderoffset | 0,0 | SIZE | 控件绘制时的偏移量,如(10,10),一般用于绘制动画 |
| fadealpha | false | BOOL | 是否启用控件透明渐变动画,如(true) |
| fadehot | false | BOOL | 是否启用控件悬浮状态下 的透明渐变动画,如(true) |
| fadewidth | false | BOOL | 是否启用控件宽度渐变动画,如(true) |
| fadeheight | false | BOOL | 是否启用控件高度渐变动画,如(true) |
| fadeinoutxfromleft | false | BOOL | 是否启用控件从左到右的动画,如(true) |
| fadeinoutxfromright | false | BOOL | 是否启用控件从右到左的动画,如(true) | 
| fadeinoutyfromtop | false | BOOL | 是否启用控件从上到下的动画,如(true) | 
| fadeinoutyfrombottom | false | BOOL | 是否启用控件从下到上的动画,如(true) |

## 可用接口

`Control` 继承了 `PlaceHolder` 提供的接口，包含了所有基础控件共有的属性。

| 接口名称 | 用途 |
| :--- | :--- |
| [GetParent](#GetParent) | 获取父容器指针 |
| [GetAncestor](#GetAncestor) | 根据名称获取祖先容器指针 |
| [GetName](#GetName) | 获取控件名称，对应 xml 中 name 属性 |
| [GetUTF8Name](#GetUTF8Name) | 获取控件名称，对应 xml 中 name 属性 |
| [SetName](#SetName) | 设置控件名称，内存中设置不会写入 xml 中 |
| [SetUTF8Name](#SetUTF8Name) | 设置控件名称，内存中设置不会写入 xml 中（UTF8 编码） |
| [GetWindow](#GetWindow) | 获取关联的窗口指针 |
| [SetWindow](#SetWindow) | 设置容器所属窗口 |
| [Init](#Init) | 初始化函数 |
| [DoInit](#DoInit) | 由 Init 调用，功能与 Init 相同 |
| [IsVisible](#IsVisible) | 判断是否可见 |
| [IsInternVisible](#IsInternVisible) | 待补充 |
| [IsFloat](#IsFloat) | 判断控件是否浮动，对应 xml 中 float 属性 |
| [SetFloat](#SetFloat) | 设置控件是否浮动 |
| [GetFixedWidth](#GetFixedWidth) | 获取固定宽度，对应 xml 中 width 属性 |
| [SetFixedWidth](#SetFixedWidth) | 设置控件固定宽度 |
| [GetFixedHeight](#GetFixedHeight) | 获取固定高度 |
| [SetFixedHeight](#SetFixedHeight) | 设置固定高度 |
| [GetMinWidth](#GetMinWidth) | 获取最小宽度 |
| [SetMinWidth](#SetMinWidth) | 设置最小宽度 |
| [GetMaxWidth](#GetMaxWidth) | 获取最大宽度 |
| [SetMaxWidth](#SetMaxWidth) | 设置最大宽度 |
| [GetMinHeight](#GetMinHeight) | 获取最小高度 |
| [SetMinHeight](#SetMinHeight) | 设置最小高度 |
| [GetMaxHeight](#GetMaxHeight) | 获取最大高度 |
| [SetMaxHeight](#SetMaxHeight) | 设置最大高度 |
| [GetWidth](#GetWidth) | 获取实际宽度 |
| [GetHeight](#GetHeight) | 获取实际高度 |
| [GetHorAlignType](#GetHorAlignType) | 获取水平对齐方式 |
| [SetHorAlignType](#SetHorAlignType) | 设置水平对齐方式 |
| [GetVerAlignType](#GetVerAlignType) | 获取垂直对齐方式 |
| [SetVerAlignType](#SetVerAlignType) | 设置垂直对齐方式 |
| [IsReEstimateSize](#IsReEstimateSize) | 待补充 |
| [SetReEstimateSize](#SetReEstimateSize) | 待补充 |
| [EstimateSize](#EstimateSize) | 待补充 |
| [GetPos](#GetPos) | 获取控件位置 |
| [SetPos](#SetPos) | 设置控件位置 |
| [Arrange](#Arrange) | 进行布局 |
| [ArrangeAncestor](#ArrangeAncestor) | 让父容器排列 |
| [IsArranged](#IsArranged) | 判断是否已经排列过 |
| [Invalidate](#Invalidate) | 重绘控件 |
| [GetPosWithScrollOffset](#GetPosWithScrollOffset) | 待补充 |
| [GetScrollOffset](#GetScrollOffset) | 待补充 |
| [ArrangeSelf](#ArrangeSelf) | 待补充 |
| [GetBkColor](#GetBkColor) | 获取背景颜色 |
| [SetBkColor](#SetBkColor) | 设置背景颜色 |
| [GetStateColor](#GetStateColor) | 获取某个状态下的字体颜色 |
| [SetStateColor](#SetStateColor) | 设置某个状态下的字体颜色 |
| [GetBkImage](#GetBkImage) | 获取背景图片位置 |
| [GetUTF8BkImage](#GetUTF8BkImage) | 获取 UTF8 格式的背景图片位置 |
| [SetBkImage](#SetBkImage) | 设置背景图片 |
| [SetUTF8BkImage](#SetUTF8BkImage) | 设置背景图片（UTF8 格式字符串） |
| [GetStateImage](#GetStateImage) | 获取指定状态下的图片位置 |
| [SetStateImage](#SetStateImage) | 设置某个状态下的图片 |
| [GetForeStateImage](#GetForeStateImage) | 获取指定状态下的前景图片 |
| [SetForeStateImage](#SetForeStateImage) | 设置某个状态下前景图片 |
| [GetState](#GetState) | 获取控件状态 |
| [SetState](#SetState) | 设置控件状态 |
| [GetEstimateImage](#GetEstimateImage) | 获取控件图片指针 |
| [GetBorderSize](#GetBorderSize) | 获取边框大小 |
| [SetBorderSize](#SetBorderSize) | 设置边框大小 |
| [GetBorderColor](#GetBorderColor) | 获取边框颜色 |
| [SetBorderColor](#SetBorderColor) | 设置边框颜色 |
| [SetBorderSize](#SetBorderSize) | 设置边框的大小 |
| [GetLeftBorderSize](#GetLeftBorderSize) | 获取左侧边框大小 |
| [SetLeftBorderSize](#SetLeftBorderSize) | 设置左侧边框大小 |
| [GetTopBorderSize](#GetTopBorderSize) | 获取顶部边框大小 |
| [SetTopBorderSize](#SetTopBorderSize) | 设置顶部边框大小 |
| [GetRightBorderSize](#GetRightBorderSize) | 获取右侧边框大小 |
| [SetRightBorderSize](#SetRightBorderSize) | 设置右侧边框大小 |
| [GetBottomBorderSize](#GetBottomBorderSize) | 获取下方边框大小 |
| [SetBottomBorderSize](#SetBottomBorderSize) | 设置下方边框大小 |
| [GetBorderRound](#GetBorderRound) | 获取边框大小 |
| [SetBorderRound](#SetBorderRound) | 设置边框大小 |
| [GetCursorType](#GetCursorType) | 获取鼠标指针类型 |
| [SetCursorType](#SetCursorType) | 设置当前鼠标指针类型 |
| [GetToolTipText](#GetToolTipText) | 获取控件在鼠标悬浮状态下的提示文本 |
| [GetUTF8ToolTipText](#GetUTF8ToolTipText) | 获取控件在鼠标悬浮状态下的提示文本（UTF8 格式） |
| [SetToolTipText](#SetToolTipText) | 设置鼠标悬浮到控件显示的提示文本 |
| [SetUTF8ToolTipText](#SetUTF8ToolTipText) | 设置鼠标悬浮到控件显示的提示文本（UTF8 格式） |
| [SetToolTipTextId](#SetToolTipTextId) | 设置鼠标悬浮到控件显示的提示文本在语言文件中对应的文字 |
| [SetUTF8ToolTipTextId](#SetUTF8ToolTipTextId) | 设置鼠标悬浮到控件显示的提示文本在语言文件中对应的文字（UTF8 格式） |
| [SetToolTipWidth](#SetToolTipWidth) | 设置鼠标悬浮到控件上提示的文本单行最大宽度 |
| [GetToolTipWidth](#GetToolTipWidth) | 获取鼠标悬浮到控件上提示的文本单行最大宽度 |
| [IsContextMenuUsed](#IsContextMenuUsed) | 控件是否响应右键菜单消息 |
| [SetContextMenuUsed](#SetContextMenuUsed) | 设置控件响应右键菜单消息 |
| [GetDataID](#GetDataID) | 获取用户绑定到控件的数据字符串 |
| [GetUTF8DataID](#GetUTF8DataID) | 获取用户绑定到控件的数据字符串（UTF8 格式） |
| [SetDataID](#SetDataID) | 绑定一个字符串数据到控件 |
| [SetUTF8DataID](#SetUTF8DataID) | 绑定一个字符串数据到控件（UTF8 格式） |
| [GetUserDataBase](#GetUserDataBase) | 获取用户绑定的自定义数据结构 |
| [SetUserDataBase](#SetUserDataBase) | 绑定自定义数据到控件，用户可继承 UserDataBase 来补充需要绑定的数据 |
| [SetVisible](#SetVisible) | 设置控件是否可见 |
| [SetInternVisible](#SetInternVisible) | 待补充 |
| [SetVisible_](#SetVisible_) | 待补充 |
| [IsEnabled](#IsEnabled) | 检查控件是否可用 |
| [SetEnabled](#SetEnabled) | 设置控件可用状态 |
| [IsMouseEnabled](#IsMouseEnabled) | 检查控件是否响应鼠标事件 |
| [SetMouseEnabled](#SetMouseEnabled) | 设置控件是否响应鼠标事件 |
| [IsKeyboardEnabled](#IsKeyboardEnabled) | 检查控件是否响应键盘事件 |
| [SetKeyboardEnabled](#SetKeyboardEnabled) | 设置控件是否响应键盘事件 |
| [IsFocused](#IsFocused) | 检查控件是否具有焦点 |
| [SetFocus](#SetFocus) | 让控件获取焦点 |
| [SetNoFocus](#SetNoFocus) | 让控件设置永远获取不到焦点 |
| [GetControlFlags](#GetControlFlags) | 返回控件的标识，用于判断是否可以响应 TAB 切换事件 |
| [IsMouseFocused](#IsMouseFocused) | 判断当前鼠标焦点是否在控件上 |
| [SetMouseFocused](#SetMouseFocused) | 设置是否将鼠标焦点到控件上 |
| [IsActivatable](#IsActivatable) | 判断控件当前是否是激活状态 |
| [Activate](#Activate) | 待补充 |
| [FindControl](#FindControl) | 根据坐标查找指定控件 |
| [GetPos](#GetPos) | 获取控件位置 |
| [SetPos](#SetPos) | 设置控件位置 |
| [GetMargin](#GetMargin) | 获取控件的外边距 |
| [SetMargin](#SetMargin) | 设置控件的外边距 |
| [EstimateSize](#EstimateSize) | 计算控件大小 |
| [EstimateText](#EstimateText) | 待补充 |
| [IsPointInWithScrollOffset](#IsPointInWithScrollOffset) | 检查指定坐标是否在滚动条当前滚动位置的范围内 |
| [HasHotState](#HasHotState) | 判断控件是否处于 HOT 状态 |
| [SetReceivePointerMsg](#SetReceivePointerMsg) | 设置控件是否响应触控消息 |
| [IsReceivePointerMsg](#IsReceivePointerMsg) | 判断控件是否响应触控消息 |
| [SetNeedButtonUpWhenKillFocus](#SetNeedButtonUpWhenKillFocus) | 设置控件失去焦点时是否发送鼠标弹起消息 |
| [IsNeedButtonUpWhenKillFocus](#IsNeedButtonUpWhenKillFocus) | 判断控件失去焦点时是否发送鼠标弹起消息 |
| [SetAttribute](#SetAttribute) | 设置控件指定属性 |
| [SetClass](#SetClass) | 设置控件的 class 全局属性 |
| [ApplyAttributeList](#ApplyAttributeList) | 应用一套属性列表 |
| [OnApplyAttributeList](#OnApplyAttributeList) | 待补充 |
| [HandleMessageTemplate](#HandleMessageTemplate) | 控件统一的消息处理入口，将传统 Windows 消息转换为自定义格式的消息 |
| [HandleMessageTemplate](#HandleMessageTemplate) | 将转换后的消息派发到消息处理函数 |
| [GetImage](#GetImage) | 根据图片路径缓存图片信息 |
| [DrawImage](#DrawImage) | 绘制图片 |
| [GetRenderContext](#GetRenderContext) | 获取绘制上下文对象 |
| [ClearRenderContext](#ClearRenderContext) | 清理绘制上下文对象 |
| [AlphaPaint](#AlphaPaint) | 待补充 |
| [Paint](#Paint) | 绘制控件的入口函数 |
| [PaintChild](#PaintChild) | 绘制控件子项入口函数 |
| [SetClip](#SetClip) | 设置是否对绘制范围做剪裁限制 |
| [IsClip](#IsClip) | 判断是否对绘制范围做剪裁限制 |
| [SetAlpha](#SetAlpha) | 设置控件透明度 |
| [GetAlpha](#GetAlpha) | 获取控件透明度 |
| [IsAlpha](#IsAlpha) | 检查控件是否有透明属性 |
| [SetHotAlpha](#SetHotAlpha) | 设置焦点状态透明度 |
| [GetHotAlpha](#GetHotAlpha) | 获取焦点状态透明度 |
| [GetRenderOffset](#GetRenderOffset) | 获取控件绘制偏移量 |
| [SetRenderOffset](#SetRenderOffset) | 设置控件绘制偏移量 |
| [SetRenderOffsetX](#SetRenderOffsetX) | 设置控件偏移的 X 坐标 |
| [SetRenderOffsetY](#SetRenderOffsetY) | 设置控件偏移的 Y 坐标 |
| [StartGifPlayForUI](#StartGifPlayForUI) | 播放 GIF |
| [StopGifPlayForUI](#StopGifPlayForUI) | 停止播放 GIF |
| [AttachGifPlayStop](#AttachGifPlayStop) | 监听 GIF 播放完成通知 |

## GetParent

获取父容器指针

```cpp
Box* GetParent()
```

 - 参&emsp;数：无  
 - 返回值：返回父容器指针
 
## GetAncestor

根据名称获取祖先容器指针

```cpp
Box* GetAncestor(const std::wstring& strName)
```

 - 参&emsp;数：  
    - `strName` 要获取的祖先容器名称
 - 返回值：返回祖先容器指针

## GetName

获取控件名称，对应 xml 中 name 属性

```cpp
std::wstring GetName()
```

 - 参&emsp;数：无  
 - 返回值：返回控件名称

## GetUTF8Name

获取控件名称，对应 xml 中 name 属性

```cpp
std::string GetUTF8Name()
```

 - 参&emsp;数：无  
 - 返回值：返回控件名称（UTF8 编码）

## SetName

设置控件名称，内存中设置不会写入 xml 中

```cpp
void SetName(const std::wstring& strName)
```

 - 参&emsp;数：  
    - `strName` 要设置的名称
 - 返回值：无

## SetUTF8Name

设置控件名称，内存中设置不会写入 xml 中（UTF8 编码）

```cpp
void SetUTF8Name(const std::string& strName)
```

 - 参&emsp;数：  
    - `strName` 要设置的名称
 - 返回值：无

## GetWindow

获取关联的窗口指针

```cpp
virtual Window* GetWindow()
```

 - 参&emsp;数：无  
 - 返回值：返回关联窗口的指针

## SetWindow

设置容器所属窗口

```cpp
virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true)
```

 - 参&emsp;数：  
    - `pManager` 窗口指针
    - `pParent` 父容器
    - `bInit` 设置后是否调用 Init 初始化控件
 - 返回值：无

## SetWindow

设置容器所属窗口

```cpp
virtual void SetWindow(Window* pManager)
```

 - 参&emsp;数：  
    - `pManager` 窗口指针
 - 返回值：无

## Init

初始化函数

```cpp
virtual void Init()
```

 - 参&emsp;数：无  
 - 返回值：无

## DoInit

由 Init 调用，功能与 Init 相同

```cpp
virtual void DoInit()
```

 - 参&emsp;数：无  
 - 返回值：无

## IsVisible

判断是否可见

```cpp
virtual bool IsVisible()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为可见，否则 false 为不可见

## IsInternVisible

待补充

```cpp
bool IsInternVisible()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

## IsFloat

判断控件是否浮动，对应 xml 中 float 属性

```cpp
bool IsFloat()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 是浮动状态，否则 false 为非浮动状态

## SetFloat

设置控件是否浮动

```cpp
void SetFloat(bool bFloat = true)
```

 - 参&emsp;数：  
    - `bFloat` 设置为 true 为浮动，false 为不浮动
 - 返回值：无

## GetFixedWidth

获取固定宽度，对应 xml 中 width 属性

```cpp
int GetFixedWidth()
```

 - 参&emsp;数：无  
 - 返回值：返回控件宽度

## SetFixedWidth

设置控件固定宽度

```cpp
void SetFixedWidth(int cx, bool bArrange = true, bool bNeedDpiScale = true)
```

 - 参&emsp;数：  
    - `cx` 要设置的宽度
    - `bArrange` 是否重新排列，默认为 true
    - `bNeedDpiScale` 兼容 DPI 缩放，默认为 true
 - 返回值：无

## GetFixedHeight

获取固定高度

```cpp
int GetFixedHeight()
```

 - 参&emsp;数：无  
 - 返回值：返回固定高度

## SetFixedHeight

设置固定高度

```cpp
void SetFixedHeight(int cy, bool bNeedDpiScale = true)
```

 - 参&emsp;数：  
    - `cy` 要设置的固定高度
    - `bNeedDpiScale` 兼容 DPI 缩放，默认为 true
 - 返回值：无

## GetMinWidth

获取最小宽度

```cpp
int GetMinWidth()
```

 - 参&emsp;数：无  
 - 返回值：返回最小宽度

## SetMinWidth

设置最小宽度

```cpp
void SetMinWidth(int cx)
```

 - 参&emsp;数：  
    - `cx` 要设置的最小宽度
 - 返回值：无

## GetMaxWidth

获取最大宽度

```cpp
int GetMaxWidth()
```

 - 参&emsp;数：无  
 - 返回值：返回最大宽度

## SetMaxWidth

设置最大宽度

```cpp
void SetMaxWidth(int cx)
```

 - 参&emsp;数：  
    - `cx` 要设置的最大宽度
 - 返回值：无

## GetMinHeight

获取最小高度

```cpp
int GetMinHeight()
```

 - 参&emsp;数：无  
 - 返回值：返回最小高度

## SetMinHeight

设置最小高度

```cpp
void SetMinHeight(int cy)
```

 - 参&emsp;数：  
    - `cy` 要设置的最小高度
 - 返回值：无

## GetMaxHeight

获取最大高度

```cpp
int GetMaxHeight()
```

 - 参&emsp;数：无  
 - 返回值：返回最大高度

## SetMaxHeight

设置最大高度

```cpp
void SetMaxHeight(int cy)
```

 - 参&emsp;数：  
    - `cy` 要设置的最大高度
 - 返回值：无

## GetWidth

获取实际宽度

```cpp
int GetWidth()
```

 - 参&emsp;数：无  
 - 返回值：返回实际宽度

## GetHeight

获取实际高度

```cpp
int GetHeight()
```

 - 参&emsp;数：无  
 - 返回值：返回实际高度

## GetHorAlignType

获取水平对齐方式

```cpp
HorAlignType GetHorAlignType()
```

 - 参&emsp;数：无  
 - 返回值：返回水平对齐方式，参考 HorAlignType 枚举

## SetHorAlignType

设置水平对齐方式

```cpp
void SetHorAlignType(HorAlignType horAlignType)
```

 - 参&emsp;数：  
    - `horAlignType` 要设置的对齐方式，参考 枚举
 - 返回值：无

## GetVerAlignType

获取垂直对齐方式

```cpp
VerAlignType GetVerAlignType()
```

 - 参&emsp;数：无  
 - 返回值：返回垂直对齐方式，参见 VerAlignType 枚举

## SetVerAlignType

设置垂直对齐方式

```cpp
void SetVerAlignType(VerAlignType verAlignType)
```

 - 参&emsp;数：  
    - `vorAlignType` 要设置的对齐方式，参考 VerAlignType 枚举
 - 返回值：无

## IsReEstimateSize

待补充

```cpp
bool IsReEstimateSize()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

## SetReEstimateSize

待补充

```cpp
void SetReEstimateSize(bool bReEstimateSize)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

## EstimateSize

待补充

```cpp
virtual CSize EstimateSize(CSize szAvailable)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

## GetPos

获取控件位置

```cpp
virtual	UiRect GetPos(bool bContainShadow = true)
```

 - 参&emsp;数：  
    - `bContainShadow` 是否包含阴影位置，默认为 true
 - 返回值：返回控件位置

## SetPos

设置控件位置

```cpp
virtual void SetPos(UiRect rc)
```

 - 参&emsp;数：  
    - `rc` 要设置的位置信息
 - 返回值：无

## Arrange

进行布局

```cpp
virtual void Arrange()
```

 - 参&emsp;数：无  
 - 返回值：无

## ArrangeAncestor

让父容器排列

```cpp
virtual void ArrangeAncestor()
```

 - 参&emsp;数：无  
 - 返回值：无

## IsArranged

判断是否已经排列过

```cpp
bool IsArranged()
```

 - 参&emsp;数：无  
 - 返回值：true 为已经排列过，false 为尚未排列

## Invalidate

重绘控件

```cpp
virtual void Invalidate()
```

 - 参&emsp;数：无  
 - 返回值：无

## GetPosWithScrollOffset

待补充

```cpp
UiRect GetPosWithScrollOffset()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

## GetScrollOffset

待补充

```cpp
CPoint GetScrollOffset()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

## ArrangeSelf

待补充

```cpp
virtual void ArrangeSelf()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### GetBkColor

获取背景颜色

```cpp
std::wstring GetBkColor()
```

 - 参&emsp;数：无  
 - 返回值：返回背景颜色的字符串，该值在 global.xml 中定义

### SetBkColor

设置背景颜色

```cpp
void SetBkColor(const std::wstring& strColor)
```

 - 参&emsp;数：  
    - `strColor` 要设置的背景颜色值，该值必须在 global.xml 中存在
 - 返回值：无

### GetStateColor

获取某个状态下的字体颜色

```cpp
std::wstring GetStateColor(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取何种状态下的颜色值，参考 Control枚举
 - 返回值：指定状态下设定的颜色字符串，对应 global.xml 中指定色值

### SetStateColor

设置某个状态下的字体颜色

```cpp
void SetStateColor(ControlStateType stateType, const std::wstring& strColor)
```

 - 参&emsp;数：  
    - `stateType` 要设置何种状态下的颜色值，参考 Control枚举
    - `strColor` 要设置的颜色值，该值必须在 global.xml 中存在
 - 返回值：无

### GetBkImage

获取背景图片位置

```cpp
std::wstring GetBkImage()
```

 - 参&emsp;数：无  
 - 返回值：背景图片位置  

### GetUTF8BkImage

获取 UTF8 格式的背景图片位置

```cpp
std::string GetUTF8BkImage()
```

 - 参&emsp;数：无  
 - 返回值：背景图片位置

### SetBkImage

设置背景图片

```cpp
void SetBkImage(const std::wstring& strImage)
```

 - 参&emsp;数：  
    - `strImage` 要设置的图片路径
 - 返回值：无

### SetUTF8BkImage

设置背景图片（UTF8 格式字符串）

```cpp
void SetUTF8BkImage(const std::string& strImage)
```

 - 参&emsp;数：  
    - `strImage` 要设置的图片路径
 - 返回值：无

### GetStateImage

获取指定状态下的图片位置

```cpp
std::wstring GetStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取何种状态下的图片，参考 `ControlStateType` 枚举
 - 返回值：指定状态下的图片位置

### SetStateImage

设置某个状态下的图片

```cpp
void SetStateImage(ControlStateType stateType, const std::wstring& strImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置何种状态下的图片，参考 Control枚举
    - `strImage` 要设置的图片路径
 - 返回值：无

### GetForeStateImage

获取指定状态下的前景图片

```cpp
std::wstring GetForeStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取何种状态下的图片，参考 `ControlStateType` 枚举
 - 返回值：指定状态下前景图片位置

### SetForeStateImage

设置某个状态下前景图片

```cpp
void SetForeStateImage(ControlStateType stateType, const std::wstring& strImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置何种状态下的图片，参考 `ControlStateType` 枚举
    - `strImage` 要设置的前景图片路径
 - 返回值：无

### GetState

获取控件状态

```cpp
ControlStateType GetState()
```

 - 参&emsp;数：无  
 - 返回值：控件的状态，请参考 `ControlStateType` 枚举

### SetState

设置控件状态

```cpp
void SetState(ControlStateType pStrState)
```

 - 参&emsp;数：  
    - `pStrState` 要设置的控件状态，请参考 `ControlStateType` 枚举
 - 返回值：无

### GetEstimateImage

获取控件图片指针

```cpp
virtual Image* GetEstimateImage()
```

 - 参&emsp;数：无  
 - 返回值：返回图片对象指针

### GetBorderSize

获取边框大小

```cpp
int GetBorderSize()
```

 - 参&emsp;数：无  
 - 返回值：返回边框的大小

### SetBorderSize

设置边框大小

```cpp
void SetBorderSize(int nSize)
```

 - 参&emsp;数：  
    - `nSize` 要设置的边框大小
 - 返回值：无

### GetBorderColor

获取边框颜色

```cpp
std::wstring GetBorderColor()
```

 - 参&emsp;数：无  
 - 返回值：边框的颜色字符串，对应 global.xml 中的具体颜色值

### SetBorderColor

设置边框颜色

```cpp
void SetBorderColor(const std::wstring& strBorderColor)
```

 - 参&emsp;数：  
    - `strBorderColor` 设置边框的颜色字符串值，该值必须在 global.xml 中存在
 - 返回值：无

### SetBorderSize

设置边框的大小

```cpp
void SetBorderSize(UiRect rc)
```

 - 参&emsp;数：  
    - `rc` 一个 `UiRect` 结构的边框大小集合
 - 返回值：无

### GetLeftBorderSize

获取左侧边框大小

```cpp
int GetLeftBorderSize()
```

 - 参&emsp;数：无  
 - 返回值：左侧边框的大小  

### SetLeftBorderSize

设置左侧边框大小

```cpp
void SetLeftBorderSize(int nSize)
```

 - 参&emsp;数：  
    - `nSize` 要设置的左侧边框大小
 - 返回值：无

### GetTopBorderSize

获取顶部边框大小

```cpp
int GetTopBorderSize()
```

 - 参&emsp;数：无  
 - 返回值：顶部边框大小

### SetTopBorderSize

设置顶部边框大小

```cpp
void SetTopBorderSize(int nSize)
```

 - 参&emsp;数：  
    - `nSize` 要设置的上方边框大小
 - 返回值：无

### GetRightBorderSize

获取右侧边框大小

```cpp
int GetRightBorderSize()
```

 - 参&emsp;数：无  
 - 返回值：右侧的边框大小

### SetRightBorderSize

设置右侧边框大小

```cpp
void SetRightBorderSize(int nSize)
```

 - 参&emsp;数：  
    - `nSize` 要设置的右侧边框大小
 - 返回值：无

### GetBottomBorderSize

获取下方边框大小

```cpp
int GetBottomBorderSize()
```

 - 参&emsp;数：无  
 - 返回值：下方边框大小

### SetBottomBorderSize

设置下方边框大小

```cpp
void SetBottomBorderSize(int nSize)
```

 - 参&emsp;数：  
    - `nSize` 要设置的下方边框大小
 - 返回值：无

### GetBorderRound

获取边框大小

```cpp
CSize GetBorderRound()
```

 - 参&emsp;数：无  
 - 返回值：四个方向的边框大小

### SetBorderRound

设置边框大小

```cpp
void SetBorderRound(CSize cxyRound)
```

 - 参&emsp;数：  
    - `cxyRound` 一个 CSize 结构表示了四个方向边框的大小
 - 返回值：无

### GetCursorType

获取鼠标指针类型

```cpp
virtual CursorType GetCursorType()
```

 - 参&emsp;数：无  
 - 返回值：当前鼠标类型

### SetCursorType

设置当前鼠标指针类型

```cpp
void SetCursorType(CursorType flag)
```

 - 参&emsp;数：  
    - `flag` 要设置的鼠标类型，参考 CursorType 枚举
 - 返回值：无

### GetToolTipText

获取控件在鼠标悬浮状态下的提示文本

```cpp
virtual std::wstring GetToolTipText()
```

 - 参&emsp;数：无  
 - 返回值：返回当前鼠标悬浮状态提示的文本

### GetUTF8ToolTipText

获取控件在鼠标悬浮状态下的提示文本（UTF8 格式）

```cpp
virtual std::string GetUTF8ToolTipText()
```

 - 参&emsp;数：无  
 - 返回值：返回当前鼠标悬浮状态提示的文本

### SetToolTipText

设置鼠标悬浮到控件显示的提示文本

```cpp
virtual void SetToolTipText(const std::wstring& strText)
```

 - 参&emsp;数：  
    - `strText` 要设置的文本
 - 返回值：无

### SetUTF8ToolTipText

设置鼠标悬浮到控件显示的提示文本（UTF8 格式）

```cpp
virtual void SetUTF8ToolTipText(const std::string& strText)
```

 - 参&emsp;数：  
    - `strText` 要设置的文本
 - 返回值：无

### SetToolTipTextId

设置鼠标悬浮到控件显示的提示文本在语言文件中对应的文字

```cpp
virtual void SetToolTipTextId(const std::wstring& strTextId)
```

 - 参&emsp;数：  
    - `strTextId` 在语言文件中对应的提示文字 ID
 - 返回值：无

### SetUTF8ToolTipTextId

设置鼠标悬浮到控件显示的提示文本在语言文件中对应的文字（UTF8 格式）

```cpp
virtual void SetUTF8ToolTipTextId(const std::string& strTextId)
```

 - 参&emsp;数：  
    - `strTextId` 在语言文件中对应的提示文字 ID
 - 返回值：无

### SetToolTipWidth

设置鼠标悬浮到控件上提示的文本单行最大宽度

```cpp
virtual void SetToolTipWidth(int nWidth)
```

 - 参&emsp;数：  
    - `nWidth` 要设置的宽度值
 - 返回值：无

### GetToolTipWidth

获取鼠标悬浮到控件上提示的文本单行最大宽度

```cpp
virtual int GetToolTipWidth(void)
```

 - 参&emsp;数：无  
 - 返回值：当前提示文本最大宽度

### IsContextMenuUsed

控件是否响应右键菜单消息

```cpp
virtual bool IsContextMenuUsed()
```

 - 参&emsp;数：无  
 - 返回值：返回结果表示了是否响应右键菜单消息，true 响应右键菜单消息，false 不响应右键菜单消息

### SetContextMenuUsed

设置控件响应右键菜单消息

```cpp
virtual void SetContextMenuUsed(bool bMenuUsed)
```

 - 参&emsp;数：  
    - `bMenuUsed` 是否响应右键菜单消息，true 为是，false 为否
 - 返回值：无

### GetDataID

获取用户绑定到控件的数据字符串

```cpp
virtual std::wstring GetDataID()
```

 - 参&emsp;数：无  
 - 返回值：返回具体数据字符串

### GetUTF8DataID

获取用户绑定到控件的数据字符串（UTF8 格式）

```cpp
virtual std::string GetUTF8DataID()
```

 - 参&emsp;数：无  
 - 返回值：返回具体数据字符串

### SetDataID

绑定一个字符串数据到控件

```cpp
virtual void SetDataID(const std::wstring& strText)
```

 - 参&emsp;数：  
    - `strText` 要绑定的字符串数据
 - 返回值：无

### SetUTF8DataID

绑定一个字符串数据到控件（UTF8 格式）

```cpp
virtual void SetUTF8DataID(const std::string& strText)
```

 - 参&emsp;数：  
    - `strText` 要绑定的字符串数据
 - 返回值：无

### GetUserDataBase

获取用户绑定的自定义数据结构

```cpp
virtual UserDataBase* GetUserDataBase()
```

 - 参&emsp;数：无  
 - 返回值：用户绑定的自定义结构数据指针

### SetUserDataBase

绑定自定义数据到控件，用户可继承 UserDataBase 来补充需要绑定的数据

```cpp
virtual void SetUserDataBase(UserDataBase* pUserDataBase)
```

 - 参&emsp;数：  
    - `pUserDataBase` 数据指针
 - 返回值：无

### SetVisible

设置控件是否可见

```cpp
virtual void SetVisible(bool bVisible = true)
```

 - 参&emsp;数：  
    - `bVisible` 为 true 时控件可见，为 false 时控件被隐藏
 - 返回值：无

### SetInternVisible

待补充

```cpp
virtual void SetInternVisible(bool bVisible = true)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### SetVisible_

待补充

```cpp
virtual void SetVisible_(bool bVisible)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### IsEnabled

检查控件是否可用

```cpp
virtual bool IsEnabled()
```

 - 参&emsp;数：无  
 - 返回值：控件可用状态，返回 true 控件可用，否则为 false

### SetEnabled

设置控件可用状态

```cpp
virtual void SetEnabled(bool bEnable = true)
```

 - 参&emsp;数：  
    - `bEnable` 为 true 时控件可用，为 false 时控件为禁用状态则不可用
 - 返回值：无

### IsMouseEnabled

检查控件是否响应鼠标事件

```cpp
virtual bool IsMouseEnabled()
```

 - 参&emsp;数：无  
 - 返回值：返回控件是否响应鼠标事件，返回 true 响应鼠标事件，false 为不响应

### SetMouseEnabled

设置控件是否响应鼠标事件

```cpp
virtual void SetMouseEnabled(bool bEnable = true)
```

 - 参&emsp;数：  
    - `bEnable` 为 true 响应鼠标事件，为 false 时不响应鼠标事件
 - 返回值：无

### IsKeyboardEnabled

检查控件是否响应键盘事件

```cpp
virtual bool IsKeyboardEnabled()
```

 - 参&emsp;数：无  
 - 返回值：返回控件是否响应键盘事件，返回 true 响应键盘事件，false 不响应键盘事件

### SetKeyboardEnabled

设置控件是否响应键盘事件

```cpp
virtual void SetKeyboardEnabled(bool bEnable = true)
```

 - 参&emsp;数：  
    - `bEnable` 为 true 响应键盘事件，为 false 时不响应键盘事件
 - 返回值：无

### IsFocused

检查控件是否具有焦点

```cpp
virtual bool IsFocused()
```

 - 参&emsp;数：无  
 - 返回值：返回控件是否具有检点，为 true 时是当前具有焦点，为 false 时控件没有焦点

### SetFocus

让控件获取焦点

```cpp
virtual void SetFocus()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetNoFocus

让控件设置永远获取不到焦点

```cpp
void SetNoFocus()
```

 - 参&emsp;数：无  
 - 返回值：无

### GetControlFlags

返回控件的标识，用于判断是否可以响应 TAB 切换事件

```cpp
virtual UINT GetControlFlags() const
```

 - 参&emsp;数：无  
 - 返回值：返回控件的标识类型，`UIFLAG_DEFAULT` 为默认状态不支持焦点切换，`UIFLAG_TABSTOP` 为支持焦点切换

### IsMouseFocused

判断当前鼠标焦点是否在控件上

```cpp
virtual bool IsMouseFocused()
```

 - 参&emsp;数：无  
 - 返回值：返回鼠标焦点是否在控件上，true 鼠标焦点在控件上，false 鼠标焦点不在控件上

### SetMouseFocused

设置是否将鼠标焦点到控件上

```cpp
virtual void SetMouseFocused(bool bMouseFocused)
```

 - 参&emsp;数：  
    - `bMouseFocused` 为 true 时设置鼠标焦点到控件上，为 false 时让控件失去鼠标焦点
 - 返回值：无

### IsActivatable

判断控件当前是否是激活状态

```cpp
virtual bool IsActivatable()
```

 - 参&emsp;数：无  
 - 返回值：返回控件状态，true 控件当前是激活状态，可见并可用，false 控件当前非激活状态，可能不可见或被禁用

### Activate

待补充

```cpp
virtual void Activate()
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### FindControl

根据坐标查找指定控件

```cpp
virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint()
```

 - 参&emsp;数：  
    - `Proc
    - `pData
    - `uFlags
    - `scrollPos
 - 返回值：返回控件的指针

### GetPos

获取控件位置

```cpp
virtual	UiRect GetPos(bool bContainShadow = true)
```

 - 参&emsp;数：  
    - `bContainShadow` 是否包含阴影，默认为 true 则包含阴影范围，否则 false 为不包含
 - 返回值：返回控件的位置信息

### SetPos

设置控件位置

```cpp
virtual void SetPos(UiRect rc)
```

 - 参&emsp;数：  
    - `rc` 控件位置的描述信息
 - 返回值：无

### GetMargin

获取控件的外边距

```cpp
virtual UiRect GetMargin()
```

 - 参&emsp;数：无  
 - 返回值：返回控件的外边距

### SetMargin

设置控件的外边距

```cpp
virtual void SetMargin(UiRect rcMargin, bool bNeedDpiScale = true)
```

 - 参&emsp;数：  
    - `rcMargin` 控件的外边距信息
    - `bNeedDpiScale` 是否让外边距根据 DPI 适配，默认为 true，false 不适配 DPI
 - 返回值：无

### EstimateSize

计算控件大小

```cpp
virtual CSize EstimateSize(CSize szAvailable)
```

 - 参&emsp;数：  
    - `szAvailable` 暂无意义
 - 返回值：szAvailable 控件实际大小（如果设置了图片并设置 width 或 height 任意一项为 auto，将根据图片来计算最终大小）

### EstimateText

待补充

```cpp
virtual CSize EstimateText(CSize szAvailable, bool& bReEstimateSize)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### IsPointInWithScrollOffset

检查指定坐标是否在滚动条当前滚动位置的范围内

```cpp
virtual bool IsPointInWithScrollOffset(const CPoint& point)
```

 - 参&emsp;数：  
    - `point` 具体坐标
 - 返回值：返回是否在范围内，true 在滚动条当前滚动位置范围内，false 不在滚动条当前滚动位置范围内

### HasHotState

判断控件是否处于 HOT 状态

```cpp
virtual bool HasHotState()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示当前处于 HOT 状态， 否则为 false

### SetReceivePointerMsg

设置控件是否响应触控消息

```cpp
void SetReceivePointerMsg(bool bRecv)
```

 - 参&emsp;数：  
    - `bRecv` 设置为 true 表示响应触控消息，false 为不响应
 - 返回值：无

### IsReceivePointerMsg

判断控件是否响应触控消息

```cpp
bool IsReceivePointerMsg() const
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为响应，否则为 false

### SetNeedButtonUpWhenKillFocus

设置控件失去焦点时是否发送鼠标弹起消息

```cpp
void SetNeedButtonUpWhenKillFocus(bool bNeed)
```

 - 参&emsp;数：  
    - `bNeed` bNeed 设置为 true 表示响应触控消息，false 为不响应
 - 返回值：无

### IsNeedButtonUpWhenKillFocus

判断控件失去焦点时是否发送鼠标弹起消息

```cpp
bool IsNeedButtonUpWhenKillFocus() const
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为需要，false 为不需要

### SetAttribute

设置控件指定属性

```cpp
virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue)
```

 - 参&emsp;数：  
    - `strName` 要设置的属性名称（如 width）
    - `strValue` 要设置的属性值（如 100）
 - 返回值：无

### SetClass

设置控件的 class 全局属性

```cpp
void SetClass(const std::wstring& strClass)
```

 - 参&emsp;数：  
    - `strClass` 要设置的 class 名称，该名称必须在 global.xml 中存在
 - 返回值：无

### ApplyAttributeList

应用一套属性列表

```cpp
void ApplyAttributeList(const std::wstring& strList)
```

 - 参&emsp;数：  
    - `strList` 属性列表的字符串表示，如 `width="100" height="30"`
 - 返回值：无

### OnApplyAttributeList

待补充

```cpp
bool OnApplyAttributeList(const std::wstring& strReceiver, const std::wstring& strList, EventArgs* eventArgs)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充

### HandleMessageTemplate

控件统一的消息处理入口，将传统 Windows 消息转换为自定义格式的消息

```cpp
void HandleMessageTemplate(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0, TCHAR tChar = 0, CPoint mousePos = CPoint()
```

 - 参&emsp;数：  
    - `eventType	消息内容
    - `wParam	消息附加内容
    - `lParam	消息附加内容
    - `tChar		按键信息
    - `mousePos	鼠标信息
 - 返回值：无

### HandleMessageTemplate

将转换后的消息派发到消息处理函数

```cpp
virtual void HandleMessageTemplate(EventArgs& msg)
```

 - 参&emsp;数：  
    - `msg` 消息内容
 - 返回值：无

### GetImage

根据图片路径缓存图片信息

```cpp
void GetImage(Image& duiImage)
```

 - 参&emsp;数：无  
 - 返回值：无

### DrawImage

绘制图片

```cpp
bool DrawImage(IRenderContext* pRender, Image& duiImage, const std::wstring& strModify = L"", int nFade = DUI_NOSET_VALUE)
```

 - 参&emsp;数：  
    - `pRender` 绘制上下文
    - `duiImage` 图片对象
    - `strModify` 图片的附加属性
    - `nFade` 控件的透明度，如果启用动画效果该值在绘制时是不断变化的
 - 返回值：成功返回 true，失败返回 false

### AlphaPaint

待补充

```cpp
void AlphaPaint(IRenderContext* pRender, const UiRect& rcPaint)
```

 - 参&emsp;数：  
    - `待补充
 - 返回值：待补充
 
### GetRenderContext

获取绘制上下文对象

```cpp
IRenderContext* GetRenderContext()
```

 - 参&emsp;数：无  
 - 返回值：返回绘制上下文对象

### ClearRenderContext

清理绘制上下文对象

```cpp
void ClearRenderContext()
```

 - 参&emsp;数：无  
 - 返回值：无

### Paint

绘制控件的入口函数

```cpp
virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint)
```

 - 参&emsp;数：  
    - `pRender` 指定绘制区域
    - `rcPaint` 指定绘制坐标
 - 返回值：无
 
### PaintChild

绘制控件子项入口函数

```cpp
virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint)
```

 - 参&emsp;数：  
    - `pRender` 指定绘制区域
    - `rcPaint` 指定绘制坐标
 - 返回值：无
 
### SetClip

设置是否对绘制范围做剪裁限制

```cpp
void SetClip(bool clip)
```

 - 参&emsp;数：  
    - `clip` 设置 true 为需要，否则为不需要，见绘制函数
 - 返回值：无

### IsClip

判断是否对绘制范围做剪裁限制

```cpp
bool IsClip() const
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为需要，false 为不需要

### SetAlpha

设置控件透明度

```cpp
void SetAlpha(int alpha)
```

 - 参&emsp;数：  
    - `alpha` 0 ~ 255 的透明度值，255 为不透明
 - 返回值：无

### GetAlpha

获取控件透明度

```cpp
int GetAlpha()
```

 - 参&emsp;数：无  
 - 返回值：返回控件的透明度

### IsAlpha

检查控件是否有透明属性

```cpp
bool IsAlpha()
```

 - 参&emsp;数：无  
 - 返回值：返回控件是否透明，true 控件当前有透明属性，false 控件没有透明属性

### SetHotAlpha

设置焦点状态透明度

```cpp
void SetHotAlpha(int nHotAlpha)
```

 - 参&emsp;数：  
    - `alpha` 0 ~ 255 的透明度值，255 为不透明
 - 返回值：无

### GetHotAlpha

获取焦点状态透明度

```cpp
int GetHotAlpha()
```

 - 参&emsp;数：无  
 - 返回值：返回控件焦点状态的透明度

### GetRenderOffset

获取控件绘制偏移量

```cpp
CPoint GetRenderOffset()
```

 - 参&emsp;数：无  
 - 返回值：返回当前控件的绘制偏移量

### SetRenderOffset

设置控件绘制偏移量

```cpp
void SetRenderOffset(CPoint renderOffset)
```

 - 参&emsp;数：  
    - `renderOffset` 控件偏移数据
 - 返回值：无

### SetRenderOffsetX

设置控件偏移的 X 坐标

```cpp
void SetRenderOffsetX(int renderOffsetX)
```

 - 参&emsp;数：  
    - `renderOffsetX` X 坐标值
 - 返回值：无

### SetRenderOffsetY

设置控件偏移的 Y 坐标

```cpp
void SetRenderOffsetY(int renderOffsetY)
```

 - 参&emsp;数：  
    - `renderOffsetY` Y 坐标值
 - 返回值：无

### StartGifPlayForUI

播放 GIF

```cpp
void StartGifPlayForUI(GifStopType frame = kGifStopFirst,int playcount = -1)
```

 - 参&emsp;数：  
    - `playcount` 播放完成停止在哪一帧，可设置第一帧、当前帧和最后一帧。请参考 GifStopType 枚举
 - 返回值：无

### StopGifPlayForUI

停止播放 GIF

```cpp
void StopGifPlayForUI(bool transfer = false, GifStopType frame = kGifStopCurrent)
```

 - 参&emsp;数：  
    - `transfer` 是否将停止事件通知给订阅者，参考 AttachGifPlayStop 方法
    - `frame` 播放结束停止在哪一帧，可设置第一帧、当前帧和最后一帧。请参考 GifStopType 枚举
 - 返回值：无

### AttachGifPlayStop

监听 GIF 播放完成通知

```cpp
void AttachGifPlayStop(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 要监听 GIF 停止播放的回调函数
 - 返回值：无