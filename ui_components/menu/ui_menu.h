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
};

/////////////////////////////////////////////////////////////////////////////////////
//

extern const TCHAR* const kMenuElementUIInterfaceName;// = _T("MenuElement);
class CMenuElementUI;
class CMenuWnd : public ui::WindowImplBase
{
public:
	virtual Control* CreateControl(const std::wstring& pstrClass) override;

	enum PopupPosType
	{
		RIGHT_BOTTOM,
		RIGHT_TOP
	};

	CMenuWnd(HWND hParent = NULL);
	void Init(STRINGorID xml, LPCTSTR pSkinType, POINT point, PopupPosType popupPosType = RIGHT_BOTTOM, bool no_focus = false);
	std::wstring GetWindowClassName() const;

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual std::wstring GetSkinFolder() override {
		return L"menu";
	}
	virtual std::wstring GetSkinFile() override {
		return m_xml.m_lpstr;
	}

	void Show();

public:
	HWND m_hParent;
	POINT m_BasedPoint;
	PopupPosType m_popupPosType;
	STRINGorID m_xml;
	std::wstring m_sType;
	bool no_focus_;
};

class ListContainerElement;
class CMenuElementUI : public ui::ListContainerElement
{
	friend CMenuWnd;
public:
	CMenuElementUI();
	~CMenuElementUI();

	virtual bool ButtonUp(EventArgs& msg) override;
};

} // namespace nim_comp

#endif // __UIMENU_H__
