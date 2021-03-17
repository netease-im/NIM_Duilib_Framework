#include "browser_box.h"
#include "browser/multi_browser_form.h"
#include "browser/multi_browser_manager.h"
#include "taskbar/taskbar_manager.h"

using namespace ui;
using namespace std;

BrowserBox::BrowserBox(std::string id)
{
	taskbar_item_ = nullptr;
	browser_form_ = nullptr;
	cef_control_ = nullptr;
	browser_id_ = id;
}

MultiBrowserForm* BrowserBox::GetBrowserForm() const
{
	ASSERT(NULL != browser_form_);
	ASSERT(::IsWindow(browser_form_->GetHWND()));
	return browser_form_;
}

nim_comp::CefControlBase* BrowserBox::GetCefControl()
{
	return cef_control_;
}

std::wstring& BrowserBox::GetTitle()
{
	return title_;
}

void BrowserBox::InitBrowserBox(const std::wstring &url)
{
	cef_control_ = static_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control"));
	cef_control_->AttachBeforeContextMenu(nbase::Bind(&BrowserBox::OnBeforeMenu, this, std::placeholders::_1, std::placeholders::_2));
	cef_control_->AttachMenuCommand(nbase::Bind(&BrowserBox::OnMenuCommand, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	cef_control_->AttachTitleChange(nbase::Bind(&BrowserBox::OnTitleChange, this, std::placeholders::_1));
	cef_control_->AttachUrlChange(nbase::Bind(&BrowserBox::OnUrlChange, this, std::placeholders::_1));
	cef_control_->AttachLinkClick(nbase::Bind(&BrowserBox::OnLinkClick, this, std::placeholders::_1));
	cef_control_->AttachBeforeNavigate(nbase::Bind(&BrowserBox::OnBeforeNavigate, this, std::placeholders::_1, std::placeholders::_2));
	cef_control_->AttachLoadingStateChange(nbase::Bind(&BrowserBox::OnLoadingStateChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	cef_control_->AttachLoadStart(nbase::Bind(&BrowserBox::OnLoadStart, this));
	cef_control_->AttachLoadEnd(nbase::Bind(&BrowserBox::OnLoadEnd, this, std::placeholders::_1));
	cef_control_->AttachLoadError(nbase::Bind(&BrowserBox::OnLoadError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	// 加载默认网页
	std::wstring html_path = url;
	if (html_path.empty())
		html_path = nbase::win32::GetCurrentModuleDirectory() + L"resources\\themes\\default\\cef\\cef.html";

	cef_control_->LoadURL(html_path);

	// 初始化任务栏缩略图
	if ((GetWindowExStyle(m_pWindow->GetHWND()) & WS_EX_LAYERED) != 0)
	{
		taskbar_item_ = new TaskbarTabItem(this);
		if (taskbar_item_)
			taskbar_item_->Init(url, browser_id_);
	}

	// Box获取焦点时把焦点转移给Cef控件
	this->AttachSetFocus([this](ui::EventArgs* param)->bool
	{
		cef_control_->SetFocus();
		return true;
	});
}

void BrowserBox::UninitBrowserBox()
{
	MultiBrowserManager::GetInstance()->RemoveBorwserBox(browser_id_, this);

	if (taskbar_item_)
		taskbar_item_->UnInit();
}

ui::Control* BrowserBox::CreateControl(const std::wstring& pstrClass)
{
	if (pstrClass == L"CefControl")
	{
		if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			return new nim_comp::CefControl;
		else
			return new nim_comp::CefNativeControl;
	}

	return NULL;
}

TaskbarTabItem* BrowserBox::GetTaskbarItem()
{
	return taskbar_item_;
}

void BrowserBox::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	browser_form_ = dynamic_cast<MultiBrowserForm*>(pManager);
	ASSERT(NULL != browser_form_);

	__super::SetWindow(pManager, pParent, bInit);
}

void BrowserBox::SetInternVisible(bool bVisible /*= true*/)
{
	Control::SetInternVisible(bVisible);
	if (m_items.empty()) return;
	for (auto it = m_items.begin(); it != m_items.end(); it++) {
		(*it)->SetInternVisible(bVisible);
	}
}

void BrowserBox::Invalidate()
{
	__super::Invalidate();

	if (taskbar_item_)
		taskbar_item_->InvalidateTab();
}

void BrowserBox::SetPos(UiRect rc)
{
	__super::SetPos(rc);

	if (taskbar_item_)
		taskbar_item_->InvalidateTab();
}

void BrowserBox::OnBeforeMenu(CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{

}

bool BrowserBox::OnMenuCommand(CefRefPtr<CefContextMenuParams> params, int command_id, CefContextMenuHandler::EventFlags event_flags)
{
	return false;
}

void BrowserBox::OnTitleChange(const std::wstring& title)
{
	title_ = title;
	browser_form_->SetTabItemName(nbase::UTF8ToUTF16(browser_id_), title);
}

void BrowserBox::OnUrlChange(const std::wstring& url)
{
	url_ = url;
	browser_form_->SetURL(browser_id_, url);
}

bool BrowserBox::OnLinkClick(const std::wstring& url)
{
	return true;
}

CefRequestHandler::ReturnValue BrowserBox::OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect)
{
	// 返回RV_CANCEL截断导航
	return RV_CONTINUE;
}

void BrowserBox::OnLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward)
{
	return;
}

void BrowserBox::OnLoadStart()
{
	return;
}

void BrowserBox::OnLoadEnd(int httpStatusCode)
{
	// 注册一个方法提供前端调用
	cef_control_->RegisterCppFunc(L"ShowMessageBox", ToWeakCallback([](const std::string& params, nim_comp::ReportResultFunction callback) {
		MessageBoxA(NULL, params.c_str(), "接收到 JavaScript 发来的消息", MB_OK);
		callback(false, R"({ "message": "Success." })");
	}));
}

void BrowserBox::OnLoadError(CefLoadHandler::ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	return;
}