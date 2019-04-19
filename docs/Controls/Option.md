# Option（单选框）

`Option` 继承了 `CheckBox` 控件属性和方法，更多可用属性和方法请参考：[CheckBox](CheckBox.md) 控件

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| group |  | STRING | 所属组的名称 |

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [GetGroup](#GetGroup) | 获取所属组名称 |
| [SetGroup](#SetGroup) | 设置所属组 |

### GetGroup

获取所属组名称

```cpp
virtual  std::wstring GetGroup()
```

 - 参&emsp;数：无  
 - 返回值：返回组名称

### SetGroup

设置所属组

```cpp
virtual void SetGroup(const std::wstring& strGroupName)
```

 - 参&emsp;数：  
    - `strGroupName` 组名称
 - 返回值：无