#include "stdafx.h"
#include "cef_control.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "cef_control/handler/browser_handler.h"
#include "cef_control/manager/cef_manager.h"
#include "cef_control/app/cef_js_bridge.h"

namespace nim_comp {

namespace {
	#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
	#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
}

CefControl::CefControl(void)
{

}

CefControl::~CefControl(void)
{
	if (browser_handler_.get())
	{
		browser_handler_->SetHostWindow(NULL);
		browser_handler_->SetHandlerDelegate(NULL);

		if (browser_handler_->GetBrowser().get()) {
			// Request that the main browser close.
			browser_handler_->CloseAllBrowser();
		}
	}
	m_pWindow->RemoveMessageFilter(this);
}

void CefControl::Init()
{
	if (browser_handler_.get() == nullptr)
	{
		m_pWindow->AddMessageFilter(this);
		
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

void CefControl::ReCreateBrowser()
{
	if (browser_handler_->GetBrowser() == nullptr)
	{
		// 使用无窗模式，离屏渲染
		CefWindowInfo window_info;
		window_info.SetAsWindowless(m_pWindow->GetHWND(), false);
		CefBrowserSettings browser_settings;
		//browser_settings.file_access_from_file_urls = STATE_ENABLED;
		//browser_settings.universal_access_from_file_urls = STATE_ENABLED;
		CefBrowserHost::CreateBrowser(window_info, browser_handler_, L"", browser_settings, NULL);
	}	
}

void CefControl::SetPos(UiRect rc)
{
	__super::SetPos(rc);

	if (browser_handler_.get())
	{
		browser_handler_->SetViewRect(rc);
	}
}

void CefControl::HandleMessage(EventArgs& event)
{
	if (browser_handler_.get() && browser_handler_->GetBrowser().get() == NULL)
		return __super::HandleMessage(event);

	else if (event.Type == kEventInternalSetFocus)
	{
		if (browser_handler_->GetBrowserHost().get())
		{
			browser_handler_->GetBrowserHost()->SendFocusEvent(true);
		}
	}
	else if (event.Type == kEventInternalKillFocus)
	{
		if (browser_handler_->GetBrowserHost().get())
		{
			browser_handler_->GetBrowserHost()->SendFocusEvent(false);
		}
	}

	__super::HandleMessage(event);
}

void CefControl::SetVisible(bool bVisible /*= true*/)
{
	__super::SetVisible(bVisible);
	if (browser_handler_.get() && browser_handler_->GetBrowserHost().get())
	{
		browser_handler_->GetBrowserHost()->WasHidden(!bVisible);
	}
}

void CefControl::SetInternVisible(bool bVisible)
{
	__super::SetInternVisible(bVisible);
	if (browser_handler_.get() && browser_handler_->GetBrowserHost().get())
	{
		browser_handler_->GetBrowserHost()->WasHidden(!bVisible);
	}	
}

void CefControl::Paint(IRenderContext* pRender, const UiRect& rcPaint)
{
	__super::Paint(pRender, rcPaint);

	if (dc_cef_.IsValid() && browser_handler_.get() && browser_handler_->GetBrowser().get())
	{
		// 绘制cef PET_VIEW类型的位图
		BitBlt(pRender->GetDC(), m_rcItem.left, m_rcItem.top, m_rcItem.GetWidth(), m_rcItem.GetHeight(), dc_cef_.GetDC(), 0, 0, SRCCOPY);

		// 绘制cef PET_POPUP类型的位图
		if (!rect_popup_.IsEmpty() && dc_cef_popup_.IsValid())
		{
			// 假如popup窗口位置在控件的范围外，则修正到控件范围内，指绘制控件范围内的popup窗口
			int paint_x = rect_popup_.x;
			int paint_y = rect_popup_.y;
			int paint_buffer_x = 0;
			int paint_buffer_y = 0;
			if (rect_popup_.x < 0)
			{
				paint_x = 0;
				paint_buffer_x = -rect_popup_.x;
			}
			if (rect_popup_.y < 0)
			{
				paint_y = 0;
				paint_buffer_y = -rect_popup_.y;
			}

			BitBlt(pRender->GetDC(), m_rcItem.left + paint_x, m_rcItem.top + paint_y, rect_popup_.width, rect_popup_.height, dc_cef_popup_.GetDC(), paint_buffer_x, paint_buffer_y, SRCCOPY);
		}
	}
}

void CefControl::SetWindow(ui::Window* pManager, ui::Box* pParent, bool bInit)
{
	if (!browser_handler_)
	{
		__super::SetWindow(pManager, pParent, bInit);
		return;
	}

	if (m_pWindow)
	{
		m_pWindow->RemoveMessageFilter(this);
		__super::SetWindow(pManager, pParent, bInit);
		pManager->AddMessageFilter(this);
	}

	browser_handler_->SetHostWindow(pManager->GetHWND());
}

LRESULT CefControl::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!IsVisible() || !IsEnabled())
	{
		bHandled = false;
		return 0;
	}

	bHandled = false;
	if (browser_handler_.get() == nullptr || browser_handler_->GetBrowser().get() == nullptr)
		return 0;

	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		return SendMouseMoveEvent(uMsg, wParam, lParam, bHandled);
	}
	break;

	case WM_SETCURSOR:
	{
		// 这里拦截WM_SETCURSOR消息，不让duilib处理（duilib会改变光标样式），否则会影响Cef中的鼠标光标
		POINT pt = { 0 };
		::GetCursorPos(&pt);
		::ScreenToClient(m_pWindow->GetHWND(), &pt);
		if (!m_rcItem.IsPointIn(pt))
			return 0;

		m_pWindow->CallWindowProc(uMsg, wParam, lParam);
		bHandled = true;
		return 0;
	}
	break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		return SendButtonDownEvent(uMsg, wParam, lParam, bHandled);
	}
	break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	{
		return SendButtonUpEvent(uMsg, wParam, lParam, bHandled);
	}
	break;

	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	{
		if (IsFocused())
		{
			return SendKeyEvent(uMsg, wParam, lParam, bHandled);
		}
	}
	break;

	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	{
		return SendButtonDoubleDownEvent(uMsg, wParam, lParam, bHandled);
	}
	break;

	case WM_CAPTURECHANGED:
	case WM_CANCELMODE:
	{
		return SendCaptureLostEvent(uMsg, wParam, lParam, bHandled);
	}
	break;

	case WM_MOUSEWHEEL:
	{
		return SendMouseWheelEvent(uMsg, wParam, lParam, bHandled);
	}
	break;

	case WM_MOUSELEAVE:
	{
		return SendMouseLeaveEvent(uMsg, wParam, lParam, bHandled);
	}
	break;

	default:
		bHandled = false;
	}

	return 0;
}

bool CefControl::AttachDevTools(Control* control)
{
	CefControl *view = dynamic_cast<CefControl*>(control);
	if (devtool_attached_ || !view)
		return true;

	auto browser = browser_handler_->GetBrowser();
	auto view_browser = view->browser_handler_->GetBrowser();
	if (browser == nullptr || view_browser == nullptr)
	{
		auto weak = view->GetWeakFlag();
		auto task = [this, weak, view](){
			nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([this, weak, view](){
				if (weak.expired())
					return;
				AttachDevTools(view);
			}));
		};
		view->browser_handler_->AddAfterCreateTask(task);
	}
	else
	{
		CefWindowInfo windowInfo;
		windowInfo.SetAsWindowless(GetWindow()->GetHWND(), false);
		CefBrowserSettings settings;
		browser->GetHost()->ShowDevTools(windowInfo, view_browser->GetHost()->GetClient(), settings, CefPoint());
		devtool_attached_ = true;
		if (cb_devtool_visible_change_ != nullptr)
			cb_devtool_visible_change_(devtool_attached_);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
LRESULT CefControl::SendButtonDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();

	CPoint pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	pt.Offset(GetScrollOffset());
	if (!m_rcItem.IsPointIn(pt))
		return 0;

	this->SetFocus();
	CefMouseEvent mouse_event;
	mouse_event.x = pt.x - m_rcItem.left;
	mouse_event.y = pt.y - m_rcItem.top;
	mouse_event.modifiers = GetCefMouseModifiers(wParam);

	CefBrowserHost::MouseButtonType btnType =
		(uMsg == WM_LBUTTONDOWN ? MBT_LEFT : (
		uMsg == WM_RBUTTONDOWN ? MBT_RIGHT : MBT_MIDDLE));

	host->SendMouseClickEvent(mouse_event, btnType, false, 1);

	bHandled = false;
	return 0;
}

LRESULT CefControl::SendButtonDoubleDownEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();

	CPoint pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	pt.Offset(GetScrollOffset());
	if (!m_rcItem.IsPointIn(pt))
		return 0;

	CefMouseEvent mouse_event;
	mouse_event.x = pt.x - m_rcItem.left;
	mouse_event.y = pt.y - m_rcItem.top;
	mouse_event.modifiers = GetCefMouseModifiers(wParam);

	CefBrowserHost::MouseButtonType btnType =
		(uMsg == WM_LBUTTONDOWN ? MBT_LEFT : (
		uMsg == WM_RBUTTONDOWN ? MBT_RIGHT : MBT_MIDDLE));

	host->SendMouseClickEvent(mouse_event, btnType, false, 2);

	bHandled = true;
	return 0;
}

LRESULT CefControl::SendButtonUpEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();

	CPoint pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	pt.Offset(GetScrollOffset());
	if (!m_rcItem.IsPointIn(pt) && !m_pWindow->IsCaptured())
		return 0;

	CefMouseEvent mouse_event;
	if (uMsg == WM_RBUTTONUP)
	{
		mouse_event.x = pt.x/* - m_rcItem.left*/;	// 这里不进行坐标转换，否则右键菜单位置不正确
		mouse_event.y = pt.y/* - m_rcItem.top*/;
	}
	else
	{
		mouse_event.x = pt.x - m_rcItem.left;
		mouse_event.y = pt.y - m_rcItem.top;
	}

	mouse_event.modifiers = GetCefMouseModifiers(wParam);

	CefBrowserHost::MouseButtonType btnType =
		(uMsg == WM_LBUTTONUP ? MBT_LEFT : (
		uMsg == WM_RBUTTONUP ? MBT_RIGHT : MBT_MIDDLE));

	host->SendMouseClickEvent(mouse_event, btnType, true, 1);

	bHandled = false;
	return 0;
}

LRESULT CefControl::SendMouseMoveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();

	CPoint pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	pt.Offset(GetScrollOffset());
	if (!m_rcItem.IsPointIn(pt) && !m_pWindow->IsCaptured())
		return 0;

	CefMouseEvent mouse_event;
	mouse_event.x = pt.x - m_rcItem.left;
	mouse_event.y = pt.y - m_rcItem.top;
	mouse_event.modifiers = GetCefMouseModifiers(wParam);
	host->SendMouseMoveEvent(mouse_event, false);

	bHandled = false;
	return 0;
}

LRESULT CefControl::SendMouseWheelEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();

	CPoint pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	HWND scrolled_wnd = ::WindowFromPoint(pt);
	if (scrolled_wnd != m_pWindow->GetHWND())
		return 0;

	ScreenToClient(m_pWindow->GetHWND(), &pt);
	pt.Offset(GetScrollOffset());
	if (!m_rcItem.IsPointIn(pt))
		return 0;

	int delta = GET_WHEEL_DELTA_WPARAM(wParam);

	CefMouseEvent mouse_event;
	mouse_event.x = pt.x - m_rcItem.left;
	mouse_event.y = pt.y - m_rcItem.top;
	mouse_event.modifiers = GetCefMouseModifiers(wParam);
	host->SendMouseWheelEvent(mouse_event, IsKeyDown(VK_SHIFT) ? delta : 0, !IsKeyDown(VK_SHIFT) ? delta : 0);

	bHandled = true;
	return 0;
}

LRESULT CefControl::SendMouseLeaveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();

	CPoint pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	pt.Offset(GetScrollOffset());
	if (!m_rcItem.IsPointIn(pt))
		return 0;

	CefMouseEvent mouse_event;
	mouse_event.x = pt.x - m_rcItem.left;
	mouse_event.y = pt.y - m_rcItem.top;
	mouse_event.modifiers = GetCefMouseModifiers(wParam);

	host->SendMouseMoveEvent(mouse_event, true);

	bHandled = true;
	return 0;
}

LRESULT CefControl::SendKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();
	
	CefKeyEvent event;
	event.windows_key_code = wParam;
	event.native_key_code = lParam;
	event.is_system_key = uMsg == WM_SYSCHAR || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP;

	if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN)
		event.type = KEYEVENT_RAWKEYDOWN;
	else if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP)
		event.type = KEYEVENT_KEYUP;
	else
		event.type = KEYEVENT_CHAR;
	event.modifiers = GetCefKeyboardModifiers(wParam, lParam);

	host->SendKeyEvent(event);

	bHandled = true;
	return 0;
}

LRESULT CefControl::SendCaptureLostEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CefRefPtr<CefBrowserHost> host = browser_handler_->GetBrowserHost();

	host->SendCaptureLostEvent();
	bHandled = true;
	return 0;
}

bool CefControl::IsKeyDown(WPARAM wparam) 
{
	return (GetKeyState(wparam) & 0x8000) != 0;
}

int CefControl::GetCefMouseModifiers(WPARAM wparam)
{
	int modifiers = 0;
	if (wparam & MK_CONTROL)
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (wparam & MK_SHIFT)
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (IsKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;
	if (wparam & MK_LBUTTON)
		modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
	if (wparam & MK_MBUTTON)
		modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
	if (wparam & MK_RBUTTON)
		modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

	// Low bit set from GetKeyState indicates "toggled".
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;
	return modifiers;
}

int CefControl::GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam)
{
	int modifiers = 0;
	if (IsKeyDown(VK_SHIFT))
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (IsKeyDown(VK_CONTROL))
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (IsKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;

	// Low bit set from GetKeyState indicates "toggled".
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;

	switch (wparam) {
	case VK_RETURN:
		if ((lparam >> 16) & KF_EXTENDED)
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		if (!((lparam >> 16) & KF_EXTENDED))
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_NUMLOCK:
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
	case VK_DIVIDE:
	case VK_MULTIPLY:
	case VK_SUBTRACT:
	case VK_ADD:
	case VK_DECIMAL:
	case VK_CLEAR:
		modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_SHIFT:
		if (IsKeyDown(VK_LSHIFT))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RSHIFT))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_CONTROL:
		if (IsKeyDown(VK_LCONTROL))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RCONTROL))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_MENU:
		if (IsKeyDown(VK_LMENU))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RMENU))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_LWIN:
		modifiers |= EVENTFLAG_IS_LEFT;
		break;
	case VK_RWIN:
		modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	}
	return modifiers;
}

//////////////////////////////////////////////////////////////////////////////////

}