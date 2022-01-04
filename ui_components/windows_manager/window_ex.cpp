#include "stdafx.h"
#include "window_ex.h"
#include "windows_manager.h"

namespace nim_comp
{
WindowEx::WindowEx()
{
}

WindowEx::~WindowEx()
{
}

HWND WindowEx::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, bool isLayeredWindow, const ui::UiRect& rc)
{
	if (!RegisterWnd())
	{
		return NULL;
	}

	HWND hwnd = __super::Create(hwndParent, pstrName, dwStyle, dwExStyle, isLayeredWindow, rc);
	ASSERT(hwnd);
	return hwnd;
}

LRESULT WindowEx::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UnRegisterWnd();
	return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
}

void WindowEx::OnEsc(BOOL &bHandled)
{
	bHandled = FALSE;
}

bool WindowEx::RegisterWnd()
{
	std::wstring wnd_class_name = GetWindowClassName();
	std::wstring wnd_id = GetWindowId();
	if (!WindowsManager::GetInstance()->RegisterWindow(wnd_class_name, wnd_id, this))
	{
		return false;
	}
	return true;
}

void WindowEx::UnRegisterWnd()
{
	std::wstring wnd_class_name = GetWindowClassName();
	std::wstring wnd_id = GetWindowId();
	WindowsManager::GetInstance()->UnRegisterWindow(wnd_class_name, wnd_id, this);
}

LRESULT WindowEx::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE)
	{
		if (!::IsWindowEnabled(m_hWnd))
		{
			::SetForegroundWindow(m_hWnd);
			return FALSE;
		}
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		KillFocus();
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_ESCAPE)
		{
			BOOL bHandled = FALSE;
			OnEsc(bHandled);
			if (!bHandled)
				this->Close();
		}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

POINT GetPopupWindowPos(WindowEx* window)
{
	ASSERT(window && IsWindow(window->GetHWND()));

	//屏幕大小
	MONITORINFO oMonitor = { sizeof(oMonitor) };
	::GetMonitorInfo(::MonitorFromWindow(window->GetHWND(), MONITOR_DEFAULTTONEAREST), &oMonitor);
	RECT screen = oMonitor.rcWork;

	ui::UiRect rect = window->GetPos(true);

	POINT pt = { 0, 0 };
	pt.x = screen.right - rect.GetWidth();
	pt.y = screen.bottom - rect.GetHeight();

	return pt;
}

}