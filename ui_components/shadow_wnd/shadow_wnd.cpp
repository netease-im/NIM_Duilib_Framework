#include "stdafx.h"
#include "shadow_wnd.h"

namespace nim_comp {

ShadowWnd::ShadowWnd()
{

}

std::wstring ShadowWnd::GetSkinFolder()
{
	return L"shadow";
}

std::wstring ShadowWnd::GetSkinFile()
{
	return L"shadow.xml";
}

std::wstring ShadowWnd::GetWindowClassName() const
{
	return L"ShadowWindow";
}

HWND ShadowWnd::Create(Window* window)
{
	window_ = window;
	return Window::Create(NULL, L"ShadowWindow", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, true);
}

LRESULT ShadowWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (window_ == NULL || window_->GetHWND() == NULL)
	{
		return FALSE;
	}

	switch (uMsg)
	{
	case WM_ERASEBKGND:
	case WM_PAINT:
	case WM_MOVE:
	case WM_ACTIVATE:
	case WM_NCACTIVATE:
	{
		if (::IsWindowVisible(window_->GetHWND()))
		{
			RECT rc;
			::GetWindowRect(window_->GetHWND(), &rc);
			SetPos(rc, false, SWP_SHOWWINDOW | SWP_NOACTIVATE, window_->GetHWND());
		}
		break;
	}
	case WM_CLOSE:
	{
		ShowWindow(false, false);
		Close(0);
		break;
	}
	case WM_SHOWWINDOW:
	{
		ShowWindow(wParam == 0 ? false : true, false);
		break;
	}
	default:
	{
		break;
	}

	}

	bHandled = FALSE;
	return FALSE;
}

ShadowWndBase::ShadowWndBase()
{
	shadow_wnd_ = new ShadowWnd;
	this->AddMessageFilter(shadow_wnd_);
}

HWND ShadowWndBase::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, 
	bool isLayeredWindow /*= false*/, const ui::UiRect& rc /*= ui::UiRect(0, 0, 0, 0)*/)
{
	__super::Create(hwndParent, pstrName, dwStyle, dwExStyle, false, rc);

	shadow_wnd_->Create(this);
	::EnableWindow(shadow_wnd_->GetHWND(), FALSE);
	if (::IsWindowVisible(GetHWND()))
		shadow_wnd_->ShowWindow();
	return GetHWND();
}

} // namespace ui
