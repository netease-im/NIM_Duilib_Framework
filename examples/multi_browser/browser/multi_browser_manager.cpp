#include "multi_browser_manager.h"
#include "multi_browser_form.h"
#include "browser_box.h"
#include "dragdrop/drag_form.h"
#include "dragdrop/drag_drop.h"

namespace
{
	const int kSplitFormXOffset = 20;	//自动拆分浏览器窗口后新窗口的x偏移坐标
	const int kSplitFormYOffset = 20;	//自动拆分浏览器窗口后新窗口的y偏移坐标
	const int kDragFormXOffset = -100;	//拖拽出新浏览器窗口后的相对鼠标的x偏移坐标
	const int kDragFormYOffset = -20;	//拖拽出新浏览器窗口后的相对鼠标的y偏移坐标
}

BrowserBox* MultiBrowserManager::CreateBorwserBox(MultiBrowserForm *browser_form, const std::string &id, const std::wstring &url)
{
	BrowserBox *browser_box = NULL;
	// 如果启用了窗口合并功能，就把新浏览器盒子都集中创建到某一个浏览器窗口里
	// 否则每个浏览器盒子都创建一个浏览器窗口
	if (enable_merge_)
	{
		if (!browser_form)
		{
			browser_form = new MultiBrowserForm;
			HWND hwnd = browser_form->Create(NULL, L"MultiBrowser", UI_WNDSTYLE_FRAME, 0, false);
			if (hwnd == NULL)
			{
				browser_form = NULL;
				return NULL;
			}

			browser_form->CenterWindow();
		}

		browser_box = browser_form->CreateBox(id, url);
		if (NULL == browser_box)
			return NULL;
	}
	else
	{
		browser_form = new MultiBrowserForm;
		HWND hwnd = browser_form->Create(NULL, L"MultiBrowser", UI_WNDSTYLE_FRAME, 0, false);
		if (hwnd == NULL)
			return NULL;

		browser_box = browser_form->CreateBox(id, url);
		if (NULL == browser_box)
			return NULL;

		browser_form->CenterWindow();
	}

	box_map_[id] = browser_box;
	return browser_box;
}

MultiBrowserManager::MultiBrowserManager()
{
	enable_merge_ = true;
	use_custom_drag_image_ = true;
}

MultiBrowserManager::~MultiBrowserManager()
{

}

bool MultiBrowserManager::IsBorwserBoxActive(const std::string& id)
{
	BrowserBox *browser_box = FindBorwserBox(id);
	if (NULL != browser_box)
	{
		MultiBrowserForm *parent_form = browser_box->GetBrowserForm();
		return parent_form->IsActiveBox(browser_box);
	}

	return false;
}

BrowserBox* MultiBrowserManager::FindBorwserBox( const std::string &id )
{
	std::map<std::string, BrowserBox*>::const_iterator i = box_map_.find(id);
	if (i == box_map_.end())
		return NULL;
	else
		return i->second;
}

void MultiBrowserManager::RemoveBorwserBox( std::string id, const BrowserBox* box /*=NULL*/)
{
	auto it_box = box_map_.find(id);
	if (it_box == box_map_.end())
	{
		assert(0);
	}
	else
	{
		if (NULL == box || box == it_box->second)
		{
			box_map_.erase(it_box);
		}
		else
		{
			assert(0);
		}
	}

	if (box_map_.empty())
		nim_comp::CefManager::GetInstance()->PostQuitMessage(0);
}

void MultiBrowserManager::SetEnableMerge(bool enable)
{
	if (enable_merge_ == enable)
		return;

	enable_merge_ = enable;

	if (enable_merge_)
	{
		// 如果当前只有一个浏览器窗口或者浏览器盒子，就不需要进行合并操作
		if (box_map_.size() <= 1)
			return;

		// 选择第一个浏览器盒子所属的窗口作为合并窗口
		MultiBrowserForm *merge_form = box_map_.begin()->second->GetBrowserForm();

		// 遍历所有浏览器盒子，脱离原浏览器窗口，再附加到合并窗口里
		for (auto it_box : box_map_)
		{
			ASSERT(NULL != it_box.second);
			MultiBrowserForm *parent_form = it_box.second->GetBrowserForm();
			if (merge_form != parent_form)
			{
				if (parent_form->DetachBox(it_box.second))
				{
					merge_form->AttachBox(it_box.second);
				}
			}
		}
	}
	else
	{
		// 如果当前只有一个浏览器盒子，就不需要进行拆分操作
		if (box_map_.size() <= 1)
			return;

		// 给新拆分的窗口设置坐标
		bool first_sort = true;
		ui::UiRect rect_old_form;
		MultiBrowserForm *sort_form = NULL;

		// 遍历所有浏览器盒子，脱离原浏览器窗口，创建新的浏览器窗口并附加浏览器盒子
		for (auto it_box : box_map_)
		{
			ASSERT(NULL != it_box.second);
			MultiBrowserForm *parent_form = it_box.second->GetBrowserForm();
			if (1 == parent_form->GetBoxCount())
			{
				sort_form = parent_form;
			}
			else if (parent_form->DetachBox(it_box.second))
			{
				MultiBrowserForm *browser_form = new MultiBrowserForm;
				HWND hwnd = browser_form->Create(NULL, L"MultiBrowser", UI_WNDSTYLE_FRAME, 0, false);
				if (hwnd == NULL)
				{
					ASSERT(0);
					continue;
				}

				if (!browser_form->AttachBox(it_box.second))
				{
					ASSERT(0);
					continue;
				}

				sort_form = browser_form;
			}

			if (NULL != sort_form)
			{
				if (first_sort)
				{
					first_sort = false;
					sort_form->CenterWindow();
					rect_old_form = sort_form->GetPos(true);
				}
				else
				{
					rect_old_form.left += kSplitFormXOffset;
					rect_old_form.top += kSplitFormXOffset;
					sort_form->SetPos(rect_old_form, true, SWP_NOSIZE, NULL, true);
				}
			}
		}
	}
}

bool MultiBrowserManager::IsEnableMerge() const
{
	return enable_merge_;
}

void MultiBrowserManager::SetUseCustomDragImage(bool use)
{
	use_custom_drag_image_ = use;
}

bool MultiBrowserManager::IsUseCustomDragImage() const
{
	return use_custom_drag_image_;
}

bool MultiBrowserManager::IsDragingBorwserBox() const
{
	return enable_merge_ && NULL != draging_box_;
}

void MultiBrowserManager::SetDropForm(MultiBrowserForm *browser_form)
{
	if (NULL == browser_form)
		return;

	drop_browser_form_ = browser_form;
}

bool MultiBrowserManager::DoDragBorwserBox(BrowserBox *browser_box, HBITMAP bitmap, POINT pt_offset)
{
	if (!enable_merge_)
		return false;

	SdkDropSource* drop_src = new SdkDropSource;
	if (drop_src == NULL)
		return false;

	SdkDataObject* data_object = CreateDragDataObject(bitmap, pt_offset);
	if (data_object == NULL)
		return false;

	// 无论什么时候都让拖拽时光标显示为箭头
	drop_src->SetFeedbackCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));

	OnBeforeDragBorwserBox(browser_box, bitmap, pt_offset);

	// 此函数会阻塞直到拖拽完成
	DWORD dwEffect;
	HRESULT hr = ::DoDragDrop(data_object, drop_src, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);

	OnAfterDragBorwserBox();

	// 销毁位图
	DeleteObject(bitmap);
	drop_src->Release();
	data_object->Release();
	return true;
}

SdkDataObject* MultiBrowserManager::CreateDragDataObject(HBITMAP bitmap, POINT pt_offset)
{
	SdkDataObject* data_object = new SdkDataObject;
	if (data_object == NULL)
		return NULL;

	if (use_custom_drag_image_)
	{
		FORMATETC fmtetc = { 0 };
		fmtetc.dwAspect = DVASPECT_CONTENT;
		fmtetc.lindex = -1;
		fmtetc.cfFormat = CF_HDROP;
		fmtetc.tymed = TYMED_NULL;

		STGMEDIUM medium = { 0 };
		medium.tymed = TYMED_NULL;
		data_object->SetData(&fmtetc, &medium, FALSE);
	}
	else
	{
		FORMATETC fmtetc = { 0 };
		fmtetc.dwAspect = DVASPECT_CONTENT;
		fmtetc.lindex = -1;
		fmtetc.cfFormat = CF_BITMAP;
		fmtetc.tymed = TYMED_GDI;

		STGMEDIUM medium = { 0 };
		medium.tymed = TYMED_GDI;
		HBITMAP hBitmap = (HBITMAP)OleDuplicateData(bitmap, fmtetc.cfFormat, NULL);
		medium.hBitmap = hBitmap;
		data_object->SetData(&fmtetc, &medium, FALSE);

		BITMAP bitmap_info;
		GetObject(hBitmap, sizeof(BITMAP), &bitmap_info);
		SIZE bitmap_size = { bitmap_info.bmWidth, bitmap_info.bmHeight };
		SdkDragSourceHelper dragSrcHelper;
		dragSrcHelper.InitializeFromBitmap(hBitmap, pt_offset, bitmap_size, data_object, RGB(255, 0, 255));
	}

	return data_object;
}

void MultiBrowserManager::OnBeforeDragBorwserBox(BrowserBox *browser_box, HBITMAP bitmap, POINT pt_offset)
{
	// 获取当前被拖拽的浏览器盒子所属的浏览器窗口
	draging_box_ = browser_box;
	MultiBrowserForm *drag_browser_form = draging_box_->GetBrowserForm();
	ASSERT(NULL != drag_browser_form);

	// 获取被拖拽浏览器窗口中浏览器盒子的数量
	int box_count = drag_browser_form->GetBoxCount();
	ASSERT(box_count > 0);
	drop_browser_form_ = NULL;

	drag_browser_form->OnBeforeDragBoxCallback(nbase::UTF8ToUTF16(draging_box_->GetId()));

	if (use_custom_drag_image_)
		DragForm::CreateCustomDragImage(bitmap, pt_offset);
}

void MultiBrowserManager::OnAfterDragBorwserBox()
{
	if (use_custom_drag_image_)
		DragForm::CloseCustomDragImage();

	if (NULL == draging_box_)
		return;

	// 获取当前被拖拽的浏览器盒子所属的浏览器窗口
	MultiBrowserForm *drag_browser_form = draging_box_->GetBrowserForm();
	ASSERT(NULL != drag_browser_form);

	// 获取被拖拽浏览器窗口中浏览器盒子的数量
	int box_count = drag_browser_form->GetBoxCount();
	ASSERT(box_count > 0);

	// 如果被拖拽的浏览器盒子放入到一个浏览器窗口里
	if (NULL != drop_browser_form_)
	{
		if (drag_browser_form == drop_browser_form_)
		{
			drag_browser_form->OnAfterDragBoxCallback(false);
		}
		else
		{
			drag_browser_form->OnAfterDragBoxCallback(true);
			if (drag_browser_form->DetachBox(draging_box_))
			{
				drop_browser_form_->AttachBox(draging_box_);
			}
		}

		// 如果被拖拽的浏览器窗口包含多个浏览器盒子，就投递一个WM_LBUTTONUP消息给窗口
		// (因为窗口被拖拽时触发了ButtonDown和ButtonMove消息，但是最终的ButtonUp消息会被忽略，这里补上)
		// 如果只有一个浏览器盒子，则浏览器盒子脱离浏览器窗口时，浏览器窗口就会关闭，不需要投递
		if (box_count > 1)
			drag_browser_form->PostMessage(WM_LBUTTONUP, 0, 0);
	}
	// 如果没有被拖拽到另一个浏览器窗口里
	else
	{
		// 如果被拖拽的浏览器窗口里只有一个浏览器盒子,则拖拽失败
		if (1 == box_count)
		{
			drag_browser_form->OnAfterDragBoxCallback(false);
		}
		// 如果有多个浏览器盒子, 就把浏览器盒子脱离原浏览器窗口，附加到新的浏览器窗口，拖拽成功
		else
		{
			drag_browser_form->OnAfterDragBoxCallback(true);

			if (drag_browser_form->DetachBox(draging_box_))
			{
				MultiBrowserForm *browser_form = new MultiBrowserForm;
				HWND hwnd = browser_form->Create(NULL, L"MultiBrowser", UI_WNDSTYLE_FRAME, 0, false);
				if (hwnd != NULL)
				{
					if (browser_form->AttachBox(draging_box_))
					{
						// 这里设置新浏览器窗口的位置，设置到偏移鼠标坐标100,20的位置
						POINT pt_mouse;
						::GetCursorPos(&pt_mouse);
						ui::UiRect rect(pt_mouse.x + kDragFormXOffset, pt_mouse.y + kDragFormYOffset, 0, 0);
						browser_form->SetPos(rect, false, SWP_NOSIZE);
					}
				}
			}
		}

		// 如果没有被拖拽到另一个浏览器窗口里，这时不会有浏览器窗口被关闭，所以直接投递ButtonUp消息
		drag_browser_form->PostMessage(WM_LBUTTONUP, 0, 0);
	}

	draging_box_ = NULL;
	drop_browser_form_ = NULL;
}