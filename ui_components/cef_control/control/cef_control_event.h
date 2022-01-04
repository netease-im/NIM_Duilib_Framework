/** @file cef_control_event.h
* @brief Cef控件事件处理接口
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2016/7/22
*/
#pragma once
#include "include/cef_load_handler.h"
#include "include/cef_request_handler.h"
#include "include/cef_context_menu_handler.h"
#include "include/cef_download_handler.h"
#include "include/cef_dialog_handler.h"

namespace ui 
{
	typedef std::function<void(CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)> OnBeforeMenuEvent;
	typedef std::function<bool(CefRefPtr<CefContextMenuParams> params, int command_id, CefContextMenuHandler::EventFlags event_flags)> OnMenuCommandEvent;

	typedef std::function<void(const std::wstring& title)> OnTitleChangeEvent;
	typedef std::function<void(const std::wstring& url)> OnUrlChangeEvent;

	typedef std::function<bool(const std::wstring& url)> OnLinkClickEvent;
	typedef std::function<CefRequestHandler::ReturnValue(CefRefPtr<CefRequest> request, bool is_redirect)> OnBeforeResourceLoadEvent;
	typedef std::function<void(const CefString& old_url, const CefString& new_url)> OnMainURLChengeEvent;

	typedef std::function<void(bool isLoading, bool canGoBack, bool canGoForward)> OnLoadingStateChangeEvent;
	typedef std::function<void()> OnLoadStartEvent;
	typedef std::function<void(int httpStatusCode)> OnLoadEndEvent;
	typedef std::function<void(CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)> OnLoadErrorEvent;
	typedef std::function<void(bool visible)> OnDevToolAttachedStateChangeEvent;

	typedef std::function<void(CefRefPtr<CefBrowser> browser)> OnAfterCreatedEvent;
	typedef std::function<void(CefRefPtr<CefBrowser> browser)> OnBeforeCloseEvent;

	typedef std::function<bool(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)> OnBeforeBrowserEvent;
	typedef std::function<void(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution)> OnProtocolExecutionEvent;

	typedef std::function<void(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)> OnBeforeDownloadEvent;
	typedef std::function<void(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)> OnDownloadUpdatedEvent;

	typedef std::function<bool(CefDialogHandler::FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback)> OnFileDialogEvent;
}