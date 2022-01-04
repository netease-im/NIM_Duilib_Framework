#include "stdafx.h"
#include "client_app.h" 
#include <string>

#include "include/cef_cookie.h"
#include "include/cef_process_message.h"
#include "include/cef_task.h"
#include "include/cef_v8.h"
#include "cef_control/manager/cef_manager.h"
#include "cef_control/util/util.h"

namespace nim_comp
{

ClientApp::ClientApp() 
{
	last_node_is_editable_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// CefApp methods.
void ClientApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	// Pass additional command-line flags to the browser process.
	if (process_type.empty()) 
	{
		command_line->AppendSwitchWithValue("ppapi-flash-version", "32.0.0.238");
		command_line->AppendSwitchWithValue("ppapi-flash-path", "pepperflash\\pepflashplayer32_32_0_0_238.dll");

		//同一个域下的使用同一个渲染进程
		command_line->AppendSwitch("process-per-site");
		command_line->AppendSwitch("disable-gpu");
		command_line->AppendSwitch("disable-gpu-compositing");
		//command_line->AppendSwitchWithValue("proxy-server", "SOCKS5://127.0.0.1:1080");	

		// 开启离屏渲染
		if (CefManager::GetInstance()->IsEnableOffsetRender())
		{
			command_line->AppendSwitch("disable-surfaces");
			command_line->AppendSwitch("enable-begin-frame-scheduling");
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void ClientApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
{
	// Default schemes that support cookies.
	cookieable_schemes_.push_back("http");
	cookieable_schemes_.push_back("https");
}

}