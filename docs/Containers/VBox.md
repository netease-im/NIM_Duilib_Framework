# VBox（垂直布局容器）

`VBox` 继承于 [Box](Box.md) 但具有自定义垂直布局行为，该容器下的所有控件将有上向下依次垂直排布布局。以下为示例代码：

```xml
<!-- 三个 Button 按钮将垂直从上向下依次排列 -->
<VBox>
    <Button text="button1"/>
    <Button text="button2"/>
    <Button text="button3"/>
</VBox>
```

```xml
<!-- 一个垂直布局容器包含了三个水平布局容器，三个水平布局容器依次从上向下排列 -->
<VBox>
    <HBox name="vbox1">
        <Label text="label1"/>
    </HBox>
    <HBox name="vbox2">
        <Label text="label2"/>
    </HBox>
    <HBox name="vbox3">
        <Label text="label3"/>
    </HBox>
</VBox>
```

## 可用属性

`VBox` 控件继承了 `Box` 容器属性，更多可用属性请参考：[Box](Box.md) 容器

## 可用接口

`VBox` 控件继承了 `Box` 容器接口，更多可用属性请参考：[Box](Box.md) 容器