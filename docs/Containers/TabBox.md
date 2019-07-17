# TabBox（多标签布局容器）

`TabBox` 继承了 `Box` 容器属性和方法，更多可用属性和方法请参考：[Box](Box.md) 容器

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| fadeswitch | false | BOOL | 当切换页面时是否使用动画效果,如(false) |
| selectedid | 0 | INT | 默认选中的页面id,如(0) |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [GetCurSel](#GetCurSel) | 获取当前选择项 |
| [SelectItem](#SelectItem) | 根据控件索引选择一个子项 |
| [SelectItem](#SelectItem) | 根据控件指针选择一个子项 |
| [SelectItem](#SelectItem) | 根据控件名称选择一个子项 |
| [SetFadeSwitch](#SetFadeSwitch) | 设置是否显示动画效果 |
| [IsFadeSwitch](#IsFadeSwitch) | 判断是否需要显示动画效果 |
| [ShowTabItem](#ShowTabItem) | 显示一个 TAB 项 |
| [HideTabItem](#HideTabItem) | 隐藏一个 TAB 项 |

### GetCurSel

获取当前选择项

```cpp
int GetCurSel()
```

 - 参&emsp;数：无  
 - 返回值：返回当前选择项索引

### SelectItem

根据控件索引选择一个子项

```cpp
bool SelectItem(int iIndex)
```

 - 参&emsp;数：  
    - `iIndex` 要选择的子项索引
 - 返回值：成功返回 true，否则返回 false

### SelectItem

根据控件指针选择一个子项

```cpp
bool SelectItem(Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 要选择的子项指针
 - 返回值：成功返回 true，否则返回 false

### SelectItem

根据控件名称选择一个子项

```cpp
bool SelectItem(const std::wstring& pControlName)
```

 - 参&emsp;数：  
    - `pControlName` 要选择的控件名称
 - 返回值：成功返回 true，否则返回 false

### SetFadeSwitch

设置是否显示动画效果

```cpp
void SetFadeSwitch(bool bFadeSwitch)
```

 - 参&emsp;数：  
    - `bFadeSwitch` 设置为 true 则显示，false 为不显示动画
 - 返回值：无

### IsFadeSwitch

判断是否需要显示动画效果

```cpp
bool IsFadeSwitch()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 显示动画，false 为不显示动画效果

### ShowTabItem

显示一个 TAB 项

```cpp
void ShowTabItem(std::size_t it)
```

 - 参&emsp;数：  
    - `it` TAB 项索引
 - 返回值：无

### HideTabItem

隐藏一个 TAB 项

```cpp
void HideTabItem(std::size_t it)
```

 - 参&emsp;数：  
    - `it` TAB 项索引
 - 返回值：无