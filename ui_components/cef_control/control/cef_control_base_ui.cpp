#include "stdafx.h"
#include "cef_control_base.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "cef_control/handler/browser_handler.h"
#include "cef_control/manager/cef_manager.h"
#include "cef_control/app/cef_js_bridge.h"

namespace nim_comp {

CefControlBase::CefControlBase(void)
{
	devtool_attached_ = false;
#if !defined(SUPPORT_CEF)
	ASSERT(FALSE && L"要使用Cef功能请开启SUPPORT_CEF宏");
#endif
}

CefControlBase::~CefControlBase(void)
{

}
void CefControlBase::LoadURL(const CefString& url)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		CefRefPtr<CefFrame> frame = browser_handler_->GetBrowser()->GetMainFrame();
		if (!frame)
			return;

		frame->LoadURL(url);
	}
	else
	{
		if (browser_handler_.get())
		{
			StdClosure cb = ToWeakCallback([this, url]()
			{
				LoadURL(url);
			});
			browser_handler_->AddAfterCreateTask(cb);
		}
	}
}

void CefControlBase::LoadString(const CefString& stringW, const CefString& url)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		CefRefPtr<CefFrame> frame = browser_handler_->GetBrowser()->GetMainFrame();
		if (!frame)
			return;

		frame->LoadStringW(stringW, url);
	}
	else
	{
		if (browser_handler_.get())
		{
			StdClosure cb = ToWeakCallback([this, stringW, url]()
			{
				LoadString(stringW, url);
			});
			browser_handler_->AddAfterCreateTask(cb);
		}
	}
}

void CefControlBase::GoBack()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->GoBack();
	}
}

void CefControlBase::GoForward()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->GoForward();
	}
}

bool CefControlBase::CanGoBack()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->CanGoBack();
	}
	return false;
}

bool CefControlBase::CanGoForward()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->CanGoForward();
	}
	return false;
}

void CefControlBase::Refresh()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->Reload();
	}
}

void CefControlBase::StopLoad()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->StopLoad();
	}
}

bool CefControlBase::IsLoading()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->IsLoading();
	}
	return false;
}

void CefControlBase::StartDownload(const CefString& url)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		browser_handler_->GetBrowser()->GetHost()->StartDownload(url);
	}
}

void CefControlBase::SetZoomLevel(float zoom_level)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		browser_handler_->GetBrowser()->GetHost()->SetZoomLevel(zoom_level);
	}
}

HWND CefControlBase::GetCefHandle() const
{
	if (browser_handler_.get() && browser_handler_->GetBrowserHost().get())
		return browser_handler_->GetBrowserHost()->GetWindowHandle();

	return NULL;
}

CefString CefControlBase::GetURL()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return browser_handler_->GetBrowser()->GetMainFrame()->GetURL();
	}

	return CefString();
}

std::string CefControlBase::GetUTF8URL()
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		return nbase::UTF16ToUTF8(std::wstring(GetURL().c_str()));
	}

	return CefString();
}

CefString CefControlBase::GetMainURL(const CefString& url)
{
	std::string temp = url.ToString();
	int end_pos = temp.find("#") == std::string::npos ? temp.length() : temp.find("#");
	temp = temp.substr(0, end_pos);
	return CefString(temp.c_str());
}

bool CefControlBase::RegisterCppFunc(const std::wstring& function_name, nim_comp::CppFunction function, bool global_function/* = false*/)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get() && js_bridge_.get())
	{
		return js_bridge_->RegisterCppFunc(nbase::UTF16ToUTF8(function_name).c_str(), function, global_function ? nullptr : browser_handler_->GetBrowser());
	}

	return false;
}

void CefControlBase::UnRegisterCppFunc(const std::wstring& function_name)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get() && js_bridge_.get())
	{
		js_bridge_->UnRegisterCppFunc(nbase::UTF16ToUTF8(function_name).c_str(), browser_handler_->GetBrowser());
	}
}

bool CefControlBase::CallJSFunction(const std::wstring& js_function_name, const std::wstring& params, nim_comp::CallJsFunctionCallback callback, const std::wstring& frame_name /*= L""*/)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get() && js_bridge_.get())
	{
		CefRefPtr<CefFrame> frame = frame_name == L"" ? browser_handler_->GetBrowser()->GetMainFrame() : browser_handler_->GetBrowser()->GetFrame(frame_name);

		if (!js_bridge_->CallJSFunction(nbase::UTF16ToUTF8(js_function_name).c_str(),
			nbase::UTF16ToUTF8(params).c_str(), frame, callback))
		{
			return false;
		}

		return true;
	}

	return false;
}

bool CefControlBase::CallJSFunction(const std::wstring& js_function_name, const std::wstring& params, nim_comp::CallJsFunctionCallback callback, int frame_id)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get() && js_bridge_.get())
	{
		CefRefPtr<CefFrame> frame = browser_handler_->GetBrowser()->GetFrame(frame_id);

		if (!js_bridge_->CallJSFunction(nbase::UTF16ToUTF8(js_function_name).c_str(),
			nbase::UTF16ToUTF8(params).c_str(), frame, callback))
		{
			return false;
		}

		return true;
	}

	return false;
}

void CefControlBase::RepairBrowser()
{
	ReCreateBrowser();
}

void CefControlBase::DettachDevTools()
{
	if (!devtool_attached_)
		return;
	auto browser = browser_handler_->GetBrowser();
	if (browser != nullptr)
	{
		browser->GetHost()->CloseDevTools();
		devtool_attached_ = false;
		if (cb_devtool_visible_change_ != nullptr)
			cb_devtool_visible_change_(devtool_attached_);
	}
}

}