#include "stdafx.h"
#include "cef_control_base.h"
#include "cef_control/util/util.h"

namespace nim_comp {

void CefControlBase::OnPaint(CefRefPtr<CefBrowser> browser, CefRenderHandler::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const std::string* buffer, int width, int height)
{
	return;
}

void CefControlBase::ClientToControl(POINT &pt)
{
	return;
}

void CefControlBase::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
	return;
}

void CefControlBase::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
	return;
}

void CefControlBase::UpdateWindowPos()
{
	this->SetPos(this->GetPos());
}

void CefControlBase::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	if (cb_before_menu_)
		cb_before_menu_(params, model);
}

bool CefControlBase::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, CefContextMenuHandler::EventFlags event_flags)
{
	if (cb_menu_command_)
		return cb_menu_command_(params, command_id, event_flags);

	return false;
}

void CefControlBase::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
	if (cb_url_change_)
		cb_url_change_(url);

	if (frame->IsMain())
	{
		auto old_url = url_;
		url_ = frame->GetURL();
		if (cb_main_url_change_ != nullptr && GetMainURL(old_url).compare(GetMainURL(url_)) != 0)
		{
			cb_main_url_change_(old_url, url_);
		}
	}
}

void CefControlBase::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	if (cb_title_change_)
		cb_title_change_(title);
}

void CefControlBase::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
	if (cb_loadstate_change_)
		cb_loadstate_change_(isLoading, canGoBack, canGoForward);
}

void CefControlBase::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
	if (cb_load_start_)
		cb_load_start_();
}

void CefControlBase::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	if (cb_load_end_)
		cb_load_end_(httpStatusCode);
}

void CefControlBase::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	if (cb_load_error_)
		cb_load_error_(errorCode, errorText, failedUrl);
}

bool CefControlBase::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	CefLifeSpanHandler::WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	if (cb_link_click_ && !target_url.empty())
		return cb_link_click_(target_url);

	return false;
}

bool CefControlBase::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (cb_after_created_)
		cb_after_created_(browser);

	return false;
}

void CefControlBase::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	if (cb_before_close_)
		cb_before_close_(browser);
}

bool CefControlBase::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
	bool result = false;

	if (cb_before_browser_)
		result = cb_before_browser_(browser, frame, request, is_redirect);

	return result;
}

void CefControlBase::OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution)
{
	if (cb_protocol_execution_)
		cb_protocol_execution_(browser, url, allow_os_execution);
}

CefRequestHandler::ReturnValue CefControlBase::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
{
	if (cb_before_resource_load_)
		return cb_before_resource_load_(request, false);

	return RV_CONTINUE;
}

void CefControlBase::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status)
{
	return;
}

void CefControlBase::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
{
	if (cb_before_download_)
		cb_before_download_(browser, download_item, suggested_name, callback);
}

void CefControlBase::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
{
	if (cb_download_updated_)
		cb_download_updated_(browser, download_item, callback);
}

bool CefControlBase::OnFileDialog(CefRefPtr<CefBrowser> browser, CefDialogHandler::FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback)
{
	if (cb_file_dialog_)
		return cb_file_dialog_(mode, title, default_file_path, accept_filters, selected_accept_filter, callback);
	else
		return false;
}

bool CefControlBase::OnExecuteCppFunc(const CefString& function_name, const CefString& params, int js_callback_id, CefRefPtr<CefBrowser> browser)
{
	if (js_bridge_.get())
	{
		js_callback_thread_id_ = nbase::FrameworkThread::GetManagedThreadId();
		return js_bridge_->ExecuteCppFunc(function_name, params, js_callback_id, browser);
	}

	return false;
}

bool CefControlBase::OnExecuteCppCallbackFunc(int cpp_callback_id, const CefString& json_string)
{
	if (js_bridge_.get())
	{
		if (js_callback_thread_id_ != -1)
		{
			nbase::ThreadManager::PostTask(js_callback_thread_id_, [this, cpp_callback_id, json_string]
			{
				js_bridge_->ExecuteCppCallbackFunc(cpp_callback_id, json_string);
			});
		}
		else
		{
			return js_bridge_->ExecuteCppCallbackFunc(cpp_callback_id, json_string);
		}

	}

	return false;
}

}