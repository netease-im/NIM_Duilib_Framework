# ListContainerElement（列表项）

`ListContainerElement` 控件继承了 `CheckBox` 控件和 `Box` 布局属性和方法，更多可用属性和方法请参考：[CheckBox](../Controls/CheckBox.md) 控件和 [Box](../Containers/Box.md) 自由布局容器

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [GetOwner](#GetOwner) | 获取父容器 |
| [SetOwner](#SetOwner) | 设置父容器 |
| [GetIndex](#GetIndex) | 获取当前索引 |
| [SetIndex](#SetIndex) | 设置索引 |
| [InvokeDoubleClickEvent](#InvokeDoubleClickEvent) | 触发双击事件 |
| [AttachDoubleClick](#AttachDoubleClick) | 监听控件双击事件 |
| [AttachReturn](#AttachReturn) | 监听回车事件 |

### GetOwner

获取父容器

```cpp
IListOwner* GetOwner()
```

 - 参&emsp;数：无  
 - 返回值：返回父容器指针

### SetOwner

设置父容器

```cpp
void SetOwner(IListOwner* pOwner)
```

 - 参&emsp;数：  
    - `pOwner` 父容器指针
 - 返回值：无

### GetIndex

获取当前索引

```cpp
int GetIndex()
```

 - 参&emsp;数：无  
 - 返回值：返回当前索引

### SetIndex

设置索引

```cpp
void SetIndex(int iIndex)
```

 - 参&emsp;数：  
    - `iIndex` 索引值
 - 返回值：无

### InvokeDoubleClickEvent

触发双击事件

```cpp
void InvokeDoubleClickEvent()
```

 - 参&emsp;数：无  
 - 返回值：无

### AttachDoubleClick

监听控件双击事件

```cpp
void AttachDoubleClick(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 收到双击消息时的回调函数
 - 返回值：无

### AttachReturn

监听回车事件

```cpp
void AttachReturn(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 收到回车时的回调函数
 - 返回值：无
