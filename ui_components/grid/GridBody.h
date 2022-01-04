#pragma once
#include "GridBase.h"

namespace ui
{
	class Grid;
	class UILIB_API GridBody : public Box
	{
		friend class Grid;
		friend class GridSelRange;
	protected:
		GridBody(Grid *pGrid);
		virtual ~GridBody(){
			if (m_pReEdit)
			{
				delete m_pReEdit;
				m_pReEdit = nullptr;
			}
			if (m_pComboEdit)
			{
				delete m_pComboEdit;
				m_pComboEdit = nullptr;
			}
		};

		int GetDefaultRowHeight(){ return m_defaultRowHeight; };
		void SetDefaultRowHeight(int height){ m_defaultRowHeight = height; };
		
		int GetDefaultColWidth(){ return m_defaultColWidth; };
		void SetDefaultColWidth(int width){ m_defaultColWidth = width; };
		int GetTotalRowHeight(){};

	protected:	//pass by grid
		/**
		* @brief 总列数
		*/
		int GetColCount() const;
		virtual void SetColCount(int count);

		/**
		* @brief 总行数
		*/
		int GetRowCount() const;
		void SetRowCount(int count);

		/**
		* @brief 固定列数
		*/
		int GetFixedColCount() const;
		void SetFixedColCount(int fixed);

		/**
		* @brief 固定行数
		*/
		int GetFixedRowCount() const;
		void SetFixedRowCount(int fixed);

		/**
		* @brief 列宽;
		* @param[in] col_index: 第几列,base on 0;
		*/
		int GetColumnWidth(int col_index) const;
		void SetColumnWidth(int col_index, int width);

		/**
		* @brief 行高;
		* @param[in] row_index: 第几行,base on 0;
		*/
		int GetRowHeight(int row_index) const;
		void SetRowHeight(int row_index, int height);

		/**
		* @brief 表头高度
		*/
		int GetHeaderHeight() const;
		void SetHeaderHeight(int height);

		/**
		* @brief 固定行列的背景色
		*/
		std::wstring GetFixedBkColor() const;
		void SetFixedBkColor(std::wstring color);

		/**
		* @brief 选中单元格的前景色
		*/
		std::wstring GetSelForeColor() const;
		void SetSelForeColor(std::wstring color);

		/**
		* @brief 网格线颜色
		*/
		std::wstring GetGridLineColor() const;
		void SetGridLineColor(std::wstring color);
	
		/**
		*/
		int GetFixedColWidth() const;
		int GetFixedRowHeight() const;

		/**
		* @brief 设置/获取单元格文本内容;
		* @param[in] row_index: 第几行,base on 0;
		* @param[in] col_index: 第几列,base on 0;
		*/
		std::wstring GetGridItemText(int row_index, int col_index);
		bool SetGridItemText(std::wstring text, int row_index, int col_index);

		/**
		* @brief 判断单元格是否fixed;
		* @param[in] row_index: 第几行,base on 0;
		* @param[in] col_index: 第几列,base on 0;
		*/
		bool IsGridItemFixed(int row_index, int col_index);

		/**
		* @brief 插入列在表格最后;
		* @param[in] text: 表头文本;
		* @param[in] width: 列宽度;
		* @return 返回新加的表头单元格对象指针,null表示未成功;
		*/
		virtual GridHeaderItem* AddCol(std::wstring text, int width = 80);

		/**
		* @brief 插入行在表格最后
		* @return 返回 true为成功，false 为失败
		*/
		virtual bool AddRow();

		/*
		* @brief 获取表头行信息;
		* @return 表头行信息;
		*/
		GridHeader* GetHeader() const;

		/**
		* @brief 获取单元格对象
		* @param[in] row_index: 第几行,base on 0;
		* @param[in] col_index: 第几列,base on 0;
		* @return 返回单元格对象指针,null表示未找到;
		*/
		GridItem *GetGridItem(int row_index, int col_index);

		/**
		* @brief 移除一行
		* @param[in] row_index: 第几行,base on 0;
		* @return 返回 true为成功，false 为失败
		*/
		bool RemoveRow(int row_index);

		/**
		* @brief 移除一列
		* @param[in] col_index: 第几列,base on 0;
		* @return 返回 true为成功，false 为失败
		*/
		bool RemoveCol(int col_index);

		/**
		* @brief 清除整表
		* @param[in] include_header: true表示清除header, false表示保留header;
		*/
		void Clear(bool include_header);

		/**
		* @brief 表格内容变更后的回调注册
		* @param[in] callback
		*/
		void AttachTextChange(const EventCallback& callback)	{ this->OnEvent[kEventTextChange] += callback; }

		/**
		* @brief 导入excel
		* @param[in] file:	excel路劲;
		* @param[in] sheet_num:	导入的excel的第几页, base on 0;
		* @param[in] touch_header: true表示excel的第一行做为Grid的Header内容
		* @return 返回 true为成功，false 为失败
		*/
		bool LoadExcel(std::wstring file, int sheet_num, bool touch_header);

		/**
		* @brief 列根据内容自动调整宽度
		* @param[in] col_index:	要调整的列序号, base on 0;
		* @param[in] min_width:	最小宽度;
		* @param[in] max_width: 最大宽度, -1代表没有限制;
		* @return 返回 true为成功，false 为失败;
		*/
		bool AutoFixColWidth(int col_index, int min_width = 30, int max_width = -1);

		/**
		* @brief 获取选择信息
		* @return 返回 GridSelRange
		*/
		const GridSelRange& GetSelRange() const;
	protected:
		virtual void HandleMessage(EventArgs& event) override;
		virtual bool ButtonDown(EventArgs& msg) override;
		virtual bool ButtonUp(EventArgs& msg) override;
		virtual bool OnMouseDoubleClick(EventArgs& msg);		//not override
		virtual bool OnMouseMove(EventArgs& msg);				//not override
		virtual bool OnKeyDown(EventArgs& msg);					//not override

		virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;
		virtual void Paint(IRenderContext* pRender, const UiRect& rcPaint) override;
		virtual void PaintBody(IRenderContext* pRender);		//not override
		virtual void PaintBorder(IRenderContext* pRender) override;
	protected:
		/**
		* @brief 编辑的单元格为Combo类型时, 选中了ComboItem调用;
		* @return 不关键;
		*/
		bool OnComboEditSelected(EventArgs *args);

		/**
		* @brief SetColumnWidth中调用, 更改了列宽;
		* @param[in] col_index:	要调整的列序号, base on 0;
		* @param[in] width:	新的列宽;
		*/
		virtual void OnColumnWidthChanged(int col_index, int width){};

		/**
		* @brief 增/删列;
		* @param[in] col_index:	列序号, base on 0, -1代表多列;
		* @param[in] bRemove:	true:删除列; false:新增列;
		*/
		virtual void OnColumnCountChanged(int col_index, bool bRemove){};

	protected:
		/*
		* @brief 计算vector<int>之和;
		* @return 和;
		*/
		int _SumIntList(const std::vector<int> &vec);

		virtual void _BeginEditGridItem(GridItem *item);
		virtual void _EndEdit();

		/*
		* @brief 获取鼠标位置下的GridItem
		* @param[in] pt: 窗体鼠标位置;
		* @param[out] position:	获得的GridItem坐标, base on 0
		* @param[in] fixed:	是否包含fixed区域;
		* @return true为成功, 坐标从position获取, false为失败;
		*/
		bool _GetGridItemByMouse(CPoint pt, CPoint& position, bool fixed = false);

		/*
		* @brief 获取row_index的GridItem的top位置;
		* @param[in] row_index:	base on 0;
		* @return: row_index的GridItem的top位置, 没有去除滚动位移
		*/
		int _GetGridItemTop(int row_index);

		/*
		* @brief 获取col_index的GridItem的left位置;
		* @param[in] col_index:	base on 0;
		* @return: col_index的GridItem的left位置, 没有去除滚动位移
		*/
		int _GetGridItemLeft(int col_index);

		/*
		* @brief 获取GridItem区域位置;
		* @param[in] row_index: 第几行,base on 0;
		* @param[in] col_index: 第几列,base on 0;
		* @return 返回区域位置, 以Grid控件[left,top]为0点, 没有去除滚动位移;
		*/
		UiRect _GetGridItemPos(int row_index, int col_index);

		/*
		* @brief 清除编辑及选中状态;
		*/
		void _ClearModifyAndSel();

		/* (暂不用)
		* @brief 获取非fixed区域的显示GridItem坐标区域;
		* 赋值到m_rcPaintRange;
		* return true表示获取成功, false表示获取失败,不用继续绘制了
		*/
		bool _GetPaintRangeRect();


	protected:	//给GridSelRange调用的函数
		void _SelItem(int row_index, int col_index, bool selected);
		void _SelRow(int row_index, bool selected);
		void _SelCol(int col_index, bool selected);
		void _SelRange(const UiRect& rc, bool selected);


	protected:
		Grid *m_pGrid = nullptr;

		/* 保存列宽 */
		GridLayout m_hLayout;

		/* 保存行高 */
		GridLayout m_vLayout;

		/* 所有行信息, 第一行为Header */
		std::vector<GridRow*> m_vecRow;

		/* 固定行/列数 */
		unsigned int m_nFixedCol = 0;
		unsigned int m_nFixedRow = 0;

		/* 保存选中信息 */
		GridSelRange m_selRange;

		/* 拖动表头相关 */
		int m_nDragColIndex = -1;					//被拖动的col_index
		CPoint m_ptDragColumnStart;					//鼠标按下时mousepoint, 以Grid为zero点
		CPoint m_ptDragColumnMoving;				//拖动列宽状态下鼠标移动的位置, 以Grid为zero点
		int m_nDrawDragColumnMovingOffX = 0;		// = m_nDragColIndex的右边界 -  m_ptDragColumnMoving.x; 为了保证拖动线的位置准确性

		/* 拖动选中相关 */
		bool m_bDragSel = false;
		bool m_bDrageSelChanged = false;
		CPoint m_ptDragSelStart;					//保存选中拖动起始row_index和col_index
		CPoint m_ptDragSelMoving;					//拖动选中状态下row_index和col_index的位置

		/*	(暂不用)
		*  非fixed区域的显示GridItem坐标区域;
		*  top,bottom代表row_index, left, right代表col_index;
		*  PaintBkColor获取该值, 其他Paint...函数使用该值
		*/
		UiRect m_rcPaintRange;

		/* 一些属性 */
		int m_defaultRowHeight = 24;
		int m_defaultColWidth = 80;
		std::wstring m_strFixedBkColor = L"splitline_level2";
		std::wstring m_strSelForeColor = L"grid_sel_fore";
		std::wstring m_strGridLineColor = L"grid_line";
		std::wstring m_strGridFont = L"system_12";
		UINT	m_uTextStyle =  DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		
#define GRIDBODY_CHILD_COUNT	2
		/* 编辑时候的RichEdit/Combo控件 */
		RichEdit *m_pReEdit = nullptr;
		Combo *m_pComboEdit = nullptr;

		GridItem *m_pReEditGridItem = nullptr;
		GridItem *m_pComboEditGridItem = nullptr;
	};
}