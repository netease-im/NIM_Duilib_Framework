#ifndef UI_CONTROL_MENU_H_
#define UI_CONTROL_MENU_H_

#pragma once

#include <map>
#include <vector>

// #define SUB_MENU_POPUP_CALC_SHADOW

namespace ui {

/** @class ContextMenuParam
  * @brief 用于通知菜单关闭
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
  * @brief 观察者模式：主题基类
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
  * @brief 观察者模式：观察者基类
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
  * @brief 观察者模式：主题实现
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
  * @brief 观察者模式：观察者实现
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
  * @brief 菜单管理器，管理菜单主题、当前弹出的菜单
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/20
  */
class UILIB_API MenuManager
{
public:
	static MenuManager* GetInstance();

	/** @brief 获取菜单主题对象
	  * @return SubjectImpl& 菜单主题
	  */
	SubjectImpl& GetSubject();

	/** @brief 根据菜单名注册菜单
	  * @param[in] strMenuName 菜单名
	  * @param[in] menu 菜单窗口指针
	  * @return void 无返回值
	  */
	void RegisterMenu(const std::wstring strMenuName, MenuWndEx *menu);

	/** @brief 根据菜单名注销菜单
	  * @param [in] strMenuName 菜单名
	  * @return  void 无返回值
	  */
	void UnRegisterMenu(const std::wstring strMenuName);

	/** @brief 根据菜单名获取菜单
	  * @param[in] strMenuName 菜单名
	  * @return MenuWndEx* 菜单指针
	  */
	MenuWndEx* GetMenu(const std::wstring &strMenuName);

	/** @brief 根据菜单名显示菜单
	  * @param[in] strMenuName 菜单名
	  * @return true 成功，false菜单不存在
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

	/** @brief 向菜单容器里添加子控件，被添加的控件一直会存在
	  * @param[in] pControl 子控件
	  * @return bool 是否成功
	  */
	virtual bool Add(Control* pControl) override;
	virtual bool AddAt(Control* pControl, std::size_t iIndex) override;

	/** @brief 移除所有子控件，如果是临时菜单项，则一定会被销毁
	  * @return void 无返回值
	  */
	virtual void RemoveAll() override;

	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
	std::wstring GetClass() { return m_class; };

private:
	std::wstring m_class;
};

/** @enum MenuAlign
  * @brief 菜单弹出的位置
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/23
  */
enum MenuAlign
{
	kLeft = 1 << 1,		// 鼠标左侧弹出
	kRight = 1 << 2,	// 鼠标右侧弹出
	kCenter = 1 << 3,	// 鼠标点作为菜单水平方向弹出中间点
	kTop = 1 << 4,		// 鼠标上侧弹出
	kBottom = 1 << 5,	// 鼠标下侧弹出
    kVCenter = 1 << 6,	// 鼠标点作为菜单竖直方向弹出中间点
};
/** @brief 把字符串的对齐信息转换为enum
  * @param[in] value 类似"left bottom"的字符串信息
  * @return int 对齐信息
  */
int StringToMenuAlign(const std::wstring &value);

/** @brief 把字符串的弹出信息转换为CPoint
  * @param[in] rect 控件位置
  * @param[in] value 类似"left vcenter"的字符串信息
  * @return CPoint 菜单弹出位置
  */
CPoint StringToMenuPopup(UiRect rect, const std::wstring &value);

/** @brief 把控件的menupopup、munualign属性解析为菜单弹出信息
  * @param[in] control 包含信息的控件
  * @param[out] point 菜单弹出位置
  * @param[out] align 菜单弹出方向
  * @return void 无返回值
  */
void GetMenuPopupInfo(Control *control, CPoint &point, int &align);

/** @enum MenuFlags$
  * @brief 菜单额外的控制参数￥
  * @copyright (c) 2020, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2020/3/27
  */
enum MenuFlags
{
	kNoFocus = 1 << 1,			// 菜单弹出时不获取焦点
	kUseDefaultShadow = 1 << 2	// 菜单弹出时使用默认阴影（即使xml中指定了其他阴影）
};

class MenuElement;
class UILIB_API MenuWndEx : public ui::WindowImplBase, public ObserverImpl
{
public:
	/** @brief 创建菜单
	  * @param xml		菜单的布局文件
	  * @param point	弹出菜单的坐标
	  * @param strMenuName	菜单名字，用于唯一标识一个菜单
	  * @param align	菜单的出现位置，默认出现在鼠标的右下侧
	  * @param hParnet	菜单的父窗体句柄
	  * @param flags	MenuFlags，一些额外的控制参数	
	  * @return MenuWndEx*	菜单窗口
	  */
	static MenuWndEx* CreateMenu(
		STRINGorID xml,
		CPoint point,
		const std::wstring &strMenuName = L"",
		int align = kRight | kBottom,
		HWND hParent = NULL,
		int flags = 0
		);

	/** @brief 获取根菜单控件，用于动态添加子菜单
	  * @return MenuBox* 根菜单控件
	  */
	MenuBox* GetMenu();

	/** @brief 动态添加子菜单后，重新调整菜单的大小
	  * @return void 无返回值
	  */
	void ResizeMenu();

	/** @brief 动态添加子菜单后，重新调整子菜单的大小
	  * @return void 无返回值
	  */
	void ResizeSubMenu();

	/** @brief 检查当前设置信息里是否有对应的MenuFlags
	  * @return bool 是否有这个flag
	  */
	bool CheckFlag(MenuFlags flag);

	using ControlName = std::wstring;
	using EnableMap = std::map<ControlName, bool>;
	using SelectMap = std::map<ControlName, bool>;
	using VisibleMap = std::map<ControlName, bool>;
	using TextIdMap = std::map<ControlName, std::wstring>;
	/** @brief 批量设置子控件的状态(禁用、选择、隐藏、文本)，用于快速初始化菜单
	  * @param[in] data 状态数据
	  * @return void 无返回值
	  */
	void SetEnableItems(const EnableMap &data);
	void SetSelectItems(const SelectMap &data);
	void SetVisibleItems(const VisibleMap &data);
	void SetTextIdItems(const TextIdMap &data);

	/** @brief 设置菜单的宽度
	  * @return void 无
	  */
	void SetWidth(const int width);

	/** @brief 监听某个菜单项的选择事件
	  * @param[in] strName 控件名
	  * @param[in] callback 事件处理器
	  * @return void 无返回值
	  */
	void AttachSelect(const std::wstring& strName, const ui::EventCallback& callback);

	/** @brief 监听某个子菜单即将弹出的事件
	  * @param[in] 子菜单对应的菜单项名称
	  * @param[in] callback 事件处理器
	  * @return void 无返回值
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
	int				m_nAlignment = kRight | kBottom;	//菜单对齐方式
	int				m_nFlags = 0;;
};

class ListContainerElement;
class UILIB_API MenuElement : public ListContainerElement
{
	friend MenuWndEx;
public:
    MenuElement();
	~MenuElement();

	/** @brief 向菜单项里添加子控件，被添加的控件一直会存在
	  * @param[in] pControl 子控件
	  * @return bool 是否成功
	  */
	virtual bool Add(Control* pControl) override;
	virtual bool AddAt(Control* pControl, std::size_t iIndex) override;

	/** @brief 向已经弹出的子菜单窗口里添加临时菜单项，子菜单窗口销毁后菜单项也销毁
	  * @param[in] pMenuItem 临时菜单项
	  * @return bool 是否成功
	  */
	bool AddTempItem(MenuElement* pMenuItem);

	/** @brief 监听某个子菜单即将弹出的事件
	  * @param[in] callback 事件处理器
	  * @return void 无返回值
	  */
	void AttachPopupSubMenu(const EventCallback& callback) { OnEvent[kEventNotify] += callback; }

	/** @brief 菜单项被单击后，是否自动关闭菜单
	  * @param[in] value 是否自动关闭菜单
	  * @return void 无返回值
	  */
	void SetAutoCloseWhenClick(bool value);
	bool IsAutoCloseWhenClick();

	/** @brief 菜单项是否包含子菜单
	  * @param[in] has 是否包含子菜单
	  * @return void 无返回值
	  */
	void SetHasSubMenu(bool has);
	bool HasSubMenu();

	/** @brief 菜单项对应的子菜单的阴影效果
	  * @return void 无返回值
	  */
	void SetSubMenuShadowImage(const std::wstring &image);
	std::wstring GetSubMenuShadowImage() const;
	void SetSubMenuShadowCorner(const UiRect &rect);
	UiRect GetSubMenuShadowCorner() const;

	/** @brief 是否为临时菜单项，子菜单销毁时临时菜单项也会被销毁
	  * @return bool 是否为临时菜单项
	  */
	bool IsTempItem();

protected:
	virtual Control* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags, CPoint scrollPos = CPoint()) override;
	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;

	virtual bool MouseEnter(EventArgs& msg) override;
	virtual bool MouseLeave(EventArgs& msg) override;
	virtual bool ButtonUp(EventArgs& msg) override;
	virtual void HandleMessage(EventArgs& event) override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	virtual void PaintStatusColor(IRenderContext* pRender) override;
	virtual void PaintStatusImage(IRenderContext* pRender) override;
	virtual void PaintText(IRenderContext* pRender) override;

	/** @brief 准备创建子菜单
	  * @return bool 是否真的需要创建子菜单
	  */
	bool PrepareCreateMenu();

	/** @brief 创建子菜单
	  * @return void 无返回值
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