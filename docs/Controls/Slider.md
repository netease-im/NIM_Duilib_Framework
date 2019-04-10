# Slider（滑动条）

`Slider` 继承了 `Progress` 控件属性和方法，更多可用属性和方法请参考：[Progress](Progress.md) 控件

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| thumbnormalimage |  | STRING | 拖动滑块普通状态图片 |
| thumbhotimage |  | STRING | 拖动滑块鼠标悬浮状态图片 |
| thumbpushedimage |  | STRING | 拖动滑块鼠标按下状态图片 |
| thumbdisabledimage |  | STRING | 拖动滑块鼠标禁用状态图片 |
| thumbsize | 10,10 | SIZE | 拖动滑块大小,如(10,10) |
| step | 1 | INT | 进度步长,如(1) |
| progressbarpadding | 0,0,0,0 | RECT | 滑动条绘制时缩小的内边距 |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [GetChangeStep](#GetChangeStep) | 获取步进步长 |
| [SetChangeStep](#SetChangeStep) | 设置步进步长 |
| [SetThumbSize](#SetThumbSize) | 设置多动滑块的大小 |
| [GetThumbRect](#GetThumbRect) | 获取滑块的矩形信息 |
| [GetThumbStateImage](#GetThumbStateImage) | 获取指定状态下滑块的图片 |
| [SetThumbStateImage](#SetThumbStateImage) | 设置指定状态下滑块的图片 |
| [GetProgressBarPadding](#GetProgressBarPadding) | 获取进度条内边距 |
| [SetProgressBarPadding](#SetProgressBarPadding) | 设置进度条内边距 |
| [AttachValueChange](#AttachValueChange) | 监听进度条进度改变事件 |


### GetChangeStep

获取步进步长

```cpp
int GetChangeStep()
```

 - 参&emsp;数：无  
 - 返回值：返回步进步长

### SetChangeStep

设置步进步长

```cpp
void SetChangeStep(int step)
```

 - 参&emsp;数：  
    - `step` 要设置的步长
 - 返回值：无

### SetThumbSize

设置多动滑块的大小

```cpp
void SetThumbSize(CSize szXY)
```

 - 参&emsp;数：  
    - `szXY` 要设置的大小
 - 返回值：无

### GetThumbRect

获取滑块的矩形信息

```cpp
UiRect GetThumbRect()
```

 - 参&emsp;数：无  
 - 返回值：返回滑块的矩形信息

### GetThumbStateImage

获取指定状态下滑块的图片

```cpp
std::wstring GetThumbStateImage(ControlStateType stateType)
```

 - 参&emsp;数：  
    - `stateType` 要获取的状态标识，参考 Control枚举
 - 返回值：返回图片路径

### SetThumbStateImage

设置指定状态下滑块的图片

```cpp
void SetThumbStateImage(ControlStateType stateType, const std::wstring& pStrImage)
```

 - 参&emsp;数：  
    - `stateType` 要设置的状态标识，参考 Control枚举
    - `pStrImage` 要设置的图片位置
 - 返回值：无

### GetProgressBarPadding

获取进度条内边距

```cpp
UiRect GetProgressBarPadding()
```

 - 参&emsp;数：无  
 - 返回值：返回内边距信息

### SetProgressBarPadding

设置进度条内边距

```cpp
void SetProgressBarPadding(UiRect rc)
```

 - 参&emsp;数：  
    - `rc` 要设置的边距信息
 - 返回值：无

### AttachValueChange

监听进度条进度改变事件

```cpp
void AttachValueChange(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 进度条进度改变后调用的回调函数
 - 返回值：无