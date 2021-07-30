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
	eMenuAlignment_Intelligent = 1 <<5    //���ܵķ�ֹ���ڱ�
};

enum MenuCloseType
{
	eMenuCloseThis,  //�����ڹرյ�ǰ����Ĳ˵����ڣ����������ʱ
	eMenuCloseAll     //�ر����в˵����ڣ���ʧȥ����ʱ
};

//���ӹر��¼��Ĵ��ݡ�
/*
���ĳһ�˵�����ȡ�ò˵����ھ����֪ͨ�ò˵����ڿ��Թر��Ӳ˵����ˡ�
��ĳ�Ӳ˵���Ŀ�ĸ����ڵ��ڸô��ڣ����Ӳ˵��رա�
���ڲ˵��ĸ��ӹ�ϵ�����Զ��ر�����������˵�����
������¼�������ƿ���ԭ��Duilib��MenuDemo������Redrain��Menu���ܸ��ã�֧�ֲ˵���ѡ��������δʵ��
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
	enum PopupPosType  //�������point���ڲ˵����ĸ�λ��    1.-----.2       1���� 2����
	{                                                 //      |     |
		//����ٶ��û���ϲ�����ܵ�                            3.-----.4       3���� 4����
		RIGHT_BOTTOM = eMenuAlignment_Right | eMenuAlignment_Bottom | eMenuAlignment_Intelligent,
		RIGHT_TOP = eMenuAlignment_Right | eMenuAlignment_Top | eMenuAlignment_Intelligent,
		LEFT_BOTTOM = eMenuAlignment_Left | eMenuAlignment_Bottom | eMenuAlignment_Intelligent,
		LEFT_TOP = eMenuAlignment_Intelligent | eMenuAlignment_Top | eMenuAlignment_Intelligent,
		//������normal�������ܵ�
		RIGHT_BOTTOM_N = eMenuAlignment_Right | eMenuAlignment_Bottom,
		RIGHT_TOP_N = eMenuAlignment_Right | eMenuAlignment_Top,
		LEFT_BOTTOM_N = eMenuAlignment_Left | eMenuAlignment_Bottom,
		LEFT_TOP_N = eMenuAlignment_Intelligent | eMenuAlignment_Top
	};
	CMenuWnd(HWND hParent = NULL);
	void Init(STRINGorID xml, LPCTSTR pSkinType, POINT point, PopupPosType popupPosType = LEFT_TOP, bool no_focus = false, CMenuElementUI* pOwner = NULL);
	void Show();
	// ���µ����˵��Ĵ�С
	void ResizeMenu();
	// ���µ����Ӳ˵��Ĵ�С
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
	void OnFinalMessage(HWND hWnd) override;
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

	virtual void PaintChild(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;

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
