#include "stdafx.h"
#include "datetime_form.h"
#include "DateTime.h"
#include <fstream>

const std::wstring DateTimeForm::kClassName = L"DateTimeSample";


DateTimeForm::DateTimeForm()
{
}


DateTimeForm::~DateTimeForm()
{
}


ui::Control* DateTimeForm::CreateControl(const std::wstring& pstrClass)
{
    if (pstrClass == L"DateTime")
    {
        return new ui::DateTime();
    }

    return nullptr;
}

std::wstring DateTimeForm::GetSkinFolder()
{
	return L"datetime";
}

std::wstring DateTimeForm::GetSkinFile()
{
	return L"datetime.xml";
}

std::wstring DateTimeForm::GetWindowClassName() const
{
	return kClassName;
}

void DateTimeForm::InitWindow()
{
    
}

LRESULT DateTimeForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

