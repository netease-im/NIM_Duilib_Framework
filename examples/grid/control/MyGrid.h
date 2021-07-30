#pragma once
#include "ui_components\grid\GridBase.h"
#include "ui_components\grid\Grid.h"
#include "ui_components\grid\GridBody.h"

namespace ui{
	class Combo;
}
 
class MyGridBody : public ui::GridBody
{
public:
	MyGridBody(ui::Grid *pGrid);
	virtual ~MyGridBody();

	virtual void SetColCount(int count) override;
	virtual ui::GridHeaderItem* AddCol(std::wstring text, int width) override;
	virtual void PaintBody(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;

	/**
	* @brief SetColumnWidth中调用, 更改了列宽;
	* @param[in] col_index:	要调整的列序号, base on 0;
	* @param[in] width:	新的列宽;
	*/
	virtual void OnColumnWidthChanged(int col_index, int width) override;

	/**
	* @brief 增/删列;
	* @param[in] col_index:	列序号, base on 0, -1代表多列;
	* @param[in] bRemove:	true:删除列; false:新增列;
	*/
	virtual void OnColumnCountChanged(int col_index, bool bRemove) override;

	bool OnScrollPosChanged(ui::EventArgs* args);

private:
	void ResetHeanderComboPos();
private:
	std::vector<ui::Combo*> vec_combo_;
};



class  MyGrid : public ui::Grid
{
public:
	MyGrid();
	virtual ~MyGrid();

	virtual ui::GridBody* CreateGridBody() override{ return new MyGridBody(this); };
};
