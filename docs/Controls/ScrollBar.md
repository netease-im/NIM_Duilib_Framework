# ScrollBar（滚动条）

`ScrollBar` 继承了 `Control` 控件属性和方法，更多可用属性和方法请参考：[Control](Control.md) 控件

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| button1normalimage |  | STRING | 左或上按钮普通状态图片 |
| button1hotimage |  | STRING | 左或上按钮鼠标悬浮状态图片 |
| button1pushedimage |  | STRING | 左或上按钮鼠标按下状态图片 |
| button1disabledimage |  | STRING | 左或上按钮禁用状态图片 |
| button2normalimage |  | STRING | 右或下按钮普通状态图片 |
| button2hotimage |  | STRING | 右或下按钮鼠标悬浮状态图片 |
| button2pushedimage |  | STRING | 右或下按钮鼠标按下状态图片 |
| button2disabledimage |  | STRING | 右或下按钮禁用状态图片 |
| thumbnormalimage |  | STRING | 滑块普通状态图片 |
| thumbhotimage |  | STRING | 滑块鼠标悬浮状态图片 |
| thumbpushedimage |  | STRING | 滑块鼠标按下状态图片 |
| thumbdisabledimage |  | STRING | 滑块禁用状态图片 |
| railnormalimage |  | STRING | 滑块中间标识普通状态图片 |
| railhotimage |  | STRING | 滑块中间标识鼠标悬浮状态图片 |
| railpushedimage |  | STRING | 滑块中间标识鼠标按下状态图片 |
| raildisabledimage |  | STRING | 滑块中间标识禁用状态图片 |
| bknormalimage |  | STRING | 背景普通状态图片 |
| bkhotimage |  | STRING | 背景鼠标悬浮状态图片 |
| bkpushedimage |  | STRING | 背景鼠标按下状态图片 |
| bkdisabledimage |  | STRING | 背景禁用状态图片 |
| hor | false | BOOL | 水平或垂直,如(true) |
| linesize | 8 | INT | 滚动一行的大小,如(8) |
| thumbminlength | 30 | INT | 滑块的最小长度 |
| range | 100 | INT | 滚动范围,如(100) |
| value | 0 | INT | 滚动位置,如(0) |
| showbutton1 | true | BOOL | 是否显示左或上按钮,如(true) |
| showbutton2 | true | BOOL | 是否显示右或下按钮,如(true) |
| autohidescroll | true | BOOL | 是否自动隐藏滚动条,如(true) |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [IsHorizontal](#IsHorizontal) | 是否是水平滚动条 |
| [SetHorizontal](#SetHorizontal) | 设置滚动条方向 |
| [GetScrollRange](#GetScrollRange) | 获取滚动范围，对应 XML 中 range 属性 |
| [SetScrollRange](#SetScrollRange) | 是设置滚动范围 |
| [IsValid](#IsValid) | 判断是否有效 |
| [GetScrollPos](#GetScrollPos) | 获取滚动条位置 |
| [SetScrollPos](#SetScrollPos) | 设置滚动条位置 |
| [GetLineSize](#GetLineSize) | 获取滚动一行的大小 |
| [SetLineSize](#SetLineSize) | 设置滚动一行的大小 |
| [GetThumbMinLength](#GetThumbMinLength) | 获取滑块的最小长度 |
| [SetThumbMinLength](#SetThumbMinLength) | 设置滑块的最小长度 |
| [IsShowButton1](#IsShowButton1) | 是否显示左或上按钮 |
| [SetShowButton1](#SetShowButton1) | 设置是否显示左或上按钮 |
| [GetButton1StateImage](#GetButton1StateImage) | 获取指定状态下的向左或向上按钮图片 |
| [SetButton1StateImage](#SetButton1StateImage) | 设置指定状态下的向左或向上按钮图片 |
| [IsShowButton2](#IsShowButton2) | 是否显示右或下按钮 |
| [SetShowButton2](#SetShowButton2) | 设置是否显示右或下按钮 |
| [GetButton2StateImage](#GetButton2StateImage) | 获取指定状态下的向右或向下按钮图片 |
| [SetButton2StateImage](#SetButton2StateImage) | 设置指定状态下的向右或向下按钮图片 |
| [GetThumbStateImage](#GetThumbStateImage) | 获取滑块指定状态下的图片 |
| [SetThumbStateImage](#SetThumbStateImage) | 设置滑块指定状态下的图片 |
| [GetRailStateImage](#GetRailStateImage) | 获取指定状态下滑块中间标识图片 |
| [SetRailStateImage](#SetRailStateImage) | 设置指定状态下滑块中间标识图片 |
| [GetBkStateImage](#GetBkStateImage) | 获取指定状态下的背景图片 |
| [SetBkStateImage](#SetBkStateImage) | 设置指定状态下的背景图片 |
| [IsAutoHideScroll](#IsAutoHideScroll) | 是否自动隐藏滚动条 |
| [SetAutoHideScroll](#SetAutoHideScroll) | 设置是否自动隐藏滚动条 |
| [GetThumbState](#GetThumbState) | 获取滑块状态 |


### IsHorizontal

是否是水平滚动条

```cpp
bool IsHorizontal()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 代表是水平滚动条，否则为 false 代表是垂直滚动条

### SetHorizontal

设置滚动条方向

```cpp
void SetHorizontal(bool bHorizontal = true)
```

 - 参&emsp;数：  
    - `bHorizontal` 设置 true 代表是水平滚动条，false 代表是垂直滚动条，默认为 true
 - 返回值：无

### GetScrollRange

获取滚动范围，对应 XML 中 range 属性

```cpp
int64_t GetScrollRange()
```

 - 参&emsp;数：无  
 - 返回值：返回滚动范围

### SetScrollRange

是设置滚动范围

```cpp
void SetScrollRange(int64_t nRange)
```

 - 参&emsp;数：  
    - `nRange` 要设置的滚动范围
 - 返回值：无

### IsValid

判断是否有效

```cpp
bool IsValid()
```

 - 参&emsp;数：无  
 - 返回值：true 为有效，否则为 false

### GetScrollPos

获取滚动条位置

```cpp
int64_t GetScrollPos()
```

 - 参&emsp;数：无  
 - 返回值：返回滚动条位置

### SetScrollPos

设置滚动条位置

```cpp
void SetScrollPos(int64_t nPos)
```

 - 参&emsp;数：  
    - `nPos` 要设置的位置信息
 - 返回值：无

### GetLineSize

获取滚动一行的大小

```cpp
int GetLineSize()
```

 - 参&emsp;数：无  
 - 返回值：返回滚动一行的大小

### SetLineSize

设置滚动一行的大小

```cpp
void SetLineSize(int nSize)
```

 - 参&emsp;数：  
    - `nSize` 要设置的大小数值
 - 返回值：无

### GetThumbMinLength

获取滑块的最小长度

```cpp
int GetThumbMinLength()
```

 - 参&emsp;数：无  
 - 返回值：返回滑块的最小长度

### SetThumbMinLength

设置滑块的最小长度

```cpp
void SetThumbMinLength(int nThumbMinLength)
```

 - 参&emsp;数：  
    - `nThumbMinLength` 要设置的最小长度
 - 返回值：无

### IsShowButton1

是否显示左或上按钮

```cpp
bool IsShowButton1()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为显示，false 为不显示

### SetShowButton1

设置是否显示左或上按钮

```cpp
void SetShowButton1(bool bShow)
```

 - 参&emsp;数：  
    - `bShow` 设置 true 为显示，false 为不显示
 - 返回值：无

### GetButton1StateImage

获取指定状态下的向左或向上按钮图片

```cpp
std::wstring GetButton1StateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取的状态，参考 Control枚举
 - 返回值：返回图片位置

### SetButton1StateImage

设置指定状态下的向左或向上按钮图片

```cpp
void SetButton1StateImage(ControlStateType stateType, const std::wstring& pStrImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置哪种状态下的图片
    - `pStrImage` 图片位置
 - 返回值：无

### IsShowButton2

是否显示右或下按钮

```cpp
bool IsShowButton2()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为显示，false 为不显示

### SetShowButton2

设置是否显示右或下按钮

```cpp
void SetShowButton2(bool bShow)
```

 - 参&emsp;数：  
    - `bShow` 设置 true 为显示，false 为不显示
 - 返回值：无

### GetButton2StateImage

获取指定状态下的向右或向下按钮图片

```cpp
std::wstring GetButton2StateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取的状态，参考 Control枚举
 - 返回值：返回图片位置

### SetButton2StateImage

设置指定状态下的向右或向下按钮图片

```cpp
void SetButton2StateImage(ControlStateType stateType, const std::wstring& pStrImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置哪种状态下的图片
    - `pStrImage` 图片位置
 - 返回值：无

### GetThumbStateImage

获取滑块指定状态下的图片

```cpp
std::wstring GetThumbStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取的状态标识，参考 Control枚举
 - 返回值：返回图片位置

### SetThumbStateImage

设置滑块指定状态下的图片

```cpp
void SetThumbStateImage(ControlStateType stateType, const std::wstring& pStrImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置的状态标识，参考 Control枚举
    - `pStrImage` 图片位置
 - 返回值：无

### GetRailStateImage

获取指定状态下滑块中间标识图片

```cpp
std::wstring GetRailStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取的状态标识，参考 Control枚举
 - 返回值：返回图片位置

### SetRailStateImage

设置指定状态下滑块中间标识图片

```cpp
void SetRailStateImage(ControlStateType stateType, const std::wstring& pStrImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置的状态标识，参考 Control枚举
    - `pStrImage` 图片位置
 - 返回值：无

### GetBkStateImage

获取指定状态下的背景图片

```cpp
std::wstring GetBkStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取的状态标识，参考 Control枚举
 - 返回值：返回图片位置

### SetBkStateImage

设置指定状态下的背景图片

```cpp
void SetBkStateImage(ControlStateType stateType, const std::wstring& pStrImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置的状态标识，参考 Control枚举
    - `pStrImage` 图片位置
 - 返回值：无

### IsAutoHideScroll

是否自动隐藏滚动条

```cpp
bool IsAutoHideScroll()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为是，否则为 false

### SetAutoHideScroll

设置是否自动隐藏滚动条

```cpp
void SetAutoHideScroll(bool hide)
```

 - 参&emsp;数：  
    - `hide` 设置为 true 为自动隐藏，false 为不自动隐藏
 - 返回值：无

### GetThumbState

获取滑块状态

```cpp
ControlStateType GetThumbState()
```

 - 参&emsp;数：无  
 - 返回值：返回滑块状态，参考 ControlStateType 枚举