#ifndef UI_CONTROL_RICHEDIT_H_
#define UI_CONTROL_RICHEDIT_H_

#pragma once

#include <Imm.h>
#include <textserv.h>
#include <richedit.h>
#include <RichOle.h>

namespace ui 
{

class CTxtWinHost;
class UILIB_API RichEdit : public ScrollableBox
{
public:
	friend class CTxtWinHost;
	typedef std::function<bool(LONG, LONG, CSize&)> FunGetNaturalSize;
public:
	RichEdit();
	RichEdit(const RichEdit& r) = delete;
	RichEdit& operator=(const RichEdit& r) = delete;
    virtual ~RichEdit();

	/**
	 * @brief 判断是否接受 TAB 按键消息
	 * @return 返回 true 表示接受，false 表示不接受
	 */
    bool IsWantTab();

	/**
	 * @brief 判断是否接受 TAB 按键消息
	 * @param[in] bWantTab 为 true 则接受该消息，false 为不接受
	 * @return 无
	 */
    void SetWantTab(bool bWantTab = true);

	/**
	 * @brief 判断是否接受 RETURN 按键消息
	 * @return 返回 true 表示接受，false 表示不接受
	 */
    bool IsNeedReturnMsg();

	/**
	 * @brief 设置是否接受 RETURN 按键消息
	 * @param[in] bNeedReturnMsg 为 true 则接受该消息，false 为不接受
	 * @return 无
	 */
    void SetNeedReturnMsg(bool bNeedReturnMsg = true);

	/**
	 * @brief 判断是否接受CTRL+RETURN 组合键消息
	 * @return 返回 true 表示接受，false 表示不接受
	 */
    bool IsReturnMsgWantCtrl();

	/**
	 * @brief 设置是否接受CTRL+RETUREN 组合键消息
	 * @param[in] bReturnMsgWantCtrl 为 true 则接受该消息，false 为不接受
	 * @return 无
	 */
    void SetReturnMsgWantCtrl(bool bReturnMsgWantCtrl = true);

	/**
	 * @brief 是否是富文本状态
	 * @return 返回 true 为是，否则为 false
	 */
    bool IsRich();

	/**
	 * @brief 设置控件为富文本编辑状态
	 * @param[in] bRich 设置 true 为富文本状态，false 为普通状态
	 * @return 无
	 */
    void SetRich(bool bRich = true);

	/**
	 * @brief 是否是只读状态
	 * @return 返回 true 为只读状态，否则为 false
	 */
    bool IsReadOnly();

	/**
	 * @brief 设置控件为只读状态
	 * @param[in] bReadOnly 设置 true 让控件变为只读状态，false 为可写入状态
	 * @return 无
	 */
    void SetReadOnly(bool bReadOnly = true);

	/**
	 * @brief 是否是密码状态控件
	 * @return 返回 true 表示是密码控件，否则为 false
	 */
	bool IsPassword();

	/**
	 * @brief 设置控件为密码控件（显示 ***）
	 * @param[in] bPassword 设置为 true 让控件显示内容为 ***，false 为显示正常内容
	 * @return 无
	 */
	void SetPassword(bool bPassword);

	/**
	 * @brief 获取超出矩形区域的文本显示方式
	 * @return 返回 true 时并且在多行模式下内容被换行显示，false 则表示截断显示
	 */
    bool GetWordWrap();

	/**
	 * @brief 设置超出矩形区域的文本显示方式
	 * @param[in] bWordWrap 为 true 时并且在多行模式下内容被换行显示，false 则表示截断显示
	 * @return 无
	 */
    void SetWordWrap(bool bWordWrap = true);

	/**
	 * @brief 获取当前设置的字体索引
	 * @return 返回字体索引（对应 global.xml 中字体的顺序）
	 */
	std::wstring GetFont() const;

	/**
	 * @brief 设置字体索引
	 * @param[in] index 要设置的字体索引（对应 global.xml 中字体的顺序）
	 * @return 无
	 */
	void SetFont(const std::wstring& strFontId);
	void SetFont(HFONT font);
	/**
	 * @brief 根据字体名称设置字体
	 * @param[in] pStrFontName 字体名称
	 * @param[in] nSize 字体大小
	 * @param[in] bBold 是否粗体显示
	 * @param[in] bUnderline 是否带有下划线
	 * @param[in] bStrikeout 是否带有删除线
	 * @param[in] bItalic 是否斜体显示
	 * @return 无
	 */
	void SetFont(const std::wstring& pStrFontName, int nSize, bool bBold, bool bUnderline, bool bStrikeout, bool bItalic);

	/**
	 * @brief 获取窗口样式
	 * @return 返回窗口样式
	 */
    LONG GetWinStyle();

	/**
	 * @brief 设置窗口样式
	 * @param[in] lStyle 要设置的窗口样式
	 * @return 无
	 */
    void SetWinStyle(LONG lStyle);

	/**
	 * @brief 获取内容垂直对其方式
	 * @return 返回内容垂直对其方式（顶端对齐、居中、底端对齐）
	 */
	VerAlignType GetTextVerAlignType();

	/**
	 * @brief 设置文本颜色
	 * @param[in] dwTextColor 要设置的文本颜色，该颜色必须在 global.xml 中存在
	 * @return 无
	 */
	virtual void SetTextColor(const std::wstring& dwTextColor);
	void SetTextColor(DWORD color);

	/**
	 * @brief 获取文本颜色
	 * @return 返回当前文本颜色
	 */
	std::wstring GetTextColor();
	DWORD GetTextColorValue();

	/**
	 * @brief 获取限制字符数量
	 * @return 返回限制字符数量
	 */
    int GetLimitText();

	/**
	 * @brief 设置限制字符数量
	 * @param[in] iChars 要限制的字符数量
	 * @return 无
	 */
    void SetLimitText(int iChars);

	/**
	 * @brief 获取内容的长度
	 * @param[in] dwFlags 指定用以确定文本长度的方式，参考 https://docs.microsoft.com/en-us/windows/desktop/controls/em-gettextlengthex
	 * @return 返回内容长度
	 */
    long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;

	/**
	 * @brief 获取控件中的文本
	 * @return 返回控件中的文本内容
	 */
    std::wstring GetText() const;

	/**
	 * @brief 获取控件中的文本
	 * @return 返回控件中的文本内容 UTF8 格式
	 */
	virtual std::string GetUTF8Text() const;

	/**
	 * @brief 设置控件的文本
	 * @param[in] strText 要设置的文本内容
	 * @return 无
	 */
    void SetText(const std::wstring& strText);

	/**
	 * @brief 设置控件的文本对应 ID
	 * @param[in] strTextId 要设置的 ID，该 ID 必须在加载的语言文件中存在
	 * @return 无
	 */
	void SetTextId(const std::wstring& strTextId);

	/**
	 * @brief 设置控件的文本对应 ID
	 * @param[in] strTextId 要设置的 UTF8 格式 ID，该 ID 必须在加载的语言文件中存在
	 * @return 无
	 */
	virtual void SetUTF8Text(const std::string& strText);

	/**
	 * @brief 获取修改标志
	 * @return 返回 true 为设置了修改标志，否则为 false
	 */
    bool GetModify() const;

	/**
	 * @brief 设置修改标志
	 * @param[in] bModified 设置为 true 表示文本已经被修改，false 为未修改，默认为 true
	 * @return 无
	 */
    void SetModify(bool bModified = true) const;

	/**
	 * @brief 获取所选文本的起始位置和结束位置
	 * @param[out] cr 返回起始位置和结束位置
	 * @return 无
	 */
    void GetSel(CHARRANGE &cr) const;

	/**
	 * @brief 获取所选文本的起始位置和结束位置
	 * @param[in] nStartChar 返回起始位置
	 * @param[in] nEndChar 返回结束位置
	 * @return 无
	 */
    void GetSel(long& nStartChar, long& nEndChar) const;

	/**
	 * @brief 选择一部分内容
	 * @param[in] cr 要选择的文字起始位置和结束位置
	 * @return 返回选择的文字数量
	 */
    int SetSel(CHARRANGE &cr);

	/**
	 * @brief 选择一部分内容
	 * @param[in] nStartChar 要选择的起始位置
	 * @param[in] nEndChar 要选择的结束位置
	 * @return 返回选择的文字数量
	 */
    int SetSel(long nStartChar, long nEndChar);

	/**
	 * @brief 替换所选内容
	 * @param[in] lpszNewText 要替换的文字
	 * @param[in] bCanUndo 是否可以撤销，true 为可以，否则为 false
	 * @return 无
	 */
    void ReplaceSel(const std::wstring& lpszNewText, bool bCanUndo);

	/**
	 * @brief 替换所选内容
	 * @param[in] lpszNewText 要替换的文字
	 * @param[in] bCanUndo 是否可以撤销，true 为可以，否则为 false
	 * @return 无
	 */
    void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);

	/**
	 * @brief 获取所选文字内容
	 * @return 返回所选文字内容
	 */
    std::wstring GetSelText() const;

	/**
	 * @brief 全选
	 * @return 返回选择的内容数量
	 */
    int SetSelAll();

	/**
	 * @brief 不选择任何内容
	 * @return 始终返回 0 
	 */
    int SetSelNone();

	/**
	 * @brief 获取控件的选择类型
	 * @return 返回控件的选择类型，参考：https://docs.microsoft.com/en-us/windows/desktop/controls/em-selectiontype
	 */
    WORD GetSelectionType() const;

	/**
	 * @brief 获取当前缩放比
	 * @param[out] nNum 缩放比率分子
	 * @param[out] nDen 缩放比率分母
	 * @return 如果处理了消息则返回 TRUE
	 */
    bool GetZoom(int& nNum, int& nDen) const;

	/**
	 * @brief 设置缩放比
	 * @param[in] nNum 缩放比率分子
	 * @param[in] nDen 缩放比率分母
	 * @return 成功返回 true，失败返回 false
	 */
    bool SetZoom(int nNum, int nDen);

	/**
	 * @brief 恢复缩放到初始状态
	 * @return 成功返回 true，否则返回 false
	 */
    bool SetZoomOff();

	/**
	 * @brief 获取是否开启了自动 URL 检测
	 * @return 返回 true 表示开启了自动检测，否则为 false
	 */
    bool GetAutoURLDetect() const;

	/**
	 * @brief 设置是否开启 URL 自动检测
	 * @param[in] bAutoDetect 设置为 true 表示检测 URL，false 为不检测，默认为 true
	 * @return 返回 true 为设置成功，false 为失败
	 */
    bool SetAutoURLDetect(bool bAutoDetect = true);

	/**
	 * @brief 获取控件的事件掩码
	 * @return 返回事件掩码
	 */
    DWORD GetEventMask() const;

	/**
	 * @brief 设置控件的事件掩码
	 * @param[in] dwEventMask 要设置的事件掩码值
	 * @return 返回设置之前的事件掩码值
	 */
    DWORD SetEventMask(DWORD dwEventMask);

	/**
	 * @brief 获取指定范围的内容
	 * @param[in] nStartChar 起始位置
	 * @param[in] nEndChar 结束为止
	 * @return 返回设置的指定位置的内容
	 */
    std::wstring GetTextRange(long nStartChar, long nEndChar) const;

	/**
	 * @brief 设置是否隐藏或显示选择的值
	 * @param[in] bHide 是否显示，true 为显示，false 为不显示
	 * @param[in] bChangeStyle 是否修改样式，true 为修改，false 为不修改
	 * @return 无
	 */
    void HideSelection(bool bHide = true, bool bChangeStyle = false);

	/**
	 * @brief 设置光标到可见位置
	 * @return 无
	 */
    void ScrollCaret();

	/**
	 * @brief 插入文字
	 * @param[in] nInsertAfterChar 要插入的位置
	 * @param[in] lpstrText 要插入的文本
	 * @param[in] bCanUndo 是否可以撤销，true 为可以，否则为 false，默认为 false
	 * @return 返回插入后的文本位置
	 */
    int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);

	/**
	 * @brief 追加文字
	 * @param[in] strText 要追加的文字
	 * @param[in] bCanUndo 是否可以撤销，true 为可以，否则为 false，默认为 false
	 * @return 返回追加后的文字位置
	 */
    int AppendText(const std::wstring& strText, bool bCanUndo = false);

	/**
	 * @brief 获取字符格式
	 * @param[out] cf 返回获取的字符格式
	 * @return 返回参数 cf 中 dwMask 的值
	 */
    DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;

	/**
	 * @brief 设置默认的字符格式
	 * @param[in] cf 要设置字符格式
	 * @return 返回 true 表示成功，false 为失败
	 */
    bool SetDefaultCharFormat(CHARFORMAT2 &cf);

	/**
	 * @brief 获取被选择的字符格式
	 * @param[out] cf 返回获取的字符格式
	 * @return 返回参数 cf 中 dwMask 的值
	 */
    DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;

	/**
	 * @brief 设置被选择的字符格式
	 * @param[in] cf 要设置的字符格式
	 * @return 返回 true 表示成功，false 为失败
	 */
    bool SetSelectionCharFormat(CHARFORMAT2 &cf);

	/**
	 * @brief 设置当前插入点的单词格式
	 * @param[in] cf 要设置的单词格式
	 * @return 成功返回 true，失败返回 false
	 */
    bool SetWordCharFormat(CHARFORMAT2 &cf);

	/**
	 * @brief 获取当前段落格式
	 * @param[out] pf 返回当前段落格式
	 * @return 返回 pf 参数的 dwMask 成员
	 */
    DWORD GetParaFormat(PARAFORMAT2 &pf) const;

	/**
	 * @brief 设置当前段落格式
	 * @param[in] pf 要设置的段落格式样式
	 * @return 成功返回 true，否则返回 false
	 */
    bool SetParaFormat(PARAFORMAT2 &pf);

	/**
	 * @brief 恢复操作
	 * @return 成功返回 true，失败返回 false
	 */
    bool Redo();

	/**
	 * @brief 撤销操作
	 * @return 成功返回 true，失败返回 false
	 */
    bool Undo();

	/**
	 * @brief 清空
	 * @return 无
	 */
    void Clear();

	/**
	 * @brief 复制所选内容
	 * @return 无
	 */
    void Copy();

	/**
	 * @brief 剪切所选内容
	 * @return 无
	 */
    void Cut();

	/**
	 * @brief 粘贴
	 * @return 无
	 */
	void Paste();

	/**
	 * @brief 检测是否可以粘贴指定剪切板格式
	 * @param[in] nFormat 要检测的格式
	 * @return 可以返回 true，否则返回 false
	 */
	BOOL CanPaste(UINT nFormat = 0);

	/**
	 * @brief 在控件中粘贴特定的剪贴板格式
	 * @param[in] uClipFormat 指定剪切板格式
	 * @param[in] dwAspect 指定展示形式
	 * @param[in] hMF 如果 dwAspect 为 DVASPECT_ICON，该函数应该包含图标句柄
	 * @return 无
	 */
	void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0);

	/**
	 * @brief 获取总行数
	 * @return 返回总行数
	 */
    int GetLineCount() const;

	/**
	 * @brief 获取一行数据
	 * @param[in] nIndex 要获取的行数
	 * @param[in] nMaxLength 要获取当前行最大的数据长度
	 * @return 返回获取的一行数据
	 */
    std::wstring GetLine(int nIndex, int nMaxLength) const;

	/**
	 * @brief 获取指定行的第一个字符索引
	 * @param[in] nLine 要获取第几行数据，默认为 -1
	 * @return 返回指定行的第一个字符索引
	 */
    int LineIndex(int nLine = -1) const;

	/**
	 * @brief 获取指定行的数据长度
	 * @param[in] nLine 要获取第几行数据，默认为 -1
	 * @return 返回指定行的数据长度
	 */
    int LineLength(int nLine = -1) const;

	/**
	 * @brief 滚动文本
	 * @param[in] nLines 指定垂直滚动方向
	 * @param[in] nChars 指定水平滚动方向
	 * @return 成功返回 true，失败返回 false
	 */
    bool LineScroll(int nLines, int nChars = 0);

	/**
	 * @brief 获取指定位置字符的客户区坐标
	 * @param[in] lChar 字符索引位置
	 * @return 返回客户区坐标
	 */
	CPoint GetCharPos(long lChar) const;

	/**
	 * @brief 获取指定字符所在行数
	 * @param[in] nIndex 字符的索引位置
	 * @return 返回当前字符所在的行数
	 */
    long LineFromChar(long nIndex) const;

	/**
	 * @brief 获取指定位置字符的客户区坐标
	 * @param[in] nChar 字符索引位置
	 * @return 返回客户区坐标
	 */
    CPoint PosFromChar(UINT nChar) const;

	/**
	 * @brief 根据坐标返回指定字符索引
	 * @param[in] pt 坐标信息
	 * @return 返回最接近参数 pt 所指定的坐标位置
	 */
	int CharFromPos(CPoint pt) const;

	/**
	 * @brief 清空撤销列表
	 * @return 无
	 */
    void EmptyUndoBuffer();

	/**
	 * @brief 设置撤销列表容纳的内容数量
	 * @param[in] nLimit
	 * @return 返回设置后的撤销列表可容纳内容数量
	 */
    UINT SetUndoLimit(UINT nLimit);

	/**
	 * @brief 增加一个回调用于控制输入内容
	 * @param[in] nFormat 指定数据格式的替换选项，见：https://docs.microsoft.com/en-us/windows/desktop/controls/em-streamin
	 * @param[in] es 包含自定义回调的结构体
	 * @return 返回读入数据流的数据大小
	 */
    long StreamIn(int nFormat, EDITSTREAM &es);

	/**
	 * @brief 指定一个回调用于控制输出内容
	 * @param[in] nFormat 指定数据格式的替换选项，见：https://docs.microsoft.com/en-us/windows/desktop/controls/em-streamin
	 * @param[in] es 包含自定义回调的结构体
	 * @return 返回写入数据流的数据大小
	 */
    long StreamOut(int nFormat, EDITSTREAM &es);

    // 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
    // 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换	
    virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
	IDropTarget* GetTxDropTarget();
	bool SetDropAcceptFile(bool bAccept);
	virtual void OnTxNotify(DWORD iNotify, void *pv);
    virtual bool OnTxTextChanged();
	ITextHost* GetTextHost();
	ITextServices* GetTextServices();
	HWND GetWindowHandle();
	HDC GetWindowDC();
	BOOL SetOleCallback(IRichEditOleCallback* pCallback);
	CSize GetNaturalSize(LONG width, LONG height);
	void SetImmStatus(BOOL bOpen);
	void SetTimer(UINT idTimer, UINT uTimeout);
	void KillTimer(UINT idTimer);

	/**
	 * @brief 设置滚动条位置
	 * @param[in] szPos 要设置的滚动条位置信息
	 * @return 无
	 */
    void SetScrollPos(CSize szPos);

	/**
	 * @brief 向上一行
	 * @return 无
	 */
    void LineUp();

	/**
	 * @brief 向下一行
	 * @return 无
	 */
    void LineDown();

	/**
	 * @brief 向上翻页
	 * @return 无
	 */
    void PageUp();

	/**
	 * @brief 向下翻页
	 * @return 无
	 */
    void PageDown();

	/**
	 * @brief 返回到顶端
	 * @return 无
	 */
    void HomeUp();

	/**
	 * @brief 返回到底部
	 * @return 无
	 */
    void EndDown();

	/**
	 * @brief 水平向左滚动
	 * @return 无
	 */
    void LineLeft();

	/**
	 * @brief 水平向右滚动
	 * @return 无
	 */
    void LineRight();

	/**
	 * @brief 水平向左翻页
	 * @return 无
	 */
    void PageLeft();

	/**
	 * @brief 水平向右翻页
	 * @return 无
	 */
    void PageRight();

	/**
	 * @brief 返回到最左侧
	 * @return 无
	 */
    void HomeLeft();

	/**
	 * @brief 返回到最后侧
	 * @return 无
	 */
    void EndRight();

	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual void DoInit() override;
	virtual void SetEnabled(bool bEnable = true) override;
	virtual CSize EstimateSize(CSize szAvailable) override;
	virtual CSize EstimateText(CSize szAvailable);
	virtual void SetPos(UiRect rc) override;
	virtual UINT GetControlFlags() const override;
	virtual void HandleMessage(EventArgs& event) override;
	void OnSetCursor(EventArgs& event);
	void OnSetFocus(EventArgs& event);
	void OnKillFocus(EventArgs& event);
	void OnChar(EventArgs& event);
	void OnKeyDown(EventArgs& event);
	void OnImeStartComposition(EventArgs& event);
	void OnImeEndComposition(EventArgs& event);
	void OnMouseMessage(UINT uMsg, EventArgs& event);

	virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint) override;
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;
	virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;

	/**
	 * @brief 创建光标
	 * @param[in] xWidth 光标宽度
	 * @param[in] yHeight 光标高度
	 * @return 成功返回 true，失败返回 false
	 */
	BOOL CreateCaret(INT xWidth, INT yHeight);

	/**
	 * @brief 设置是否显示光标
	 * @param[in] fShow 设置 true 为显示，false 为不显示
	 * @return 成功返回 true，失败返回 false
	 */
	BOOL ShowCaret(BOOL fShow);

	/**
	 * @brief 设置光标颜色
	 * @param[in] dwColor 要设置的颜色值，该值必须在 global.xml 中存在
	 * @return 无
	 */
	void SetCaretColor(const std::wstring& dwColor);

	/**
	 * @brief 获取光标颜色
	 * @return 返回光标颜色
	 */
	std::wstring GetCaretColor();

	/**
	 * @brief 获取光标矩形位置
	 * @return 返回光标矩形位置
	 */
	RECT GetCaretRect();

	/**
	 * @brief 设置光标位置
	 * @param[in] x X 轴坐标
	 * @param[in] y Y 轴坐标
	 * @return 成功返回 true，失败返回 false
	 */
	BOOL SetCaretPos(INT x, INT y);

	/**
	 * @brief 切换光标是否显示
	 * @return 无
	 */
	void ChangeCaretVisiable();

	/**
	 * @brief 绘制光标
	 * @param[in] pRender 绘制引擎
	 * @param[in] rcPaint 绘制位置
	 * @return 无
	 */
	void PaintCaret(IRenderContext* pRender, const UiRect& rcPaint);

	/**
	 * @brief 设置是否显示提示文字
	 * @param[in] bPrompt 设置为 true 为显示，false 为不显示
	 * @return 无
	 */
	void SetPromptMode(bool bPrompt);

	/**
	 * @brief 获取提示文字
	 * @return 返回提示文字内容
	 */
	std::wstring GetPromptText() const;

	/**
	 * @brief 获取提示文字
	 * @return 返回 UTF8 格式的提示文字
	 */
	std::string GetUTF8PromptText() const;

	/**
	 * @brief 设置提示文字
	 * @param[in] strText 要设置的提示文字
	 * @return 无
	 */
	void SetPromptText(const std::wstring& strText);

	/**
	 * @brief 设置提示文字
	 * @param[in] strText 要设置的 UTF8 格式提示文字
	 * @return 无
	 */
	void SetUTF8PromptText(const std::string& strText);

	/**
	 * @brief 设置提示文字 ID
	 * @param[in] strText 要设置的提示文字 ID，该 ID 必须在加载的语言文件中存在
	 * @return 无
	 */
	void SetPromptTextId(const std::wstring& strTextId);

	/**
	 * @brief 设置提示文字 ID
	 * @param[in] strText 要设置的 UTF8 格式提示文字 ID，该 ID 必须在加载的语言文件中存在
	 * @return 无
	 */
	void SetUTF8PromptTextId(const std::string& strTextId);

	/**
	 * @brief 绘制提示文字
	 * @param[in] pRender 绘制引擎
	 * @return 无
	 */
	void PaintPromptText(IRenderContext* pRender);

	/**
	 * @brief 获取焦点状态下的图片
	 * @return 返回焦点状态下的图片
	 */
	std::wstring GetFocusedImage();

	/**
	 * @brief 设置焦点状态下的图片
	 * @param[in] strImage 要设置的图片位置
	 * @return 无
	 */
	void SetFocusedImage(const std::wstring& strImage);

	/**
	 * @brief 绘制指定状态下的图片
	 * @param[in] pRender 绘制引擎
	 * @return 无
	 */
	virtual void PaintStatusImage(IRenderContext* pRender) override;

	/**
	 * @brief 设置失去焦点后是否取消选择项
	 * @param[in] bOnSel 设置为 true 表示取消选择项，false 为不取消
	 * @return 无
	 */
	void SetNoSelOnKillFocus(bool bOnSel);

	/**
	 * @brief 设置获取焦点后是否选择所有内容
	 * @param[in] bSelAll 设置 true 表示在获取焦点时选择所有内容，false 为不选择
	 * @return 无
	 */
	void SetSelAllOnFocus(bool bSelAll);

	/**
	 * @brief 设置只读模式不显示光标
	 * @return 无
	 */
	void SetNoCaretReadonly();

	/**
	 * @brief 添加带颜色的文本
	 * @param[in] str 文本内容
	 * @param[in] color 颜色值，该值必须在 global.xml 中存在
	 * @return 无
	 */
	void AddColorText(const std::wstring &str, const std::wstring &color);

	/**
	 * @brief 添加一个带有文字颜色的超链接
	 * @param[in] str 文字内容
	 * @param[in] color 文字颜色
	 * @param[in] linkInfo 链接地址
	 * @return 无
	 */
	void AddLinkColorText(const std::wstring &str, const std::wstring &color, const std::wstring &linkInfo = L"");

	/**
	 * @brief 添加一个指定字体带有文字颜色的超链接
	 * @param[in] str 文字内容
	 * @param[in] color 文字颜色
	 * @param[in] linkInfo 链接地址
	 * @param[in] font 字体索引
	 * @return 无
	 */
	void AddLinkColorTextEx(const std::wstring& str, const std::wstring &color, const std::wstring &linkInfo = L"", const std::wstring& strFontId = L"");

	/**
	 * @brief 添加一个范围用于 hittest 判断是否是链接信息
	 * @param[in] cr 范围的起始位置和结束位置
	 * @param[in] linkInfo 自定义 link 属性
	 * @return 无
	 */
	void AddLinkInfo(const CHARRANGE cr, const std::wstring &linkInfo);

	/**
	 * @brief 添加一个范围用于 hittest 判断是否是链接信息,并将该范围内文字样式改为系统链接样式
	 * @param[in] str 文字内容
	 * @param[in] cr 范围的起始位置和结束位置
	 * @param[in] linkInfo 自定义 link 属性
	 * @return 无
	 */
	void AddLinkInfoEx(const CHARRANGE cr, const std::wstring& linkInfo);

	/**
	 * @brief 根据point来hittest自定义link的数据
	 * @param[in] pt 位置信息
	 * @param[in] info 表示 link 的自定义属性
	 * @return 返回 true 表示在 link 上
	 */
	bool HittestCustomLink(CPoint pt, std::wstring& info);

	/**
	 * @brief 清理图片缓存
	 * @return 无
	 */
	virtual void ClearImageCache() override;

	/**
	 * @brief 监听回车按键按下事件
	 * @param[in] callback 回车被按下的自定义回调函数
	 * @return 无
	 */
	void AttachReturn(const EventCallback& callback) { OnEvent[kEventReturn] += callback; }

	/**
	 * @brief 监听 TAB 按键按下事件
	 * @param[in] callback TAB 被按下的自定义回调函数
	 * @return 无
	 */
	void AttachTab(const EventCallback& callback) {	OnEvent[kEventTab] += callback;	}

	/**
	 * @brief 监听文本被修改事件
	 * @param[in] callback 文本被修改后的自定义回调函数
	 * @return 无
	 */
	void AttachTextChange(const EventCallback& callback) { OnEvent[kEventTextChange] += callback; }

	/**
	 * @brief 监听自定义链接被点击事件
	 * @param[in] callback 自定义链接被点击后的自定义回调函数
	 * @return 无
	 */
	void AttachCustomLinkClk(const EventCallback& callback)	{ OnEvent[kEventCustomLinkClick] += callback; }

	/**
	 * @brief 监听大小变化事件
	 * @param[in] callback 大小被改变后的自定义回调函数
	 * @return 无
	 */
	void AttachGetNaturalSize(const FunGetNaturalSize& callback) { m_cbGetNaturalSize = callback; };

	/**
	 * @brief 通知控件值发生变化
	 * @param[in] oldText 旧值
	 * @param[in] newText 新值
	 * @return 无
	 */
	void RaiseUIAValueEvent(const std::wstring oldText, const std::wstring newText);

protected:
    CTxtWinHost* m_pTwh;
    bool m_bVScrollBarFixing;
    bool m_bWantTab;
    bool m_bNeedReturnMsg;
    bool m_bReturnMsgWantCtrl;
    bool m_bRich;
    bool m_bReadOnly;
	bool m_bPassword;
    bool m_bWordWrap;
	bool m_bNumberOnly;
	bool m_bInited;
	bool m_bAllowPrompt;
	bool m_bSelAllEver;			//只在第一次时全选
	bool m_bNoSelOnKillFocus;	//针对 m_bEnabled && m_bReadOnly
	bool m_bSelAllOnFocus;		//针对 m_bEnabled && !m_bReadOnly
	bool m_bNoCaretReadonly;
	bool m_bIsCaretVisiable;
	bool m_bIsComposition;
	int	 m_iCaretPosX;
	int  m_iCaretPosY;
	int  m_iCaretWidth;
	int  m_iCaretHeight;
	std::wstring m_sFontId;
	int  m_iLimitText;
	LONG m_lTwhStyle;
	VerAlignType m_textVerAlignType;
	std::wstring m_sCurrentColor;
	std::wstring m_sTextColor;
	std::wstring m_sDisabledTextColor;
	std::wstring m_sPromptColor;
	std::wstring m_sCaretColor;
	std::wstring m_sText;
	std::wstring m_sPromptText;
	std::wstring m_sPromptTextId;
	nbase::WeakCallbackFlag m_drawCaretFlag;
	std::weak_ptr<nbase::WeakFlag> m_windowFlag; //记录所属窗体的flag
	FunGetNaturalSize m_cbGetNaturalSize;

protected:
	struct LinkInfo
	{
		CHARRANGE cr;
		std::wstring info;
	};
	std::map<UINT, nbase::WeakCallbackFlag> m_timeFlagMap;
	std::vector<LinkInfo> m_linkInfo;
	Image m_sFocusedImage;
};

//判断是否是字节： 可打印字符（0x20-0x7e）
bool IsAsciiChar(const wchar_t ch);
//获取字符串的字节数
int  GetAsciiCharNumber(const std::wstring &str);
//删除字符串中超过limit字节个数之后的字符
void LimitAsciiNumber(std::wstring &src, int limit);
//获取粘贴板字符串
void GetClipboardText(std::wstring &out);
//设置粘贴板字符串
void SetClipBoardText(const std::wstring &str);

} // namespace ui

#endif // UI_CONTROL_RICHEDIT_H_
