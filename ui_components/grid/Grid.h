#pragma once
#include "GridBase.h"
//#include "GridBody.h"

namespace ui
{
	class GridBody;
	class UILIB_API Grid : public ScrollableBox
	{
		friend class GridBody;
	public:
		Grid();
		virtual ~Grid();
		virtual GridBody* CreateGridBody();		//继承类需要重载这个接口 并实现自定义GridBody
		virtual void Init() override;
	public:

		
		
	public: //pass to gridbody

		/**
		* @brief 默认行高
		*/
		int GetDefaultRowHeight();
		void SetDefaultRowHeight(int height);

		/**
		* @brief 默认列宽
		*/
		int GetDefaultColWidth();
		void SetDefaultColWidth(int width);

		/**
		* @brief 总列数
		*/
		int GetColCount() const;
		void SetColCount(int count);

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
		* @brief 列宽
		* @param[in] col_index: 第几列,base on 0
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

		int GetFixedColWidth() const;
		int GetFixedRowHeight() const;

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
		GridHeaderItem* AddCol(std::wstring text, int width = 80);

		/**
		* @brief 插入行在表格最后;
		* @return 返回 true为成功，false 为失败
		*/
		bool AddRow();

		/**
		* @brief 获取表头信息
		* @return 返回表头信息,null表示未找到;
		*/
		GridHeader *GetHeader();

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
		void AttachTextChange(const EventCallback& callback);

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
		* @param[in] max_width: 最大宽度, -1代表没有限制
		* @return 返回 true为成功，false 为失败
		*/
		bool AutoFixColWidth(int col_index, int min_width = 30, int max_width = -1);

		/**
		* @brief 获取选择信息
		* @return 返回 GridSelRange
		*/
		const GridSelRange& GetSelRange() const;
	protected:
		

	protected:
		virtual bool ButtonDown(EventArgs& msg) override;
		virtual void PaintChild(IRenderContext* pRender, const UiRect& rcPaint) override;
	protected:
		GridBody *m_pBody = nullptr;

		/* 是否显示网格 */
		bool m_bPaintGridLine = true;

		/* 是否可编辑表格 */
		bool m_bEditable = true;
	private:
		/* 防止virtual void Init()重复调用 */
		bool m_bIsInit = false;		
	};

	
}