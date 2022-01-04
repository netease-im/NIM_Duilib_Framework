#include "stdafx.h"
#include "cef_native_control.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "cef_control/handler/browser_handler.h"
#include "cef_control/manager/cef_manager.h"

namespace nim_comp {

CefNativeControl::CefNativeControl(void)
{

}

CefNativeControl::~CefNativeControl(void)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		// Request that the main browser close.
		browser_handler_->GetBrowserHost()->CloseBrowser(true);
		browser_handler_->SetHostWindow(NULL);
		browser_handler_->SetHandlerDelegate(NULL);
	}
}

void CefNativeControl::Init()
{
	if (browser_handler_.get() == nullptr)
	{
		LONG style = GetWindowLong(m_pWindow->GetHWND(), GWL_STYLE);
		SetWindowLong(m_pWindow->GetHWND(), GWL_STYLE, style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		ASSERT((GetWindowExStyle(m_pWindow->GetHWND()) & WS_EX_LAYERED) == 0 && L"无法在分层窗口内使用本控件");

		browser_handler_ = new nim_comp::BrowserHandler;
		browser_handler_->SetHostWindow(m_pWindow->GetHWND());
		browser_handler_->SetHandlerDelegate(this);
		ReCreateBrowser();
	}

	if (!js_bridge_.get())
	{
		js_bridge_.reset(new nim_comp::CefJSBridge);
	}

	__super::Init();
}

void CefNativeControl::ReCreateBrowser()
{
	if (browser_handler_->GetBrowser() == nullptr)
	{
		// 使用有窗模式
		CefWindowInfo window_info;
		window_info.SetAsChild(this->m_pWindow->GetHWND(), m_rcItem);

		CefBrowserSettings browser_settings;
		CefBrowserHost::CreateBrowser(window_info, browser_handler_, L"", browser_settings, NULL);
	}
}

void CefNativeControl::SetPos(UiRect rc)
{
	__super::SetPos(rc);

	HWND hwnd = GetCefHandle();
	if (hwnd) 
	{
		SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), SWP_NOZORDER);
	}
}

void CefNativeControl::HandleMessage(EventArgs& event)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get() == NULL)
		return __super::HandleMessage(event);

	else if (event.Type == kEventInternalSetFocus)
	{
		browser_handler_->GetBrowserHost()->SetFocus(true);
	}
	else if (event.Type == kEventInternalKillFocus)
	{
		browser_handler_->GetBrowserHost()->SetFocus(false);
	}

	__super::HandleMessage(event);
}

void CefNativeControl::SetVisible(bool bVisible /*= true*/)
{
	__super::SetVisible(bVisible);
	
	HWND hwnd = GetCefHandle();
	if (hwnd)
	{
		if (bVisible)
		{
			ShowWindow(hwnd, SW_SHOW);
		}
		else
		{
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}
}

void CefNativeControl::SetInternVisible(bool bVisible)
{
	__super::SetInternVisible(bVisible);

	HWND hwnd = GetCefHandle();
	if (hwnd)
	{
		if (bVisible)
		{
			ShowWindow(hwnd, SW_SHOW);
		}
		else
		{
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}
}

void CefNativeControl::SetWindow(ui::Window* pManager, ui::Box* pParent, bool bInit)
{
	if (browser_handler_)
		browser_handler_->SetHostWindow(pManager->GetHWND());

	// 设置Cef窗口句柄为新的主窗口的子窗口
	auto hwnd = GetCefHandle();
	if (hwnd)
		SetParent(hwnd, pManager->GetHWND());

	// 为新的主窗口重新设置WS_CLIPSIBLINGS、WS_CLIPCHILDREN样式，否则Cef窗口刷新会出问题
	LONG style = GetWindowLong(pManager->GetHWND(), GWL_STYLE);
	SetWindowLong(pManager->GetHWND(), GWL_STYLE, style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	__super::SetWindow(pManager, pParent, bInit);
}

bool CefNativeControl::AttachDevTools(Control* /*view*/)
{
	if (devtool_attached_)
		return true;

	auto browser = browser_handler_->GetBrowser();
	if (browser == nullptr)
	{
		auto task = ToWeakCallback([this]()
		{
			nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([this](){
				AttachDevTools(nullptr);
			}));
		});

		browser_handler_->AddAfterCreateTask(task);
	}
	else
	{
		CefWindowInfo windowInfo;
		windowInfo.SetAsPopup(NULL, L"cef_devtools");
		CefBrowserSettings settings;
		windowInfo.width = 900;
		windowInfo.height = 700;
		browser->GetHost()->ShowDevTools(windowInfo, new nim_comp::BrowserHandler, settings, CefPoint());
		devtool_attached_ = true;
		if (cb_devtool_visible_change_ != nullptr)
			cb_devtool_visible_change_(devtool_attached_);
	}
	return true;
}

}