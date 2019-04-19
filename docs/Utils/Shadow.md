# Shadow（阴影）

| 方法名称 | 用途 |
| :--- | :--- |
| [SetShadowAttached](#SetShadowAttached) | 设置是否附加阴影效果 |
| [IsShadowAttached](#IsShadowAttached) | 判断是否已经附加了阴影 |
| [SetShadowCorner](#SetShadowCorner) | 设置阴影的九宫格属性 |
| [GetShadowCorner](#GetShadowCorner) | 获取阴影的九宫格属性 |
| [ResetShadowBox](#ResetShadowBox) | 根据初始化的阴影范围重置阴影边框 |
| [SetShadowImage](#SetShadowImage) | 设置阴影图片 |
| [GetShadowImage](#GetShadowImage) | 获取阴影图片位置 |
| [AttachShadow](#AttachShadow) | 将阴影附加到窗口 |
| [MaximizedOrRestored](#MaximizedOrRestored) | 设置窗口最大化还是还原状态 |
| [GetRoot](#GetRoot) | 获取附加阴影后的容器指针 |
| [ClearImageCache](#ClearImageCache) | 清理图片缓存 |
| [SetPadding](#SetPadding) | 设置阴影内边距 |

### SetShadowAttached

设置是否附加阴影效果

```cpp
void SetShadowAttached(bool bShadowAttached)
```

 - 参&emsp;数：  
    - `bShadowAttached` 设置 true 为是，false 为否
 - 返回值：无

### IsShadowAttached

判断是否已经附加了阴影

```cpp
bool IsShadowAttached()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示已经附加，否则为 false

### SetShadowCorner

设置应用的九宫格属性

```cpp
void SetShadowCorner(const UiRect &rect)
```

 - 参&emsp;数：  
    - `rect` 要设置的九宫格属性
 - 返回值：无

### GetShadowCorner

获取阴影的九宫格属性

```cpp
UiRect GetShadowCorner()
```

 - 参&emsp;数：无  
 - 返回值：返回阴影的九宫格属性

### ResetShadowBox

根据初始化的阴影范围重置阴影边框

```cpp
void ResetShadowBox()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetShadowImage

设置阴影图片

```cpp
void SetShadowImage(const std::wstring &image)
```

 - 参&emsp;数：  
    - `image` 图片路径
 - 返回值：无

### GetShadowImage

获取阴影图片位置

```cpp
std::wstring GetShadowImage()
```

 - 参&emsp;数：无  
 - 返回值：返回阴影图片位置

### AttachShadow

将阴影附加到窗口

```cpp
Box* AttachShadow(Box* pRoot)
```

 - 参&emsp;数：  
    - `pRoot` 窗口的顶层容器
 - 返回值：返回附加阴影后的容器指针

### MaximizedOrRestored

设置窗口最大化还是还原状态

```cpp
void MaximizedOrRestored(bool isMaximized)
```

 - 参&emsp;数：  
    - `isMaximized` 设置为 true 表示最大化，false 为还原初始状态
 - 返回值：无

### GetRoot

获取附加阴影后的容器指针

```cpp
Control* GetRoot()
```

 - 参&emsp;数：无  
 - 返回值：返回容器指针

### ClearImageCache

清理图片缓存

```cpp
void ClearImageCache()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetPadding

设置阴影内边距

```cpp
void SetPadding()
```

 - 参&emsp;数：无  
 - 返回值：无

