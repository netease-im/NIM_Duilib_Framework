#include "stdafx.h"
#include "about_form.h"
#include <shellapi.h>

const LPCTSTR AboutForm::kClassName = L"About";

AboutForm::AboutForm()
{
}


AboutForm::~AboutForm()
{
}

std::wstring AboutForm::GetSkinFolder()
{
	return L"controls";
}

std::wstring AboutForm::GetSkinFile()
{
	return L"about.xml";
}

std::wstring AboutForm::GetWindowClassName() const
{
	return kClassName;
}

void AboutForm::InitWindow()
{
	ui::Label* link = static_cast<ui::Label*>(FindControl(L"link"));
	if (link)
	{
		link->AttachButtonUp([link](ui::EventArgs* args) {
			ShellExecute(NULL, L"open", link->GetText().c_str(), NULL, NULL, SW_SHOWDEFAULT);
			return true;
		});
	}
}

