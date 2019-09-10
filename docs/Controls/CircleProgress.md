# CircleProgress（环形进度条）

`CircleProgress` 继承了 `Progress` 控件属性和方法，更多可用属性和方法请参考：[Progress](Progress.md) 控件

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| circlewidth | 1 | INT | 环形进度条的宽度，如(10) |
| indicator |  | STRING | 设置进度指示移动图标 |
| clockwise | true | BOOL |设置递增方向 |
| bgcolor |  | STRING | 设置进度条背景颜色 |
| fgcolor |  | STRING | 设置进度条背前景色 |
| gradientcolor |  | STRING | 设置进度条前景渐变颜色，与 fgcolor 同时使用，可以不设置则无渐变效果 |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [SetCircular](#SetCircular) | 设置环形进度条样式，默认为普通进度条 |
| [SetClockwiseRotation](#SetClockwiseRotation) | 设置递增方向 |
| [SetCircleWidth](#SetCircleWidth) | 设置圆环宽度 |
| [SetBackgroudColor](#SetBackgroudColor) | 设置进度条背景颜色 |
| [SetForegroudColor](#SetForegroudColor) | 设置进度条前景颜色 |
| [SetCircleGradientColor](#SetCircleGradientColor) | 设置进度条前景渐变颜色，与 SetForegroudColor 同时使用，可以不设置则无渐变效果 |
| [SetIndicator](#SetIndicator) | 设置进度指示移动图标 |


### SetCircular

设置环形进度条，默认为普通进度条

```cpp
void SetCircular(bool bCircular = true)
```

 - 参&emsp;数：  
    - `bCircular` bCircular 为 true 时设置为环形进度条，false 时设置为父级滚动条样式，默认为 true
 - 返回值：无

### SetClockwiseRotation

设置递增方向

```cpp
void SetClockwiseRotation(bool bClockwise = true)
```

 - 参&emsp;数：  
    - `bClockwise` bClockwise 为 true 时设置为顺时针，false 时设置为逆时针，默认为 true
 - 返回值：无

### SetCircleWidth

设置圆环宽度

```cpp
void SetCircleWidth(int nCircleWidth)
```

 - 参&emsp;数：  
    - `nCircleWidth` nCircleWidth 宽度数值
 - 返回值：无

### SetBackgroudColor

设置进度条背景颜色

```cpp
void SetBackgroudColor(const std::wstring& strColor)
```

 - 参&emsp;数：  
    - `strColor` strColor要设置的背景颜色字符串，该字符串必须在 global.xml 中存在
 - 返回值：无

### SetForegroudColor

设置进度条前景颜色

```cpp
void SetForegroudColor(const std::wstring& strColor)
```

 - 参&emsp;数：  
    - `strColor` strColor要设置的前景颜色字符串，该字符串必须在 global.xml 中存在
 - 返回值：无

### SetCircleGradientColor

设置进度条前景渐变颜色，与 SetForegroudColor 同时使用，可以不设置,则无渐变效果

```cpp
void SetCircleGradientColor(const std::wstring& strColor)
```

 - 参&emsp;数：  
    - `strColor` strColor要设置的前景渐变颜色字符串，该字符串必须在 global.xml 中存在
 - 返回值：无

### SetIndicator

设置进度指示移动图标

```cpp
void SetIndicator(const std::wstring& sIndicatorImage);
```

 - 参&emsp;数：  
    - `sIndicatorImage` sIndicatorImage 要设置的图片
 - 返回值：无
