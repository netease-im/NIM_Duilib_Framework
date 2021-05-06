// popover.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "popover_form.h"

enum ThreadId
{
  kThreadUI
};

class MainThread : public nbase::FrameworkThread
{
public:
  MainThread() : nbase::FrameworkThread("MainThread") {}
  virtual ~MainThread() {}

private:

  virtual void Init() override
  {
	nbase::ThreadManager::RegisterThread(kThreadUI);

	// 获取资源路径，初始化全局参数
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();

	ui::GlobalManager::EnableAutomation(true);
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), true);


	// 创建一个默认带有阴影的居中窗口
	PopoverForm* window = new PopoverForm();
	window->Create(NULL, PopoverForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
  }

  /**
  * 虚函数，主线程退出时，做一些清理工作
  * @return void	无返回值
  */
  virtual void Cleanup() override
  {
    ui::GlobalManager::Shutdown();
    SetThreadWasQuitProperly(true);
    nbase::ThreadManager::UnregisterThread();
  }
};


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