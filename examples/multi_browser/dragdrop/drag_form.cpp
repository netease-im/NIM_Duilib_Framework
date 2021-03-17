#include "drag_form.h"
#include "bitmap_control.h"

using namespace ui;

DragForm *DragForm::s_drag_form = NULL;
HHOOK DragForm::s_mouse_hook = NULL;
POINT DragForm::s_point_offset = { 0, 0 };
const LPCTSTR DragForm::kClassName = L"NimDragForm";

LRESULT CALLBACK DragForm::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_MOUSEMOVE &&::GetKeyState(VK_LBUTTON) < 0)
		{
			MOUSEHOOKSTRUCT *pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
			if (NULL != pMouseStruct)
			{
				if (NULL != s_drag_form)
				{
					//系统拖拽窗口。标题：Drag，类名：SysDragImage，GWL_EXSTYLE:524456
					//WS_EX_TOPMOST--WS_EX_TRANSPARENT--WS_EX_PALETTEWINDOW--WS_EX_LAYERED--WS_EX_TOOLWINDOW
					ui::UiRect rc(pMouseStruct->pt.x - s_point_offset.x, pMouseStruct->pt.y - s_point_offset.y, 0, 0);
					s_drag_form->SetPos(rc, false, SWP_NOSIZE);
				}
			}
		}
	}

	return CallNextHookEx(s_mouse_hook, nCode, wParam, lParam);
}

void DragForm::CreateCustomDragImage(HBITMAP bitmap, POINT pt_offset)
{
	ASSERT(NULL != bitmap);
	s_mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)DragForm::LowLevelMouseProc, GetModuleHandle(NULL), 0);
	s_drag_form = DragForm::CreateDragForm(bitmap, pt_offset);
	s_point_offset = pt_offset;
}

void DragForm::CloseCustomDragImage()
{
	if (NULL != s_drag_form)
	{
		s_drag_form->Close();
		s_drag_form = NULL;
	}

	if (NULL != s_mouse_hook)
	{
		UnhookWindowsHookEx(s_mouse_hook);
		s_mouse_hook = NULL;
	}
	
}

DragForm* DragForm::CreateDragForm(HBITMAP bitmap, POINT pt_offset)
{
	DragForm *drag_form = new DragForm;

	HWND hwnd = drag_form->Create(NULL, kClassName, UI_WNDSTYLE_FRAME, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
	ASSERT(hwnd != NULL);
	drag_form->SetDragImage(bitmap);

	POINT pt;
	BITMAP bitmap_info;
	::GetCursorPos(&pt);
	GetObject(bitmap, sizeof(BITMAP), &bitmap_info);
	
	// 窗口初始化到鼠标光标的中上方位置
	UiRect rect;
	rect.left = pt.x - pt_offset.x;
	rect.top = pt.y - pt_offset.y;
	rect.right = rect.left + bitmap_info.bmWidth;
	rect.bottom = rect.top + bitmap_info.bmHeight;
	drag_form->SetPos(rect, false, SWP_SHOWWINDOW);

	return drag_form;
}

DragForm::DragForm()
{
	bitmap_control_ = NULL;
}

std::wstring DragForm::GetSkinFolder()
{
	return L"multi_browser";
}

std::wstring DragForm::GetSkinFile()
{
	return L"drag_form.xml";
}

ui::Control* DragForm::CreateControl(const std::wstring& pstrClass)
{
	if (pstrClass == L"BitmapControl")
	{
		return new BitmapControl;
	}

	return NULL;
}

std::wstring DragForm::GetWindowClassName() const
{
	return kClassName;
}

UINT DragForm::GetClassStyle() const
{
	return (UI_CLASSSTYLE_FRAME);
}

void DragForm::InitWindow()
{
	bitmap_control_ = static_cast<BitmapControl*>(FindControl(L"bitmap"));
	ASSERT(NULL != bitmap_control_);

	// 设置背景透明度
	bitmap_control_->SetAlpha(128);
	this->SetFocus(bitmap_control_);
}

void DragForm::SetDragImage(HBITMAP bitmap)
{
	ASSERT(NULL != bitmap);
	bitmap_control_->SetBitmapImage(bitmap);
}