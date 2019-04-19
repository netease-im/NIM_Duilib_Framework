# 控件容器

控件容器不同于普通控件，也不同于普通容器，他们是容器和控件的集合，既有容器拥有的布局功能也有控件拥有的事件响应机制。

以 ButtonBox 举例，如果你想制作一个保存按钮，按钮左侧是一个保存的 icon，紧接着 icon 右边是按钮的文字 “保存”。
这种情况下，你就可以使用 ButtonBox 来完成你的需求。

```xml
<ButtonBox width="70" height="30" bkcolor="darkcolor" padding="10,8,8,10">
  <Control width="auto" height="auto" bkimage="save.png" mouse="false"/>
  <Label text="保存" normaltextcolor="white" margin="20" mouse="false"/>
</ButtonBox>
```

更多类似于 `ButtonBox` 的控件容器如下

 - `LabelBox` 继承于 [Box](../Containers/Box.md) 容器，具有 [Label](Label.md) 控件的基本功能
 - `ButtonBox` 继承于 [Box](../Containers/Box.md) 容器，具有 [Button](Button.md) 控件的基本功能
 - `OptionBox` 继承于 [Box](../Containers/Box.md) 容器，具有 [Option](Option.md) 控件的基本功能
 - `CheckBoxBox` 继承于 [Box](../Containers/Box.md) 容器，具有 [CheckBox](CheckBox.md) 控件的基本功能