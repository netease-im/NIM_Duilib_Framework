#include "stdafx.h"
#include "layouts_form.h"

LayoutsForm::LayoutsForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
	: class_name_(class_name)
	, theme_directory_(theme_directory)
	, layout_xml_(layout_xml)
{
}


LayoutsForm::~LayoutsForm()
{
}

std::wstring LayoutsForm::GetSkinFolder()
{
	return theme_directory_;
}

std::wstring LayoutsForm::GetSkinFile()
{
	return layout_xml_;
}

std::wstring LayoutsForm::GetWindowClassName() const
{
	return class_name_;
}

void LayoutsForm::InitWindow()
{

}

LRESULT LayoutsForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void LayoutsForm::ShowCustomWindow(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
{
	LayoutsForm* window = new LayoutsForm(class_name, theme_directory, layout_xml);
	window->Create(NULL, class_name.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
}
