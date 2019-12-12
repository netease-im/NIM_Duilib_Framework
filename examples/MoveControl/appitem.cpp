#include "stdafx.h"
#include "appitem.h"


AppItemUi* AppItemUi::Create(const AppItem& item)
{
	AppItemUi* uiItem = new AppItemUi;
	uiItem->SetAppdata(item, false);
	ui::GlobalManager::FillBoxWithCache(uiItem, L"movecontrol/app_item.xml");
	return uiItem;
}

void AppItemUi::DoInit()
{
	app_icon_ = static_cast<ui::Control*>(FindSubControl(L"app_icon"));
	if (app_icon_)
	{
		app_icon_->SetBkImage(app_data_._icon);
	}
	app_name_ = static_cast<ui::Label*>(FindSubControl(L"app_name"));
	if (app_name_)
	{
		app_name_->SetText(app_data_._name);
	}

	//绑定事件
	 
}

void AppItemUi::SetAppdata(const AppItem& item,bool refresh)
{
	app_data_ = item;
	if (refresh)
	{
		if (app_icon_)
		{
			app_icon_->SetBkImage(app_data_._icon);
		}
		if (app_name_)
		{
			app_name_->SetText(app_data_._name);
		}
	}
}

void AppItemUi::FixPos(int step, int index)
{
	if (index != -1)
	{
		index_ = index;
	}
	index_ += step;

	ui::UiRect marginRect = { (index_ % EACH_LINE)*APP_WIDTH, (index_ / EACH_LINE)*APP_HEIGHT,0,0 };
	
	SetMargin(marginRect);
}



AppWindow::AppWindow()
{

}

AppWindow::~AppWindow()
{

}

std::wstring AppWindow::GetSkinFolder()
{
	return L"movecontrol";
}

std::wstring AppWindow::GetSkinFile()
{
	return L"app_window.xml";
}

std::wstring AppWindow::GetWindowClassName() const
{
	return L"movecontrol";
}

void AppWindow::InitWindow()
{
	ui::VBox* root = static_cast<ui::VBox*>(FindControl(L"root"));
	if (root)
	{
		auto app_item = AppItemUi::Create(item_);
		root->Add(app_item);
	}

	//设置消息钩子，不然无法即时移动
	InstallHook();

	//移动到合适的位置
	AdjustPos();
	
}

LRESULT AppWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//清理hook
	UnInstallHook();
	pThis_ = nullptr;
	
	return 0;
}

HHOOK AppWindow::mouse_Hook_;

AppWindow* AppWindow::pThis_;

void AppWindow::AdjustPos()
{
	//移动到合适位置，并接管鼠标
	//移植pos的位置,注意去掉阴影
	//
	ui::UiRect rcCorner = GetShadowCorner();
	POINT ptCursor;
	::GetCursorPos(&ptCursor);
	//左上角的位置
	ptCursor.x -= pt_.x;
	ptCursor.y -= pt_.y;

	::SetWindowPos(GetHWND(), NULL, ptCursor.x - rcCorner.left, ptCursor.y - rcCorner.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
}

void AppWindow::InstallHook()
{
	if (mouse_Hook_)   UnInstallHook();
	mouse_Hook_ = SetWindowsHookEx(WH_MOUSE_LL,
		(HOOKPROC)AppWindow::LowLevelMouseProc, GetModuleHandle(NULL), NULL);
}

void AppWindow::UnInstallHook()
{
	if (mouse_Hook_) {
		UnhookWindowsHookEx(mouse_Hook_);
		mouse_Hook_ = NULL;  //set NULL  
	}
}

LRESULT CALLBACK AppWindow::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_MOUSEMOVE &&::GetKeyState(VK_LBUTTON) < 0)
		{
			MOUSEHOOKSTRUCT *pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
			if (NULL != pMouseStruct)
			{
				if (pThis_)
				{
					pThis_->AdjustPos();
				}
			}
		}
		else if (wParam == WM_LBUTTONUP)
		{
			//鼠标弹起，无论什么时候都需要销毁窗口
			if (pThis_)
			{
				//通知主窗口事件
				::PostMessage(GetParent(pThis_->GetHWND()), WM_LBUTTONUP, 0, 0);
				pThis_->Close();
			}
		}
	}

	return CallNextHookEx(mouse_Hook_, nCode, wParam, lParam);
}



