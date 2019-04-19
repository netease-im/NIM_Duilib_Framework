# Label（文本控件）

Label 是一个模板类，当直接使用传统控件如 Button、CheckBox 等控件时，将从 [Control](Control.md) 继承属性和方法。
当使用包含布局属性的控件如 ButtonBox、ComboBoxBox、LabelBox 等控件时，Label 将继承 [Box](../Containers/Box.md) 的方法和属性。

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| width | auto | INT / STRING | 可以设置INT或STRING类型的值.当值为INT是则设置控件的最小宽度,如(100);当值为STRING时,stretch代表由父容器计算控件宽度,auto代表根据内容情况自动计算宽度 |
| height | auto | INT / STRING | 可以设置INT或STRING类型的值.当值为INT是则设置控件的最小高度,如(100);当值为STRING时,stretch代表由父容器计算控件高度,auto代表根据内容情况自动计算高度 |
| text |  | STRING | 显示文本,如(测试文本) |
| textid |  | STRING | 多语言功能的ID,如(TEXT_OUT) |
| align | left | STRING | 文本的输出位置,如(center),支持left、center、right、top、vcenter、bottom |
| font | -1 | INT | 字体id,如(0) |
| endellipsis | false | BOOL | 句末显示不完是否使用...代替,如(true) |
| textpadding | 0,0,0,0 | RECT | 文字显示的边距,如(2,2,2,2) |
| normaltextcolor |  | STRING | 普通字体颜色,不指定则使用默认颜色,如(blue) |
| hottextcolor |  | STRING | 鼠标悬浮字体颜色,不指定则使用默认颜色,如(blue) |
| pushedtextcolor |  | STRING | 鼠标按下字体颜色,不指定则使用默认颜色,如(blue) |
| disabledtextcolor |  | STRING | disabled字体颜色,不指定则使用默认颜色,如(blue) |
| linelimit | false | BOOL | 是否限制正行输出,如(true) |
| singleline | true | BOOL | 是否单行输出文字,如(true) |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [SetTextStyle](#SetTextStyle) | 设置文本样式 |
| [GetTextStyle](#GetTextStyle) | 获取文本样式 |
| [GetStateTextColor](#GetStateTextColor) | 获取指定状态下的文本颜色 |
| [SetStateTextColor](#SetStateTextColor) | 设置指定状态下的文本颜色 |
| [GetFont](#GetFont) | 获取当前字体编号 |
| [SetFont](#SetFont) | 设置当前字体 |
| [GetTextPadding](#GetTextPadding) | 获取文字边距 |
| [SetTextPadding](#SetTextPadding) | 设置文字边距信息 |
| [IsSingleLine](#IsSingleLine) | 判断是否是单行模式 |
| [SetSingleLine](#SetSingleLine) | 设置为单行输入模式 |
| [IsLineLimit](#IsLineLimit) | 是否限制整行输出 |
| [SetLineLimit](#SetLineLimit) | 限制整行输出 |

### SetTextStyle

设置文本样式

```cpp
void SetTextStyle(UINT uStyle)
```

 - 参&emsp;数：  
    - `uStyle` 要设置的样式
 - 返回值：无

### GetTextStyle

获取文本样式

```cpp
UINT GetTextStyle()
```

 - 参&emsp;数：无  
 - 返回值：返回文本样式

### GetStateTextColor

获取指定状态下的文本颜色

```cpp
std::wstring GetStateTextColor(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取的状态标志
 - 返回值：返回指定状态下的文本颜色

### SetStateTextColor

设置指定状态下的文本颜色

```cpp
void SetStateTextColor(ControlStateType stateType, const std::wstring& dwTextColor)
```

 - 参&emsp;数：  
    - `stateType` 要设置的状态标志
    - `dwTextColor` 要设置的状态颜色字符串，该值必须在 global.xml 中存在
 - 返回值：无

### GetFont

获取当前字体编号

```cpp
int GetFont()
```

 - 参&emsp;数：无  
 - 返回值：返回字体编号，该编号在 global.xml 中标识

### SetFont

设置当前字体

```cpp
void SetFont(int index)
```

 - 参&emsp;数：  
    - `index` 要设置的字体编号，该编号必须在 global.xml 中存在
 - 返回值：无

### GetTextPadding

获取文字边距

```cpp
UiRect GetTextPadding()
```

 - 参&emsp;数：无  
 - 返回值：返回文字的边距信息

### SetTextPadding

设置文字边距信息

```cpp
void SetTextPadding(UiRect rc)
```

 - 参&emsp;数：  
    - `rc` 边距信息
 - 返回值：无

### IsSingleLine

判断是否是单行模式

```cpp
bool IsSingleLine()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示单行模式，否则为 false

### SetSingleLine

设置为单行输入模式

```cpp
void SetSingleLine(bool bSingleLine)
```

 - 参&emsp;数：  
    - `bSingleLine` 为 true 时为单行模式，否则为 false
 - 返回值：无

### IsLineLimit

是否限制整行输出

```cpp
bool IsLineLimit()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为限制，false 为不限制

### SetLineLimit

限制整行输出

```cpp
void SetLineLimit(bool bLineLimit)
```

 - 参&emsp;数：  
    - `bLineLimit` 设置 true 为限制，false 为不限制
 - 返回值：无