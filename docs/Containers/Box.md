# Box（自由布局）

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| padding | 0,0,0,0 | RECT | 内边距,如(2,2,2,2) |
| childmargin | 0 | INT | 子控件之间的额外距离,如(4) |
| mousechild | true | BOOL | 本控件的子控件是否可以响应用户操作,如(true) |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [SetOwner](#SetOwner) | 设置所有者 |
| [SetFloatPos](#SetFloatPos) | 设置浮动状态下的坐标信息 |
| [SetAttribute](#SetAttribute) | 设置布局属性 |
| [ArrangeChild](#ArrangeChild) | 调整内部所有控件的位置信息 |
| [AjustSizeByChild](#AjustSizeByChild) | 根据内部子控件大小调整容器自身大小 |
| [GetPadding](#GetPadding) | 获取内边距 |
| [SetPadding](#SetPadding) | 设置内边距，相当于设置客户区 |
| [GetChildMargin](#GetChildMargin) | 获取子控件之间的额外边距 |
| [SetChildMargin](#SetChildMargin) | 设置子控件之间的额外边距 |
| [GetInternalPos](#GetInternalPos) | 获取除了内边距外的可用范围 |
| [FindSubControl](#FindSubControl) | 查找指定子控件 |
| [FindSelectable](#FindSelectable) | 查找下一个可选控件的索引（面向 list、combo） |
| [GetItemAt](#GetItemAt) | 根据索引查找指定控件 |
| [GetItemIndex](#GetItemIndex) | 根据控件指针获取索引 |
| [SetItemIndex](#SetItemIndex) | 设置控件索引（内部会重新排列控件位置） |
| [GetCount](#GetCount) | 获取子控件数量 |
| [Add](#Add) | 添加一个控件到容器中 |
| [AddAt](#AddAt) | 向指定位置添加一个控件 |
| [Remove](#Remove) | 根据控件指针从容器中移除一个控件 |
| [RemoveAt](#RemoveAt) | 根据控件索引从容器中移除一个控件 |
| [RemoveAll](#RemoveAll) | 移除所有子控件 |
| [SwapChild](#SwapChild) | 交换子控件位置 |
| [ResetChildIndex](#ResetChildIndex) | 重置一个子控件位置 |
| [IsAutoDestroy](#IsAutoDestroy) | 判断是否自动销毁 |
| [SetAutoDestroy](#SetAutoDestroy) | 设置控件是否自动销毁 |
| [IsDelayedDestroy](#IsDelayedDestroy) | 判断窗口关闭后是否自动销毁 |
| [SetDelayedDestroy](#SetDelayedDestroy) | 设置窗口关闭后是否自动销毁 |
| [IsMouseChildEnabled](#IsMouseChildEnabled) | 获取容器是否响应鼠标操作 |
| [SetMouseChildEnabled](#SetMouseChildEnabled) | 设置容器响应鼠标操作 |
| [GetLayout](#GetLayout) | 获取容器布局对象指针 |
| [RetSetLayout](#RetSetLayout) | 重新关联布局对象 |
| [GetPaddingPos](#GetPaddingPos) | 获取内边距的位置信息 |
| [AttachBubbledEvent](#AttachBubbledEvent) | 绑定事件处理函数 |
| [AttachXmlBubbledEvent](#AttachXmlBubbledEvent) | 绑定 XML 中编写的 Event 和 BubbleEvent 事件的处理函数 |

## SetOwner

设置所有者

```cpp
void SetOwner(Box* pOwner)
```

 - 返回值：无

## SetFloatPos

设置浮动状态下的坐标信息

```cpp
static CSize SetFloatPos(Control* pControl, UiRect rcContainer)
```

 - 参数：  
    - `pControl` 控件句柄
    - `rcContainer` 要设置的位置信息
 - 返回值：返回控件最终的位置信息

## SetAttribute

设置布局属性

```cpp
virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue)
```

 - 参数：  
    - `strName` 要设置的属性名
    - `strValue` 要设置的属性值
 - 返回值：true 设置成功，false 属性不存在

## ArrangeChild

调整内部所有控件的位置信息

```cpp
virtual CSize ArrangeChild(const std::vector<Control*>& items, UiRect rc)
```

 - 参数：  
    - `items` 控件列表
    - `rc` 当前容器位置信息
 - 返回值：返回排列后最终盒子的宽度和高度信息

## AjustSizeByChild

根据内部子控件大小调整容器自身大小

```cpp
virtual CSize AjustSizeByChild(const std::vector<Control*>& items, CSize szAvailable)
```

 - 参数：  
    - `items` 控件列表
    - `szAvailable` 子控件允许的最大宽度
 - 返回值：返回排列后最终盒子的宽度和高度信息

## GetPadding

获取内边距

```cpp
virtual UiRect GetPadding()
```

 - 返回值：返回内边距四边的大小

## SetPadding

设置内边距，相当于设置客户区

```cpp
virtual void SetPadding(UiRect rcPadding, bool bNeedDpiScale = true)
```

 - 参数：  
    - `rcPadding` 内边距数据
    - `bNeedDpiScale` 是否根据 DPI 自适应，默认为 true
 - 返回值：无

## GetChildMargin

获取子控件之间的额外边距

```cpp
virtual int GetChildMargin()
```

 - 返回值：返回额外间距的数值

## SetChildMargin

设置子控件之间的额外边距

```cpp
virtual void SetChildMargin(int iMargin)
```

 - 参数：  
    - `iMargin` 要设置的边距数值
 - 返回值：无

## GetInternalPos

获取除了内边距外的可用范围

```cpp
UiRect GetInternalPos()
```

 - 返回值：返回可用范围位置信息

## FindSubControl

查找指定子控件

```cpp
Control* FindSubControl(const std::wstring& pstrSubControlName)
```

 - 参数：  
    - `pstrSubControlName` 子控件名称
 - 返回值：返回子控件指针

## FindSelectable

查找下一个可选控件的索引（面向 list、combo）

```cpp
virtual int FindSelectable(int iIndex, bool bForward = true)
```

 - 参数：  
    - `iIndex` 指定要起始查找的索引
    - `bForward` true 为递增查找， false 为递减查找
 - 返回值：下一个可选控件的索引，返回 -1 为没有可选控件

## GetItemAt

根据索引查找指定控件

```cpp
virtual Control* GetItemAt(std::size_t iIndex)
```

 - 参数：  
    - `iIndex` 控件索引
 - 返回值：返回控件指针

## GetItemIndex

根据控件指针获取索引

```cpp
virtual int GetItemIndex(Control* pControl)
```

 - 参数：  
    - `pControl` 控件指针
 - 返回值：返回 pControl 所指向的控件索引

## SetItemIndex

设置控件索引（内部会重新排列控件位置）

```cpp
virtual bool SetItemIndex(Control* pControl, std::size_t iIndex)
```

 - 参数：  
    - `pControl` 控件指针
    - `iIndex` 要设置的索引值
 - 返回值：返回 true 设置成功，false 设置失败

## GetCount

获取子控件数量

```cpp
virtual int GetCount()
```

 - 返回值：返回子控件数量

## Add

添加一个控件到容器中

```cpp
virtual bool Add(Control* pControl)
```

 - 参数：  
    - `pControl` 控件指针
 - 返回值：返回 true 添加成功，false 为添加失败

## AddAt

向指定位置添加一个控件

```cpp
virtual bool AddAt(Control* pControl, std::size_t iIndex)
```

 - 参数：  
    - `pControl` 控件指针
    - `iIndex` 在该索引之后插入控件
 - 返回值：返回 true 为添加成功，false 为添加失败

## Remove

根据控件指针从容器中移除一个控件

```cpp
virtual bool Remove(Control* pControl)
```

 - 参数：  
    - `pControl` 控件的指针
 - 返回值：返回 true 为移除成功，false 为移除失败（控件可能不存在）

## RemoveAt

根据控件索引从容器中移除一个控件

```cpp
virtual bool RemoveAt(std::size_t iIndex)
```

 - 参数：  
    - `iIndex` 要移除的控件索引
 - 返回值：返回 true 为移除成功，false 为移除失败（索引值太小或超出了子控件总数）

## RemoveAll

移除所有子控件

```cpp
virtual void RemoveAll()
```

 - 返回值：无

## SwapChild

交换子控件位置

```cpp
void SwapChild(Control* pChild1, Control* pChild2)
```

 - 参数：  
    - `pChild1` 子控件1 指针
    - `pChild2` 子控件2 指针
 - 返回值：无

## ResetChildIndex

重置一个子控件位置

```cpp
void ResetChildIndex(Control* pChild, std::size_t iIndex)
```

 - 参数：  
    - `pChild` 控件指针
    - `iIndex` 要重置的位置
 - 返回值：无

## IsAutoDestroy

判断是否自动销毁

```cpp
virtual bool IsAutoDestroy()
```

 - 返回值：true 为自动销毁，false 为不自动销毁

## SetAutoDestroy

设置控件是否自动销毁

```cpp
virtual void SetAutoDestroy(bool bAuto)
```

 - 参数：  
    - `bAuto` true 为自动销毁，false 为不自动销毁
 - 返回值：无

## IsDelayedDestroy

判断窗口关闭后是否自动销毁

```cpp
virtual bool IsDelayedDestroy()
```

 - 返回值：true 为自动销毁，false 为不自动销毁

## SetDelayedDestroy

设置窗口关闭后是否自动销毁

```cpp
virtual void SetDelayedDestroy(bool bDelayed)
```

 - 参数：  
    - `bDelayed` true 为自动销毁，false 为不自动销毁
 - 返回值：无

## IsMouseChildEnabled

获取容器是否响应鼠标操作

```cpp
virtual bool IsMouseChildEnabled()
```

 - 返回值：true 为响应，false 为不响应

## SetMouseChildEnabled

设置容器响应鼠标操作

```cpp
virtual void SetMouseChildEnabled(bool bEnable = true)
```

 - 参数：  
    - `bEnable` 设置为 true 为响应鼠标操作，设置为 false 为不响应，默认为 true
 - 返回值：无

## GetLayout

获取容器布局对象指针

```cpp
virtual Layout* GetLayout()
```

 - 返回值：返回容器关联的布局对象指针

## RetSetLayout

重新关联布局对象

```cpp
virtual void RetSetLayout(Layout* pLayout)
```

 - 参数：  
    - `pLayout` 布局对象指针
 - 返回值：无

## GetPaddingPos

获取内边距的位置信息

```cpp
virtual	UiRect GetPaddingPos()
```

 - 返回值：返回内边距的位置信息

## AttachBubbledEvent

绑定事件处理函数

```cpp
void AttachBubbledEvent(EventType eventType, const EventCallback& callback)
```

 - 参数：  
    - `eventType` 事件类型
 - 返回值：callback 指定回调函数

## AttachXmlBubbledEvent

绑定 XML 中编写的 Event 和 BubbleEvent 事件的处理函数

```cpp
void AttachXmlBubbledEvent(EventType eventType, const EventCallback& callback)
```

 - 参数：  
    - `eventType` 事件类型
 - 返回值：callback 指定回调函数
