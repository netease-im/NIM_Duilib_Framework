# Progress（进度条）

`Progress` 继承了 `Label` 控件属性和方法，更多可用属性和方法请参考：[Label](Label.md) 控件

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| hor | true | BOOL | 水平或垂直,如(true) |
| min | 0 | INT | 进度最小值,如(0) |
| max | 100 | INT | 进度最大值,如(100) |
| value | 0 | INT | 进度值,如(50) |
| isstretchfore | TRUE | BOOL | 指定进度条前景图片是否缩放显示 |
| progresscolor |  | STRING | 进度条前景颜色,不指定则使用默认颜色,如(blue) |
| progressimage |  | STRING | 进度条前景图片 |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [IsHorizontal](#IsHorizontal) | 判断是否是水平进度条 |
| [SetHorizontal](#SetHorizontal) | 设置水平或垂直滚动条 |
| [GetMinValue](#GetMinValue) | 获取进度条最小值 |
| [SetMinValue](#SetMinValue) | 设置进度条最小值 |
| [GetMaxValue](#GetMaxValue) | 获取进度条最大值 |
| [SetMaxValue](#SetMaxValue) | 设置进度条最大值 |
| [GetValue](#GetValue) | 获取当前进度百分比 |
| [SetValue](#SetValue) | 设置当前进度百分比 |
| [IsStretchForeImage](#IsStretchForeImage) | 进度条前景图片是否缩放显示 |
| [SetStretchForeImage](#SetStretchForeImage) | 设置进度条前景图片是否缩放显示 |
| [GetProgressImage](#GetProgressImage) | 获取滚动条背景图片 |
| [SetProgressImage](#SetProgressImage) | 设置进度条背景图片 |
| [GetProgressColor](#GetProgressColor) | 获取进度条背景颜色 |
| [SetProgressColor](#SetProgressColor) | 设置进度条背景颜色 |
| [GetProgressPos](#GetProgressPos) | 获取进度条位置 |


### IsHorizontal

判断是否是水平进度条

```cpp
bool IsHorizontal()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 是水平进度条，false 为垂直滚动条

### SetHorizontal

设置水平或垂直滚动条

```cpp
void SetHorizontal(bool bHorizontal = true)
```

 - 参&emsp;数：  
    - `bHorizontal` 为 true 时设置为水平滚动条，false 时设置为垂直滚动条，默认为 true
 - 返回值：无

### GetMinValue

获取进度条最小值

```cpp
int GetMinValue()
```

 - 参&emsp;数：无  
 - 返回值：返回进度条最小值

### SetMinValue

设置进度条最小值

```cpp
void SetMinValue(int nMin)
```

 - 参&emsp;数：  
    - `nMin` 最小值数值
 - 返回值：无

### GetMaxValue

获取进度条最大值

```cpp
int GetMaxValue()
```

 - 参&emsp;数：无  
 - 返回值：返回进度条最大值

### SetMaxValue

设置进度条最大值

```cpp
void SetMaxValue(int nMax)
```

 - 参&emsp;数：  
    - `nMax` 要设置的最大值
 - 返回值：无

### GetValue

获取当前进度百分比

```cpp
double GetValue()
```

 - 参&emsp;数：无  
 - 返回值：返回当前进度百分比

### SetValue

设置当前进度百分比

```cpp
void SetValue(double nValue)
```

 - 参&emsp;数：  
    - `nValue` 要设置的百分比数值
 - 返回值：无

### IsStretchForeImage

进度条前景图片是否缩放显示

```cpp
bool IsStretchForeImage()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为缩放显示，false 为不缩放显示

### SetStretchForeImage

设置进度条前景图片是否缩放显示

```cpp
void SetStretchForeImage(bool bStretchForeImage = true)
```

 - 参&emsp;数：  
    - `bStretchForeImage` true 为缩放显示，false 为不缩放显示
 - 返回值：无

### GetProgressImage

获取滚动条背景图片

```cpp
std::wstring GetProgressImage()
```

 - 参&emsp;数：无  
 - 返回值：返回背景图片位置

### SetProgressImage

设置进度条背景图片

```cpp
void SetProgressImage(const std::wstring& strImage)
```

 - 参&emsp;数：  
    - `strImage` 图片地址
 - 返回值：无

### GetProgressColor

获取进度条背景颜色

```cpp
std::wstring GetProgressColor()
```

 - 参&emsp;数：无  
 - 返回值：返回背景颜色的字符串值，对应 global.xml 中的指定色值

### SetProgressColor

设置进度条背景颜色

```cpp
void SetProgressColor(const std::wstring& strProgressColor)
```

 - 参&emsp;数：  
    - `strProgressColor` 要设置的背景颜色字符串，该字符串必须在 global.xml 中存在
 - 返回值：无

### GetProgressPos

获取进度条位置

```cpp
virtual UiRect GetProgressPos()
```

 - 参&emsp;数：无  
 - 返回值：返回进度条当前位置
