#ifndef __UIMENU_H__
#define __UIMENU_H__

#pragma once

namespace nim_comp {

using namespace ui;
enum MenuAlignment
{
	eMenuAlignment_Left = 1 << 1,
	eMenuAlignment_Top = 1 << 2,
	eMenuAlignment_Right = 1 << 3,
	eMenuAlignment_Bottom = 1 << 4,
	eMenuAlignment_Intelligent = 1 <<5    //智能的防止被遮蔽
};

enum MenuCloseType
{
	eMenuCloseThis,  //适用于关闭当前级别的菜单窗口，如鼠标移入时
	eMenuCloseAll     //关闭所有菜单窗口，如失去焦点时
};

//增加关闭事件的传递。
/*
点击某一菜单，获取该菜单窗口句柄，通知该菜单窗口可以关闭子菜单项了。
即某子菜单项目的父窗口等于该窗口，该子菜单关闭。
由于菜单的父子关系，会自动关闭其所有子孙菜单窗口
这里的事件传递设计拷贝原生Duilib的MenuDemo，不过Redrain的Menu功能更好，支持菜单复选，这里暂未实现
*/
#include "observer_impl_base.hpp"   //copy from menuDemo
struct ContextMenuParam
{
	MenuCloseType wParam;
	HWND hWnd;
};

typedef class ObserverImpl<BOOL, ContextMenuParam> ContextMenuObserver;
typedef class ReceiverImpl<BOOL, ContextMenuParam> ContextMenuReceiver;

/////////////////////////////////////////////////////////////////////////////////////
//


extern const TCHAR* const kMenuElementUIInterfaceName;// = _T("MenuElement);
class CMenuElementUI;
class CMenuWnd : public ui::WindowImplBase, public ContextMenuReceiver
{
public:
	enum PopupPosType  //鼠标点击的point属于菜单的哪个位置    1.-----.2       1左上 2右上
	{                                                 //      |     |
		//这里假定用户是喜欢智能的                            3.-----.4       3左下 4右下
		RIGHT_BOTTOM = eMenuAlignment_Right | eMenuAlignment_Bottom | eMenuAlignment_Intelligent,
		RIGHT_TOP = eMenuAlignment_Right | eMenuAlignment_Top | eMenuAlignment_Intelligent,
		LEFT_BOTTOM = eMenuAlignment_Left | eMenuAlignment_Bottom | eMenuAlignment_Intelligent,
		LEFT_TOP = eMenuAlignment_Intelligent | eMenuAlignment_Top | eMenuAlignment_Intelligent,
		//这里是normal，非智能的
		RIGHT_BOTTOM_N = eMenuAlignment_Right | eMenuAlignment_Bottom,
		RIGHT_TOP_N = eMenuAlignment_Right | eMenuAlignment_Top,
		LEFT_BOTTOM_N = eMenuAlignment_Left | eMenuAlignment_Bottom,
		LEFT_TOP_N = eMenuAlignment_Intelligent | eMenuAlignment_Top
	};
	CMenuWnd(HWND hParent = NULL);
	void Init(STRINGorID xml, LPCTSTR pSkinType, POINT point, PopupPosType popupPosType = LEFT_TOP, bool no_focus = false, CMenuElementUI* pOwner = NULL);
	void Show();
	// 重新调整菜单的大小
	void ResizeMenu();
	// 重新调整子菜单的大小
	void ResizeSubMenu();

	void DetouchOwner();		//add by djj 20200506

	static ContextMenuObserver& GetMenuObserver()
	{
		static ContextMenuObserver s_context_menu_observer;
		return s_context_menu_observer;
	}
	BOOL Receive(ContextMenuParam param) override;
	
	virtual Control* CreateControl(const std::wstring& pstrClass) override;
	virtual std::wstring GetSkinFolder() override {
		return L"menu";
	}
	virtual std::wstring GetSkinFile() override {
		return m_xml.m_lpstr;
	}
	std::wstring GetWindowClassName() const override;

public:
	HWND m_hParent;
	POINT m_BasedPoint;
	PopupPosType m_popupPosType;
	STRINGorID m_xml;
	bool no_focus_;
	CMenuElementUI* m_pOwner;
	ListBox* m_pLayout;
private:
	virtual void InitWindow() override;
	void CMenuWnd::OnFinalMessage(HWND hWnd) override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class ListContainerElement;
class CMenuElementUI : public ui::ListContainerElement
{
	friend CMenuWnd;
public:
	CMenuElementUI();
	~CMenuElementUI();

	virtual bool ButtonUp(EventArgs& msg) override;
	virtual bool MouseEnter(EventArgs& msg) override;

	virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;

	bool CheckSubMenuItem();

	virtual bool AddSubMenuItem(CMenuElementUI* pMenuItem);
	virtual bool AddSubMenuItemAt(CMenuElementUI* pMenuItem, std::size_t iIndex);
	virtual bool RemoveSubMenuItem(CMenuElementUI* pMenuItem);
	virtual bool RemoveSubMenuItemAt(std::size_t iIndex);
	virtual bool RemoveAllSubMenuItem();
	virtual CMenuElementUI* GetSubMenuItemAt(std::size_t iIndex) const;
	virtual int GetSubMenuItemCount() const{ return m_child_menus.size(); };

	virtual bool Add(Control* pControl) override;

private:
	void CreateMenuWnd();
	CMenuWnd*	m_pSubWindow;

protected:
	std::vector<CMenuElementUI*> m_child_menus;
};

} // namespace nim_comp

#endif // __UIMENU_H__
