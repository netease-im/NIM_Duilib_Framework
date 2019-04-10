# VirtualListBox（虚拟列表）

`VirtualListBox` 继承了 `ListBox` 容器属性和方法，更多可用属性和方法请参考：[ListBox](ListBox.md) 容器

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [SetDataProvider](#SetDataProvider) | 设置数据代理对象 |
| [SetElementHeight](#SetElementHeight) | 设置子项高度 |
| [InitElement](#InitElement) | 初始化子项 |
| [Refresh](#Refresh) | 刷新列表 |
| [RemoveAll](#RemoveAll) | 删除所有子项 |
| [SetForceArrange](#SetForceArrange) | 设置是否强制重新布局 |
| [GetDisplayCollection](#GetDisplayCollection) | 获取当前所有可见控件的索引 |
| [EnsureVisible](#EnsureVisible) | 让控件在可见范围内 |

### SetDataProvider

设置数据代理对象

```cpp
virtual void SetDataProvider(VirtualListInterface *pProvider)
```

 - 参&emsp;数：  
    - `pProvider` 开发者需要重写 VirtualListInterface 的接口来作为数据代理对象
 - 返回值：无

### SetElementHeight

设置子项高度

```cpp
virtual void SetElementHeight(int nHeight)
```

 - 参&emsp;数：  
    - `nHeight` 高度值
 - 返回值：无

### InitElement

初始化子项

```cpp
virtual void InitElement(int nMaxItemCount = 50)
```

 - 参&emsp;数：  
    - `nMaxItemCount` 初始化数量，默认 50
 - 返回值：无

### Refresh

刷新列表

```cpp
virtual void Refresh()
```

 - 参&emsp;数：无  
 - 返回值：无

### RemoveAll

删除所有子项

```cpp
virtual void RemoveAll()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetForceArrange

设置是否强制重新布局

```cpp
void SetForceArrange(bool bForce)
```

 - 参&emsp;数：  
    - `bForce` 设置为 true 为强制，否则为不强制
 - 返回值：无

### GetDisplayCollection

获取当前所有可见控件的索引

```cpp
void GetDisplayCollection(std::vector<int>& collection)
```

 - 参&emsp;数：  
    - `collection` 索引列表
 - 返回值：无

### EnsureVisible

让控件在可见范围内

```cpp
void EnsureVisible(int iIndex, bool bToTop = false)
```

 - 参&emsp;数：  
    - `iIndex` 控件索引
    - `bToTop` 是否在最上方
 - 返回值：无
