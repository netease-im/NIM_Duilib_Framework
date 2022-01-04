#include "stdafx.h"
#include "browser_handler.h"
#include "include/cef_frame.h"
#include "cef_control/manager/cef_manager.h"
#include "cef_control/util/util.h"
#include "cef_control/app/ipc_string_define.h"
#include "cef_control/app/cef_js_bridge.h"

namespace nim_comp
{
BrowserHandler::BrowserHandler()
{
	handle_delegate_ = NULL;
	is_focus_oneditable_field_ = false;
	ZeroMemory(&rect_cef_control_, sizeof(RECT));
}

void BrowserHandler::SetViewRect(RECT rc)
{
	if (!CefCurrentlyOn(TID_UI)) {
		// 把操作跳转到Cef线程执行
		CefPostTask(TID_UI, base::Bind(&BrowserHandler::SetViewRect, this, rc));
		return;
	}

	rect_cef_control_ = rc;
	// 调用WasResized接口，调用后，BrowserHandler会调用GetViewRect接口来获取浏览器对象新的位置
	if (browser_.get() && browser_->GetHost().get())
		browser_->GetHost()->WasResized();
}

CefRefPtr<CefBrowserHost> BrowserHandler::GetBrowserHost()
{
	if (browser_.get())
	{
		return browser_->GetHost();
	}
	return NULL;
}

UnregisterCallback BrowserHandler::AddAfterCreateTask(const StdClosure& cb)
{
	return task_list_after_created_.AddCallback(cb);
}

void BrowserHandler::CloseAllBrowser()
{
	class CloseAllBrowserTask : public CefTask
	{
		IMPLEMENT_REFCOUNTING(CloseAllBrowserTask);
	public:
		CloseAllBrowserTask(const std::vector<CefRefPtr<CefBrowser>>& browser_list)
		{
			browser_list_.assign(browser_list.begin(), browser_list.end());
		}
	public:
		void Execute()
		{
			for (auto it : browser_list_) {
				if (it != nullptr)
					it->GetHost()->CloseBrowser(true);
			}
		}
	private:
		std::vector<CefRefPtr<CefBrowser>> browser_list_;
	};
	CefPostTask(TID_UI, new CloseAllBrowserTask(browser_list_));
}

bool BrowserHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	// 处理render进程发来的消息
	std::string message_name = message->GetName();
	if (message_name == kFocusedNodeChangedMessage)
	{
		is_focus_oneditable_field_ = message->GetArgumentList()->GetBool(0);
		return true;
	}
	else if (message_name == kCallCppFunctionMessage)
	{
		CefString fun_name	= message->GetArgumentList()->GetString(0);
		CefString param		= message->GetArgumentList()->GetString(1);
		int js_callback_id	= message->GetArgumentList()->GetInt(2);

		if (handle_delegate_)
			handle_delegate_->OnExecuteCppFunc(fun_name, param, js_callback_id, browser);

		return true;
	}
	else if (message_name == kExecuteCppCallbackMessage)
	{
		CefString param = message->GetArgumentList()->GetString(0);
		int callback_id = message->GetArgumentList()->GetInt(1);

		if (handle_delegate_)
			handle_delegate_->OnExecuteCppCallbackFunc(callback_id, param);
	}

	return false;
}

#pragma region CefLifeSpanHandler
// CefLifeSpanHandler methods
bool BrowserHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
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
	// 让新的链接在原浏览器对象中打开
	if (browser_.get() && !target_url.empty())
	{
		if (handle_delegate_)
		{
			// 返回true则继续在控件内打开新链接，false则禁止访问
			bool bRet = handle_delegate_->OnBeforePopup(browser, frame, target_url, target_frame_name, target_disposition, user_gesture, popupFeatures, windowInfo, client, settings, no_javascript_access);
			if (bRet)
				browser_->GetMainFrame()->LoadURL(target_url);
		}
		else
			browser_->GetMainFrame()->LoadURL(target_url);
	}

	// 禁止弹出popup窗口
	return true;
}

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	REQUIRE_UI_THREAD();
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([this, browser](){
		browser_list_.emplace_back(browser);
		if (browser_ != nullptr)
			browser_->GetHost()->WasHidden(true);
		browser_ = browser;
		CefManager::GetInstance()->AddBrowserCount();

		if (handle_delegate_)
		{
			handle_delegate_->OnAfterCreated(browser);
		}

		// 有窗模式下，浏览器创建完毕后，让上层更新一下自己的位置；因为在异步状态下，上层更新位置时可能Cef窗口还没有创建出来
		if (!CefManager::GetInstance()->IsEnableOffsetRender() && handle_delegate_)
		{
			handle_delegate_->UpdateWindowPos();
		}

		task_list_after_created_();
		task_list_after_created_.Clear();
	}));

	// 必须在cef ui线程调用RegisterDragDrop
	// 执行::DoDragDrop时，会在调用RegisterDragDrop的线程触发的DragOver、DragLeave、Drop、Drop回调
	// 进而调用browser_->GetHost()->DragTargetDragEnter、DragTargetDragOver、DragTargetDragLeave、DragTargetDrop
	// 这几个cef接口内部发现不在cef ui线程触发，则会转发到cef ui线程
	// 导致DragSourceEndedAt接口被调用时有部分DragTarget*方法没有被调用
	// 最终拖拽效果就会有问题，详见DragSourceEndedAt接口描述
	// 所以在cef ui线程调用RegisterDragDrop，让后面一系列操作都在cef ui线程里同步执行，则没问题
	//
	// RegisterDragDrop内部会在调用这个API的线程里创建一个窗口，用过这个窗口来做消息循环模拟阻塞的过程
	// 所以哪个线程调用RegisterDragDrop，就会在哪个线程阻塞并触发IDragTarget回调
	// 见https://docs.microsoft.com/zh-cn/windows/win32/api/ole2/nf-ole2-registerdragdrop
	if (hwnd_)
		drop_target_ = CefManager::GetInstance()->GetDropTarget(hwnd_);
}

bool BrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	return false;
}

void BrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	REQUIRE_UI_THREAD();
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([this, browser](){
		CefManager::GetInstance()->SubBrowserCount();
		auto it = std::find_if(browser_list_.begin(), browser_list_.end(), [&](const CefRefPtr<CefBrowser>& item){
			return item->IsSame(browser);
		});
		if (it != browser_list_.end())
		{
			auto closed_browser = *it;
			browser_list_.erase(it);
			if (closed_browser->IsSame(browser_))
			{
				browser_ = browser_list_.size() > 0 ? *browser_list_.rbegin() : nullptr;
				if (browser_ != nullptr)
				{
					browser_->GetHost()->WasHidden(false);
					browser_->Reload();
				}
			}
		}

		if (handle_delegate_)
		{
			handle_delegate_->OnBeforeClose(browser);
		}
	}));
}
#pragma endregion

#pragma region CefRenderHandler
// CefRenderHandler methods
bool BrowserHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	RECT window_rect = { 0 };
	HWND root_window = GetAncestor(hwnd_, GA_ROOT);
	if (::GetWindowRect(root_window, &window_rect))
	{
		rect = CefRect(window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top);
		return true;
	}
	return false;
}

bool BrowserHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	if (handle_delegate_)
	{
		rect.x = 0;
		rect.y = 0;
		rect.width = rect_cef_control_.right - rect_cef_control_.left;
		rect.height = rect_cef_control_.bottom - rect_cef_control_.top;
		return true;
	}
	else
	{
		RECT clientRect;
		if (!::GetClientRect(hwnd_, &clientRect))
			return false;
		rect.x = rect.y = 0;
		rect.width = clientRect.right;
		rect.height = clientRect.bottom;
		return true;
	}

}

bool BrowserHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
	if (!::IsWindow(hwnd_))
		return false;

	// Convert the point from view coordinates to actual screen coordinates.
	POINT screen_pt = { viewX, viewY };
	ClientToScreen(hwnd_, &screen_pt);
	screenX = screen_pt.x;
	screenY = screen_pt.y;
	return true;
}

void BrowserHandler::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnPopupShow, handle_delegate_, browser, show));
}

void BrowserHandler::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnPopupSize, handle_delegate_, browser, rect));
}

void BrowserHandler::OnPaint(CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList& dirtyRects,
	const void* buffer,
	int width,
	int height)
{
	if (handle_delegate_)
	{
		// 多线程消息循环模式中，OnPaint在Cef的线程被触发，这时把数据保存到paint_buffer_中，跳转到UI线程执行渲染操作。
		// 这里不对paint_buffer_加锁，即使两个线程操作paint_buffer_发生竞争，也只是让某一次渲染效果有瑕疵，不会崩溃，这个瑕疵是可以接受的
		int buffer_length = width * height * 4;
		if (buffer_length > (int)paint_buffer_.size())
			paint_buffer_.resize(buffer_length + 1);
		memcpy(&paint_buffer_[0], (char*)buffer, width * height * 4);

		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnPaint, handle_delegate_, browser, type, dirtyRects, &paint_buffer_, width, height));
	}
}

void BrowserHandler::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info)
{
	SetClassLongPtr(hwnd_, GCLP_HCURSOR, static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
	SetCursor(cursor);
}

bool BrowserHandler::StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, CefRenderHandler::DragOperationsMask allowed_ops, int x, int y)
{
	REQUIRE_UI_THREAD();
	if (!handle_delegate_)
		return false;

	if (!drop_target_)
		return false;

	current_drag_op_ = DRAG_OPERATION_NONE;
	CefBrowserHost::DragOperationsMask result =
		drop_target_->StartDragging(this, drag_data, allowed_ops, x, y);
	current_drag_op_ = DRAG_OPERATION_NONE;
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(hwnd_, &pt);
	handle_delegate_->ClientToControl(pt);
	browser->GetHost()->DragSourceEndedAt(
		pt.x,
		pt.y,
		result);
	browser->GetHost()->DragSourceSystemDragEnded();

	return true;
}

void BrowserHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser, CefRenderHandler::DragOperation operation)
{
	REQUIRE_UI_THREAD();
	current_drag_op_ = operation;
}

CefBrowserHost::DragOperationsMask BrowserHandler::OnDragEnter(CefRefPtr<CefDragData> drag_data, CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect)
{
	REQUIRE_UI_THREAD();
	if (browser_) {
		POINT pt = { ev.x, ev.y };
		handle_delegate_->ClientToControl(pt);
		ev.x = pt.x;
		ev.y = pt.y;
		browser_->GetHost()->DragTargetDragEnter(drag_data, ev, effect);
		browser_->GetHost()->DragTargetDragOver(ev, effect);
	}
	return current_drag_op_;
}

CefBrowserHost::DragOperationsMask BrowserHandler::OnDragOver(CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect)
{
	REQUIRE_UI_THREAD();
	if (browser_) {
		POINT pt = { ev.x, ev.y };
		handle_delegate_->ClientToControl(pt);
		ev.x = pt.x;
		ev.y = pt.y;
		browser_->GetHost()->DragTargetDragOver(ev, effect);
	}
	return current_drag_op_;
}

void BrowserHandler::OnDragLeave()
{
	REQUIRE_UI_THREAD();
	if (browser_)
		browser_->GetHost()->DragTargetDragLeave();
}

CefBrowserHost::DragOperationsMask BrowserHandler::OnDrop(CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect)
{
	if (browser_) {
		POINT pt = { ev.x, ev.y };
		handle_delegate_->ClientToControl(pt);
		ev.x = pt.x;
		ev.y = pt.y;
		browser_->GetHost()->DragTargetDragOver(ev, effect);
		browser_->GetHost()->DragTargetDrop(ev);
	}
	return current_drag_op_;
}

#pragma endregion

#pragma region CefContextMenuHandler
// CefContextMenuHandler methods
void BrowserHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
CefRefPtr<CefFrame> frame,
CefRefPtr<CefContextMenuParams> params,
CefRefPtr<CefMenuModel> model)
{
	REQUIRE_UI_THREAD();

	if (handle_delegate_)
	{
		handle_delegate_->OnBeforeContextMenu(browser, frame, params, model);
	}
	else
	{
		// Customize the context menu...
		if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0)
		{
			if (model->GetCount() > 0)
			{
				// 禁止右键菜单
				model->Clear();
			}
		}
	}
}

bool BrowserHandler::RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback)
{
	return false;
}

bool BrowserHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
{
	if (handle_delegate_)
		return handle_delegate_->OnContextMenuCommand(browser, frame, params, command_id, event_flags);
	else
		return false;
}

void BrowserHandler::OnContextMenuDismissed(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{

}

#pragma endregion

#pragma region CefDisplayHandler

// CefDisplayHandler methods
void BrowserHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
	// Update the URL in the address bar...
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnAddressChange, handle_delegate_, browser, frame, url));
}

void BrowserHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	// Update the browser window title...
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnTitleChange, handle_delegate_, browser, title));
}

bool BrowserHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line)
{
	// Log a console message...
	return true;
}
#pragma endregion

// CefLoadHandler methods
void BrowserHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
	// Update UI for browser state...
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnLoadingStateChange, handle_delegate_, browser, isLoading, canGoBack, canGoForward));
}

void BrowserHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
	// A frame has started loading content...
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnLoadStart, handle_delegate_, browser, frame));
}

void BrowserHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	// A frame has finished loading content...
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnLoadEnd, handle_delegate_, browser, frame, httpStatusCode));
}

void BrowserHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	// A frame has failed to load content...
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnLoadError, handle_delegate_, browser, frame, errorCode, errorText, failedUrl));
}

bool BrowserHandler::OnJSDialog(CefRefPtr<CefBrowser> browser, const CefString& origin_url, const CefString& accept_lang, JSDialogType dialog_type, const CefString& message_text, const CefString& default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message)
{
	// release时阻止弹出js对话框
#ifndef _DEBUG
	suppress_message = true;
#endif

	return false;
}

// CefRequestHandler methods
bool BrowserHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
	if (handle_delegate_)
		return handle_delegate_->OnBeforeBrowse(browser, frame, request, is_redirect);
	
	return false;
}

void BrowserHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution)
{
	if (handle_delegate_)
		handle_delegate_->OnProtocolExecution(browser, url, allow_os_execution);
}

CefRequestHandler::ReturnValue BrowserHandler::OnBeforeResourceLoad(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefRequestCallback> callback)
{
 	if (handle_delegate_)
		return handle_delegate_->OnBeforeResourceLoad(browser, frame, request, callback);

	return RV_CONTINUE;
}

void BrowserHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status)
{
	if (handle_delegate_)
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&HandlerDelegate::OnRenderProcessTerminated, handle_delegate_, browser, status));

}

bool BrowserHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
	const CefString& origin_url,
	int64 new_size,
	CefRefPtr<CefRequestCallback> callback)
{
	static const int64 max_size = 1024 * 1024 * 20;  // 20mb.

	// Grant the quota request if the size is reasonable.
	callback->Continue(new_size <= max_size);
	return true;
}

void BrowserHandler::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback)
{
	if (handle_delegate_)
		handle_delegate_->OnBeforeDownload(browser, download_item, suggested_name, callback);
}

void BrowserHandler::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback)
{
	if (handle_delegate_)
		handle_delegate_->OnDownloadUpdated(browser, download_item, callback);
}

bool BrowserHandler::OnFileDialog(
	CefRefPtr<CefBrowser> browser, 
	FileDialogMode mode, 
	const CefString& title, 
	const CefString& default_file_path, 
	const std::vector<CefString>& accept_filters, 
	int selected_accept_filter, 
	CefRefPtr<CefFileDialogCallback> callback)
{
	if (handle_delegate_)
		return handle_delegate_->OnFileDialog(browser, mode, title, default_file_path, accept_filters, selected_accept_filter, callback);
	else
		return false;
}

}