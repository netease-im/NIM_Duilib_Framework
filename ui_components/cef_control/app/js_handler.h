/** @file js_handler.h
 * @brief 定义一个C++与JS进行交互的JsHandler类
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/7/19
 */
#pragma once
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "cef_control/app/cef_js_bridge.h"

namespace nim_comp
{
	
class CefJSHandler : public CefV8Handler
{
public:
	CefJSHandler() {}
	virtual bool Execute(const CefString& name,	CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;
	void AttachJSBridge(std::shared_ptr<CefJSBridge> js_bridge) { js_bridge_ = js_bridge; }

	IMPLEMENT_REFCOUNTING(CefJSHandler);

private:
	std::shared_ptr<CefJSBridge> js_bridge_;
};
}

