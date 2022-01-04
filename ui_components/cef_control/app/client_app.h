/** @file client_app.h
  * @brief 定义Browser进程的CefApp类，管理Cef模块的生命周期
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2016/7/19
*/
#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "include/cef_app.h"
#include "cef_control/app/cef_js_bridge.h"

namespace nim_comp
{
class ClientApp : public CefApp,
                  public CefBrowserProcessHandler,
                  public CefRenderProcessHandler
{
public:
	ClientApp();

private:
	// CefApp methods.
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	virtual void OnRegisterCustomSchemes( CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE;
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE{ return this; }

	// CefBrowserProcessHandler methods.
	virtual void OnContextInitialized() OVERRIDE;
	virtual void OnBeforeChildProcessLaunch(
		CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;

	// CefRenderProcessHandler methods.
	virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
	virtual void OnWebKitInitialized() OVERRIDE;
	virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
	virtual bool OnBeforeNavigation(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		NavigationType navigation_type,
		bool is_redirect) OVERRIDE;
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnUncaughtException(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context,
		CefRefPtr<CefV8Exception> exception,
		CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;
	virtual void OnFocusedNodeChanged(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefDOMNode> node) OVERRIDE;
	virtual bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) OVERRIDE;
private:
	std::shared_ptr<CefJSBridge>	render_js_bridge_;
	std::vector<CefString>			cookieable_schemes_;
	// Schemes that will be registered with the global cookie manager. Used in
	bool							last_node_is_editable_;
	// both the browser and renderer process.
	IMPLEMENT_REFCOUNTING(ClientApp);
};
}