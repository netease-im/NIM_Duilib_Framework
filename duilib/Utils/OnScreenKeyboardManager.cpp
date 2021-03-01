#include "StdAfx.h"
#include "OnScreenKeyboardManager.h"
#include <initguid.h>
#include <shlobj.h>

namespace ui
{
DEFINE_GUID(CLSID_UIHostNoLaunch,
	0x4CE576FA, 0x83DC, 0x4f88, 0x95, 0x1C, 0x9D, 0x07, 0x82, 0xB4, 0xE3, 0x76);


DEFINE_GUID(IID_ITipInvocation,
	0x37c994e7, 0x432b, 0x4834, 0xa2, 0xf7, 0xdc, 0xe1, 0xf1, 0x3b, 0x83, 0x4b);

struct ITipInvocation : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE Toggle(HWND wnd) = 0;
};

OnScreenKeyboardManager* OnScreenKeyboardManager::GetInstance()
{
	static OnScreenKeyboardManager instance;
	return &instance;
}

OnScreenKeyboardManager::OnScreenKeyboardManager()
{

}

bool OnScreenKeyboardManager::IsVisible()
{
	auto hwnd = FindWindow(L"IPTip_Main_Window", NULL);
	if (hwnd) {
		auto style = GetWindowLong(hwnd, GWL_STYLE);
		return (style & WS_DISABLED) == 0 && (style & WS_VISIBLE) != 0;
	}

	return false;
}

void OnScreenKeyboardManager::ShowOSK(bool show)
{
	static bool bNeedOsk = IsWindows8OrGreater();
	if (bNeedOsk) {
		m_bShow = show;
		auto callback = nbase::Bind(&OnScreenKeyboardManager::TimerCallback, this);
		TimerManager::GetInstance()->AddCancelableTimer(this->GetWeakFlag(), callback, 10, 1);
	}
}

void OnScreenKeyboardManager::TimerCallback()
{
	// https://stackoverflow.com/questions/38774139/show-touch-keyboard-tabtip-exe-in-windows-10-anniversary-edition/40921638#40921638
	// https://github.com/WeAreROLI/JUCE/blob/develop/modules/juce_gui_basics/native/juce_win32_Windowing.cpp#L918
	// https://github.com/wojtkowiak/electron-windows-osk/blob/master/main.cpp
	// https://github.com/TransposonY/GestureSign/blob/master/GestureSign.CorePlugins/TouchKeyboard/TouchKeyboard.cs
	const bool isActive = IsVisible();
	if (isActive != m_bShow) {
		if (!isActive) {
			ITipInvocation *tip = nullptr;
			CoCreateInstance(CLSID_UIHostNoLaunch, 0, CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER, IID_ITipInvocation, (void**)&tip);

			if (tip) {
				tip->Toggle(GetDesktopWindow());
				tip->Release();
				tip = nullptr;
			}
			else {
				StartTabTip();
			}
		}
		else {
			if (auto hwnd = FindWindow(L"IPTip_Main_Window", NULL))
				PostMessage(hwnd, WM_SYSCOMMAND, (int)SC_CLOSE, 0);
		}
	}
}

void OnScreenKeyboardManager::StartTabTip() 
{
	DWORD bufferSize = GetEnvironmentVariable(L"CommonProgramW6432", nullptr, 0);

	std::wstring tabTipPath;
	if (bufferSize) {
		tabTipPath.resize(bufferSize);
		GetEnvironmentVariable(L"CommonProgramW6432", &tabTipPath[0], bufferSize);

		if (!tabTipPath.empty()) {
			if (tabTipPath[tabTipPath.length() - 1] == '\0') {
				tabTipPath = tabTipPath.substr(0, tabTipPath.length() - 1);
			}
			tabTipPath = tabTipPath + LR"(\microsoft shared\ink\TabTip.exe)";
		}
	}

	if (tabTipPath.empty()) {
		tabTipPath = LR"(C:\\Program Files\Common Files\microsoft shared\ink\TabTip.exe)";
	}

	ShellExecute(NULL, L"open", tabTipPath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

}