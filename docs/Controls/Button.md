# Button（按钮）

按钮一般用于提供用户主动发起一项功能。

`Button` 控件继承了 `Label` 控件属性和方法，更多可用属性和方法请参考：[Label](Label.md) 控件

## 可用属性

```xml
<!-- 一个名称为 btn_submit，宽度为 100px，高度为 30px 的按钮 -->
<Button name="btn_submit" width="100" height="30"/>
```

## 可用接口

| 接口名称 | 用途 |
| :--- | :--- |
| [AttachClick](#AttachClick) | 绑定鼠标点击处理函数 |


### AttachClick

绑定鼠标点击处理函数

```cpp
void AttachClick(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 要绑定的回调函数
 - 返回值：无
