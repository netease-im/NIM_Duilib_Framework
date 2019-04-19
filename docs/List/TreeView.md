# TreeView

`TreeView` 继承了 `ListBox` 容器属性和方法，更多可用属性和方法请参考：[ListBox](ListBox.md) 容器

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| indent | 0 | INT | 子控件的缩进,如(3) |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [GetRootNode](#GetRootNode) | 获取根节点 |
| [GetIndent](#GetIndent) | 获取子节点缩进值 |
| [SetIndent](#SetIndent) | 设置子节点缩进值 |


### GetRootNode

获取根节点

```cpp
TreeNode* GetRootNode()
```

 - 参&emsp;数：无  
 - 返回值：返回根节点指针

### GetIndent

获取子节点缩进值

```cpp
int GetIndent()
```

 - 参&emsp;数：无  
 - 返回值：返回子节点缩进值

### SetIndent

设置子节点缩进值

```cpp
void SetIndent(int indent)
```

 - 参&emsp;数：  
    - `indent` 要设置的缩进值
 - 返回值：无