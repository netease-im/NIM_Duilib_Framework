// basic.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include "move_control.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 创建主线程
	MainThread thread;

	// 执行主线程循环
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	return 0;
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	// 获取资源路径，初始化全局参数
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);
	MoveControlForm::ShowCustomWindow(L"move_control", L"move_control", L"main.xml");
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
