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
	 * @brief �ж��Ƿ���� TAB ������Ϣ
	 * @return ���� true ��ʾ���ܣ�false ��ʾ������
	 */
    bool IsWantTab();

	/**
	 * @brief �ж��Ƿ���� TAB ������Ϣ
	 * @param[in] bWantTab Ϊ true ����ܸ���Ϣ��false Ϊ������
	 * @return ��
	 */
    void SetWantTab(bool bWantTab = true);

	/**
	 * @brief �ж��Ƿ���� RETURN ������Ϣ
	 * @return ���� true ��ʾ���ܣ�false ��ʾ������
	 */
    bool IsNeedReturnMsg();

	/**
	 * @brief �����Ƿ���� RETURN ������Ϣ
	 * @param[in] bNeedReturnMsg Ϊ true ����ܸ���Ϣ��false Ϊ������
	 * @return ��
	 */
    void SetNeedReturnMsg(bool bNeedReturnMsg = true);

	/**
	 * @brief �ж��Ƿ����CTRL+RETURN ��ϼ���Ϣ
	 * @return ���� true ��ʾ���ܣ�false ��ʾ������
	 */
    bool IsReturnMsgWantCtrl();

	/**
	 * @brief �����Ƿ����CTRL+RETUREN ��ϼ���Ϣ
	 * @param[in] bReturnMsgWantCtrl Ϊ true ����ܸ���Ϣ��false Ϊ������
	 * @return ��
	 */
    void SetReturnMsgWantCtrl(bool bReturnMsgWantCtrl = true);

	/**
	 * @brief �Ƿ��Ǹ��ı�״̬
	 * @return ���� true Ϊ�ǣ�����Ϊ false
	 */
    bool IsRich();

	/**
	 * @brief ���ÿؼ�Ϊ���ı��༭״̬
	 * @param[in] bRich ���� true Ϊ���ı�״̬��false Ϊ��ͨ״̬
	 * @return ��
	 */
    void SetRich(bool bRich = true);

	/**
	 * @brief �Ƿ���ֻ��״̬
	 * @return ���� true Ϊֻ��״̬������Ϊ false
	 */
    bool IsReadOnly();

	/**
	 * @brief ���ÿؼ�Ϊֻ��״̬
	 * @param[in] bReadOnly ���� true �ÿؼ���Ϊֻ��״̬��false Ϊ��д��״̬
	 * @return ��
	 */
    void SetReadOnly(bool bReadOnly = true);

	/**
	 * @brief �Ƿ�������״̬�ؼ�
	 * @return ���� true ��ʾ������ؼ�������Ϊ false
	 */
	bool IsPassword();

	/**
	 * @brief ���ÿؼ�Ϊ����ؼ�����ʾ ***��
	 * @param[in] bPassword ����Ϊ true �ÿؼ���ʾ����Ϊ ***��false Ϊ��ʾ��������
	 * @return ��
	 */
	void SetPassword(bool bPassword);

	/**
	 * @brief ��ȡ��������������ı���ʾ��ʽ
	 * @return ���� true ʱ�����ڶ���ģʽ�����ݱ�������ʾ��false ���ʾ�ض���ʾ
	 */
    bool GetWordWrap();

	/**
	 * @brief ���ó�������������ı���ʾ��ʽ
	 * @param[in] bWordWrap Ϊ true ʱ�����ڶ���ģʽ�����ݱ�������ʾ��false ���ʾ�ض���ʾ
	 * @return ��
	 */
    void SetWordWrap(bool bWordWrap = true);

	/**
	 * @brief ��ȡ��ǰ���õ���������
	 * @return ����������������Ӧ global.xml �������˳��
	 */
	std::wstring GetFont() const;

	/**
	 * @brief ������������
	 * @param[in] index Ҫ���õ�������������Ӧ global.xml �������˳��
	 * @return ��
	 */
	void SetFont(const std::wstring& strFontId);
	void SetFont(HFONT font);
	/**
	 * @brief ��������������������
	 * @param[in] pStrFontName ��������
	 * @param[in] nSize �����С
	 * @param[in] bBold �Ƿ������ʾ
	 * @param[in] bUnderline �Ƿ�����»���
	 * @param[in] bStrikeout �Ƿ����ɾ����
	 * @param[in] bItalic �Ƿ�б����ʾ
	 * @return ��
	 */
	void SetFont(const std::wstring& pStrFontName, int nSize, bool bBold, bool bUnderline, bool bStrikeout, bool bItalic);

	/**
	 * @brief ��ȡ������ʽ
	 * @return ���ش�����ʽ
	 */
    LONG GetWinStyle();

	/**
	 * @brief ���ô�����ʽ
	 * @param[in] lStyle Ҫ���õĴ�����ʽ
	 * @return ��
	 */
    void SetWinStyle(LONG lStyle);

	/**
	 * @brief ��ȡ���ݴ�ֱ���䷽ʽ
	 * @return �������ݴ�ֱ���䷽ʽ�����˶��롢���С��׶˶��룩
	 */
	VerAlignType GetTextVerAlignType();

	/**
	 * @brief �����ı���ɫ
	 * @param[in] dwTextColor Ҫ���õ��ı���ɫ������ɫ������ global.xml �д���
	 * @return ��
	 */
	virtual void SetTextColor(const std::wstring& dwTextColor);
	void SetTextColor(DWORD color);

	/**
	 * @brief ��ȡ�ı���ɫ
	 * @return ���ص�ǰ�ı���ɫ
	 */
	std::wstring GetTextColor();
	DWORD GetTextColorValue();

	/**
	 * @brief ��ȡ�����ַ�����
	 * @return ���������ַ�����
	 */
    int GetLimitText();

	/**
	 * @brief ���������ַ�����
	 * @param[in] iChars Ҫ���Ƶ��ַ�����
	 * @return ��
	 */
    void SetLimitText(int iChars);

	/**
	 * @brief ��ȡ���ݵĳ���
	 * @param[in] dwFlags ָ������ȷ���ı����ȵķ�ʽ���ο� https://docs.microsoft.com/en-us/windows/desktop/controls/em-gettextlengthex
	 * @return �������ݳ���
	 */
    long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;

	/**
	 * @brief ��ȡ�ؼ��е��ı�
	 * @return ���ؿؼ��е��ı�����
	 */
    std::wstring GetText() const;

	/**
	 * @brief ��ȡ�ؼ��е��ı�
	 * @return ���ؿؼ��е��ı����� UTF8 ��ʽ
	 */
	virtual std::string GetUTF8Text() const;

	/**
	 * @brief ���ÿؼ����ı�
	 * @param[in] strText Ҫ���õ��ı�����
	 * @return ��
	 */
    void SetText(const std::wstring& strText);

	/**
	 * @brief ���ÿؼ����ı���Ӧ ID
	 * @param[in] strTextId Ҫ���õ� ID���� ID �����ڼ��ص������ļ��д���
	 * @return ��
	 */
	void SetTextId(const std::wstring& strTextId);

	/**
	 * @brief ���ÿؼ����ı���Ӧ ID
	 * @param[in] strTextId Ҫ���õ� UTF8 ��ʽ ID���� ID �����ڼ��ص������ļ��д���
	 * @return ��
	 */
	virtual void SetUTF8Text(const std::string& strText);

	/**
	 * @brief ��ȡ�޸ı�־
	 * @return ���� true Ϊ�������޸ı�־������Ϊ false
	 */
    bool GetModify() const;

	/**
	 * @brief �����޸ı�־
	 * @param[in] bModified ����Ϊ true ��ʾ�ı��Ѿ����޸ģ�false Ϊδ�޸ģ�Ĭ��Ϊ true
	 * @return ��
	 */
    void SetModify(bool bModified = true) const;

	/**
	 * @brief ��ȡ��ѡ�ı�����ʼλ�úͽ���λ��
	 * @param[out] cr ������ʼλ�úͽ���λ��
	 * @return ��
	 */
    void GetSel(CHARRANGE &cr) const;

	/**
	 * @brief ��ȡ��ѡ�ı�����ʼλ�úͽ���λ��
	 * @param[in] nStartChar ������ʼλ��
	 * @param[in] nEndChar ���ؽ���λ��
	 * @return ��
	 */
    void GetSel(long& nStartChar, long& nEndChar) const;

	/**
	 * @brief ѡ��һ��������
	 * @param[in] cr Ҫѡ���������ʼλ�úͽ���λ��
	 * @return ����ѡ�����������
	 */
    int SetSel(CHARRANGE &cr);

	/**
	 * @brief ѡ��һ��������
	 * @param[in] nStartChar Ҫѡ�����ʼλ��
	 * @param[in] nEndChar Ҫѡ��Ľ���λ��
	 * @return ����ѡ�����������
	 */
    int SetSel(long nStartChar, long nEndChar);

	/**
	 * @brief �滻��ѡ����
	 * @param[in] lpszNewText Ҫ�滻������
	 * @param[in] bCanUndo �Ƿ���Գ�����true Ϊ���ԣ�����Ϊ false
	 * @return ��
	 */
    void ReplaceSel(const std::wstring& lpszNewText, bool bCanUndo);

	/**
	 * @brief �滻��ѡ����
	 * @param[in] lpszNewText Ҫ�滻������
	 * @param[in] bCanUndo �Ƿ���Գ�����true Ϊ���ԣ�����Ϊ false
	 * @return ��
	 */
    void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);

	/**
	 * @brief ��ȡ��ѡ��������
	 * @return ������ѡ��������
	 */
    std::wstring GetSelText() const;

	/**
	 * @brief ȫѡ
	 * @return ����ѡ�����������
	 */
    int SetSelAll();

	/**
	 * @brief ��ѡ���κ�����
	 * @return ʼ�շ��� 0 
	 */
    int SetSelNone();

	/**
	 * @brief ��ȡ�ؼ���ѡ������
	 * @return ���ؿؼ���ѡ�����ͣ��ο���https://docs.microsoft.com/en-us/windows/desktop/controls/em-selectiontype
	 */
    WORD GetSelectionType() const;

	/**
	 * @brief ��ȡ��ǰ���ű�
	 * @param[out] nNum ���ű��ʷ���
	 * @param[out] nDen ���ű��ʷ�ĸ
	 * @return �����������Ϣ�򷵻� TRUE
	 */
    bool GetZoom(int& nNum, int& nDen) const;

	/**
	 * @brief �������ű�
	 * @param[in] nNum ���ű��ʷ���
	 * @param[in] nDen ���ű��ʷ�ĸ
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
    bool SetZoom(int nNum, int nDen);

	/**
	 * @brief �ָ����ŵ���ʼ״̬
	 * @return �ɹ����� true�����򷵻� false
	 */
    bool SetZoomOff();

	/**
	 * @brief ��ȡ�Ƿ������Զ� URL ���
	 * @return ���� true ��ʾ�������Զ���⣬����Ϊ false
	 */
    bool GetAutoURLDetect() const;

	/**
	 * @brief �����Ƿ��� URL �Զ����
	 * @param[in] bAutoDetect ����Ϊ true ��ʾ��� URL��false Ϊ����⣬Ĭ��Ϊ true
	 * @return ���� true Ϊ���óɹ���false Ϊʧ��
	 */
    bool SetAutoURLDetect(bool bAutoDetect = true);

	/**
	 * @brief ��ȡ�ؼ����¼�����
	 * @return �����¼�����
	 */
    DWORD GetEventMask() const;

	/**
	 * @brief ���ÿؼ����¼�����
	 * @param[in] dwEventMask Ҫ���õ��¼�����ֵ
	 * @return ��������֮ǰ���¼�����ֵ
	 */
    DWORD SetEventMask(DWORD dwEventMask);

	/**
	 * @brief ��ȡָ����Χ������
	 * @param[in] nStartChar ��ʼλ��
	 * @param[in] nEndChar ����Ϊֹ
	 * @return �������õ�ָ��λ�õ�����
	 */
    std::wstring GetTextRange(long nStartChar, long nEndChar) const;

	/**
	 * @brief �����Ƿ����ػ���ʾѡ���ֵ
	 * @param[in] bHide �Ƿ���ʾ��true Ϊ��ʾ��false Ϊ����ʾ
	 * @param[in] bChangeStyle �Ƿ��޸���ʽ��true Ϊ�޸ģ�false Ϊ���޸�
	 * @return ��
	 */
    void HideSelection(bool bHide = true, bool bChangeStyle = false);

	/**
	 * @brief ���ù�굽�ɼ�λ��
	 * @return ��
	 */
    void ScrollCaret();

	/**
	 * @brief ��������
	 * @param[in] nInsertAfterChar Ҫ�����λ��
	 * @param[in] lpstrText Ҫ������ı�
	 * @param[in] bCanUndo �Ƿ���Գ�����true Ϊ���ԣ�����Ϊ false��Ĭ��Ϊ false
	 * @return ���ز������ı�λ��
	 */
    int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);

	/**
	 * @brief ׷������
	 * @param[in] strText Ҫ׷�ӵ�����
	 * @param[in] bCanUndo �Ƿ���Գ�����true Ϊ���ԣ�����Ϊ false��Ĭ��Ϊ false
	 * @return ����׷�Ӻ������λ��
	 */
    int AppendText(const std::wstring& strText, bool bCanUndo = false);

	/**
	 * @brief ��ȡ�ַ���ʽ
	 * @param[out] cf ���ػ�ȡ���ַ���ʽ
	 * @return ���ز��� cf �� dwMask ��ֵ
	 */
    DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;

	/**
	 * @brief ����Ĭ�ϵ��ַ���ʽ
	 * @param[in] cf Ҫ�����ַ���ʽ
	 * @return ���� true ��ʾ�ɹ���false Ϊʧ��
	 */
    bool SetDefaultCharFormat(CHARFORMAT2 &cf);

	/**
	 * @brief ��ȡ��ѡ����ַ���ʽ
	 * @param[out] cf ���ػ�ȡ���ַ���ʽ
	 * @return ���ز��� cf �� dwMask ��ֵ
	 */
    DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;

	/**
	 * @brief ���ñ�ѡ����ַ���ʽ
	 * @param[in] cf Ҫ���õ��ַ���ʽ
	 * @return ���� true ��ʾ�ɹ���false Ϊʧ��
	 */
    bool SetSelectionCharFormat(CHARFORMAT2 &cf);

	/**
	 * @brief ���õ�ǰ�����ĵ��ʸ�ʽ
	 * @param[in] cf Ҫ���õĵ��ʸ�ʽ
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
    bool SetWordCharFormat(CHARFORMAT2 &cf);

	/**
	 * @brief ��ȡ��ǰ�����ʽ
	 * @param[out] pf ���ص�ǰ�����ʽ
	 * @return ���� pf ������ dwMask ��Ա
	 */
    DWORD GetParaFormat(PARAFORMAT2 &pf) const;

	/**
	 * @brief ���õ�ǰ�����ʽ
	 * @param[in] pf Ҫ���õĶ����ʽ��ʽ
	 * @return �ɹ����� true�����򷵻� false
	 */
    bool SetParaFormat(PARAFORMAT2 &pf);

	/**
	 * @brief �ָ�����
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
    bool Redo();

	/**
	 * @brief ��������
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
    bool Undo();

	/**
	 * @brief ���
	 * @return ��
	 */
    void Clear();

	/**
	 * @brief ������ѡ����
	 * @return ��
	 */
    void Copy();

	/**
	 * @brief ������ѡ����
	 * @return ��
	 */
    void Cut();

	/**
	 * @brief ճ��
	 * @return ��
	 */
	void Paste();

	/**
	 * @brief ����Ƿ����ճ��ָ�����а��ʽ
	 * @param[in] nFormat Ҫ���ĸ�ʽ
	 * @return ���Է��� true�����򷵻� false
	 */
	BOOL CanPaste(UINT nFormat = 0);

	/**
	 * @brief �ڿؼ���ճ���ض��ļ������ʽ
	 * @param[in] uClipFormat ָ�����а��ʽ
	 * @param[in] dwAspect ָ��չʾ��ʽ
	 * @param[in] hMF ��� dwAspect Ϊ DVASPECT_ICON���ú���Ӧ�ð���ͼ����
	 * @return ��
	 */
	void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0);

	/**
	 * @brief ��ȡ������
	 * @return ����������
	 */
    int GetLineCount() const;

	/**
	 * @brief ��ȡһ������
	 * @param[in] nIndex Ҫ��ȡ������
	 * @param[in] nMaxLength Ҫ��ȡ��ǰ���������ݳ���
	 * @return ���ػ�ȡ��һ������
	 */
    std::wstring GetLine(int nIndex, int nMaxLength) const;

	/**
	 * @brief ��ȡָ���еĵ�һ���ַ�����
	 * @param[in] nLine Ҫ��ȡ�ڼ������ݣ�Ĭ��Ϊ -1
	 * @return ����ָ���еĵ�һ���ַ�����
	 */
    int LineIndex(int nLine = -1) const;

	/**
	 * @brief ��ȡָ���е����ݳ���
	 * @param[in] nLine Ҫ��ȡ�ڼ������ݣ�Ĭ��Ϊ -1
	 * @return ����ָ���е����ݳ���
	 */
    int LineLength(int nLine = -1) const;

	/**
	 * @brief �����ı�
	 * @param[in] nLines ָ����ֱ��������
	 * @param[in] nChars ָ��ˮƽ��������
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
    bool LineScroll(int nLines, int nChars = 0);

	/**
	 * @brief ��ȡָ��λ���ַ��Ŀͻ�������
	 * @param[in] lChar �ַ�����λ��
	 * @return ���ؿͻ�������
	 */
	CPoint GetCharPos(long lChar) const;

	/**
	 * @brief ��ȡָ���ַ���������
	 * @param[in] nIndex �ַ�������λ��
	 * @return ���ص�ǰ�ַ����ڵ�����
	 */
    long LineFromChar(long nIndex) const;

	/**
	 * @brief ��ȡָ��λ���ַ��Ŀͻ�������
	 * @param[in] nChar �ַ�����λ��
	 * @return ���ؿͻ�������
	 */
    CPoint PosFromChar(UINT nChar) const;

	/**
	 * @brief �������귵��ָ���ַ�����
	 * @param[in] pt ������Ϣ
	 * @return ������ӽ����� pt ��ָ��������λ��
	 */
	int CharFromPos(CPoint pt) const;

	/**
	 * @brief ��ճ����б�
	 * @return ��
	 */
    void EmptyUndoBuffer();

	/**
	 * @brief ���ó����б����ɵ���������
	 * @param[in] nLimit
	 * @return �������ú�ĳ����б��������������
	 */
    UINT SetUndoLimit(UINT nLimit);

	/**
	 * @brief ����һ���ص����ڿ�����������
	 * @param[in] nFormat ָ�����ݸ�ʽ���滻ѡ�����https://docs.microsoft.com/en-us/windows/desktop/controls/em-streamin
	 * @param[in] es �����Զ���ص��Ľṹ��
	 * @return ���ض��������������ݴ�С
	 */
    long StreamIn(int nFormat, EDITSTREAM &es);

	/**
	 * @brief ָ��һ���ص����ڿ����������
	 * @param[in] nFormat ָ�����ݸ�ʽ���滻ѡ�����https://docs.microsoft.com/en-us/windows/desktop/controls/em-streamin
	 * @param[in] es �����Զ���ص��Ľṹ��
	 * @return ����д�������������ݴ�С
	 */
    long StreamOut(int nFormat, EDITSTREAM &es);

    // ע�⣺TxSendMessage��SendMessage��������ģ�TxSendMessageû��multibyte��unicode�Զ�ת���Ĺ��ܣ�
    // ��richedit2.0�ڲ�����unicodeʵ�ֵģ���multibyte�����У������Լ�����unicode��multibyte��ת��	
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
	 * @brief ���ù�����λ��
	 * @param[in] szPos Ҫ���õĹ�����λ����Ϣ
	 * @return ��
	 */
    void SetScrollPos(CSize szPos);

	/**
	 * @brief ����һ��
	 * @return ��
	 */
    void LineUp();

	/**
	 * @brief ����һ��
	 * @return ��
	 */
    void LineDown();

	/**
	 * @brief ���Ϸ�ҳ
	 * @return ��
	 */
    void PageUp();

	/**
	 * @brief ���·�ҳ
	 * @return ��
	 */
    void PageDown();

	/**
	 * @brief ���ص�����
	 * @return ��
	 */
    void HomeUp();

	/**
	 * @brief ���ص��ײ�
	 * @return ��
	 */
    void EndDown();

	/**
	 * @brief ˮƽ�������
	 * @return ��
	 */
    void LineLeft();

	/**
	 * @brief ˮƽ���ҹ���
	 * @return ��
	 */
    void LineRight();

	/**
	 * @brief ˮƽ����ҳ
	 * @return ��
	 */
    void PageLeft();

	/**
	 * @brief ˮƽ���ҷ�ҳ
	 * @return ��
	 */
    void PageRight();

	/**
	 * @brief ���ص������
	 * @return ��
	 */
    void HomeLeft();

	/**
	 * @brief ���ص�����
	 * @return ��
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

	virtual void Paint(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;
	virtual void PaintChild(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;
	virtual void SetAttribute(const std::wstring& pstrName, const std::wstring& pstrValue) override;

	/**
	 * @brief �������
	 * @param[in] xWidth �����
	 * @param[in] yHeight ���߶�
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	BOOL CreateCaret(INT xWidth, INT yHeight);

	/**
	 * @brief �����Ƿ���ʾ���
	 * @param[in] fShow ���� true Ϊ��ʾ��false Ϊ����ʾ
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	BOOL ShowCaret(BOOL fShow);

	/**
	 * @brief ���ù����ɫ
	 * @param[in] dwColor Ҫ���õ���ɫֵ����ֵ������ global.xml �д���
	 * @return ��
	 */
	void SetCaretColor(const std::wstring& dwColor);

	/**
	 * @brief ��ȡ�����ɫ
	 * @return ���ع����ɫ
	 */
	std::wstring GetCaretColor();

	/**
	 * @brief ��ȡ������λ��
	 * @return ���ع�����λ��
	 */
	RECT GetCaretRect();

	/**
	 * @brief ���ù��λ��
	 * @param[in] x X ������
	 * @param[in] y Y ������
	 * @return �ɹ����� true��ʧ�ܷ��� false
	 */
	BOOL SetCaretPos(INT x, INT y);

	/**
	 * @brief �л�����Ƿ���ʾ
	 * @return ��
	 */
	void ChangeCaretVisiable();

	/**
	 * @brief ���ƹ��
	 * @param[in] pRender ��������
	 * @param[in] rcPaint ����λ��
	 * @return ��
	 */
	void PaintCaret(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint);

	/**
	 * @brief �����Ƿ���ʾ��ʾ����
	 * @param[in] bPrompt ����Ϊ true Ϊ��ʾ��false Ϊ����ʾ
	 * @return ��
	 */
	void SetPromptMode(bool bPrompt);

	/**
	 * @brief ��ȡ��ʾ����
	 * @return ������ʾ��������
	 */
	std::wstring GetPromptText() const;

	/**
	 * @brief ��ȡ��ʾ����
	 * @return ���� UTF8 ��ʽ����ʾ����
	 */
	std::string GetUTF8PromptText() const;

	/**
	 * @brief ������ʾ����
	 * @param[in] strText Ҫ���õ���ʾ����
	 * @return ��
	 */
	void SetPromptText(const std::wstring& strText);

	/**
	 * @brief ������ʾ����
	 * @param[in] strText Ҫ���õ� UTF8 ��ʽ��ʾ����
	 * @return ��
	 */
	void SetUTF8PromptText(const std::string& strText);

	/**
	 * @brief ������ʾ���� ID
	 * @param[in] strText Ҫ���õ���ʾ���� ID���� ID �����ڼ��ص������ļ��д���
	 * @return ��
	 */
	void SetPromptTextId(const std::wstring& strTextId);

	/**
	 * @brief ������ʾ���� ID
	 * @param[in] strText Ҫ���õ� UTF8 ��ʽ��ʾ���� ID���� ID �����ڼ��ص������ļ��д���
	 * @return ��
	 */
	void SetUTF8PromptTextId(const std::string& strTextId);

	/**
	 * @brief ������ʾ����
	 * @param[in] pRender ��������
	 * @return ��
	 */
	void PaintPromptText(dui::common::dui_refptr<dui::render::IRenderContext> pRender);

	/**
	 * @brief ��ȡ����״̬�µ�ͼƬ
	 * @return ���ؽ���״̬�µ�ͼƬ
	 */
	std::wstring GetFocusedImage();

	/**
	 * @brief ���ý���״̬�µ�ͼƬ
	 * @param[in] strImage Ҫ���õ�ͼƬλ��
	 * @return ��
	 */
	void SetFocusedImage(const std::wstring& strImage);

	/**
	 * @brief ����ָ��״̬�µ�ͼƬ
	 * @param[in] pRender ��������
	 * @return ��
	 */
	virtual void PaintStatusImage(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;

	/**
	 * @brief ����ʧȥ������Ƿ�ȡ��ѡ����
	 * @param[in] bOnSel ����Ϊ true ��ʾȡ��ѡ���false Ϊ��ȡ��
	 * @return ��
	 */
	void SetNoSelOnKillFocus(bool bOnSel);

	/**
	 * @brief ���û�ȡ������Ƿ�ѡ����������
	 * @param[in] bSelAll ���� true ��ʾ�ڻ�ȡ����ʱѡ���������ݣ�false Ϊ��ѡ��
	 * @return ��
	 */
	void SetSelAllOnFocus(bool bSelAll);

	/**
	 * @brief ����ֻ��ģʽ����ʾ���
	 * @return ��
	 */
	void SetNoCaretReadonly();

	/**
	 * @brief ��Ӵ���ɫ���ı�
	 * @param[in] str �ı�����
	 * @param[in] color ��ɫֵ����ֵ������ global.xml �д���
	 * @return ��
	 */
	void AddColorText(const std::wstring &str, const std::wstring &color);

	/**
	 * @brief ���һ������������ɫ�ĳ�����
	 * @param[in] str ��������
	 * @param[in] color ������ɫ
	 * @param[in] linkInfo ���ӵ�ַ
	 * @return ��
	 */
	void AddLinkColorText(const std::wstring &str, const std::wstring &color, const std::wstring &linkInfo = L"");

	/**
	 * @brief ���һ��ָ���������������ɫ�ĳ�����
	 * @param[in] str ��������
	 * @param[in] color ������ɫ
	 * @param[in] linkInfo ���ӵ�ַ
	 * @param[in] font ��������
	 * @return ��
	 */
	void AddLinkColorTextEx(const std::wstring& str, const std::wstring &color, const std::wstring &linkInfo = L"", const std::wstring& strFontId = L"");

	/**
	 * @brief ���һ����Χ���� hittest �ж��Ƿ���������Ϣ
	 * @param[in] cr ��Χ����ʼλ�úͽ���λ��
	 * @param[in] linkInfo �Զ��� link ����
	 * @return ��
	 */
	void AddLinkInfo(const CHARRANGE cr, const std::wstring &linkInfo);

	/**
	 * @brief ���һ����Χ���� hittest �ж��Ƿ���������Ϣ,�����÷�Χ��������ʽ��Ϊϵͳ������ʽ
	 * @param[in] str ��������
	 * @param[in] cr ��Χ����ʼλ�úͽ���λ��
	 * @param[in] linkInfo �Զ��� link ����
	 * @return ��
	 */
	void AddLinkInfoEx(const CHARRANGE cr, const std::wstring& linkInfo);

	/**
	 * @brief ����point��hittest�Զ���link������
	 * @param[in] pt λ����Ϣ
	 * @param[in] info ��ʾ link ���Զ�������
	 * @return ���� true ��ʾ�� link ��
	 */
	bool HittestCustomLink(CPoint pt, std::wstring& info);

	/**
	 * @brief ����ͼƬ����
	 * @return ��
	 */
	virtual void ClearImageCache() override;

	/**
	 * @brief �����س����������¼�
	 * @param[in] callback �س������µ��Զ���ص�����
	 * @return ��
	 */
	void AttachReturn(const EventCallback& callback) { OnEvent[kEventReturn] += callback; }

	/**
	 * @brief ���� TAB ���������¼�
	 * @param[in] callback TAB �����µ��Զ���ص�����
	 * @return ��
	 */
	void AttachTab(const EventCallback& callback) {	OnEvent[kEventTab] += callback;	}

	/**
	 * @brief �����ı����޸��¼�
	 * @param[in] callback �ı����޸ĺ���Զ���ص�����
	 * @return ��
	 */
	void AttachTextChange(const EventCallback& callback) { OnEvent[kEventTextChange] += callback; }

	/**
	 * @brief �����Զ������ӱ�����¼�
	 * @param[in] callback �Զ������ӱ��������Զ���ص�����
	 * @return ��
	 */
	void AttachCustomLinkClk(const EventCallback& callback)	{ OnEvent[kEventCustomLinkClick] += callback; }

	/**
	 * @brief ������С�仯�¼�
	 * @param[in] callback ��С���ı����Զ���ص�����
	 * @return ��
	 */
	void AttachGetNaturalSize(const FunGetNaturalSize& callback) { m_cbGetNaturalSize = callback; };

	/**
	 * @brief ֪ͨ�ؼ�ֵ�����仯
	 * @param[in] oldText ��ֵ
	 * @param[in] newText ��ֵ
	 * @return ��
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
	bool m_bSelAllEver;			//ֻ�ڵ�һ��ʱȫѡ
	bool m_bNoSelOnKillFocus;	//��� m_bEnabled && m_bReadOnly
	bool m_bSelAllOnFocus;		//��� m_bEnabled && !m_bReadOnly
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
	std::weak_ptr<nbase::WeakFlag> m_windowFlag; //��¼���������flag
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

//�ж��Ƿ����ֽڣ� �ɴ�ӡ�ַ���0x20-0x7e��
bool IsAsciiChar(const wchar_t ch);
//��ȡ�ַ������ֽ���
int  GetAsciiCharNumber(const std::wstring &str);
//ɾ���ַ����г���limit�ֽڸ���֮����ַ�
void LimitAsciiNumber(std::wstring &src, int limit);
//��ȡճ�����ַ���
void GetClipboardText(std::wstring &out);
//����ճ�����ַ���
void SetClipBoardText(const std::wstring &str);

} // namespace ui

#endif // UI_CONTROL_RICHEDIT_H_
