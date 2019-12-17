#include "stdafx.h"
#include "move_control.h"

using namespace ui;
using namespace std;


MoveControlForm::MoveControlForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
	: class_name_(class_name)
	, theme_directory_(theme_directory)
	, layout_xml_(layout_xml)
{
}


MoveControlForm::~MoveControlForm()
{
}

std::wstring MoveControlForm::GetSkinFolder()
{
	return theme_directory_;
}

std::wstring MoveControlForm::GetSkinFile()
{
	return layout_xml_;
}

std::wstring MoveControlForm::GetWindowClassName() const
{
	return class_name_;
}

void MoveControlForm::InitWindow()
{
	//添加应用。应用有可能是服务器下发的，一般本地也有保存的
	//loadFromDb
	//getFromServer---->后台可以先保存到db，再post个消息出来，界面重新从db load。

	//作为demo，先写死
	std::vector<AppItem> applist;
	CAppDb::GetInstance().LoadFromDb(applist);

	frequent_app_ = static_cast<ui::Box*>(FindControl(L"frequent_app"));
	my_app_ = static_cast<ui::Box*>(FindControl(L"my_app"));
	
	for (const auto& item: applist)
	{
		AppItemUi* pAppUi = AppItemUi::Create(item);
		pAppUi->AttachAllEvents(nbase::Bind(&MoveControlForm::OnProcessAppItemDrag, this, std::placeholders::_1));
		if (item._isFrequent)
		{
			pAppUi->FixPos(0, frequent_app_->GetCount());
			frequent_app_->Add(pAppUi);
		}
		else
		{
			pAppUi->FixPos(0, my_app_->GetCount());
			my_app_->Add(pAppUi);
		}
	}
}

LRESULT MoveControlForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}


LRESULT MoveControlForm::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (current_item_ == nullptr)
	{
		return __super::OnLButtonUp(uMsg,wParam,lParam,bHandled);
	}

	Box* pParent = current_item_->GetParent();
	pParent->SetAutoDestroy(true);

	if (!DoAfterDrag(frequent_app_) && !DoAfterDrag(my_app_))
	{
		//回滚
		pParent->AddAt(current_item_, current_item_->getIndex());
		//从index处开始补缺口
		for (int index = current_item_->getIndex()+1; index < pParent->GetCount(); ++index)
		{
			AppItemUi* _pItem = dynamic_cast<AppItemUi*>(pParent->GetItemAt(index));
			if (_pItem)
			{
				_pItem->FixPos(+1);
			}
		}
	}

	//更新App信息到数据库
	CAppDb::GetInstance().SaveToDb(current_item_->getAppData());

	is_drag_state_ = false;
	current_item_ = nullptr;
	SetForegroundWindow(m_hWnd);
	SetActiveWindow(m_hWnd);
	return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);
}

void MoveControlForm::ShowCustomWindow(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
{
	MoveControlForm* window = new MoveControlForm(class_name, theme_directory, layout_xml);
	window->Create(NULL, class_name.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
}

//得想办法抓起鼠标弹起的一刻
bool MoveControlForm::OnProcessAppItemDrag(ui::EventArgs* param)
{
	switch (param->Type)
	{
	case kEventMouseMove:
	{
		if (::GetKeyState(VK_LBUTTON) >= 0)
			break;
		if (!is_drag_state_)
		{
			break;
		}
		//检测位移
		LONG cx = abs(param->ptMouse.x - old_drag_point_.x);
		LONG cy = abs(param->ptMouse.y - old_drag_point_.y);
		if (cx < 2 && cy < 2)
		{
			break;
		}
		//在拖拽模式下
		//获取鼠标相对AppItem的位置
		ui::UiRect rect = param->pSender->GetPos(); //左上角有效
		POINT pt = { param->ptMouse.x - rect.left, param->ptMouse.y - rect.top };

		DoDrag(param->pSender, pt);
		is_drag_state_ = false;
	}
		break;
	case kEventMouseButtonDown:
	{
		is_drag_state_ = true;
		old_drag_point_ = param->ptMouse;
	}
		break;
	case kEventMouseButtonUp:
	{
		is_drag_state_ = false;
		//DoDrop

	}
		break;
	}
	return true;
}

void MoveControlForm::DoDrag(ui::Control* pAppItem, POINT pos)
{
	current_item_ = dynamic_cast<AppItemUi*>(pAppItem);
	if (nullptr==current_item_)
	{
		return;
	}
	DoBeforeDrag();
	DoDraging(pos);

}

void MoveControlForm::DoBeforeDrag()
{
	//抠出该项目，后面的项目全部左移
	ASSERT(current_item_);
	if (current_item_)
	{
		Box* pParent = current_item_->GetParent();
		ASSERT(pParent);
		pParent->SetAutoDestroy(false);  //子控件不销毁
		pParent->Remove(current_item_);

		//从index处开始补缺口
		for (int index = current_item_->getIndex(); index < pParent->GetCount(); ++index)
		{
			AppItemUi* _pItem = dynamic_cast<AppItemUi*>(pParent->GetItemAt(index));
			if (_pItem)
			{
				_pItem->FixPos(-1);
			}
		}
	}
}

void MoveControlForm::DoDraging(POINT pos)
{
	//这里注意，如果只是父控件内部移动的话，会简单很多
	//设置下current_item_的setmargin，重新add回去，先保留在父控件的最后一个
	//index_保存之前的位置（防取消），当鼠标弹起时，再设置下合理的值，包括在父控件的位置

	//跨进程移动的话，需要借用drag-drop，也是可以实现的，这里从略

	//本Demo实现的是跨父控件移动（兼容父控件内部移动），并且可以移动出窗口范围，因此创建临时窗口
	//非常遗憾，当临时窗口创建时，临时窗口并没有即时的拖拽感，这里采取Hook方法，在mousemove消息移动。


	//这里创建新窗口 当然得确保不能重复有窗口，这里省略
	AppWindow* pWindow = AppWindow::CreateAppWindow(GetHWND(), pos, current_item_->getAppData());
	ASSERT(pWindow);
}

bool MoveControlForm::DoAfterDrag(ui::Box* check)
{
	//获取鼠标的位置
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	int findIndex = 0;
	UiRect rectBox = check->GetPos();
	if (rectBox.IsPointIn(pt))
	{
		//最好是重合面积更大的，这里根据鼠标位置来了
		for (findIndex = 0; findIndex < check->GetCount(); findIndex++)
		{
			auto control = check->GetItemAt(findIndex);
			UiRect rectCtrl = control->GetPos();
			if (rectCtrl.IsPointIn(pt))
			{
				//插入到该index
				break;
			}
		}
		//合理安排区域
		if (findIndex < check->GetCount())
		{
			current_item_->FixPos(0, findIndex);
			check->AddAt(current_item_, findIndex);
			//从index处开始补缺口
			for (int index = findIndex + 1; index < check->GetCount(); ++index)
			{
				AppItemUi* _pItem = dynamic_cast<AppItemUi*>(check->GetItemAt(index));
				if (_pItem)
				{
					_pItem->FixPos(+1);
				}
			}
			return true;
		}
		else
		{
			//放到最后面
			current_item_->FixPos(0, findIndex);
			check->Add(current_item_);
			return true;
		}
	}
	else
	{
		return false;
	}
	
}


