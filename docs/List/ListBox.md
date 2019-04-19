# ListBox

`ListBox` 继承了 `ScrollableBox` 容器属性和方法，更多可用属性和方法请参考：[ScrollableBox](../Containers/ScrollableBox.md) 容器

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| scrollselect | false | BOOL | 是否随滚动改变选中项,如(false) |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [ScrollItemToTop](#ScrollItemToTop) | 滚动到指定子项位置 |
| [GetTopItem](#GetTopItem) | 获取当前位置第一个子项 |
| [SetItemIndex](#SetItemIndex) | 设置子项的位置索引 |
| [Previous](#Previous) | 选中上一项 |
| [Next](#Next) | 选中下一项 |
| [ActiveItem](#ActiveItem) | 触发选中项的双击事件 |
| [Add](#Add) | 追加一个子项到末尾 |
| [AddAt](#AddAt) | 在指定位置之后插入一个子项 |
| [Remove](#Remove) | 根据子项指针 |
| [RemoveAt](#RemoveAt) | 根据索引移除一个子项 |
| [RemoveAll](#RemoveAll) | 移除所有子项 |
| [SortItems](#SortItems) | 排列子项 |
| [ItemComareFunc](#ItemComareFunc) | 默认的子项对比方法 |
| [GetScrollSelect](#GetScrollSelect) | 获取是否随滚动改变选中项设置 |
| [SetScrollSelect](#SetScrollSelect) | 设置是否随滚动改变选中项设置 |
| [AttachSelect](#AttachSelect) | 监听选择子项的事件 |
| [SelectNextWhenActiveRemoved](#SelectNextWhenActiveRemoved) | 在移除一个子项后自动选择下一项 |


### ScrollItemToTop

滚动到指定子项位置

```cpp
virtual bool ScrollItemToTop(const std::wstring& strItemName)
```

 - 参&emsp;数：  
    - `strItemName` 子项名称
 - 返回值：成功返回 true，否则为 false，可能控件不存在

### GetTopItem

获取当前位置第一个子项

```cpp
virtual Control* GetTopItem()
```

 - 参&emsp;数：无  
 - 返回值：返回第一个子项指针

### SetItemIndex

设置子项的位置索引

```cpp
bool SetItemIndex(Control* pControl, std::size_t iIndex)
```

 - 参&emsp;数：  
    - `pControl` 子项指针
    - `iIndex` 索引号
 - 返回值：成功返回 true，否则返回 false

### Previous

选中上一项

```cpp
void Previous()
```

 - 参&emsp;数：无  
 - 返回值：无

### Next

选中下一项

```cpp
void Next()
```

 - 参&emsp;数：无  
 - 返回值：无

### ActiveItem

触发选中项的双击事件

```cpp
void ActiveItem()
```

 - 参&emsp;数：无  
 - 返回值：无

### Add

追加一个子项到末尾

```cpp
bool Add(Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 子项指针
 - 返回值：成功返回 true，失败返回 false

### AddAt

在指定位置之后插入一个子项

```cpp
bool AddAt(Control* pControl, int iIndex)
```

 - 参&emsp;数：  
    - `pControl` 子项指针
    - `iIndex` 要插入的位置索引
 - 返回值：成功返回 true，失败返回 false

### Remove

根据子项指针

```cpp
bool Remove(Control* pControl)
```

 - 参&emsp;数：  
    - `pControl` 子项指针
 - 返回值：成功返回 true，失败返回 false

### RemoveAt

根据索引移除一个子项

```cpp
bool RemoveAt(int iIndex)
```

 - 参&emsp;数：  
    - `iIndex` 子项索引
 - 返回值：成功返回 true，失败返回 false

### RemoveAll

移除所有子项

```cpp
void RemoveAll()
```

 - 参&emsp;数：无  
 - 返回值：无

### SortItems

排列子项

```cpp
bool SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData)
```

 - 参&emsp;数：  
    - `pfnCompare` 自定义排序函数
    - `dwData` 用于子项对比时的数据
 - 返回值：成功返回 true，失败返回 false

### ItemComareFunc

默认的子项对比方法

```cpp
static int __cdecl ItemComareFunc(void *pvlocale, const void *item1, const void *item2)
```

 - 参&emsp;数：  
    - `pvlocale` 保存 List 指针
    - `item1` 子项1
    - `item2` 子项2
 - 返回值：返回对比结果

### GetScrollSelect

获取是否随滚动改变选中项设置

```cpp
bool GetScrollSelect()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示跟随滚动条改变选择项，否则为 false

### SetScrollSelect

设置是否随滚动改变选中项设置

```cpp
void SetScrollSelect(bool bScrollSelect)
```

 - 参&emsp;数：  
    - `bScrollSelect` 为 true 是为跟随滚动条改变选中项，false 为不跟随
 - 返回值：无

### AttachSelect

监听选择子项的事件

```cpp
void AttachSelect(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 选择子项时的回调函数
 - 返回值：无

### SelectNextWhenActiveRemoved

在移除一个子项后自动选择下一项

```cpp
void SelectNextWhenActiveRemoved(bool bSelectNextItem)
```

 - 参&emsp;数：  
    - `bSelectNextItem` 为 true 时自动选择下一项，false 为不自动选择
 - 返回值：无
