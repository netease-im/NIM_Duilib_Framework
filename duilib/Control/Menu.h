#ifndef UI_CONTROL_MENU_H_
#define UI_CONTROL_MENU_H_

#pragma once

#include <map>
#include <vector>

// #define SUB_MENU_POPUP_CALC_SHADOW

namespace ui {

/** @class ContextMenuParam
  * @brief ����֪ͨ�˵��ر�
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/20
  */
struct ContextMenuParam
{
	enum RemoveType
	{
		kRemoveAllMenu,
		kRemoveSubMenu
	};
	RemoveType type;
	HWND hWnd;
};

class ObserverImplBase;
/** @class SubjectImplBase
  * @brief �۲���ģʽ���������
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/20
  */
class SubjectImplBase
{
public:
	~SubjectImplBase() {};
	virtual void AddObserver(ObserverImplBase* receiver) = 0;
	virtual void RemoveObserver(ObserverImplBase* receiver) = 0;
	virtual BOOL Notify(ContextMenuParam param) = 0;
};

/** @class ObserverImplBase
  * @brief �۲���ģʽ���۲��߻���
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/20
  */
class ObserverImplBase
{
public:
	~ObserverImplBase() {};
	virtual void AddSubject(SubjectImplBase* observer) = 0;
	virtual void RemoveSubject() = 0;
	virtual BOOL OnNotify(ContextMenuParam param) = 0;
};

class ObserverImpl;
/** @class SubjectImpl
  * @brief �۲���ģʽ������ʵ��
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/20
  */
class UILIB_API SubjectImpl : public SubjectImplBase
{
	friend class Iterator;
public:
	virtual void AddObserver(ObserverImplBase* receiver)
	{
		if (receiver == NULL)
			return;

		m_receivers.push_back(receiver);
		receiver->AddSubject(this);
	}

	virtual void RemoveObserver(ObserverImplBase* receiver)
	{
		if (receiver == NULL)
			return;

		for (auto it = m_receivers.begin(); it != m_receivers.end(); ++it) {
			if (*it == receiver) {
				m_receivers.erase(it);
				break;
			}
		}
	}

	virtual BOOL Notify(ContextMenuParam param)
	{
		auto it = m_receivers.rbegin();
		for (; it != m_receivers.rend(); ++it) {
			(*it)->OnNotify(param);
		}

		return BOOL();
	}

	class Iterator
	{
		SubjectImpl & m_table;
		DWORD m_index;
		ObserverImplBase* m_ptr;
	public:
		Iterator( SubjectImpl & table )
			: m_table( table ), m_index(0), m_ptr(NULL)
		{}

		Iterator( const Iterator & v )
			: m_table( v.m_table ), m_index(v.m_index), m_ptr(v.m_ptr)
		{}

		ObserverImplBase* next()
		{
			if ( m_index >= m_table.m_receivers.size() )
				return NULL;

			for ( ; m_index < m_table.m_receivers.size(); ) {
				m_ptr = (m_table.m_receivers)[ m_index++ ];
				if ( m_ptr )
					return m_ptr;
			}
			return NULL;
		}
	};

protected:
	using ReceiversVector = std::vector<ObserverImplBase*>;
	ReceiversVector m_receivers;
};

/** @class ObserverImpl
  * @brief �۲���ģʽ���۲���ʵ��
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/20
  */
class UILIB_API ObserverImpl : public ObserverImplBase
{
public:
	virtual void AddSubject(SubjectImplBase* observer)
	{
		m_observers.push_back(observer);
	}

	virtual void RemoveSubject()
	{
		for (auto &it:m_observers) {
			it->RemoveObserver(this);
		}
	}

	virtual BOOL OnNotify(ContextMenuParam param)
	{
		return BOOL();
	}

protected:
	using ObserversVector = std::vector<SubjectImplBase*>;
	ObserversVector m_observers;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class MenuWndEx;
/** @class MenuManager
  * @brief �˵�������������˵����⡢��ǰ�����Ĳ˵�
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/20
  */
class UILIB_API MenuManager
{
public:
	static MenuManager* GetInstance();

	/** @brief ��ȡ�˵��������
	  * @return SubjectImpl& �˵�����
	  */
	SubjectImpl& GetSubject();

	/** @brief ���ݲ˵���ע��˵�
	  * @param[in] strMenuName �˵���
	  * @param[in] menu �˵�����ָ��
	  * @return void �޷���ֵ
	  */
	void RegisterMenu(const std::wstring strMenuName, MenuWndEx *menu);

	/** @brief ���ݲ˵���ע���˵�
	  * @param [in] strMenuName �˵���
	  * @return  void �޷���ֵ
	  */
	void UnRegisterMenu(const std::wstring strMenuName);

	/** @brief ���ݲ˵�����ȡ�˵�
	  * @param[in] strMenuName �˵���
	  * @return MenuWndEx* �˵�ָ��
	  */
	MenuWndEx* GetMenu(const std::wstring &strMenuName);

	/** @brief ���ݲ˵�����ʾ�˵�
	  * @param[in] strMenuName �˵���
	  * @return true �ɹ���false�˵�������
	  */
	bool ShowMenu(const std::wstring &strMenuName);
private:
	SubjectImpl m_subject;
	std::map<std::wstring, MenuWndEx*> m_menuMap;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class ListBox;
class UILIB_API MenuBox : public ListBox
{
public:
	MenuBox();
	virtual ~MenuBox();

	/** @brief ��˵�����������ӿؼ�������ӵĿؼ�һֱ�����
	  * @param[in] pControl �ӿؼ�
	  * @return bool �Ƿ�ɹ�
	  */
	virtual bool Add(Control* pControl) override;
	virtual bool AddAt(Control* pControl, std::size_t iIndex) override;

	/** @brief �Ƴ������ӿؼ����������ʱ�˵����һ���ᱻ����
	  * @return void �޷���ֵ
	  */
	virtual void RemoveAll() override;

	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	std::wstring GetClass() { return m_class; };

private:
	std::wstring m_class;
};

/** @enum MenuAlign
  * @brief �˵�������λ��
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/23
  */
enum MenuAlign
{
	kLeft = 1 << 1,		// �����൯��
	kRight = 1 << 2,	// ����Ҳ൯��
	kCenter = 1 << 3,	// ������Ϊ�˵�ˮƽ���򵯳��м��
	kTop = 1 << 4,		// ����ϲ൯��
	kBottom = 1 << 5,	// ����²൯��
    kVCenter = 1 << 6,	// ������Ϊ�˵���ֱ���򵯳��м��
};
/** @brief ���ַ����Ķ�����Ϣת��Ϊenum
  * @param[in] value ����"left bottom"���ַ�����Ϣ
  * @return int ������Ϣ
  */
int StringToMenuAlign(const std::wstring &value);

/** @brief ���ַ����ĵ�����Ϣת��ΪCPoint
  * @param[in] rect �ؼ�λ��
  * @param[in] value ����"left vcenter"���ַ�����Ϣ
  * @return CPoint �˵�����λ��
  */
CPoint StringToMenuPopup(UiRect rect, const std::wstring &value);

/** @brief �ѿؼ���menupopup��munualign���Խ���Ϊ�˵�������Ϣ
  * @param[in] control ������Ϣ�Ŀؼ�
  * @param[out] point �˵�����λ��
  * @param[out] align �˵���������
  * @return void �޷���ֵ
  */
void GetMenuPopupInfo(Control *control, CPoint &point, int &align);

/** @enum MenuFlags$
  * @brief �˵�����Ŀ��Ʋ�����
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/27
  */
enum MenuFlags
{
	kNoFocus = 1 << 1,			// �˵�����ʱ����ȡ����
	kUseDefaultShadow = 1 << 2	// �˵�����ʱʹ��Ĭ����Ӱ����ʹxml��ָ����������Ӱ��
};

class MenuElement;
class UILIB_API MenuWndEx : public ui::WindowImplBase, public ObserverImpl
{
public:
	/** @brief �����˵�
	  * @param xml		�˵��Ĳ����ļ�
	  * @param point	�����˵�������
	  * @param strMenuName	�˵����֣�����Ψһ��ʶһ���˵�
	  * @param align	�˵��ĳ���λ�ã�Ĭ�ϳ������������²�
	  * @param hParnet	�˵��ĸ�������
	  * @param flags	MenuFlags��һЩ����Ŀ��Ʋ���	
	  * @return MenuWndEx*	�˵�����
	  */
	static MenuWndEx* CreateMenu(
		STRINGorID xml,
		CPoint point,
		const std::wstring &strMenuName = L"",
		int align = kRight | kBottom,
		HWND hParent = NULL,
		int flags = 0
		);

	/** @brief ��ȡ���˵��ؼ������ڶ�̬����Ӳ˵�
	  * @return MenuBox* ���˵��ؼ�
	  */
	MenuBox* GetMenu();

	/** @brief ��̬����Ӳ˵������µ����˵��Ĵ�С
	  * @return void �޷���ֵ
	  */
	void ResizeMenu();

	/** @brief ��̬����Ӳ˵������µ����Ӳ˵��Ĵ�С
	  * @return void �޷���ֵ
	  */
	void ResizeSubMenu();

	/** @brief ��鵱ǰ������Ϣ���Ƿ��ж�Ӧ��MenuFlags
	  * @return bool �Ƿ������flag
	  */
	bool CheckFlag(MenuFlags flag);

	using ControlName = std::wstring;
	using EnableMap = std::map<ControlName, bool>;
	using SelectMap = std::map<ControlName, bool>;
	using VisibleMap = std::map<ControlName, bool>;
	using TextIdMap = std::map<ControlName, std::wstring>;
	/** @brief ���������ӿؼ���״̬(���á�ѡ�����ء��ı�)�����ڿ��ٳ�ʼ���˵�
	  * @param[in] data ״̬����
	  * @return void �޷���ֵ
	  */
	void SetEnableItems(const EnableMap &data);
	void SetSelectItems(const SelectMap &data);
	void SetVisibleItems(const VisibleMap &data);
	void SetTextIdItems(const TextIdMap &data);

	/** @brief ���ò˵��Ŀ��
	  * @return void ��
	  */
	void SetWidth(const int width);

	/** @brief ����ĳ���˵����ѡ���¼�
	  * @param[in] strName �ؼ���
	  * @param[in] callback �¼�������
	  * @return void �޷���ֵ
	  */
	void AttachSelect(const std::wstring& strName, const ui::EventCallback& callback);

	/** @brief ����ĳ���Ӳ˵������������¼�
	  * @param[in] �Ӳ˵���Ӧ�Ĳ˵�������
	  * @param[in] callback �¼�������
	  * @return void �޷���ֵ
	  */
	void AttachPopupSubMenu(const std::wstring& strName, const EventCallback& callback);

private:
	MenuWndEx(const std::wstring &strMenuName = L"");
	~MenuWndEx();

	virtual void Close(UINT nRet = IDOK) override;

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual void OnFinalMessage(HWND hWnd) override;
	virtual Control* CreateControl(const std::wstring& pstrClass) override;

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnNotify(ContextMenuParam param) override;

private:
	friend class MenuElement;
	void InitMenu(
		MenuElement* pOwner,
		HWND hParent,
		STRINGorID xml,
		CPoint point,
		int align,
		int flags = 0
		);

private:
	std::wstring	m_strMenuName;
	CPoint			m_BasedPoint;
	std::wstring	m_skinFloder;
	std::wstring	m_skinFile;
    MenuElement*	m_pOwner = nullptr;
    MenuBox*		m_pMenuRoot = nullptr;
	int				m_nAlignment = kRight | kBottom;	//�˵����뷽ʽ
	int				m_nFlags = 0;;
};

class ListContainerElement;
class UILIB_API MenuElement : public ListContainerElement
{
	friend MenuWndEx;
public:
    MenuElement();
	~MenuElement();

	/** @brief ��˵���������ӿؼ�������ӵĿؼ�һֱ�����
	  * @param[in] pControl �ӿؼ�
	  * @return bool �Ƿ�ɹ�
	  */
	virtual bool Add(Control* pControl) override;
	virtual bool AddAt(Control* pControl, std::size_t iIndex) override;

	/** @brief ���Ѿ��������Ӳ˵������������ʱ�˵���Ӳ˵��������ٺ�˵���Ҳ����
	  * @param[in] pMenuItem ��ʱ�˵���
	  * @return bool �Ƿ�ɹ�
	  */
	bool AddTempItem(MenuElement* pMenuItem);

	/** @brief ����ĳ���Ӳ˵������������¼�
	  * @param[in] callback �¼�������
	  * @return void �޷���ֵ
	  */
	void AttachPopupSubMenu(const EventCallback& callback) { OnEvent[kEventNotify] += callback; }

	/** @brief �˵���������Ƿ��Զ��رղ˵�
	  * @param[in] value �Ƿ��Զ��رղ˵�
	  * @return void �޷���ֵ
	  */
	void SetAutoCloseWhenClick(bool value);
	bool IsAutoCloseWhenClick();

	/** @brief �˵����Ƿ�����Ӳ˵�
	  * @param[in] has �Ƿ�����Ӳ˵�
	  * @return void �޷���ֵ
	  */
	void SetHasSubMenu(bool has);
	bool HasSubMenu();

	/** @brief �˵����Ӧ���Ӳ˵�����ӰЧ��
	  * @return void �޷���ֵ
	  */
	void SetSubMenuShadowImage(const std::wstring &image);
	std::wstring GetSubMenuShadowImage() const;
	void SetSubMenuShadowCorner(const UiRect &rect);
	UiRect GetSubMenuShadowCorner() const;

	/** @brief �Ƿ�Ϊ��ʱ�˵���Ӳ˵�����ʱ��ʱ�˵���Ҳ�ᱻ����
	  * @return bool �Ƿ�Ϊ��ʱ�˵���
	  */
	bool IsTempItem();

protected:
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint()) override;
	virtual void PaintChild(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;

	virtual bool MouseEnter(EventArgs& msg) override;
	virtual bool MouseLeave(EventArgs& msg) override;
	virtual bool ButtonUp(EventArgs& msg) override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	virtual void PaintStatusColor(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
	virtual void PaintStatusImage(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
	virtual void PaintText(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;

	/** @brief ׼�������Ӳ˵�
	  * @return bool �Ƿ������Ҫ�����Ӳ˵�
	  */
	bool PrepareCreateMenu();

	/** @brief �����Ӳ˵�
	  * @return void �޷���ֵ
	  */
	void CreateMenuWnd();

	void SetTempItem();

protected:
	MenuWndEx*	m_pSubMenuWindow = nullptr;
	bool		m_bAutoCloseWhenClick = true;
	bool		m_bTemp = false;

	bool		m_bHasSubMenu = false;
	std::wstring m_subMenuShadowImage;
	ui::UiRect	m_rcSubMenuShadowCorner;
};

} // namespace ui

#endif // UI_CONTROL_MENU_H_