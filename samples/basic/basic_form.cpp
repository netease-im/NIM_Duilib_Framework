#include "stdafx.h"
#include "resource.h"
#include "basic_form.h"

const std::wstring BasicForm::kClassName = L"Basic";

BasicForm::BasicForm()
{
}


BasicForm::~BasicForm()
{
}

std::wstring BasicForm::GetSkinFolder()
{
	return L"basic";
}

std::wstring BasicForm::GetSkinFile()
{
	return L"basic.xml";
}

std::wstring BasicForm::GetWindowClassName() const
{
	return kClassName;
}

void BasicForm::InitWindow()
{
	TrayIcon::GetInstance()->Init(this);
	TrayIcon::GetInstance()->SetTrayIcon(::LoadIconW(nbase::win32::GetCurrentModuleHandle(), MAKEINTRESOURCE(IDI_BASIC)), L"base");
}

void BasicForm::LeftClick()
{
	this->ActiveWindow();
	::SetForegroundWindow(m_hWnd);
	::BringWindowToTop(m_hWnd);
}


void BasicForm::RightClick()
{
	MessageBox(NULL, L"test", TEXT("RightClick"), MB_OK);
}

LRESULT BasicForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
	/*PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);*/
}
