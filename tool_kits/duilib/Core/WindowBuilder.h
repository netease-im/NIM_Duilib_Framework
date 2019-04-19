#ifndef UI_CORE_WINDOWBUILDER_H_
#define UI_CORE_WINDOWBUILDER_H_

#pragma once

namespace ui 
{

class Box;
class Window;
typedef std::function<Control* (const std::wstring&)> CreateControlCallback;

class UILIB_API WindowBuilder
{
public:
    WindowBuilder();

	Box* Create(STRINGorID xml, CreateControlCallback pCallback = CreateControlCallback(),
		Window* pManager = nullptr, Box* pParent = nullptr, Box* pUserDefinedBox = nullptr);
	Box* Create(CreateControlCallback pCallback = CreateControlCallback(), Window* pManager = nullptr,
		Box* pParent = nullptr, Box* pUserDefinedBox = nullptr);

    CMarkup* GetMarkup();

    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;

private:
    Control* _Parse(CMarkupNode* parent, Control* pParent = NULL, Window* pManager = NULL);
	Control* CreateControlByClass(const std::wstring& strControlClass);
	void AttachXmlEvent(bool bBubbled, CMarkupNode& node, Control* pParent);

private:
    CMarkup m_xml;
	CreateControlCallback m_createControlCallback;
};

} // namespace ui

#endif // UI_CORE_WINDOWBUILDER_H_
