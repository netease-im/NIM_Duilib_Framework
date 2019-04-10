# TreeNode （树形列表项）

`TreeNode` 控件继承了 `ListContainerElement` 控件属性和方法，更多可用属性和方法请参考：[ListContainerElement](ListContainerElement.md) 控件

## 可用方法

| 方法名称 | 用途 |
| :--- | :--- |
| [OnClickItem](#OnClickItem) | 子项被点击时触发 |
| [SetTreeView](#SetTreeView) | 设置子项所属的树形列表容器 |
| [GetParentNode](#GetParentNode) | 获取父节点指针 |
| [SetParentNode](#SetParentNode) | 设置父节点 |
| [AddChildNode](#AddChildNode) | 添加子节点 |
| [AddChildNodeAt](#AddChildNodeAt) | 在指定位置添加子节点 |
| [RemoveChildNodeAt](#RemoveChildNodeAt) | 从指定位置移除一个子节点 |
| [RemoveChildNode](#RemoveChildNode) | 根据子节点指针移除一个子节点 |
| [RemoveAllChildNode](#RemoveAllChildNode) | 移除所有子节点 |
| [GetDescendantNodeCount](#GetDescendantNodeCount) | 递归获取所有子节点个数 |
| [GetChildNodeCount](#GetChildNodeCount) | 获取下一级子节点个数 |
| [GetChildNode](#GetChildNode) | 获取一个子节点指针 |
| [GetChildNodeIndex](#GetChildNodeIndex) | 根据子节点指针获取子节点位置索引 |
| [IsExpand](#IsExpand) | 判断是否展开状态 |
| [SetExpand](#SetExpand) | 设置是否展开显示子节点 |
| [GetDepth](#GetDepth) | 获取子项层级 |
| [RemoveSelf](#RemoveSelf) | 删除自身 |


### OnClickItem

子项被点击时触发

```cpp
virtual bool OnClickItem(EventArgs* param)
```

 - 参&emsp;数：  
    - `param` 消息体
 - 返回值：始终返回 true

### SetTreeView

设置子项所属的树形列表容器

```cpp
void SetTreeView(TreeView* pTreeView)
```

 - 参&emsp;数：  
    - `pTreeView` 容器指针
 - 返回值：无

### GetParentNode

获取父节点指针

```cpp
TreeNode* GetParentNode()
```

 - 参&emsp;数：无  
 - 返回值：返回父节点指针

### SetParentNode

设置父节点

```cpp
void SetParentNode(TreeNode* pParentTreeNode)
```

 - 参&emsp;数：  
    - `pParentTreeNode` 父节点指针
 - 返回值：无

### AddChildNode

添加子节点

```cpp
bool AddChildNode(TreeNode* pTreeNode)
```

 - 参&emsp;数：  
    - `pTreeNode` 子节点指针
 - 返回值：成功返回 true，失败返回 false

### AddChildNodeAt

在指定位置添加子节点

```cpp
bool AddChildNodeAt(TreeNode* pTreeNode, std::size_t iIndex)
```

 - 参&emsp;数：  
    - `pTreeNode` 子节点指针
    - `iIndex` 要插入哪个位置之后
 - 返回值：成功返回 true，失败返回 false

### RemoveChildNodeAt

从指定位置移除一个子节点

```cpp
bool RemoveChildNodeAt(std::size_t index)
```

 - 参&emsp;数：  
    - `index` 要移除的子节点索引
 - 返回值：成功返回 true，失败返回 false

### RemoveChildNode

根据子节点指针移除一个子节点

```cpp
bool RemoveChildNode(TreeNode* pTreeNode)
```

 - 参&emsp;数：  
    - `pTreeNode` 子节点指针
 - 返回值：成功返回 true，失败返回 false

### RemoveAllChildNode

移除所有子节点

```cpp
void RemoveAllChildNode()
```

 - 参&emsp;数：无  
 - 返回值：无

### GetDescendantNodeCount

递归获取所有子节点个数

```cpp
int GetDescendantNodeCount()
```

 - 参&emsp;数：无  
 - 返回值：返回所有子节点个数

### GetChildNodeCount

获取下一级子节点个数

```cpp
std::size_t GetChildNodeCount()
```

 - 参&emsp;数：无  
 - 返回值：返回子节点个数

### GetChildNode

获取一个子节点指针

```cpp
TreeNode* GetChildNode(std::size_t iIndex)
```

 - 参&emsp;数：  
    - `iIndex` 要获取的子节点索引
 - 返回值：返回子节点指针

### GetChildNodeIndex

根据子节点指针获取子节点位置索引

```cpp
int	GetChildNodeIndex(TreeNode* pTreeNode)
```

 - 参&emsp;数：  
    - `pTreeNode` 子节点指针
 - 返回值：返回位置索引

### IsExpand

判断是否展开状态

```cpp
bool IsExpand()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为展开状态，否则为 false

### SetExpand

设置是否展开显示子节点

```cpp
void SetExpand(bool bExpand)
```

 - 参&emsp;数：  
    - `bExpand` 为 true 时展开，为 false 是不展开
 - 返回值：无

### GetDepth

获取子项层级

```cpp
int GetDepth()
```

 - 参&emsp;数：无  
 - 返回值：返回当前层级

### RemoveSelf

删除自身

```cpp
bool RemoveSelf()
```

 - 参&emsp;数：无  
 - 返回值：成功返回 true，失败返回 false