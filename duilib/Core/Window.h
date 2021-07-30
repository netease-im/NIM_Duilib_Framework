#ifndef UI_CORE_WINDOW_H_
#define UI_CORE_WINDOW_H_

#pragma once

namespace ui 
{

class Box;

// Flags for Control::GetControlFlags()
#define UIFLAG_DEFAULT       0x00000000		// Ĭ��״̬
#define UIFLAG_TABSTOP       0x00000001		// ��ʶ�ؼ��Ƿ����յ� TAB �л�����ʱ�������ý���

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
	HFONT hFont;
	std::wstring sFontName;
	int iSize;
	int iWeight;
	bool bBold;
	bool bUnderline;
	bool bStrikeout;
	bool bItalic;
	TEXTMETRIC tm;
} TFontInfo;

// MessageFilter interface
class IUIMessageFilter
{
public:
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
};

class ITranslateAccelerator
{
public:
	virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
};
class IControlFromPointFinder
{
public:
	virtual Control* FindControlFromPoint(const CPoint& pt) = 0;
};
/////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

/////////////////////////////////////////////////////////////////////////////////////
//

class UIAWindowProvider;

class UILIB_API Window : public virtual nbase::SupportWeakCallback
{
public:
    Window();
	~Window();

	/**
	 * @brief ��ȡ���������� Windows ���
	 * @return ���ش��ڹ����� Windows ���ھ��
	 */
    HWND GetHWND() const;

	/**
	 * @brief ע�ᴰ����
	 * @return ���� true ��ʾ�ɹ��������ʾʧ��
	 */
    bool RegisterWindowClass();

	/**
	 * @brief ע��ؼ������ࣨ�봰�ڵĹ��̺�����ͬ��
	 * @return ���� true ��ʾ�ɹ��������ʾʧ��
	 */
    bool RegisterSuperClass();

	/**
	 * @brief ��ȡ����������
	 * @return ���ش���������
	 */
	virtual std::wstring GetWindowClassName() const;

	/**
	 * @brief ��ȡ�ؼ�������
	 * @return ���ؿؼ�������
	 */
	virtual std::wstring GetSuperClassName() const;

	/**
	 * @brief ��ȡ���������ʽ���÷�����ʵ����������ʵ�֣�https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles
	 * @return ���ش��������ʽ
	 */
	virtual UINT GetClassStyle() const;

	/**
	 * @brief ���໯���ڣ����Ĵ��ڹ��̺�����
	 * @param[in] ���ھ��
	 * @return �µĴ��ھ��
	 */
	HWND Subclass(HWND hWnd);

	/**
	 * @brief ȡ�����໯���ڣ��ָ�ԭ���Ĵ��ڹ��̺�����
	 * @return ��
	 */
	void Unsubclass();

	/**
	 * @brief ��������
	 * @param[in] hwndParent �����ھ��
	 * @param[in] pstrName ��������
	 * @param[in] dwStyle ������ʽ
	 * @param[in] dwExStyle ������չ��ʽ
	 * @param[in] isLayeredWindow �Ƿ���в㴰�����ԣ�Ĭ��Ϊ true
	 * @param[in] rc ���ڴ�С
	 * @return ���ش��ھ��
	 */
	virtual HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, bool isLayeredWindow = true, const UiRect& rc = UiRect(0, 0, 0, 0));

	/**
	 * @brief �رմ���
	 * @param[in] �ر���Ϣ
	 * @return ��
	 */
	virtual void Close(UINT nRet = IDOK);

	/**
	 * @brief ��ʾ�����ش���
	 * @param[in] bShow Ϊ true ʱ��ʾ���ڣ�Ϊ false ʱΪ���ش��ڣ�Ĭ��Ϊ true
	 * @param[in] bTakeFocus �Ƿ��ý��㣨����ڣ���Ĭ��Ϊ true
	 * @return ��
	 */
	virtual void ShowWindow(bool bShow = true, bool bTakeFocus = true);
	
	/**
	 * @brief ��ʾģ̬�Ի����Ƽ���
	 * @param[in] parent_hwnd �����ھ��
	 * @return ��
	 */
	void ShowModalFake(HWND parent_hwnd);

	/**
	 * @brief �Ƿ���ģ̬��ʾ
	 * @return �Ƿ���ģ̬��ʾ
	 */
	bool IsFakeModal();

	/**
	 * @brief ���д��ڣ�֧����չ��Ļ
	 * @return ��
	 */
    void CenterWindow();	

	/**
	 * @brief ���ô���ͼ��
	 * @param[in] nRes ����ͼ����Դ ID
	 * @return ��
	 */
    void SetIcon(UINT nRes);

	/**
	 * @brief ������Ϣ���� Windows SendMessage ��һ���װ
	 * @param[in] uMsg ��Ϣ����
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ���ش��ڶ���Ϣ�Ĵ�����
	 */
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**
	 * @brief Ͷ��һ����Ϣ����Ϣ����
	 * @param[in] uMsg ��Ϣ����
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ���ش��ڶ���Ϣ�Ĵ�����
	 */
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	/**
	 * @brief �������ڹر��¼�
	 * @param[in] callback ָ���رպ�Ļص�����
	 * @return ��
	 */
	void AttachWindowClose(const EventCallback& callback);

protected:
	/**
	 * @brief �ڴ����յ� WM_NCDESTROY ��Ϣʱ�ᱻ����
	 * @param[in] hWnd ���ھ��
	 * @return ��
	 */
	virtual void OnFinalMessage(HWND hWnd);

	/**
	 * @brief ���ڹ��̺���
	 * @param[in] hWnd ���ھ��
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ������Ϣ������
	 */
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief ������
	 * @param[in] ������
	 * @return ������
	 */
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/**
	 * @brief ���ڽ��յ� WM_CREATE ��Ϣʱ�ᱻ����
	 * @param[in] hWnd ���ھ��
	 * @return ��
	 */
	void Init(HWND hWnd);

	/**
	 * @brief �󶨴��ڵĶ�������
	 * @param[in] pRoot ����ָ��
	 * @return ���� true �󶨳ɹ�������Ϊʧ��
	 */
	bool AttachDialog(Box* pRoot);

	/**
	 * @brief ��ʼ���ؼ�������������ӿؼ�ʱ�ᱻ���ã����ڶԿؼ����������棩
	 * @param[in] pControl �ؼ�ָ��
	 * @param[in] pParent �ؼ�������
	 * @return true Ϊ�ɹ�������Ϊʧ��
	 */
	bool InitControls(Control* pControl, Box* pParent = NULL);

	/**
	 * @brief ���տؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @return ��
	 */
	void ReapObjects(Control* pControl);

	/// ��Դ��ز���
	/**
	 * @brief ��ȡ������Դ·��
	 * @return ���ش�����Դ·��
	 */
	std::wstring GetWindowResourcePath();

	/**
	 * @brief ���ô�����Դ·��
	 * @param[in] strPath Ҫ���õ�·��
	 * @return ��
	 */
	void SetWindowResourcePath(const std::wstring& strPath);

	/**
	 * @brief ��ȡĬ��������Ϣ
	 * @return ����Ĭ��������Ϣ�ṹ
	 */
	TFontInfo* GetDefaultFontInfo();

	/**
	 * @brief ���һ��ͨ����ʽ
	 * @param[in] strClassName ͨ����ʽ������
	 * @param[in] strControlAttrList ͨ����ʽ�� XML ת���ʽ����
	 * @return ��
	 */
	void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList);

	/**
	 * @brief ��ȡ����ͨ����ʽ
	 * @return ��������ͨ����ʽ�� map ����
	 */
	const std::map<std::wstring, std::wstring>* GetClassMap();

	/**
	 * @brief ��ȡָ��ͨ����ʽ������
	 * @param[in] strClassName ͨ����ʽ����
	 * @return ����ָ�����Ƶ�ͨ����ʽ���ݣ�XML ת���ʽ����
	 */
	std::wstring GetClassAttributes(const std::wstring& strClassName) const;

	/**
	 * @brief ɾ��һ��ͨ����ʽ
	 * @param[in] strClassName Ҫɾ����ͨ����ʽ����
	 * @return ���� true Ϊ�ɹ���false Ϊʧ�ܻ���ʽ������
	 */
	bool RemoveClass(const std::wstring& strClassName);

	/**
	 * @brief ɾ������ͨ����ʽ
	 * @return ��
	 */
	void RemoveAllClass();

	/**
	 * @brief ���һ����ɫֵ�ṩ������ʹ��
	 * @param[in] strName ��ɫ���ƣ��� white��
	 * @param[in] strValue ��ɫ������ֵ���� #FFFFFFFF��
	 * @return ��
	 */
	void AddTextColor(const std::wstring& strName, const std::wstring& strValue);

	/**
	 * @brief ���һ����ɫֵ�ṩ������ʹ��
	 * @param[in] strName ��ɫ���ƣ��� white��
	 * @param[in] strValue ��ɫ������ֵ���� #FFFFFFFF��
	 * @return ��
	 */
	void AddTextColor(const std::wstring& strName, DWORD argb);

	/**
	 * @brief �������ƻ�ȡһ����ɫ�ľ�����ֵ
	 * @param[in] strName Ҫ��ȡ����ɫ����
	 * @return ���� DWORD ��ʽ����ɫ����ֵ
	 */
	DWORD GetTextColor(const std::wstring& strName);

	/**
	 * @brief ���һ��ѡ����
	 * @param[in] strGroupName ������
	 * @param[in] pControl �ؼ�ָ��
	 * @return ���� true ��ʾ��ӳɹ���false �������Ѿ�����
	 */
	bool AddOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**
	 * @brief ��ȡָ��ѡ�����пؼ��б�
	 * @param[in] strGroupName ָ��������
	 * @return ���ظ����µ����пؼ��б�
	 */
	std::vector<Control*>* GetOptionGroup(const std::wstring& strGroupName);

	/**
	 * @brief ɾ��һ��ѡ����
	 * @param[in] strGroupName ������
	 * @param[in] pControl �ؼ�����
	 * @return ��
	 */
	void RemoveOptionGroup(const std::wstring& strGroupName, Control* pControl);

	/**
	 * @brief ɾ������ѡ����
	 * @return ��
	 */
	void RemoveAllOptionGroups();

	/**
	 * @brief ����ͼƬ����
	 * @return ��
	 */
	void ClearImageCache();

	/// ��������
	/**
	 * @brief ��ȡ���λ��
	 * @return ���λ�õ� x �� y ����
	 */
	POINT GetMousePos() const;

	/**
	 * @brief ��ȡ�����ı߿����췶Χ�Ĵ�С
	 * @return ���ش����ı߿����췶Χ�Ĵ�С
	 */
	UiRect GetSizeBox();

	/**
	 * @brief ���ô����ı߿����췶Χ�Ĵ�С
	 * @param[in] rcSizeBox Ҫ���õĴ�С
	 * @return ��
	 */
	void SetSizeBox(const UiRect& rcSizeBox);

	/**
	 * @brief ��ȡ���ڱ��������򣨿��϶����򣩣���Ӧ XML �� caption ����
	 * @return ���ر���������
	 */
	UiRect GetCaptionRect() const;

	/**
	 * @brief ���ô��ڱ���������
	 * @param[in] rcCaption Ҫ���õ�����Χ
	 * @return ��
	 */
	void SetCaptionRect(UiRect& rcCaption, bool bNeedDpiScale = true);

	/**
	 * @brief ��ȡ����Բ�Ǵ�С����Ӧ XML �� roundcorner ����
	 * @return ����Բ�Ǵ�С
	 */
	CSize GetRoundCorner() const;

	/**
	 * @brief ���ô���Բ�Ǵ�С
	 * @param[in] cx Բ�ǿ�
	 * @param[in] cy Բ�Ǹ�
	 * @return ��
	 */
	void SetRoundCorner(int cx, int cy);

	/**
	 * @brief ��ȡ���������Ϣ
	 * @return ���ش��������Ϣ
	 */
	UiRect GetMaximizeInfo() const;

	/**
	 * @brief ���ô��������Ϣ
	 * @param[in] rcMaximize Ҫ���õ������ֵ
	 * @return ��
	 */
	void SetMaximizeInfo(UiRect& rcMaximize);

	/**
	 * @brief ��ȡ͸��ͨ���޲���Χ�ĵľŹ�����������Ӧ XML �� alphafixcorner ����
	 * @return �����޲���Χ
	 */
	UiRect GetAlphaFixCorner() const;

	/**
	 * @brief ����͸��ͨ���޲���Χ�ĵľŹ�������
	 * @param[in] rc Ҫ���õ��޲���Χ
	 * @return ��
	 */
	void SetAlphaFixCorner(UiRect& rc);

	/**
	 * @brief ��ȡ���ڵĳ�ʼ�߶�ռ��Ļ�߶ȵİٷֱȣ���Ӧ XML �� heightpercent ����
	 * @return ����ָ���ٷֱ�����
	 */
	double GetHeightPercent() const;

	/**
	 * @brief ���ô��ڵĳ�ʼ�߶�ռ��Ļ�߶ȵİٷֱ�
	 * @param[in] heightPercent �ٷֱ�
	 * @return ��
	 */
	void SetHeightPercent(double heightPercent);

	/**
	 * @brief ���������б��е��ı� ID ���ô��ڱ������ı�
	 * @param[in] strTextId ���� ID���� ID �����������ļ��д���
	 * @return ��
	 */
	void SetTextId(const std::wstring& strTextId);

	/// ��Ӱ��ز���
	/**
	 * @brief ���ô����Ƿ񸽼���ӰЧ��
	 * @param[in] bShadowAttached Ϊ true ʱ���ӣ�false ʱ������
	 * @return ��
	 */
	void SetShadowAttached(bool bShadowAttached);

	/**
	* @brief ��ȡ�Ƿ񸽼���ӰЧ��
	* @return �Ƿ񸽼���ӰЧ��
	*/
	bool IsShadowAttached();

	/**
	 * @brief ��ȡ��ӰͼƬ
	 * @return ������ӰͼƬλ��
	 */
	std::wstring GetShadowImage() const;

	/**
	 * @brief ���ô�����ӰͼƬ
	 * @param[in] strImage ͼƬλ��
	 * @return ��
	 */
	void SetShadowImage(const std::wstring &strImage);

	/**
	 * @brief ��ȡ��Ӱ�ľŹ���������Ϣ
	 * @return ������Ӱ�ľŹ���������Ϣ
	 */
	UiRect GetShadowCorner() const;

	/**
	 * @brief ָ����Ӱ�زĵľŹ�������
	 * @param[in] rect �Ź���������Ϣ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 * @return ��
	 */
	void SetShadowCorner(const UiRect rect, bool bNeedDpiScale = true);

	/**
	 * @brief ��ȡ����λ����Ϣ
	 * @param[in] bContainShadow �Ƿ������Ӱ��true Ϊ������Ĭ��Ϊ false ������
	 * @return ���ش���λ����Ϣ
	 */
	UiRect GetPos(bool bContainShadow = false) const;

	/**
	 * @brief ���ô���λ�ã��� SetWindowPos ��һ���װ��
	 * @param[in] rc ����λ��
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 * @param[in] uFlags ��Ӧ SetWindowPos �� uFlags ѡ��
	 * @param[in] hWndInsertAfter ��Ӧ SetWindowPos �� hWndInsertAfter ѡ��
	 * @param[in] bContainShadow �Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 * @return ��
	 */
	void SetPos(const UiRect& rc, bool bNeedDpiScale, UINT uFlags, HWND hWndInsertAfter = NULL, bool bContainShadow = false);

	/**
	 * @brief ��ȡ������С��Χ����Ӧ XML �� mininfo ����
	 * @param[in] bContainShadow �Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 * @return ����ָ����С
	 */
	CSize GetMinInfo(bool bContainShadow = false) const;

	/**
	 * @brief ��ȡ�������Χ����Ӧ XML �� maxinfo ����
	 * @param[in] bContainShadow �Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 * @return ����ָ����С
	 */
	CSize GetMaxInfo(bool bContainShadow = false) const;

	/**
	 * @brief ��ȡ���ڳ�ʼ��С
	 * @param[in] bContainShadow �Ƿ������Ӱ��Χ��Ĭ��Ϊ false
	 * @return ����ָ����С
	 */
	CSize GetInitSize(bool bContainShadow = false) const;

	/**
	 * @brief ���ô�����С��Χ
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 * @return ��
	 */
	// 
	void SetMinInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief ���ô������Χ
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ����Ҫ�� rc ���� DPI �Զ�����
	 * @return ��
	 */
	void SetMaxInfo(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief ���ô��ڴ�С
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ������ DPI ����
	 * @return ��
	 */
	void Resize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief ���ô��ڳ�ʼ��С
	 * @param[in] cx ���
	 * @param[in] cy �߶�
	 * @param[in] bContainShadow Ϊ false ��ʾ cx cy ��������Ӱ
	 * @param[in] bNeedDpiScale Ϊ false ��ʾ������ DPI ����
	 * @return ��
	 */
	void SetInitSize(int cx, int cy, bool bContainShadow = false, bool bNeedDpiScale = true);

	/**
	 * @brief ���һ����Ϣ����������ʱ��Ϣ�Ѿ��ɷ�
	 * @param[in] pFilter һ���̳��� IUIMessageFilter �Ķ���ʵ������Ҫʵ�� MessageHandler ����
	 * @return ʼ�շ��� true
	 */
	bool AddMessageFilter(IUIMessageFilter* pFilter);

	/**
	 * @brief �Ƴ�һ����Ϣ������
	 * @param[in] pFilter һ���̳��� IUIMessageFilter �Ķ���ʵ��
	 * @return ���� true ��ʾ�Ƴ��ɹ���������ܸù�����������
	 */
	bool RemoveMessageFilter(IUIMessageFilter* pFilter);

	/**
	 * @brief ���ҿؼ�ʱ���һ������λ�ò��ҿؼ��Ĺ���
	 * @param[in] pFinder �̳��� IControlFromPointFinder ����ָ�룬��ʵ�� FindControlFromPoint ����
	 * @return ʼ�շ��� true
	 */
	bool AddControlFromPointFinder(IControlFromPointFinder* pFinder);

	/**
	 * @brief �Ƴ����ҿؼ�ʱ����λ�ò��ҿؼ��Ĺ���
	 * @param[in] pFinder �̳��� IControlFromPointFinder ����ָ��
	 * @return �ɹ����� true������ù��ӿ��ܲ�����
	 */
	bool RemoveControlFromPointFinder(IControlFromPointFinder* pFinder);

	/**
	 * @brief ���һ�� TranslateMessage ֮ǰ����Ϣ������
	 * @param[in] pTranslateAccelerator �̳��� ITranslateAccelerator ����ָ�룬��ʵ�� TranslateAccelerator ����
	 * @return ʼ�շ��� true
	 */
	bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);

	/**
	 * @brief �Ƴ�һ�� TranslateMessage ֮ǰ����Ϣ������
	 * @param[in] pTranslateAccelerator �̳��� ITranslateAccelerator ����ָ��
	 * @return �ɹ����� true�����򷵻� false ���ܸù�����������
	 */
	bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);

	/**
	 * @brief ִ�� TranslateMessage �׶εĹ�����
	 * @param[in] pMsg ��Ϣ��
	 * @return ���� true �ɹ�������Ϣ�����򷵻� false
	 */
	bool TranslateAccelerator(LPMSG pMsg);

	/**
	 * @brief ������Ϣ���ɷ�����
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ���� false ������ɷ�����Ϣ���������ɷ�����Ϣ
	 */
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief ������Ϣ��ִ���壬����ִ�и���������ʹ��������Ϣ
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @param[out] handled ��Ϣ�Ƿ��Ѿ��������� true �����Ѿ��ɹ�������Ϣ��������Ϣ�������ݸ����ڹ���
	 * @return ������Ϣ������
	 */
	LRESULT DoHandlMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled);

	/**
	 * @brief �� CallWindowProc API ��һ���װ
	 * @param[in] uMsg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ������Ϣ������
	 */
	LRESULT CallWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief �ж��Ƿ���Ҫ������������뿪��Ϣ
	 * @param[in] pt ��굱ǰλ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ���� true ��Ҫ������������뿪��Ϣ������ false Ϊ����Ҫ
	 */
	inline bool HandleMouseEnterLeave(const POINT &pt, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief �ͷ�ָ���ؼ��İ���״̬
	 * @param[in] bClickOrPointer �����ؼ����Ǵ����ؼ�
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return void ��
	 */
	void ReleaseEventClick(bool bClickOrPointer, WPARAM wParam, LPARAM lParam);

	/// �������
	/**
	 * @brief ��ȡ��ǰ���н���Ŀؼ�
	 * @return ���ؿؼ���ָ��
	 */
	Control* GetFocus() const;

	/**
	 * @brief ���ý��㵽ָ���ؼ���
	 * @param[in] pControl �ؼ�ָ��
	 * @return ��
	 */
	void SetFocus(Control* pControl);

	/**
	 * @brief ���ý��㵽ָ���ؼ����ػ����пؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @return ��
	 */
	void SetFocusNeeded(Control* pControl);

	/**
	 * @brief �ô���ʧȥ����
	 * @return ��
	 */
	void KillFocus();

	/**
	 * @brief ���õ�Ҫ�������괰�ھ��Ϊ��ǰ���ƴ���
	 * @param[in]
	 * @return ��
	 */
	void SetCapture();

	/**
	 * @brief �����ڲ���Ҫ�������ʱ�ͷ���Դ
	 * @return ��
	 */
	void ReleaseCapture();

	/**
	 * @brief �ж�ָ���ؼ��Ƿ񱻵���򴥿�
	 * @paran[in] pControl �ؼ�ָ��
	 * @return ���� true ��ʾ��ǰ�ؼ����ڱ�����򴥿�״̬
	 */
	bool IsCaptureControl(const Control* pControl);

	/**
	 * @brief �жϵ�ǰ�Ƿ񲶻��������
	 * @return ���� true Ϊ���񣬷���Ϊ������
	 */
	bool IsCaptured() const;

	/**
	 * @brief ��ȡ��ǰ������ĸ��ؼ���
	 * @return ���ؿؼ���ָ��
	 */
	Control* GetNewHover();

	/**
	 * @brief ��ȡ�����������
	 * @return ����������Ϣ
	 */
	POINT GetLastMousePos() const;

	/**
	 * @brief �����Ƿ�������Ϣ
	 * @param[in] bHandle ����Ϊ true ��ʾ����false Ϊ������
	 * @return ��
	 */
	void SetHandlePointer(bool bHandle) { m_bHandlePointer = bHandle; };

	/**
	 * @brief ��ȡ��ʾ��Ϣ�����Ĵ��ھ��
	 * @return ������ʾ��Ϣ�Ĵ��ھ��
	 */
	HWND GetTooltipWindow() const;

	/**
	 * @brief �л��ؼ����㵽��һ��������һ�����ؼ�
	 * @param[in] bForward true Ϊ��һ���ؼ�������Ϊ false��Ĭ��Ϊ true
	 * @return ʼ�շ��� true�����޲ο�����
	 */
	bool SetNextTabControl(bool bForward = true);

	/// �ؼ����
	/**
	 * @brief ��ȡ��������������
	 * @return �����������������
	 */
	Control* GetRoot() const;

	/**
	 * @brief ���ÿؼ��Ƿ��Ѿ�����
	 * @param[in] bArrange true Ϊ�Ѿ����У�����Ϊ false
	 * @return ��
	 */
	void SetArrange(bool bArrange);

	/**
	 * @brief �ӳ�����һ���ؼ�
	 * @param[in] pControl �ؼ�ָ��
	 * @return ��
	 */
	void AddDelayedCleanup(Control* pControl);

	/**
	 * @brief �����������ָ���ؼ�
	 * @param[in] pt ָ������
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindControl(POINT pt) const;

	/**
	 * @brief ���ݿؼ����Ʋ��ҿؼ�
	 * @param[in] strName �ؼ�����
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindControl(const std::wstring& strName) const;

	/**
	 * @brief ������������ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] pt Ҫ���ҵ�����
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindSubControlByPoint(Control* pParent, POINT pt) const;

	/**
	 * @brief �������ֲ����ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] strName Ҫ���ҵ�����
	 * @return ���ؿؼ�ָ��
	 */
	Control* FindSubControlByName(Control* pParent, const std::wstring& strName) const;

	/**
	 * @brief �������������ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] typeinfo ���͵���Ϣ
	 * @param[in] iIndex ���ҵ������ж����Ҫ��ȡ�ڼ���������
	 * @return �ؼ�ָ��
	 */
	Control* FindSubControlByClass(Control* pParent, const type_info& typeinfo, int iIndex = 0);

	/**
	 * @brief �������������ӿؼ�
	 * @param[in] pParent Ҫ�����Ŀؼ�
	 * @param[in] typeinfo ���͵���Ϣ
	 * @return �������з��������Ŀؼ��б�
	 */
	std::vector<Control*>* FindSubControlsByClass(Control* pParent, const type_info& typeinfo);

	/**
	 * @brief ���ر����˿ؼ����ϵ��б�
	 * @return �����б����
	 */
	std::vector<Control*>* GetSubControlsByClass();

	/**
	 * @brief ��������һ����Ϣ
	 * @param[in] eventType ת�������Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ʼ�շ��� true
	 */
	bool SendNotify(EventType eventType, WPARAM wParam = 0, LPARAM lParam = 0);

	/**
	 * @brief ��������һ����Ϣ
	 * @param[in] pControl ����Ŀ�꣬Ҫ���͸��ĸ��ؼ�
	 * @param[in] msg ��Ϣ��
	 * @param[in] wParam ��Ϣ���Ӳ���
	 * @param[in] lParam ��Ϣ���Ӳ���
	 * @return ʼ�շ��� true
	 */
	bool SendNotify(Control* pControl, EventType msg, WPARAM wParam = 0, LPARAM lParam = 0);

	/// �������
	/**
	 * @brief ��ȡ�������� DC
	 * @return ���ػ������� DC
	 */
	HDC GetPaintDC() const;

	/**
	 * @brief ��ȡ���ƶ���
	 * @return ���ػ��ƶ���
	 */
	dui::common::dui_refptr<dui::render::IRenderContext> GetRenderContext() const;

	/**
	 * @brief �����ػ���Ϣ
	 * @param[in] rcItem �ػ淶Χ
	 * @return ��
	 */
	void Invalidate(const UiRect& rcItem);

	/**
	 * @brief ���ƺ�����
	 * @return ��
	 */
	void Paint();

	/**
	 * @brief ����͸����
	 * @param[in] nAlpha ͸������ֵ
	 * @return ��
	 */
	void SetAlpha(int nAlpha);

	/**
	 * @brief �жϵ�ǰ�Ƿ���Ⱦ͸��ͼ��
	 * @return ���� true Ϊ�ǣ�����Ϊ false
	 */
	bool IsRenderTransparent() const;

	/**
	 * @brief ������Ⱦ͸��ͼ��
	 * @param[in] bCanvasTransparent ���� true Ϊ��Ⱦ͸��ͼ�㣬����Ϊ false
	 * @return ������һ�λ���״̬
	 */
	bool SetRenderTransparent(bool bCanvasTransparent);
	bool IsLayeredWindow();

	/**
	 * @brief ��ʼ������
	 * @return ��
	 */
	virtual void OnInitLayout();

	/**		
	 * @brief �Ƿ�Ҫ�ر�
	 * @return ��
	 */
	bool IsClosing(){ return m_bCloseing; };

#if defined(ENABLE_UIAUTOMATION)
	/**
     * @brief Get ui automation provider
     * @return nullptr or pointer
     */
	UIAWindowProvider* GetUIAProvider();
#endif


private:
	static Control* CALLBACK __FindControlFromNameHash(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromCount(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromPoint(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromTab(Control* pThis, LPVOID pData);
	//static Control* CALLBACK __FindControlFromShortcut(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromUpdate(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromName(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlFromClass(Control* pThis, LPVOID pData);
	static Control* CALLBACK __FindControlsFromClass(Control* pThis, LPVOID pData);

private:
	/**
	 * @brief ���û���ƫ��
	 * @param[in] renderOffset ƫ��ֵ
	 * @return ��
	 */
	void SetRenderOffset(CPoint renderOffset);

	/**
	 * @brief ���û���ƫ�� x ����
	 * @param[in] renderOffsetX ����ֵ
	 * @return ��
	 */
	void SetRenderOffsetX(int renderOffsetX);

	/**
	 * @brief ���û���ƫ�� y ����
	 * @param[in] renderOffsetY ����ֵ
	 * @return ��
	 */
	void SetRenderOffsetY(int renderOffsetY);

protected:
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	bool m_bSubclassed;

	Box* m_pRoot;
	EventMap OnEvent;

	UIAWindowProvider* m_pUIAProvider;

protected:
	CSize m_szMinWindow;
	CSize m_szMaxWindow;
	CSize m_szInitWindowSize;
	UiRect m_rcMaximizeInfo;
	UiRect m_rcSizeBox;
	UiRect m_rcAlphaFix;
	CSize m_szRoundCorner;
	UiRect m_rcCaption;
	double m_heightPercent;

	HDC m_hDcPaint;
	dui::common::dui_refptr<dui::render::IRenderContext> m_renderContext;
	bool m_bIsLayeredWindow;
	int m_nAlpha;
	CPoint m_renderOffset;
	bool m_bFirstLayout;

	HWND m_hwndTooltip;
	TOOLINFO m_ToolTip;

	Control* m_pFocus;
	Control* m_pNewHover;
	Control* m_pEventHover;
	Control* m_pEventClick;
	Control* m_pEventKey;
	CPoint m_ptLastMousePos;

	Control* m_pEventPointer;
	bool m_bHandlePointer;

	UINT m_uTimerID;
	bool m_bIsArranged;
	bool m_bFocusNeeded;
	bool m_bMouseTracking;
	bool m_bMouseCapture;

	std::wstring m_strWindowResourcePath; //ÿ�����ڵ���Դ·��,����GetSkinFolder()
	TFontInfo m_defaultFontInfo;
	std::map<std::wstring, std::wstring> m_defaultAttrHash;
	std::map<std::wstring, DWORD> m_mapTextColor;
	std::map<std::wstring, std::vector<Control*>> m_mOptionGroup;

	std::vector<IUIMessageFilter*> m_aPreMessageFilters;
	std::vector<IUIMessageFilter*> m_aMessageFilters;
	std::vector<ITranslateAccelerator*> m_aTranslateAccelerator;
	std::vector<IControlFromPointFinder*> m_aIControlFromPointFinder;

	std::vector<Control*> m_aDelayedCleanup;
	std::vector<Control*> m_aFoundControls;
	std::map<std::wstring, Control*> m_mNameHash;

	nbase::WeakCallbackFlag m_closeFlag;
	
	Shadow m_shadow;

	bool m_bFakeModal = false;
	bool m_bCloseing = false;	//add by djj 20200428 ����Closeʱ���ӳ�Post WM_CLOSE, ����ڼ���Ҫ��һ����ʶ����'���ر�״̬'
};

} // namespace ui

#endif // UI_CORE_WINDOW_H_
