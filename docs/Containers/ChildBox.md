# ChildBox（子布局）

`ChildBox` 继承了 `Box` 容器属性和方法，更多可用属性和方法请参考：[Box](Box.md) 容器

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| xmlfile |  | STRING | 要嵌入的xml文件的地址,如(childxml.xml) |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [SetChildLayoutXML](#SetChildLayoutXML) | 设置 ChildBox 关联的 XML 文件 |
| [GetChildLayoutXML](#GetChildLayoutXML) | 获取 ChildBox 关联的 XML 文件 |


### SetChildLayoutXML

设置 ChildBox 关联的 XML 文件

```cpp
void SetChildLayoutXML(std::wstring strXML)
```

 - 参&emsp;数：  
    - `strXML` XML 文件路径
 - 返回值：无

### GetChildLayoutXML

获取 ChildBox 关联的 XML 文件

```cpp
std::wstring GetChildLayoutXML()
```

 - 参&emsp;数：无  
 - 返回值：返回 ChildBox 关联的 XML 文件

