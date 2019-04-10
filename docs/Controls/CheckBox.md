# CheckBox（复选框）

`CheckBox` 控件继承了 `Button` 控件属性和方法，更多可用属性和方法请参考：[Button](Button.md) 控件

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| selected | false | BOOL | 是否选中 |
| selectednormalimage |  | STRING | 选中的普通状态图片 |
| selectedhotimage |  | STRING | 选中的鼠标悬浮的状态图片 |
| selectedpushedimage |  | STRING | 选中的鼠标按下的状态图片 |
| selecteddisabledimage |  | STRING | 选中的禁用的状态图片 |
| selectedtextcolor |  | STRING | 选中状态字体颜色,不指定则使用默认颜色,如(blue) |
| selectednormalcolor |  | STRING | 选中的普通状态字体颜色,不指定则使用默认颜色,如(blue) |
| selectedhotcolor |  | STRING | 选中的鼠标悬浮状态字体颜色,不指定则使用默认颜色,如(blue) |
| selectedpushedcolor |  | STRING | 选中的鼠标按下状态字体颜色,不指定则使用默认颜色,如(blue) |
| selecteddisabledcolor |  | STRING | 选中的禁用状态字体颜色,不指定则使用默认颜色,如(blue) |
| selectedforenormalimage |  | STRING | 选中的前景图片 |
| selectedforehotimage |  | STRING | 选中的鼠标悬浮状态的图片 |
| selectedforepushedimage |  | STRING | 选中的鼠标按下状态的前景图片 |
| selectedforedisabledimage |  | STRING | 选中的禁用状态的前景图片 |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [IsSelected](#IsSelected) | 判断当前是否是选择状态 |
| [Selected](#Selected) | 设置控件是否选择状态 |
| [GetSelectedStateImage](#GetSelectedStateImage) | 获取被选择时的图片 |
| [SetSelectedStateImage](#SetSelectedStateImage) | 设置被选择时的图片 |
| [GetSelectedTextColor](#GetSelectedTextColor) | 获取被选择时的文本颜色 |
| [SetSelectedTextColor](#SetSelectedTextColor) | 设置被选择时的文本颜色 |
| [GetSelectStateColor](#GetSelectStateColor) | 获取被选择时的控件颜色  |
| [SetSelectedStateColor](#SetSelectedStateColor) | 设置被选择时的控件颜色 |
| [GetSelectedForeStateImage](#GetSelectedForeStateImage) | 获取被选择时的前景图片 |
| [SetSelectedForeStateImage](#SetSelectedForeStateImage) | 设置被选择时的前景图片 |
| [AttachSelect](#AttachSelect) | 监听被选择时的事件 |
| [AttachUnSelect](#AttachUnSelect) | 监听取消选择时的事件 |


### IsSelected

判断当前是否是选择状态

```cpp
bool IsSelected()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为选择状态，否则为 false

### Selected

设置控件是否选择状态

```cpp
virtual void Selected(bool bSelected, bool bTriggerEvent = false)
```

 - 参&emsp;数：  
    - `bSelected` 为 true 时为选择状态，false 时为取消选择状态
    - `bTriggerEvent` 是否发送状态改变事件，true 为发送，否则为 false。默认为 false
 - 返回值：无

### GetSelectedStateImage

获取被选择时的图片

```cpp
std::wstring GetSelectedStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取何种状态下的图片，参考 `ControlStateType` 枚举
 - 返回值：返回图片位置

### SetSelectedStateImage

设置被选择时的图片

```cpp
void SetSelectedStateImage(ControlStateType stateType, const std::wstring& strImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置哪中状态下的图片
    - `strImage` 图片地址
 - 返回值：无

### GetSelectedTextColor

获取被选择时的文本颜色

```cpp
std::wstring GetSelectedTextColor()
```

 - 参&emsp;数：无  
 - 返回值：返回被选择时的文本颜色

### SetSelectedTextColor

设置被选择时的文本颜色

```cpp
void SetSelectedTextColor(const std::wstring& dwTextColor)
```

 - 参&emsp;数：  
    - `dwTextColor` 要设置的颜色字符串，该颜色必须在 global.xml 中存在
 - 返回值：无

### GetSelectStateColor

获取被选择时的控件颜色 

```cpp
std::wstring GetSelectStateColor(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取何种状态下的颜色
 - 返回值：返回颜色字符串，该值在 global.xml 中定义

### SetSelectedStateColor

设置被选择时的控件颜色

```cpp
void SetSelectedStateColor(ControlStateType stateType, const std::wstring& stateColor)
```

 - 参&emsp;数：  
    - `stateType` 要设置何种状态下的颜色
    - `stateColor` 要设置的颜色
 - 返回值：无

### GetSelectedForeStateImage

获取被选择时的前景图片

```cpp
std::wstring GetSelectedForeStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取何种状态下的前景图片
 - 返回值：返回图片位置

### SetSelectedForeStateImage

设置被选择时的前景图片

```cpp
void SetSelectedForeStateImage(ControlStateType stateType, const std::wstring& pStrImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置何种状态下的前景图片
    - `pStrImage` 图片位置
 - 返回值：无

### AttachSelect

监听被选择时的事件

```cpp
void AttachSelect(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 被选择时触发的回调函数
 - 返回值：无

### AttachUnSelect

监听取消选择时的事件

```cpp
void AttachUnSelect(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 取消选择时触发的回调函数
 - 返回值：无