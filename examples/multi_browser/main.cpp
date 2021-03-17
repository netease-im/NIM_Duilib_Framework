// basic.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include "browser/multi_browser_manager.h"

#pragma comment(lib, "dbghelp.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 将 bin\\cef 目录添加到环境变量，这样可以将所有 CEF 相关文件放到该目录下，方便管理
	// 在项目属性->连接器->输入，延迟加载 nim_libcef.dll
	nim_comp::CefManager::GetInstance()->AddCefDllToPath();

	_wsetlocale(LC_ALL, L"chs");

#ifdef _DEBUG
	AllocConsole();
	FILE* fp = NULL;
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
	wprintf_s(L"Command:\n%s\n\n", lpCmdLine);
#endif

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	// 初始化 CEF
	CefSettings settings;
	if (!nim_comp::CefManager::GetInstance()->Initialize(nbase::win32::GetCurrentModuleDirectory() + L"cef_temp\\", settings, true))
	{
		return 0;
	}

	// 创建主线程
	MainThread thread;

	// 执行主线程循环
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

    // 清理 CEF
    nim_comp::CefManager::GetInstance()->UnInitialize();

	::OleUninitialize();

	return 0;
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);

	nbase::TimeDelta time_delta = nbase::TimeDelta::FromMicroseconds(nbase::Time::Now().ToInternalValue());
	std::string timeStamp = nbase::StringPrintf("%I64u", time_delta.ToMilliseconds());
	MultiBrowserManager::GetInstance()->CreateBorwserBox(NULL, timeStamp, L"");
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
