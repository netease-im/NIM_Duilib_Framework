#include "stdafx.h"
#include "cef_manager.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/base/cef_bind.h"

#include "cef_control/app/client_app.h"
#include "cef_control/handler/browser_handler.h"

namespace nim_comp
{

BOOL CefMessageLoopDispatcher::IsIdleMessage(const MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
	case WM_PAINT:
		return FALSE;
	}

	return TRUE;
}

bool CefMessageLoopDispatcher::Dispatch(const MSG &msg)
{
	static BOOL bDoIdle = TRUE;

	TranslateMessage(&msg);
	DispatchMessage(&msg);

	if (IsIdleMessage(&msg))
	{
		bDoIdle = TRUE;
	}

	while (bDoIdle && !::PeekMessage(const_cast<MSG*>(&msg), NULL, 0, 0, PM_NOREMOVE))
	{
		CefDoMessageLoopWork();
		bDoIdle = FALSE;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////

// 发现一个非常奇葩的bug，离屏渲染+多线程消息循环模式下，在浏览器对象上右击弹出菜单，是无法正常关闭的
// 翻cef源码后发现菜单是用TrackPopupMenu函数创建的，在MSDN资料上查看后发现调用TrackPopupMenu前
// 需要给其父窗口调用SetForegroundWindow。但是在cef源码中没有调用
// 最终翻cef源码后得到的解决方法是在cef的UI线程创建一个窗口，这个窗体的父窗口必须是在主程序UI线程创建的
// 这样操作之后就不会出现菜单无法关闭的bug了，虽然不知道为什么但是bug解决了
void FixContextMenuBug(HWND hwnd)
{
	CreateWindow(L"Static", L"", WS_CHILD, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
	PostMessage(hwnd, WM_CLOSE, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////
CefManager::CefManager()
{
	browser_count_ = 0;
	is_enable_offset_render_ = true;
}

CefManager::~CefManager()
{
	ASSERT(map_drag_target_reference_.empty());
}

void CefManager::AddCefDllToPath()
{
#if !defined(SUPPORT_CEF)
	return;
#endif

	TCHAR path_envirom[4096] = { 0 };
	GetEnvironmentVariable(L"path", path_envirom, 4096);
	
	std::wstring cef_path = nbase::win32::GetCurrentModuleDirectory();

#ifdef _WIN64
	cef_path += L"cef_x64";
#else
	cef_path += L"cef";
#endif

	if (!nbase::FilePathIsExist(cef_path, true))
	{
		MessageBox(NULL, L"请解压Cef.rar压缩包", L"提示", MB_OK);
		exit(0);
	}
	std::wstring new_envirom(cef_path);
	new_envirom.append(L";").append(path_envirom);
	SetEnvironmentVariable(L"path", new_envirom.c_str());

	// 解决播放flash弹出黑框的问题
	// https://blog.csdn.net/zhuhongshu/article/details/77482985
	std::wstring cmd_path = cef_path + L"\\dummy_cmd.exe";
	SetEnvironmentVariable(L"ComSpec", cmd_path.c_str());
}

// Cef的初始化接口，同时备注了使用各个版本的Cef时遇到的各种坑
// Cef1916版本较稳定，各个功能使用正常，但是某些在debug模式网页打开时会出中断警告（但并不是错误），可能是因为对新html标准支持不够，但是在release模式下正常使用
// Cef2357版本无法使用，当程序处理重定向信息并且重新加载页面后，渲染进程会崩掉
// Cef2526、2623版本对各种新页面都支持，唯一的坑就是debug模式在多线程消息循环开启下，程序退出时会中断，但是release模式正常。
//		(PS:如果开发者不使用负责Cef功能的开发，可以切换到release模式的cef dll文件，这样即使在deubg下也不会报错，修改AddCefDllToPath代码可以切换到release目录)
bool CefManager::Initialize(const std::wstring& app_data_dir, CefSettings &settings, bool is_enable_offset_render /*= true*/)
{
#if !defined(SUPPORT_CEF)
	return true;
#endif
	is_enable_offset_render_ = is_enable_offset_render;

	CefMainArgs main_args(GetModuleHandle(NULL));
	CefRefPtr<ClientApp> app(new ClientApp);
	
	// 如果是在子进程中调用，会堵塞直到子进程退出，并且exit_code返回大于等于0
	// 如果在Browser进程中调用，则立即返回-1
	int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
	if (exit_code >= 0)
		return false;

	GetCefSetting(app_data_dir, settings);

	bool bRet = CefInitialize(main_args, settings, app.get(), NULL);
	
	if (is_enable_offset_render_)
	{
		HWND hwnd = CreateWindow(L"Static", L"", WS_POPUP, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
		CefPostTask(TID_UI, base::Bind(&FixContextMenuBug, hwnd));
	}

	return bRet;
}

void CefManager::UnInitialize()
{
#if !defined(SUPPORT_CEF)
	return;
#endif
	CefShutdown();
}

bool CefManager::IsEnableOffsetRender() const
{
	return is_enable_offset_render_;
}

nbase::Dispatcher* CefManager::GetMessageDispatcher()
{
	return &message_dispatcher_;
}

void CefManager::AddBrowserCount()
{
	browser_count_++;
}

void CefManager::SubBrowserCount()
{
	browser_count_--;
	ASSERT(browser_count_ >= 0);
}

int CefManager::GetBrowserCount()
{
	return browser_count_;
}

void CefManager::PostQuitMessage(int nExitCode)
{
#if !defined(SUPPORT_CEF)
	::PostQuitMessage(nExitCode);
	return;
#endif

	// 当我们需要结束进程时，千万不要直接调用::PostQuitMessage，这是可能还有浏览器对象没有销毁
	// 应该等所有浏览器对象都销毁后再调用::PostQuitMessage
	if (browser_count_ == 0)
	{
		nbase::ThreadManager::PostTask(kThreadUI, [nExitCode]()
	 	{
			::PostQuitMessage(nExitCode);
		});
	}
	else
	{
		auto cb = [nExitCode]()
		{
			CefManager::GetInstance()->PostQuitMessage(nExitCode);
		};

		nbase::ThreadManager::PostDelayedTask(kThreadUI, cb, nbase::TimeDelta::FromMilliseconds(500));
	}
}

client::DropTargetHandle CefManager::GetDropTarget(HWND hwnd)
{
	// 查找是否存在这个弱引用
	auto it = map_drag_target_reference_.find(hwnd);
	if (it == map_drag_target_reference_.end()) {
		auto deleter = [this](client::DropTargetWin *src) {
			auto it = map_drag_target_reference_.find(src->GetHWND());
			if (it != map_drag_target_reference_.end()) {
				RevokeDragDrop(src->GetHWND());

				// 移除弱引用对象
				map_drag_target_reference_.erase(it);
			}
			else {
				ASSERT(false);
			}

			delete src;
		};
		
		// 如果不存在就新增一个
		client::DropTargetHandle handle(new client::DropTargetWin(hwnd), deleter);
		map_drag_target_reference_[hwnd] = handle;

		HRESULT register_res = RegisterDragDrop(hwnd, handle.get());
		ASSERT(register_res == S_OK);

		return handle;
	}
	else {
		// 如果存在就返回弱引用对应的强引用指针
		return it->second.lock();
	}
}

void CefManager::GetCefSetting(const std::wstring& app_data_dir, CefSettings &settings)
{
	if (false == nbase::FilePathIsExist(app_data_dir, true))
		nbase::CreateDirectory(app_data_dir);

	settings.no_sandbox = true;

	// 设置localstorage，不要在路径末尾加"\\"，否则运行时会报错
	CefString(&settings.cache_path) = app_data_dir + L"CefLocalStorage";

	// 设置debug log文件位置
	CefString(&settings.log_file) = app_data_dir + L"cef.log";

	// 调试模型下使用单进程，但是千万不要在release发布版本中使用，官方已经不推荐使用单进程模式
	// cef1916版本debug模式:在单进程模式下程序退出时会触发中断
#ifdef _DEBUG
	settings.single_process = true;
#else
	settings.single_process = false;
#endif

	// cef2623、2526版本debug模式:在使用multi_threaded_message_loop时退出程序会触发中断
	// 加入disable-extensions参数可以修复这个问题，但是会导致一些页面打开时报错
	// 开启Cef多线程消息循环，兼容nbase库消息循环
	settings.multi_threaded_message_loop = true;

	// 开启离屏渲染
	settings.windowless_rendering_enabled = is_enable_offset_render_;
}

}