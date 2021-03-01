# RichEdit（富文本控件）

`RichEdit` 继承了 `ScrollableBox` 容器属性和方法，更多可用属性和方法请参考：[ScrollableBox](../Containers/ScrollableBox.md) 容器

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| vscrollbar | false | BOOL | 是否使用竖向滚动条,如(true) |
| autovscroll | false | BOOL | 是否随输入竖向滚动,如(true) |
| hscrollbar | false | BOOL | 是否使用横向滚动条,如(true) |
| autohscroll | false | BOOL | 是否随输入横向滚动,如(true) |
| wanttab | true | BOOL | 是否接受tab按键消息,如(true) |
| wantreturnmsg | true | BOOL | 是否接受return按键消息,如(true) |
| returnmsgwantctrl | true | BOOL | 是否接受ctrl+return按键消息,如(true) |
| rich | true | BOOL | 是否使用富格式,如(true) |
| multiline | true | BOOL | 是否使用多行,如(true) |
| readonly | false | BOOL | 是否只读,如(false) |
| password | false | BOOL | 是否显示密码符,如(true) |
| number | false | BOOL | 是否只允许输入数字,如(false) |
| align | left | STRING | 文字对齐方式,取值left、right、hcenter、top、vcenter、bottom,如(right) |
| text |  | STRING | 显示文本,如(测试文本) |
| font | -1 | INT | 字体id,如(0) |
| normaltextcolor |  | STRING | 普通状态文字颜色,不指定则使用默认颜色,如(blue) |
| disabledtextcolor |  | STRING | 禁用状态文字颜色,不指定则使用默认颜色,如(blue) |
| promptmode | false | BOOL | 是否显示提示文字,如(true) |
| prompttext |  | STRING | 文本框内提示文字,当文本框text为空时显示 |
| prompttextid |  | STRING | 多语言功能的ID,如(TEXT_OUT) |
| promptcolor |  | STRING | 文本框内提示文字的颜色 |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [IsWantTab](#IsWantTab) | 判断是否接受 TAB 按键消息 |
| [SetWantTab](#SetWantTab) | 判断是否接受 TAB 按键消息 |
| [IsNeedReturnMsg](#IsNeedReturnMsg) | 判断是否接受 RETURN 按键消息 |
| [SetNeedReturnMsg](#SetNeedReturnMsg) | 设置是否接受 RETURN 按键消息 |
| [IsReturnMsgWantCtrl](#IsReturnMsgWantCtrl) | 判断是否接受CTRL+RETURN 组合键消息 |
| [SetReturnMsgWantCtrl](#SetReturnMsgWantCtrl) | 设置是否接受CTRL+RETUREN 组合键消息 |
| [IsRich](#IsRich) | 是否是富文本状态 |
| [SetRich](#SetRich) | 设置控件为富文本编辑状态 |
| [IsReadOnly](#IsReadOnly) | 是否是只读状态 |
| [SetReadOnly](#SetReadOnly) | 设置控件为只读状态 |
| [IsPassword](#IsPassword) | 是否是密码状态控件 |
| [SetPassword](#SetPassword) | 设置控件为密码控件（显示 ***） |
| [GetWordWrap](#GetWordWrap) | 获取超出矩形区域的文本显示方式 |
| [SetWordWrap](#SetWordWrap) | 设置超出矩形区域的文本显示方式 |
| [GetFont](#GetFont) | 获取当前设置的字体索引 |
| [SetFont](#SetFont) | 设置字体索引 |
| [SetFont](#SetFont) | 根据字体名称设置字体 |
| [GetWinStyle](#GetWinStyle) | 获取窗口样式 |
| [SetWinStyle](#SetWinStyle) | 设置窗口样式 |
| [GetTextVerAlignType](#GetTextVerAlignType) | 获取内容垂直对其方式 |
| [SetTextColor](#SetTextColor) | 设置文本颜色 |
| [GetTextColor](#GetTextColor) | 获取文本颜色 |
| [GetLimitText](#GetLimitText) | 获取限制字符数量 |
| [SetLimitText](#SetLimitText) | 设置限制字符数量 |
| [GetTextLength](#GetTextLength) | 获取内容的长度 |
| [GetText](#GetText) | 获取控件中的文本 |
| [GetUTF8Text](#GetUTF8Text) | 获取控件中的文本 |
| [SetText](#SetText) | 设置控件的文本 |
| [SetTextId](#SetTextId) | 设置控件的文本对应 ID |
| [SetUTF8Text](#SetUTF8Text) | 设置控件的文本对应 ID |
| [GetModify](#GetModify) | 获取修改标志 |
| [SetModify](#SetModify) | 设置修改标志 |
| [GetSel](#GetSel) | 获取所选文本的起始位置和结束位置 |
| [GetSel](#GetSel) | 获取所选文本的起始位置和结束位置 |
| [SetSel](#SetSel) | 选择一部分内容 |
| [SetSel](#SetSel) | 选择一部分内容 |
| [ReplaceSel](#ReplaceSel) | 替换所选内容 |
| [ReplaceSelW](#ReplaceSelW) | 替换所选内容 |
| [GetSelText](#GetSelText) | 获取所选文字内容 |
| [SetSelAll](#SetSelAll) | 全选 |
| [SetSelNone](#SetSelNone) | 不选择任何内容 |
| [GetSelectionType](#GetSelectionType) | 获取控件的选择类型 |
| [GetZoom](#GetZoom) | 获取当前缩放比 |
| [SetZoom](#SetZoom) | 设置缩放比 |
| [SetZoomOff](#SetZoomOff) | 恢复缩放到初始状态 |
| [GetAutoURLDetect](#GetAutoURLDetect) | 获取是否开启了自动 URL 检测 |
| [SetAutoURLDetect](#SetAutoURLDetect) | 设置是否开启 URL 自动检测 |
| [GetEventMask](#GetEventMask) | 获取控件的事件掩码 |
| [SetEventMask](#SetEventMask) | 设置控件的事件掩码 |
| [GetTextRange](#GetTextRange) | 获取指定范围的内容 |
| [HideSelection](#HideSelection) | 设置是否隐藏或显示选择的值 |
| [ScrollCaret](#ScrollCaret) | 设置光标到可见位置 |
| [InsertText](#InsertText) | 插入文字 |
| [AppendText](#AppendText) | 追加文字 |
| [GetDefaultCharFormat](#GetDefaultCharFormat) | 获取字符格式 |
| [SetDefaultCharFormat](#SetDefaultCharFormat) | 设置默认的字符格式 |
| [GetSelectionCharFormat](#GetSelectionCharFormat) | 获取被选择的字符格式 |
| [SetSelectionCharFormat](#SetSelectionCharFormat) | 设置被选择的字符格式 |
| [SetWordCharFormat](#SetWordCharFormat) | 设置当前插入点的单词格式 |
| [GetParaFormat](#GetParaFormat) | 获取当前段落格式 |
| [SetParaFormat](#SetParaFormat) | 设置当前段落格式 |
| [Redo](#Redo) | 恢复操作 |
| [Undo](#Undo) | 撤销操作 |
| [Clear](#Clear) | 清空 |
| [Copy](#Copy) | 复制所选内容 |
| [Cut](#Cut) | 剪切所选内容 |
| [Paste](#Paste) | 粘贴 |
| [CanPaste](#CanPaste) | 检测是否可以粘贴指定剪切板格式 |
| [PasteSpecial](#PasteSpecial) | 在控件中粘贴特定的剪贴板格式 |
| [GetLineCount](#GetLineCount) | 获取总行数 |
| [GetLine](#GetLine) | 获取一行数据 |
| [LineIndex](#LineIndex) | 获取指定行的第一个字符索引 |
| [LineLength](#LineLength) | 获取指定行的数据长度 |
| [LineScroll](#LineScroll) | 滚动文本 |
| [GetCharPos](#GetCharPos) | 获取指定位置字符的客户区坐标 |
| [LineFromChar](#LineFromChar) | 获取指定字符所在行数 |
| [PosFromChar](#PosFromChar) | 获取指定位置字符的客户区坐标 |
| [CharFromPos](#CharFromPos) | 根据坐标返回指定字符索引 |
| [EmptyUndoBuffer](#EmptyUndoBuffer) | 清空撤销列表 |
| [SetUndoLimit](#SetUndoLimit) | 设置撤销列表容纳的内容数量 |
| [StreamIn](#StreamIn) | 增加一个回调用于控制输入内容 |
| [StreamOut](#StreamOut) | 指定一个回调用于控制输出内容 |
| [SetScrollPos](#SetScrollPos) | 设置滚动条位置 |
| [LineUp](#LineUp) | 向上一行 |
| [LineDown](#LineDown) | 向下一行 |
| [PageUp](#PageUp) | 向上翻页 |
| [PageDown](#PageDown) | 向下翻页 |
| [HomeUp](#HomeUp) | 返回到顶端 |
| [EndDown](#EndDown) | 返回到底部 |
| [LineLeft](#LineLeft) | 水平向左滚动 |
| [LineRight](#LineRight) | 水平向右滚动 |
| [PageLeft](#PageLeft) | 水平向左翻页 |
| [PageRight](#PageRight) | 水平向右翻页 |
| [HomeLeft](#HomeLeft) | 返回到最左侧 |
| [EndRight](#EndRight) | 返回到最后侧 |
| [CreateCaret](#CreateCaret) | 创建光标 |
| [ShowCaret](#ShowCaret) | 设置是否显示光标 |
| [SetCaretColor](#SetCaretColor) | 设置光标颜色 |
| [GetCaretColor](#GetCaretColor) | 获取光标颜色 |
| [GetCaretRect](#GetCaretRect) | 获取光标矩形位置 |
| [SetCaretPos](#SetCaretPos) | 设置光标位置 |
| [ChangeCaretVisiable](#ChangeCaretVisiable) | 切换光标是否显示 |
| [PaintCaret](#PaintCaret) | 绘制光标 |
| [SetPromptMode](#SetPromptMode) | 设置是否显示提示文字 |
| [GetPromptText](#GetPromptText) | 获取提示文字 |
| [GetUTF8PromptText](#GetUTF8PromptText) | 获取提示文字 |
| [SetPromptText](#SetPromptText) | 设置提示文字 |
| [SetUTF8PromptText](#SetUTF8PromptText) | 设置提示文字 |
| [SetPromptTextId](#SetPromptTextId) | 设置提示文字 ID |
| [SetUTF8PromptTextId](#SetUTF8PromptTextId) | 设置提示文字 ID |
| [PaintPromptText](#PaintPromptText) | 绘制提示文字 |
| [GetFocusedImage](#GetFocusedImage) | 获取焦点状态下的图片 |
| [SetFocusedImage](#SetFocusedImage) | 设置焦点状态下的图片 |
| [PaintStatusImage](#PaintStatusImage) | 绘制指定状态下的图片 |
| [SetNoSelOnKillFocus](#SetNoSelOnKillFocus) | 设置失去焦点后是否取消选择项 |
| [SetSelAllOnFocus](#SetSelAllOnFocus) | 设置获取焦点后是否选择所有内容 |
| [SetNoCaretReadonly](#SetNoCaretReadonly) | 设置只读模式不显示光标 |
| [AddColorText](#AddColorText) | 添加带颜色的文本 |
| [AddLinkColorText](#AddLinkColorText) | 添加一个带有文字颜色的超链接 |
| [AddLinkColorTextEx](#AddLinkColorTextEx) | 添加一个指定字体带有文字颜色的超链接 |
| [AddLinkInfo](#AddLinkInfo) | 添加一个范围用于 hittest 判断是否是链接信息 |
| [HittestCustomLink](#HittestCustomLink) | 根据point来hittest自定义link的数据 |
| [ClearImageCache](#ClearImageCache) | 清理图片缓存 |
| [AttachReturn](#AttachReturn) | 监听回车按键按下事件 |
| [AttachTab](#AttachTab) | 监听 TAB 按键按下事件 |
| [AttachTextChange](#AttachTextChange) | 监听文本被修改事件 |
| [AttachCustomLinkClk](#AttachCustomLinkClk) | 监听自定义链接被点击事件 |
| [AttachGetNaturalSize](#AttachGetNaturalSize) | 监听大小变化事件 |


### IsWantTab

判断是否接受 TAB 按键消息

```cpp
bool IsWantTab()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示接受，false 表示不接受

### SetWantTab

判断是否接受 TAB 按键消息

```cpp
void SetWantTab(bool bWantTab = true)
```

 - 参&emsp;数：  
    - `bWantTab` 为 true 则接受该消息，false 为不接受
 - 返回值：无

### IsNeedReturnMsg

判断是否接受 RETURN 按键消息

```cpp
bool IsNeedReturnMsg()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示接受，false 表示不接受

### SetNeedReturnMsg

设置是否接受 RETURN 按键消息

```cpp
void SetNeedReturnMsg(bool bNeedReturnMsg = true)
```

 - 参&emsp;数：  
    - `bNeedReturnMsg` 为 true 则接受该消息，false 为不接受
 - 返回值：无

### IsReturnMsgWantCtrl

判断是否接受CTRL+RETURN 组合键消息

```cpp
bool IsReturnMsgWantCtrl()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示接受，false 表示不接受

### SetReturnMsgWantCtrl

设置是否接受CTRL+RETUREN 组合键消息

```cpp
void SetReturnMsgWantCtrl(bool bReturnMsgWantCtrl = true)
```

 - 参&emsp;数：  
    - `bReturnMsgWantCtrl` 为 true 则接受该消息，false 为不接受
 - 返回值：无

### IsRich

是否是富文本状态

```cpp
bool IsRich()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为是，否则为 false

### SetRich

设置控件为富文本编辑状态

```cpp
void SetRich(bool bRich = true)
```

 - 参&emsp;数：  
    - `bRich` 设置 true 为富文本状态，false 为普通状态
 - 返回值：无

### IsReadOnly

是否是只读状态

```cpp
bool IsReadOnly()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为只读状态，否则为 false

### SetReadOnly

设置控件为只读状态

```cpp
void SetReadOnly(bool bReadOnly = true)
```

 - 参&emsp;数：  
    - `bReadOnly` 设置 true 让控件变为只读状态，false 为可写入状态
 - 返回值：无

### IsPassword

是否是密码状态控件

```cpp
bool IsPassword()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示是密码控件，否则为 false

### SetPassword

设置控件为密码控件（显示 ***）

```cpp
void SetPassword(bool bPassword)
```

 - 参&emsp;数：  
    - `bPassword` 设置为 true 让控件显示内容为 ***，false 为显示正常内容
 - 返回值：无

### GetWordWrap

获取超出矩形区域的文本显示方式

```cpp
bool GetWordWrap()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 时并且在多行模式下内容被换行显示，false 则表示截断显示

### SetWordWrap

设置超出矩形区域的文本显示方式

```cpp
void SetWordWrap(bool bWordWrap = true)
```

 - 参&emsp;数：  
    - `bWordWrap` 为 true 时并且在多行模式下内容被换行显示，false 则表示截断显示
 - 返回值：无

### GetFont

获取当前设置的字体索引

```cpp
int GetFont()
```

 - 参&emsp;数：无  
 - 返回值：返回字体索引（对应 global.xml 中字体的顺序）

### SetFont

设置字体索引

```cpp
void SetFont(int index)
```

 - 参&emsp;数：  
    - `index` 要设置的字体索引（对应 global.xml 中字体的顺序）
 - 返回值：无

### SetFont

根据字体名称设置字体

```cpp
void SetFont(const std::wstring& pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
```

 - 参&emsp;数：  
    - `pStrFontName` 字体名称
    - `nSize` 字体大小
    - `bBold` 是否粗体显示
    - `bUnderline` 是否带有下划线
    - `bItalic` 是否斜体显示
 - 返回值：无

### GetWinStyle

获取窗口样式

```cpp
LONG GetWinStyle()
```

 - 参&emsp;数：无  
 - 返回值：返回窗口样式

### SetWinStyle

设置窗口样式

```cpp
void SetWinStyle(LONG lStyle)
```

 - 参&emsp;数：  
    - `lStyle` 要设置的窗口样式
 - 返回值：无

### GetTextVerAlignType

获取内容垂直对其方式

```cpp
VerAlignType GetTextVerAlignType()
```

 - 参&emsp;数：无  
 - 返回值：返回内容垂直对其方式（顶端对齐、居中、底端对齐）

### SetTextColor

设置文本颜色

```cpp
virtual void SetTextColor(const std::wstring& dwTextColor)
```

 - 参&emsp;数：  
    - `dwTextColor` 要设置的文本颜色，该颜色必须在 global.xml 中存在
 - 返回值：无

### GetTextColor

获取文本颜色

```cpp
std::wstring GetTextColor()
```

 - 参&emsp;数：无  
 - 返回值：返回当前文本颜色

### GetLimitText

获取限制字符数量

```cpp
int GetLimitText()
```

 - 参&emsp;数：无  
 - 返回值：返回限制字符数量

### SetLimitText

设置限制字符数量

```cpp
void SetLimitText(int iChars)
```

 - 参&emsp;数：  
    - `iChars` 要限制的字符数量
 - 返回值：无

### GetTextLength

获取内容的长度

```cpp
long GetTextLength(DWORD dwFlags = GTL_DEFAULT)
```

 - 参&emsp;数：  
    - `dwFlags` 指定用以确定文本长度的方式，参考 https://docs.microsoft.com/en-us/windows/desktop/controls/em-gettextlengthex
 - 返回值：返回内容长度

### GetText

获取控件中的文本

```cpp
std::wstring GetText()
```

 - 参&emsp;数：无  
 - 返回值：返回控件中的文本内容

### GetUTF8Text

获取控件中的文本

```cpp
virtual std::string GetUTF8Text()
```

 - 参&emsp;数：无  
 - 返回值：返回控件中的文本内容 UTF8 格式

### SetText

设置控件的文本

```cpp
void SetText(const std::wstring& strText)
```

 - 参&emsp;数：  
    - `strText` 要设置的文本内容
 - 返回值：无

### SetTextId

设置控件的文本对应 ID

```cpp
void SetTextId(const std::wstring& strTextId)
```

 - 参&emsp;数：  
    - `strTextId` 要设置的 ID，该 ID 必须在加载的语言文件中存在
 - 返回值：无

### SetUTF8Text

设置控件的文本对应 ID

```cpp
virtual void SetUTF8Text(const std::string& strText)
```

 - 参&emsp;数：  
    - `strTextId` 要设置的 UTF8 格式 ID，该 ID 必须在加载的语言文件中存在
 - 返回值：无

### GetModify

获取修改标志

```cpp
bool GetModify()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 为设置了修改标志，否则为 false

### SetModify

设置修改标志

```cpp
void SetModify(bool bModified = true)
```

 - 参&emsp;数：  
    - `bModified` 设置为 true 表示文本已经被修改，false 为未修改，默认为 true
 - 返回值：无

### GetSel

获取所选文本的起始位置和结束位置

```cpp
void GetSel(CHARRANGE &cr)
```

 - 参&emsp;数：  
    - `cr` 返回起始位置和结束位置
 - 返回值：无

### GetSel

获取所选文本的起始位置和结束位置

```cpp
void GetSel(long& nStartChar, long& nEndChar)
```

 - 参&emsp;数：  
    - `nStartChar` 返回起始位置
    - `nEndChar` 返回结束位置
 - 返回值：无

### SetSel

选择一部分内容

```cpp
int SetSel(CHARRANGE &cr)
```

 - 参&emsp;数：  
    - `cr` 要选择的文字起始位置和结束位置
 - 返回值：返回选择的文字数量

### SetSel

选择一部分内容

```cpp
int SetSel(long nStartChar, long nEndChar)
```

 - 参&emsp;数：  
    - `nStartChar` 要选择的起始位置
    - `nEndChar` 要选择的结束位置
 - 返回值：返回选择的文字数量

### ReplaceSel

替换所选内容

```cpp
void ReplaceSel(const std::wstring& lpszNewText, bool bCanUndo)
```

 - 参&emsp;数：  
    - `lpszNewText` 要替换的文字
    - `bCanUndo` 是否可以撤销，true 为可以，否则为 false
 - 返回值：无

### ReplaceSelW

替换所选内容

```cpp
void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false)
```

 - 参&emsp;数：  
    - `lpszNewText` 要替换的文字
    - `bCanUndo` 是否可以撤销，true 为可以，否则为 false
 - 返回值：无

### GetSelText

获取所选文字内容

```cpp
std::wstring GetSelText()
```

 - 参&emsp;数：无  
 - 返回值：返回所选文字内容

### SetSelAll

全选

```cpp
int SetSelAll()
```

 - 参&emsp;数：无  
 - 返回值：返回选择的内容数量

### SetSelNone

不选择任何内容

```cpp
int SetSelNone()
```

 - 参&emsp;数：无  
 - 返回值：始终返回 0 

### GetSelectionType

获取控件的选择类型

```cpp
WORD GetSelectionType()
```

 - 参&emsp;数：无  
 - 返回值：返回控件的选择类型，参考：https://docs.microsoft.com/en-us/windows/desktop/controls/em-selectiontype

### GetZoom

获取当前缩放比

```cpp
bool GetZoom(int& nNum, int& nDen)
```

 - 参&emsp;数：  
    - `nNum` 缩放比率分子
    - `nDen` 缩放比率分母
 - 返回值：如果处理了消息则返回 TRUE

### SetZoom

设置缩放比

```cpp
bool SetZoom(int nNum, int nDen)
```

 - 参&emsp;数：  
    - `nNum` 缩放比率分子
    - `nDen` 缩放比率分母
 - 返回值：成功返回 true，失败返回 false

### SetZoomOff

恢复缩放到初始状态

```cpp
bool SetZoomOff()
```

 - 参&emsp;数：无  
 - 返回值：成功返回 true，否则返回 false

### GetAutoURLDetect

获取是否开启了自动 URL 检测

```cpp
bool GetAutoURLDetect()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示开启了自动检测，否则为 false

### SetAutoURLDetect

设置是否开启 URL 自动检测

```cpp
bool SetAutoURLDetect(bool bAutoDetect = true)
```

 - 参&emsp;数：  
    - `bAutoDetect` 设置为 true 表示检测 URL，false 为不检测，默认为 true
 - 返回值：返回 true 为设置成功，false 为失败

### GetEventMask

获取控件的事件掩码

```cpp
DWORD GetEventMask()
```

 - 参&emsp;数：无  
 - 返回值：返回事件掩码

### SetEventMask

设置控件的事件掩码

```cpp
DWORD SetEventMask(DWORD dwEventMask)
```

 - 参&emsp;数：  
    - `dwEventMask` 要设置的事件掩码值
 - 返回值：返回设置之前的事件掩码值

### GetTextRange

获取指定范围的内容

```cpp
std::wstring GetTextRange(long nStartChar, long nEndChar)
```

 - 参&emsp;数：  
    - `nStartChar` 起始位置
    - `nEndChar` 结束为止
 - 返回值：返回设置的指定位置的内容

### HideSelection

设置是否隐藏或显示选择的值

```cpp
void HideSelection(bool bHide = true, bool bChangeStyle = false)
```

 - 参&emsp;数：  
    - `bHide` 是否显示，true 为显示，false 为不显示
    - `bChangeStyle` 是否修改样式，true 为修改，false 为不修改
 - 返回值：无

### ScrollCaret

设置光标到可见位置

```cpp
void ScrollCaret()
```

 - 参&emsp;数：无  
 - 返回值：无

### InsertText

插入文字

```cpp
int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false)
```

 - 参&emsp;数：  
    - `nInsertAfterChar` 要插入的位置
    - `lpstrText` 要插入的文本
    - `bCanUndo` 是否可以撤销，true 为可以，否则为 false，默认为 false
 - 返回值：返回插入后的文本位置

### AppendText

追加文字

```cpp
int AppendText(const std::wstring& strText, bool bCanUndo = false)
```

 - 参&emsp;数：  
    - `strText` 要追加的文字
    - `bCanUndo` 是否可以撤销，true 为可以，否则为 false，默认为 false
 - 返回值：返回追加后的文字位置

### GetDefaultCharFormat

获取字符格式

```cpp
DWORD GetDefaultCharFormat(CHARFORMAT2 &cf)
```

 - 参&emsp;数：  
    - `cf` 返回获取的字符格式
 - 返回值：返回参数 cf 中 dwMask 的值

### SetDefaultCharFormat

设置默认的字符格式

```cpp
bool SetDefaultCharFormat(CHARFORMAT2 &cf)
```

 - 参&emsp;数：  
    - `cf` 要设置字符格式
 - 返回值：返回 true 表示成功，false 为失败

### GetSelectionCharFormat

获取被选择的字符格式

```cpp
DWORD GetSelectionCharFormat(CHARFORMAT2 &cf)
```

 - 参&emsp;数：  
    - `cf` 返回获取的字符格式
 - 返回值：返回参数 cf 中 dwMask 的值

### SetSelectionCharFormat

设置被选择的字符格式

```cpp
bool SetSelectionCharFormat(CHARFORMAT2 &cf)
```

 - 参&emsp;数：  
    - `cf` 要设置的字符格式
 - 返回值：返回 true 表示成功，false 为失败

### SetWordCharFormat

设置当前插入点的单词格式

```cpp
bool SetWordCharFormat(CHARFORMAT2 &cf)
```

 - 参&emsp;数：  
    - `cf` 要设置的单词格式
 - 返回值：成功返回 true，失败返回 false

### GetParaFormat

获取当前段落格式

```cpp
DWORD GetParaFormat(PARAFORMAT2 &pf)
```

 - 参&emsp;数：  
    - `pf` 返回当前段落格式
 - 返回值：返回 pf 参数的 dwMask 成员

### SetParaFormat

设置当前段落格式

```cpp
bool SetParaFormat(PARAFORMAT2 &pf)
```

 - 参&emsp;数：  
    - `pf` 要设置的段落格式样式
 - 返回值：成功返回 true，否则返回 false

### Redo

恢复操作

```cpp
bool Redo()
```

 - 参&emsp;数：无  
 - 返回值：成功返回 true，失败返回 false

### Undo

撤销操作

```cpp
bool Undo()
```

 - 参&emsp;数：无  
 - 返回值：成功返回 true，失败返回 false

### Clear

清空

```cpp
void Clear()
```

 - 参&emsp;数：无  
 - 返回值：无

### Copy

复制所选内容

```cpp
void Copy()
```

 - 参&emsp;数：无  
 - 返回值：无

### Cut

剪切所选内容

```cpp
void Cut()
```

 - 参&emsp;数：无  
 - 返回值：无

### Paste

粘贴

```cpp
void Paste()
```

 - 参&emsp;数：无  
 - 返回值：无

### CanPaste

检测是否可以粘贴指定剪切板格式

```cpp
BOOL CanPaste(UINT nFormat = 0)
```

 - 参&emsp;数：  
    - `nFormat` 要检测的格式
 - 返回值：可以返回 true，否则返回 false

### PasteSpecial

在控件中粘贴特定的剪贴板格式

```cpp
void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0)
```

 - 参&emsp;数：  
    - `uClipFormat` 指定剪切板格式
    - `dwAspect` 指定展示形式
    - `hMF` 如果 dwAspect 为 DVASPECT_ICON，该函数应该包含图标句柄
 - 返回值：无

### GetLineCount

获取总行数

```cpp
int GetLineCount()
```

 - 参&emsp;数：无  
 - 返回值：返回总行数

### GetLine

获取一行数据

```cpp
std::wstring GetLine(int nIndex, int nMaxLength)
```

 - 参&emsp;数：  
    - `nIndex` 要获取的行数
    - `nMaxLength` 要获取当前行最大的数据长度
 - 返回值：返回获取的一行数据

### LineIndex

获取指定行的第一个字符索引

```cpp
int LineIndex(int nLine = -1)
```

 - 参&emsp;数：  
    - `nLine` 要获取第几行数据，默认为 -1
 - 返回值：返回指定行的第一个字符索引

### LineLength

获取指定行的数据长度

```cpp
int LineLength(int nLine = -1)
```

 - 参&emsp;数：  
    - `nLine` 要获取第几行数据，默认为 -1
 - 返回值：返回指定行的数据长度

### LineScroll

滚动文本

```cpp
bool LineScroll(int nLines, int nChars = 0)
```

 - 参&emsp;数：  
    - `nLines` 指定垂直滚动方向
    - `nChars` 指定水平滚动方向
 - 返回值：成功返回 true，失败返回 false

### GetCharPos

获取指定位置字符的客户区坐标

```cpp
CPoint GetCharPos(long lChar)
```

 - 参&emsp;数：  
    - `lChar` 字符索引位置
 - 返回值：返回客户区坐标

### LineFromChar

获取指定字符所在行数

```cpp
long LineFromChar(long nIndex)
```

 - 参&emsp;数：  
    - `nIndex` 字符的索引位置
 - 返回值：返回当前字符所在的行数

### PosFromChar

获取指定位置字符的客户区坐标

```cpp
CPoint PosFromChar(UINT nChar)
```

 - 参&emsp;数：  
    - `nChar` 字符索引位置
 - 返回值：返回客户区坐标

### CharFromPos

根据坐标返回指定字符索引

```cpp
int CharFromPos(CPoint pt)
```

 - 参&emsp;数：  
    - `pt` 坐标信息
 - 返回值：返回最接近参数 pt 所指定的坐标位置

### EmptyUndoBuffer

清空撤销列表

```cpp
void EmptyUndoBuffer()
```

 - 参&emsp;数：无  
 - 返回值：无

### SetUndoLimit

设置撤销列表容纳的内容数量

```cpp
UINT SetUndoLimit(UINT nLimit)
```

 - 参&emsp;数：  
    - `nLimit
 - 返回值：返回设置后的撤销列表可容纳内容数量

### StreamIn

增加一个回调用于控制输入内容

```cpp
long StreamIn(int nFormat, EDITSTREAM &es)
```

 - 参&emsp;数：  
    - `nFormat` 指定数据格式的替换选项，见：https://docs.microsoft.com/en-us/windows/desktop/controls/em-streamin
    - `es` 包含自定义回调的结构体
 - 返回值：返回读入数据流的数据大小

### StreamOut

指定一个回调用于控制输出内容

```cpp
long StreamOut(int nFormat, EDITSTREAM &es)
```

 - 参&emsp;数：  
    - `nFormat` 指定数据格式的替换选项，见：https://docs.microsoft.com/en-us/windows/desktop/controls/em-streamin
    - `es` 包含自定义回调的结构体
 - 返回值：返回写入数据流的数据大小

### SetScrollPos

设置滚动条位置

```cpp
void SetScrollPos(CSize szPos)
```

 - 参&emsp;数：  
    - `szPos` 要设置的滚动条位置信息
 - 返回值：无

### LineUp

向上一行

```cpp
void LineUp()
```

 - 参&emsp;数：无  
 - 返回值：无

### LineDown

向下一行

```cpp
void LineDown()
```

 - 参&emsp;数：无  
 - 返回值：无

### PageUp

向上翻页

```cpp
void PageUp()
```

 - 参&emsp;数：无  
 - 返回值：无

### PageDown

向下翻页

```cpp
void PageDown()
```

 - 参&emsp;数：无  
 - 返回值：无

### HomeUp

返回到顶端

```cpp
void HomeUp()
```

 - 参&emsp;数：无  
 - 返回值：无

### EndDown

返回到底部

```cpp
void EndDown()
```

 - 参&emsp;数：无  
 - 返回值：无

### LineLeft

水平向左滚动

```cpp
void LineLeft()
```

 - 参&emsp;数：无  
 - 返回值：无

### LineRight

水平向右滚动

```cpp
void LineRight()
```

 - 参&emsp;数：无  
 - 返回值：无

### PageLeft

水平向左翻页

```cpp
void PageLeft()
```

 - 参&emsp;数：无  
 - 返回值：无

### PageRight

水平向右翻页

```cpp
void PageRight()
```

 - 参&emsp;数：无  
 - 返回值：无

### HomeLeft

返回到最左侧

```cpp
void HomeLeft()
```

 - 参&emsp;数：无  
 - 返回值：无

### EndRight

返回到最后侧

```cpp
void EndRight()
```

 - 参&emsp;数：无  
 - 返回值：无

### CreateCaret

创建光标

```cpp
BOOL CreateCaret(INT xWidth, INT yHeight)
```

 - 参&emsp;数：  
    - `xWidth` 光标宽度
    - `yHeight` 光标高度
 - 返回值：成功返回 true，失败返回 false

### ShowCaret

设置是否显示光标

```cpp
BOOL ShowCaret(BOOL fShow)
```

 - 参&emsp;数：  
    - `fShow` 设置 true 为显示，false 为不显示
 - 返回值：成功返回 true，失败返回 false

### SetCaretColor

设置光标颜色

```cpp
void SetCaretColor(const std::wstring& dwColor)
```

 - 参&emsp;数：  
    - `dwColor` 要设置的颜色值，该值必须在 global.xml 中存在
 - 返回值：无

### GetCaretColor

获取光标颜色

```cpp
std::wstring GetCaretColor()
```

 - 参&emsp;数：无  
 - 返回值：返回光标颜色

### GetCaretRect

获取光标矩形位置

```cpp
RECT GetCaretRect()
```

 - 参&emsp;数：无  
 - 返回值：返回光标矩形位置

### SetCaretPos

设置光标位置

```cpp
BOOL SetCaretPos(INT x, INT y)
```

 - 参&emsp;数：  
    - `x` 轴坐标
    - `y` 轴坐标
 - 返回值：成功返回 true，失败返回 false

### ChangeCaretVisiable

切换光标是否显示

```cpp
void ChangeCaretVisiable()
```

 - 参&emsp;数：无  
 - 返回值：无

### PaintCaret

绘制光标

```cpp
void PaintCaret(IRenderContext* pRender, const UiRect& rcPaint)
```

 - 参&emsp;数：  
    - `pRender` 绘制引擎
    - `rcPaint` 绘制位置
 - 返回值：无

### SetPromptMode

设置是否显示提示文字

```cpp
void SetPromptMode(bool bPrompt)
```

 - 参&emsp;数：  
    - `bPrompt` 设置为 true 为显示，false 为不显示
 - 返回值：无

### GetPromptText

获取提示文字

```cpp
std::wstring GetPromptText()
```

 - 参&emsp;数：无  
 - 返回值：返回提示文字内容

### GetUTF8PromptText

获取提示文字

```cpp
std::string GetUTF8PromptText()
```

 - 参&emsp;数：无  
 - 返回值：返回 UTF8 格式的提示文字

### SetPromptText

设置提示文字

```cpp
void SetPromptText(const std::wstring& strText)
```

 - 参&emsp;数：  
    - `strText` 要设置的提示文字
 - 返回值：无

### SetUTF8PromptText

设置提示文字

```cpp
void SetUTF8PromptText(const std::string& strText)
```

 - 参&emsp;数：  
    - `strText` 要设置的 UTF8 格式提示文字
 - 返回值：无

### SetPromptTextId

设置提示文字 ID

```cpp
void SetPromptTextId(const std::wstring& strTextId)
```

 - 参&emsp;数：  
    - `strText` 要设置的提示文字 ID，该 ID 必须在加载的语言文件中存在
 - 返回值：无

### SetUTF8PromptTextId

设置提示文字 ID

```cpp
void SetUTF8PromptTextId(const std::string& strTextId)
```

 - 参&emsp;数：  
    - `strText` 要设置的 UTF8 格式提示文字 ID，该 ID 必须在加载的语言文件中存在
 - 返回值：无

### PaintPromptText

绘制提示文字

```cpp
void PaintPromptText(IRenderContext* pRender)
```

 - 参&emsp;数：  
    - `pRender` 绘制引擎
 - 返回值：无

### GetFocusedImage

获取焦点状态下的图片

```cpp
std::wstring GetFocusedImage()
```

 - 参&emsp;数：无  
 - 返回值：返回焦点状态下的图片

### SetFocusedImage

设置焦点状态下的图片

```cpp
void SetFocusedImage(const std::wstring& strImage)
```

 - 参&emsp;数：  
    - `strImage` 要设置的图片位置
 - 返回值：无

### PaintStatusImage

绘制指定状态下的图片

```cpp
virtual void PaintStatusImage(IRenderContext* pRender)
```

 - 参&emsp;数：  
    - `pRender` 绘制引擎
 - 返回值：无

### SetNoSelOnKillFocus

设置失去焦点后是否取消选择项

```cpp
void SetNoSelOnKillFocus(bool bOnSel)
```

 - 参&emsp;数：  
    - `bOnSel` 设置为 true 表示取消选择项，false 为不取消
 - 返回值：无

### SetSelAllOnFocus

设置获取焦点后是否选择所有内容

```cpp
void SetSelAllOnFocus(bool bSelAll)
```

 - 参&emsp;数：  
    - `bSelAll` 设置 true 表示在获取焦点时选择所有内容，false 为不选择
 - 返回值：无

### SetNoCaretReadonly

设置只读模式不显示光标

```cpp
void SetNoCaretReadonly()
```

 - 参&emsp;数：无  
 - 返回值：无

### AddColorText

添加带颜色的文本

```cpp
void AddColorText(const std::wstring &str, const std::wstring &color)
```

 - 参&emsp;数：  
    - `str` 文本内容
    - `color` 颜色值，该值必须在 global.xml 中存在
 - 返回值：无

### AddLinkColorText

添加一个带有文字颜色的超链接

```cpp
void AddLinkColorText(const std::wstring &str, const std::wstring &color, const std::wstring &linkInfo = L"")
```

 - 参&emsp;数：  
    - `str` 文字内容
    - `color` 文字颜色
    - `linkInfo` 链接地址
 - 返回值：无

### AddLinkColorTextEx

添加一个指定字体带有文字颜色的超链接

```cpp
void AddLinkColorTextEx(const std::wstring& str, const std::wstring &color, const std::wstring &linkInfo = L"", int font = -1)
```

 - 参&emsp;数：  
    - `str` 文字内容
    - `color` 文字颜色
    - `linkInfo` 链接地址
    - `font` 字体索引
 - 返回值：无

### AddLinkInfo

添加一个范围用于 hittest 判断是否是链接信息

```cpp
void AddLinkInfo(const CHARRANGE cr, const std::wstring &linkInfo)
```

 - 参&emsp;数：  
    - `cr` 范围的起始位置和结束位置
    - `linkInfo` 自定义 link 属性
 - 返回值：无

### HittestCustomLink

根据point来hittest自定义link的数据

```cpp
bool HittestCustomLink(CPoint pt, std::wstring& info)
```

 - 参&emsp;数：  
    - `pt` 位置信息
    - `info` 表示 link 的自定义属性
 - 返回值：返回 true 表示在 link 上

### ClearImageCache

清理图片缓存

```cpp
virtual void ClearImageCache()
```

 - 参&emsp;数：无  
 - 返回值：无

### AttachReturn

监听回车按键按下事件

```cpp
void AttachReturn(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 回车被按下的自定义回调函数
 - 返回值：无

### AttachTab

监听 TAB 按键按下事件

```cpp
void AttachTab(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` TAB 被按下的自定义回调函数
 - 返回值：无

### AttachTextChange

监听文本被修改事件

```cpp
void AttachTextChange(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 文本被修改后的自定义回调函数
 - 返回值：无

### AttachCustomLinkClk

监听自定义链接被点击事件

```cpp
void AttachCustomLinkClk(const EventCallback& callback)
```

 - 参&emsp;数：  
    - `callback` 自定义链接被点击后的自定义回调函数
 - 返回值：无

### AttachGetNaturalSize

监听大小变化事件

```cpp
void AttachGetNaturalSize(const FunGetNaturalSize& callback)
```

 - 参&emsp;数：  
    - `callback` 大小被改变后的自定义回调函数
 - 返回值：无
