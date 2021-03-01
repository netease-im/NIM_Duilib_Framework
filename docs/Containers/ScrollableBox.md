# ScrollableBox（可滚动布局容器）

`ScrollableBox` 继承了 `Box` 容器属性和方法，更多可用属性和方法请参考：[Box](Box.md) 容器。

注意：此控件不能在 XML 中直接使用，仅作为可滚动布局容器的基类，此处仅记录继承该容器的其他容器所拥有的公共属性和方法。请参考使用 [ListBox](../List/ListBox.md)。

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| vscrollbar | false | BOOL | 是否使用竖向滚动条,如(true) |
| hscrollbar | false | BOOL | 是否使用横向滚动条,如(true) |
| vscrollbarstyle |  | STRING | 设置本容器的纵向滚动条的样式 |
| hscrollbarstyle |  | STRING | 设置本容器的横向滚动条的样式 |
| scrollbarpadding | 0,0,0,0 | RECT | 滚动条的外边距,可以让滚动条不占满容器,如(2,2,2,2) |
| vscrollunit | 30 | INT | 容器的纵向滚动条滚动步长,0代表使用默认步长 |
| scrollbarfloat | true | BOOL | 容器的滚动条是否悬浮在子控件上面,如(true) |
| defaultdisplayscrollbar | true | BOOL | 暂时无用,未开发 |
| holdend | false | BOOL | 是否一直保持显示末尾位置,如(true) |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [GetScrollPos](#GetScrollPos) | 获取滚动条位置 |
| [GetScrollRange](#GetScrollRange) | 获取滚动条的范围 |
| [SetScrollPos](#SetScrollPos) | 设置滚动条位置 |
| [SetScrollPosY](#SetScrollPosY) | 设置滚动条 Y 轴坐标 |
| [SetScrollPosX](#SetScrollPosX) | 设置滚动条 X 轴坐标 |
| [LineUp](#LineUp) | 向上滚动滚动条 |
| [LineDown](#LineDown) | 向下滚动滚动条 |
| [LineLeft](#LineLeft) | 向左滚动 |
| [LineRight](#LineRight) | 向右滚动 |
| [PageUp](#PageUp) | 向上滚动一个页面大小的距离 |
| [PageDown](#PageDown) | 向下滚动一个页面大小的距离 |
| [HomeUp](#HomeUp) | 回到滚动条最上方 |
| [EndDown](#EndDown) | 滚动到最下方位置 |
| [PageLeft](#PageLeft) | 向左滚动一个页面大小的距离 |
| [PageRight](#PageRight) | 向右滚动一个页面大小的距离 |
| [HomeLeft](#HomeLeft) | 滚动到最左侧 |
| [EndRight](#EndRight) | 滚动到最右侧 |
| [TouchUp](#TouchUp) | 触摸向上滚动（响应 WM_TOUCH 消息） |
| [TouchDown](#TouchDown) | 触摸向下滚动（响应 WM_TOUCH 消息） |
| [EnableScrollBar](#EnableScrollBar) | 启用滚动条 |
| [GetVerticalScrollBar](#GetVerticalScrollBar) | 获取垂直滚动条对象指针 |
| [GetHorizontalScrollBar](#GetHorizontalScrollBar) | 获取水平滚动条对象指针 |
| [ProcessVScrollBar](#ProcessVScrollBar) | 待补充 |
| [ProcessHScrollBar](#ProcessHScrollBar) | 待补充 |
| [IsVScrollBarValid](#IsVScrollBarValid) | 判断垂直滚动条是否有效 |
| [IsHScrollBarValid](#IsHScrollBarValid) | 判断水平滚动条是否有效 |
| [ReomveLastItemAnimation](#ReomveLastItemAnimation) | 待补充 |
| [PlayRenderOffsetYAnimation](#PlayRenderOffsetYAnimation) | 待补充 |
| [IsAtEnd](#IsAtEnd) | 是否已经在底部 |
| [IsHoldEnd](#IsHoldEnd) | 是否锁定到底部 |
| [SetHoldEnd](#SetHoldEnd) | 设置滚动条是否始终锁定到底部位置 |
| [GetVerScrollUnitPixels](#GetVerScrollUnitPixels) | 获取垂直滚动条滚动步长 |
| [SetVerScrollUnitPixels](#SetVerScrollUnitPixels) | 设置垂直滚动条滚动步长 |
| [GetScrollBarFloat](#GetScrollBarFloat) | 获取容器的滚动条是否悬浮在子控件上面 |
| [SetScrollBarFloat](#SetScrollBarFloat) | 设置容器的滚动条是否悬浮在子控件上面 |
| [GetScrollBarPadding](#GetScrollBarPadding) | 获取滚动条的外边距 |
| [SetScrollBarPadding](#SetScrollBarPadding) | 设置滚动条的外边距，可以让滚动条不占满容器 |
| [GetDefaultDisplayScrollbar](#GetDefaultDisplayScrollbar) | 待补充 |
| [SetDefaultDisplayScrollbar](#SetDefaultDisplayScrollbar) | 待补充 |
| [AttachScrollChange](#AttachScrollChange) | 监听滚动条位置变化事件 |
| [CalcRequiredSize](#CalcRequiredSize) | 计算所需的尺寸 |
| [LoadImageCache](#LoadImageCache) | 加载图片缓存，仅供 ScrollableBox 内部使用 |
| [SetPosInternally](#SetPosInternally) | 待补充 |

## GetScrollPos

获取滚动条位置

```cpp
virtual CSize GetScrollPos()
```

 - 返回值：返回滚动条的位置信息

## GetScrollRange

获取滚动条的范围

```cpp
virtual CSize GetScrollRange()
```

 - 返回值：返回滚动条的范围信息

## SetScrollPos

设置滚动条位置

```cpp
virtual void SetScrollPos(CSize szPos)
```

 - 参数：  
    - `szPos` 要设置的位置数据
 - 返回值：无

## SetScrollPosY

设置滚动条 Y 轴坐标

```cpp
virtual void SetScrollPosY(int y)
```

 - 参数：  
    - `y` 要设置的 y 轴坐标数值
 - 返回值：无
 
## SetScrollPosX

设置滚动条 X 轴坐标

```cpp
virtual void SetScrollPosX(int x)
```

 - 参数：  
    - `x` 要设置的 x 轴坐标数值
 - 返回值：无

## LineUp

向上滚动滚动条

```cpp
virtual void LineUp(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true)
```

 - 参数：  
    - `deltaValue` 滚动距离，默认为 DUI_NOSET_VALUE
    - `withAnimation` 是否附带动画效果，默认为 true
 - 返回值：无

## LineDown

向下滚动滚动条

```cpp
virtual void LineDown(int detaValue = DUI_NOSET_VALUE, bool withAnimation = true)
```

 - 参数：  
    - `deltaValue` 滚动距离，默认为 DUI_NOSET_VALUE
    - `withAnimation` 是否附带动画效果，默认为 true
 - 返回值：无

## LineLeft

向左滚动

```cpp
virtual void LineLeft(int detaValue = DUI_NOSET_VALUE)
```
 - 参数：  
    - `deltaValue` 滚动距离，默认为 DUI_NOSET_VALUE
 - 返回值：无

## LineRight

向右滚动

```cpp
virtual void LineRight(int detaValue = DUI_NOSET_VALUE)
```
 - 参数：  
    - `deltaValue` 滚动距离，默认为 DUI_NOSET_VALUE
 - 返回值：无

## PageUp

向上滚动一个页面大小的距离

```cpp
virtual void PageUp()
```

 - 返回值：无

## PageDown

向下滚动一个页面大小的距离

```cpp
virtual void PageDown()
```

 - 返回值：无

## HomeUp

回到滚动条最上方

```cpp
virtual void HomeUp()
```

 - 返回值：无

## EndDown

滚动到最下方位置

```cpp
virtual void EndDown(bool arrange = true, bool withAnimation = true)
```

 - 参数：  
    - `arrange` 是否重置滚动条位置，默认为 true
    - `withAnimation` 是否包含动画特效，默认为 true
 - 返回值：无

## PageLeft

向左滚动一个页面大小的距离

```cpp
virtual void PageLeft()
```

 - 返回值：无

## PageRight

向右滚动一个页面大小的距离

```cpp
virtual void PageRight()
```

 - 返回值：无

## HomeLeft

滚动到最左侧

```cpp
virtual void HomeLeft()
```

 - 返回值：无

## EndRight

滚动到最右侧

```cpp
virtual void EndRight()
```

 - 返回值：无

## TouchUp

触摸向上滚动（响应 WM_TOUCH 消息）

```cpp
virtual void TouchUp(int deltaValue)
```

 - 参数：  
    - `deltaValue` 滚动距离
 - 返回值：无

## TouchDown

触摸向下滚动（响应 WM_TOUCH 消息）

```cpp
virtual void TouchDown(int deltaValue)
```

 - 参数：  
    - `deltaValue` 滚动距离
 - 返回值：无

## EnableScrollBar

启用滚动条

```cpp
virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false)
```

 - 参数：  
    - `bEnableVertical` 是否启用垂直滚动条，默认为 true
    - `bEnableHorizontal` 是否启用水平滚动条，默认为 true
 - 返回值：无

## GetVerticalScrollBar

获取垂直滚动条对象指针

```cpp
virtual ScrollBar* GetVerticalScrollBar()
```

 - 返回值：返回垂直滚动条对象指针

## GetHorizontalScrollBar

获取水平滚动条对象指针

```cpp
virtual ScrollBar* GetHorizontalScrollBar()
```

 - 返回值：返回水平滚动条对象指针

## ProcessVScrollBar

待补充

```cpp
virtual void ProcessVScrollBar(UiRect rc, int cyRequired)
```

 - 参数：  
    - `待补充
 - 返回值：待补充

## ProcessHScrollBar

待补充

```cpp
virtual void ProcessHScrollBar(UiRect rc, int cxRequired)
```

 - 参数：  
    - `待补充
 - 返回值：待补充

## IsVScrollBarValid

判断垂直滚动条是否有效

```cpp
bool IsVScrollBarValid()
```

 - 返回值：返回 true 表示有效，否则 false 为无效

## IsHScrollBarValid

判断水平滚动条是否有效

```cpp
bool IsHScrollBarValid()
```

 - 返回值：返回 true 表示有效，否则 false 为无效

## ReomveLastItemAnimation

待补充

```cpp
void ReomveLastItemAnimation()
```

 - 参数：  
    - `待补充
 - 返回值：待补充

## PlayRenderOffsetYAnimation

待补充

```cpp
void PlayRenderOffsetYAnimation(int nRenderY)
```

 - 参数：  
    - `待补充
 - 返回值：待补充

## IsAtEnd

是否已经在底部

```cpp
bool IsAtEnd()
```

 - 返回值：返回 true 表示已经在底部，否则为 false

## IsHoldEnd

是否锁定到底部

```cpp
bool IsHoldEnd()
```

 - 返回值：回 true 表示锁定在底部，否则为 false

## SetHoldEnd

设置滚动条是否始终锁定到底部位置

```cpp
void SetHoldEnd(bool bHoldEnd)
```

 - 参数：  
    - `bHoldEnd` 设置 true 表示锁定，false 为不锁定
 - 返回值：无

## GetVerScrollUnitPixels

获取垂直滚动条滚动步长

```cpp
int GetVerScrollUnitPixels()
```

 - 返回值：返回滚动步长

## SetVerScrollUnitPixels

设置垂直滚动条滚动步长

```cpp
void SetVerScrollUnitPixels(int nUnitPixels)
```

 - 参数：  
    - `nUnitPixels` 要设置的步长
 - 返回值：无

## GetScrollBarFloat

获取容器的滚动条是否悬浮在子控件上面

```cpp
bool GetScrollBarFloat()
```

 - 返回值：返回 true 表示悬浮在滚动条上，否则为 false

## SetScrollBarFloat

设置容器的滚动条是否悬浮在子控件上面

```cpp
void SetScrollBarFloat(bool bScrollBarFloat)
```

 - 参数：  
    - `bScrollBarFloat` true 表示悬浮在滚动条上，false 表示不悬浮在控件上
 - 返回值：无

## GetScrollBarPadding

获取滚动条的外边距

```cpp
UiRect GetScrollBarPadding()
```

 - 返回值：返回边距信息 

## SetScrollBarPadding

设置滚动条的外边距，可以让滚动条不占满容器

```cpp
void SetScrollBarPadding(UiRect rcScrollBarPadding)
```

 - 参数：  
    - `rcScrollBarPadding` 要设置的边距
 - 返回值：无

## GetDefaultDisplayScrollbar

待补充

```cpp
bool GetDefaultDisplayScrollbar()
```

 - 参数：  
    - `待补充
 - 返回值：待补充

## SetDefaultDisplayScrollbar

待补充

```cpp
void SetDefaultDisplayScrollbar(bool bDefaultDisplay)
```

 - 参数：  
    - `待补充
 - 返回值：待补充

## AttachScrollChange

监听滚动条位置变化事件

```cpp
void AttachScrollChange(const EventCallback& callback)
```

 - 参数：  
    - `callback` 有变化后通知的回调函数
 - 返回值：无

## CalcRequiredSize

计算所需的尺寸

```cpp
virtual CSize CalcRequiredSize(const UiRect& rc)
```

 - 参数：  
    - `rc` 当前位置信息
 - 返回值：返回所需尺寸大小

## LoadImageCache

加载图片缓存，仅供 ScrollableBox 内部使用

```cpp
void LoadImageCache(bool bFromTopLeft)
```

 - 参数：  
    - `bFromTopLeft` 暂无意义
 - 返回值：无

## SetPosInternally

待补充

```cpp
void SetPosInternally(UiRect rc)
```

 - 参数：  
    - `待补充
 - 返回值：待补充
