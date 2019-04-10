# Combo

`Combo` 继承了 `Box` 容器属性和方法，更多可用属性和方法请参考：[Box](../Containers/Box.md) 容器

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| dropbox |  | STRING | 设置弹出框的样式,如(padding=&quot;1,1,1,1&quot;) |
| dropboxsize | 0,150 | SIZE | 弹出框大小设置 |
| vscrollbar | false | BOOL | 是否使用竖向滚动条,如(true) |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [GetText](#GetText) | 获取当前选择项文本 |
| [GetListBox](#GetListBox) | 获取当前所属的 List 对象 |
| [GetDropBoxAttributeList](#GetDropBoxAttributeList) | 获取下拉框属性信息 |
| [SetDropBoxAttributeList](#SetDropBoxAttributeList) | 设置下拉框的属性信息 |
| [GetDropBoxSize](#GetDropBoxSize) | 获取下拉框容器大小 |
| [SetDropBoxSize](#SetDropBoxSize) | 设置下拉框容器大小 |
| [SetPopupTop](#SetPopupTop) | 设置 Combobox 是否向上弹出 |
| [IsPopupTop](#IsPopupTop) | 判断 Combobox 弹出模式是否是向上弹出 |
| [SelectItem](#SelectItem) | 选择一个子项 |
| [GetItemAt](#GetItemAt) | 获取指定索引下的子项控件 |
| [GetCurSel](#GetCurSel) | 获取当前选择项索引 |
| [GetCount](#GetCount) | 获取所有子项数量 |
| [AttachSelect](#AttachSelect) | 监听子项被选择事件 |
| [OnSelectItem](#OnSelectItem) | 默认的子项被选择处理函数 |


### GetText

获取当前选择项文本

```cpp
std::wstring GetText()
```

 - 参&emsp;数：无  
 - 返回值：返回当前选择项文本

### GetListBox

获取当前所属的 List 对象

```cpp
ListBox* GetListBox()
```

 - 参&emsp;数：无  
 - 返回值：返回所属的 List 对象指针

### GetDropBoxAttributeList

获取下拉框属性信息

```cpp
std::wstring GetDropBoxAttributeList()
```

 - 参&emsp;数：无  
 - 返回值：返回字符串形式的属性信息

### SetDropBoxAttributeList

设置下拉框的属性信息

```cpp
void SetDropBoxAttributeList(const std::wstring& pstrList)
```

 - 参&emsp;数：  
    - `pstrList` 转义后的 XML 格式属性列表
 - 返回值：无

### GetDropBoxSize

获取下拉框容器大小

```cpp
CSize GetDropBoxSize()
```

 - 参&emsp;数：无  
 - 返回值：返回容器大小

### SetDropBoxSize

设置下拉框容器大小

```cpp
void SetDropBoxSize(CSize szDropBox)
```

 - 参&emsp;数：  
    - `szDropBox` 要设置的大小信息
 - 返回值：无

### SetPopupTop

设置 Combobox 是否向上弹出

```cpp
void SetPopupTop(bool top)
```

 - 参&emsp;数：  
    - `top` 为 true 则向上弹出，false 为默认向下弹出
 - 返回值：无
 
### IsPopupTop

判断 Combobox 弹出模式是否是向上弹出

```cpp
bool IsPopupTop() const
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示向上弹出，否则为 false

### SelectItem

选择一个子项

```cpp
bool SelectItem(int iIndex)
```

 - 参&emsp;数：  
    - `iIndex` 要选择的子项索引
 - 返回值：返回 true 表示成功，否则为 false

### GetItemAt

获取指定索引下的子项控件

```cpp
Control* GetItemAt(int iIndex)
```

 - 参&emsp;数：  
    - `iIndex` 要获取的子项索引
 - 返回值：返回控件指针

### GetCurSel

获取当前选择项索引

```cpp
int GetCurSel()
```

 - 参&emsp;数：无  
 - 返回值：返回当前选择项索引

### GetCount

获取所有子项数量

```cpp
virtual int GetCount()
```

 - 参&emsp;数：无  
 - 返回值：返回所有子项数量

### AttachSelect

监听子项被选择事件

```cpp
void AttachSelect(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 子项被选择后触发的回调函数
 - 返回值：无

### OnSelectItem

默认的子项被选择处理函数

```cpp
bool OnSelectItem(EventArgs* args)
```

 - 参&emsp;数：  
    - `args` 参数列表
 - 返回值：始终返回 true