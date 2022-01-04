#include "stdafx.h"
#include "main_form.h"
#include "../../ui_components/grid/Grid.h"
#include "control\MyGrid.h"

#include <fstream>

using namespace ui;

const std::wstring MainForm::kClassName = L"MainForm";

MainForm::MainForm()
{
}


MainForm::~MainForm()
{
}

std::wstring MainForm::GetSkinFolder()
{
	return L"grid";
}

std::wstring MainForm::GetSkinFile()
{
	return L"main_form.xml";
}

std::wstring MainForm::GetWindowClassName() const
{
	return kClassName;
}

Control* MainForm::CreateControl(const std::wstring& pstrClass)
{
	Control *pControl = nullptr;
	if (pstrClass == L"Grid")
	{
		pControl = new Grid;
	}
	else if (pstrClass == L"MyGrid")
	{
		pControl = new MyGrid;
	}
	return pControl;
}

void MainForm::InitWindow()
{
	Grid *grid = static_cast<Grid*>(FindControl(L"grid"));
	grid->SetHeaderHeight(48);
	std::wstring header[] = { L"代码", L"名称", L"类型", L"长度", L"顺序号",
		L"编辑模式", L"是否显示", L"参数类型", L"参数值", L"默认值" };

	std::vector <std::wstring> vecTypeString;
	vecTypeString.push_back(L"String");
	vecTypeString.push_back(L"Integer");
	vecTypeString.push_back(L"Float");
	vecTypeString.push_back(L"Date");
	vecTypeString.push_back(L"DateTime");
	vecTypeString.push_back(L"File");
	vecTypeString.push_back(L"List");
	vecTypeString.push_back(L"Text");
	vecTypeString.push_back(L"LongText");
	vecTypeString.push_back(L"Lua");

	std::vector <std::wstring> vecEditModeString;
	vecEditModeString.push_back(L"Normal");
	vecEditModeString.push_back(L"Drop");

	std::vector <std::wstring> vecParamTypeString;
	vecParamTypeString.push_back(L"List");
	vecParamTypeString.push_back(L"TreeList");
	vecParamTypeString.push_back(L"SQLList");
	vecParamTypeString.push_back(L"AutoUnique");
	vecParamTypeString.push_back(L"AutoComplete");
	vecParamTypeString.push_back(L"AutoC***eCode");
	vecParamTypeString.push_back(L"CheckTreeList");
	vecParamTypeString.push_back(L"SQLChe***eeList");

	for (int i = 0; i < sizeof(header) / sizeof(header[0]); i++)
	{
		GridItem *header_item = grid->AddCol(header[i]);
		if (i == 2)
		{
			header_item->InitTypeCombo(vecTypeString);
		}
		else if (i == 5)
		{
			header_item->InitTypeCombo(vecEditModeString);
		}
		else if (i == 7)
		{
			header_item->InitTypeCombo(vecParamTypeString);
		}
	}

	grid->SetRowCount(30);
	/*for (size_t i = 0; i < 100; i++)
	{
		grid->AddRow();
	}*/

	RichEdit *re_row = static_cast<RichEdit*>(FindControl(L"re_row"));
	RichEdit *re_col = static_cast<RichEdit*>(FindControl(L"re_col"));
	RichEdit *re_fixed_row = static_cast<RichEdit*>(FindControl(L"re_fixed_row"));
	RichEdit *re_fixed_col = static_cast<RichEdit*>(FindControl(L"re_fixed_col"));
	re_row->SetText(nbase::IntToString16(grid->GetRowCount()));
	re_col->SetText(nbase::IntToString16(grid->GetColCount()));
	re_fixed_row->SetText(nbase::IntToString16(grid->GetFixedRowCount()));
	re_fixed_col->SetText(nbase::IntToString16(grid->GetFixedColCount()));
	
	re_row->AttachBubbledEvent(kEventReturn, [this, grid, re_row](ui::EventArgs* args){
		std::wstring text = re_row->GetText();
		if (!text.empty())
		{
			int row = 0;
			if (nbase::StringToInt(text, &row))
			{
				grid->SetRowCount(row);
			}
		}
		return true;
	});

	re_col->AttachBubbledEvent(kEventReturn, [this, grid, re_col](ui::EventArgs* args){
		std::wstring text = re_col->GetText();
		if (!text.empty())
		{
			int col = 0;
			if (nbase::StringToInt(text, &col))
			{
				grid->SetColCount(col);
			}
		}
		return true;
	});
	
	re_fixed_row->AttachBubbledEvent(kEventReturn, [this, grid, re_fixed_row](ui::EventArgs* args){
		std::wstring text = re_fixed_row->GetText();
		if (!text.empty())
		{
			int fixed = 0;
			if (nbase::StringToInt(text, &fixed) && fixed != grid->GetFixedRowCount())
			{
				grid->SetFixedRowCount(fixed);
			}
		}
		return true;
	});

	re_fixed_col->AttachBubbledEvent(kEventReturn, [this, grid, re_fixed_col](ui::EventArgs* args){
		std::wstring text = re_fixed_col->GetText();
		if (!text.empty())
		{
			int fixed = 0;
			if (nbase::StringToInt(text, &fixed) && fixed != grid->GetFixedColCount())
			{
				grid->SetFixedColCount(fixed);
			}
		}
		return true;
	});

	Button *btn_add_row = static_cast<Button*>(FindControl(L"btn_add_row"));
	Button *btn_remove_row = static_cast<Button*>(FindControl(L"btn_remove_row"));
	Button *btn_add_col = static_cast<Button*>(FindControl(L"btn_add_col"));
	Button *btn_remove_col = static_cast<Button*>(FindControl(L"btn_remove_col"));
	Button *btn_import_excel = static_cast<Button*>(FindControl(L"btn_import_excel"));
	btn_add_row->AttachClick([this, grid](ui::EventArgs* args){
		grid->AddRow();
		return true;
	});

	btn_remove_row->AttachClick([this, grid](ui::EventArgs* args){
		grid->RemoveRow(2);
		return true;
	});
	btn_add_col->AttachClick([this, grid](ui::EventArgs* args){
		grid->AddCol(L"111");
		return true;
	});
	btn_remove_col->AttachClick([this, grid](ui::EventArgs* args){
		grid->RemoveCol(2);
		return true;
	});
	btn_import_excel->AttachClick([this, grid](ui::EventArgs* args){
		grid->Clear(true);
		grid->LoadExcel(L"grid_test.xlsx", 0, true);

		GridItem* pItem = grid->GetGridItem(1, 1); 
		if (pItem)
			pItem->text_color = L"blue";
		pItem = grid->GetGridItem(2, 2);
		if (pItem)
			pItem->text_style = DT_LEFT | DT_VCENTER | DT_SINGLELINE;

		pItem = grid->GetGridItem(3, 3);
		if (pItem)
			pItem->bk_color = L"gray";

		return true;
	});
}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}


