#include "stdafx.h"
#include "js_handler.h"
#include "cef_control/util/util.h"
#include "cef_control/app/ipc_string_define.h"
#include "cef_control/app/cef_js_bridge.h"

namespace nim_comp
{

bool CefJSHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	// 当Web中调用了"NimCefWebFunction"函数后，会触发到这里，然后把参数保存，转发到Broswer进程
	// Broswer进程的BrowserHandler类在OnProcessMessageReceived接口中处理kJsCallbackMessage消息，就可以收到这个消息

	if (arguments.size() < 2)
	{
		exception = "Invalid arguments.";
		return false;
	}

	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	CefRefPtr<CefFrame> frame = context->GetFrame();
	CefRefPtr<CefBrowser> browser = context->GetBrowser();

	int64_t browser_id = browser->GetIdentifier();
	int64_t frame_id = frame->GetIdentifier();

	if (name == "call")
	{
		// 允许没有参数列表的调用，第二个参数为回调
		// 如果传递了参数列表，那么回调是第三个参数
		CefString function_name = arguments[0]->GetStringValue();
		CefString params = "{}";
		CefRefPtr<CefV8Value> callback;
		if (arguments[0]->IsString() && arguments[1]->IsFunction())
		{
			callback = arguments[1];
		}
		else if (arguments[0]->IsString() && arguments[1]->IsString() && arguments[2]->IsFunction())
		{
			params = arguments[1]->GetStringValue();
			callback = arguments[2];
		}
		else
		{
			exception = "Invalid arguments.";
			return false;
		}

		// 执行 C++ 方法
		if (!js_bridge_->CallCppFunction(function_name, params, callback))
		{
			exception = nbase::StringPrintf("Failed to call function %s.", function_name).c_str();
			return false;
		}

		return true;
	}
	else if (name == "register")
	{
		if (arguments[0]->IsString() && arguments[1]->IsFunction())
		{
			std::string function_name = arguments[0]->GetStringValue();
			CefRefPtr<CefV8Value> callback = arguments[1];
			if (!js_bridge_->RegisterJSFunc(function_name, callback))
			{
				exception = "Failed to register function.";
				return false;
			}
			return true;
		}
		else
		{
			exception = "Invalid arguments.";
			return false;
		}
	}

	return false;
}

}
