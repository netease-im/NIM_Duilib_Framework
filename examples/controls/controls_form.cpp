#include "stdafx.h"
#include "controls_form.h"
#include "about_form.h"
#include "comboex\CheckCombo.h"
#include "comboex\FilterCombo.h"

#include <fstream>

const std::wstring ControlForm::kClassName = L"Controls";

ControlForm::ControlForm()
{
}


ControlForm::~ControlForm()
{
}

std::wstring ControlForm::GetSkinFolder()
{
	return L"controls";
}

std::wstring ControlForm::GetSkinFile()
{
	return L"controls.xml";
}

std::wstring ControlForm::GetWindowClassName() const
{
	return kClassName;
}

ui::Control* ControlForm::CreateControl(const std::wstring& pstrClass)
{
	ui::Control* control = nullptr;
	if (pstrClass == L"CheckCombo")
	{
		control = new nim_comp::CheckCombo;
	}
	else if (pstrClass == L"FilterCombo")
	{
		control = new nim_comp::FilterCombo;
	}
	return control;
}

void ControlForm::InitWindow()
{
	/**
	 * 为了让代码看起来相对容易理解，不需要频繁跟进才能看明白示例代码
	 * 我们将一些控件储存为局部变量，正确的使用应该是将他们作为成员变量
	 * 不要在每次使用的时候都需要 FindControl，否则会影响性能代码不易读
	 */

	/* Initialize ListBox data */
	ui::ListBox* list = static_cast<ui::ListBox*>(FindControl(L"list"));
	for (auto i = 0; i < 30; i++)
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetText(nbase::StringPrintf(L"ListElement %d", i));
		element->SetClass(L"listitem");
		element->SetFixedHeight(20);
		list->Add(element);
	}

	/* Initialize TreeView data */
	ui::TreeView* tree = static_cast<ui::TreeView*>(FindControl(L"tree"));
	ui::TreeNode* parent_node = nullptr;
	for (auto j = 0; j < 8; j++)
	{
		ui::TreeNode* node = new ui::TreeNode;
		node->SetClass(L"listitem");
		node->SetFixedHeight(20);
		if (parent_node)
		{
			node->SetText(nbase::StringPrintf(L"Child node %d", j));
			node->SetMargin({ 10, 0, 0, 0 });
			parent_node->AddChildNode(node);
		}
		else
		{
			node->SetText(nbase::StringPrintf(L"Parent node", j));
			tree->GetRootNode()->AddChildNode(node);
			parent_node = node;
		}
	}

	/* Initialize ComboBox data */
	ui::Combo* combo = static_cast<ui::Combo*>(FindControl(L"combo"));
	for (auto i = 0; i < 10; i++)
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"Combo element %d", i));
		combo->Add(element);
	}

	std::string checks[6] = { "check1", "check2", "check3", "check4", "check5", "check6" };
	nim_comp::CheckCombo* check_combo = static_cast<nim_comp::CheckCombo*>(FindControl(L"check_combo"));
	for (auto i = 0; i < 6; i++)
	{
		ui::CheckBox *item = new ui::CheckBox;
		item->SetFixedWidth(DUI_LENGTH_STRETCH);
		item->SetFixedHeight(24);
		item->SetUTF8Text(checks[i]);
		item->SetUTF8DataID(checks[i]);

		item->SetTextPadding({ 20, 2, 2, 0 });
		item->SetTextStyle(DT_LEFT | DT_VCENTER);
		std::wstring image_normal = nbase::StringPrintf(L"file='../public/checkbox/check_no.png' dest='%d,4,%d,20'", 2, 18);
		std::wstring image_select = nbase::StringPrintf(L"file='../public/checkbox/check_yes.png' dest='%d,4,%d,20'", 2, 18);

		item->SetStateImage(ui::kControlStateNormal, image_normal);
		item->SetSelectedStateImage(ui::kControlStateNormal, image_select);

		check_combo->Add(item);
	}

	nim_comp::FilterCombo* filter_combo = static_cast<nim_comp::FilterCombo*>(FindControl(L"filter_combo"));
	char buf[16] = {};
	for (auto i = 0; i < 100; i++)
	{
		nim_comp::ListElementMatch *item = new nim_comp::ListElementMatch;
		item->SetFixedHeight(20);
		//ui::GlobalManager::FillBoxWithCache(item, L"date_export/combo/date_item.xml");
		//Label *label = new label;

		std::string str = "item";
		_itoa_s(i, buf, 10);
		str += buf;
		item->SetText(nbase::UTF8ToUTF16(str));
		item->SetUTF8DataID(str);
		filter_combo->Add(item);
	}

	/* Load xml file content in global misc thread, and post update RichEdit task to UI thread */
	nbase::ThreadManager::PostTask(kThreadGlobalMisc, nbase::Bind(&ControlForm::LoadRichEditData, this));

	/* Post repeat task to update progress value 200 milliseconds once */
	/* Using ToWeakCallback to protect closure when if [ControlForm] was destoryed */
	nbase::ThreadManager::PostRepeatedTask(kThreadGlobalMisc, ToWeakCallback([this](){
		nbase::TimeDelta time_delta = nbase::TimeDelta::FromMicroseconds(nbase::Time::Now().ToInternalValue());
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&ControlForm::OnProgressValueChagned, this, time_delta.ToMilliseconds() % 100));
	}), nbase::TimeDelta::FromMilliseconds(200));

	/* Show settings menu */
	ui::Button* settings = static_cast<ui::Button*>(FindControl(L"settings"));
	settings->AttachClick([this](ui::EventArgs* args) {
		RECT rect = args->pSender->GetPos();
		ui::CPoint point;
		point.x = rect.left - 175;
		point.y = rect.top + 10;
		::ClientToScreen(m_hWnd, &point);

		nim_comp::CMenuWnd* sub_menu = new nim_comp::CMenuWnd(NULL);
		ui::STRINGorID xml(L"settings_menu.xml");
		sub_menu->Init(xml, _T("xml"), point);

		/* Sub menu example */
		nim_comp::CMenuElementUI* menu_fourth = static_cast<nim_comp::CMenuElementUI*>(sub_menu->FindControl(L"fourth"));
		nim_comp::CMenuElementUI* menu_item = new nim_comp::CMenuElementUI;
		menu_item->SetText(L"Dynamically created");
		menu_item->SetClass(L"menu_element");
		menu_item->SetFixedWidth(180);
		menu_item->SetFont(L"system_14");
		menu_item->SetTextPadding({ 20, 0, 20, 0 });
		//menu_fourth->Add(menu_item);
		menu_fourth->AddSubMenuItem(menu_item);

		/* About menu */
		nim_comp::CMenuElementUI* menu_about = static_cast<nim_comp::CMenuElementUI*>(sub_menu->FindControl(L"about"));
		menu_about->AttachClick([this](ui::EventArgs* args) {
			AboutForm *about_form = (AboutForm*)(nim_comp::WindowsManager::GetInstance()->GetWindow(AboutForm::kClassName, AboutForm::kClassName));
			if (!about_form)
			{
				about_form = new AboutForm();
				about_form->Create(GetHWND(), AboutForm::kClassName, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
				about_form->CenterWindow();
				about_form->ShowWindow();
			}
			else
			{
				about_form->ActiveWindow();
			}
			return true;
		});

		return true;
	});
}

LRESULT ControlForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void ControlForm::LoadRichEditData()
{
	std::streamoff length = 0;
	std::wstring xml;
	std::wstring controls_xml = ui::GlobalManager::GetResourcePath() + GetWindowResourcePath() + GetSkinFile();

	std::wifstream ifs(controls_xml.c_str());
	if (ifs.is_open())
	{
		ifs.seekg(0, std::ios_base::end);
		length = ifs.tellg();
		ifs.seekg(0, std::ios_base::beg);

		xml.resize(static_cast<unsigned int>(length)+1);
		ifs.read(&xml[0], length);

		ifs.close();
	}

	// Post task to UI thread
	nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&ControlForm::OnResourceFileLoaded, this, xml));
}

void ControlForm::OnResourceFileLoaded(const std::wstring& xml)
{
	if (xml.empty())
		return;

	auto control_edit = static_cast<ui::RichEdit*>(FindControl(L"edit"));
	if (control_edit)
	{
		control_edit->SetText(xml);
		control_edit->SetFocus();
		control_edit->HomeUp();
	}

	// Show about form
	nim_comp::WindowsManager::SingletonShow<AboutForm>(AboutForm::kClassName);
}

void ControlForm::OnProgressValueChagned(float value)
{
	auto progress = static_cast<ui::Progress*>(FindControl(L"progress"));
	if (progress)
		progress->SetValue(value);

	auto circleprogress = static_cast<ui::Progress*>(FindControl(L"circleprogress"));
	if (circleprogress)
	{
		circleprogress->SetValue(value);
		circleprogress->SetText(nbase::StringPrintf(L"%.0f%%", value));
	}
}
