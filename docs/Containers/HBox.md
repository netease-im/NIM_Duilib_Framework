# HBox（水平布局容器）

`HBox` 继承于 [Box](Box.md) 但具有自定义水平布局行为，该容器下的所有控件将有左向右依次水平排布布局。以下为示例代码：

```xml
<!-- 三个 Button 按钮将水平从左至右依次排列 -->
<HBox>
    <Button text="button1"/>
    <Button text="button2"/>
    <Button text="button3"/>
</HBox>
```

```xml
<!-- 一个水平布局容器包含了三个垂直布局容器，三个垂直布局容器依次从左至右排列 -->
<HBox>
    <VBox name="vbox1">
        <Label text="label1"/>
    </VBox>
    <VBox name="vbox2">
        <Label text="label2"/>
    </VBox>
    <VBox name="vbox3">
        <Label text="label3"/>
    </VBox>
</HBox>
```

```xml
<!-- 一个标准的应用程序标题栏示例，包含最小化、最大化、还原和关闭按钮 -->
<HBox width="stretch" height="35" bkcolor="bk_wnd_lightcolor">
    <Control />
    <Button class="btn_wnd_min" name="minbtn" margin="4,6,0,0"/>
    <Box width="21" margin="4,6,0,0">
        <Button class="btn_wnd_max" name="maxbtn"/>
        <Button class="btn_wnd_restore" name="restorebtn" visible="false"/>
    </Box>
    <Button class="btn_wnd_close" name="closebtn" margin="4,6,8,0"/>
</HBox>
```

## 可用属性

`HBox` 控件继承了 `Box` 容器属性，更多可用属性请参考：[Box](Box.md) 容器

## 可用接口

`HBox` 控件继承了 `Box` 容器接口，更多可用属性请参考：[Box](Box.md) 容器