#pragma once

namespace ui
{
#define DRAG_HEADER_OFF_SIZE	6
#define DRAG_ROW_OFF_SIZE		4

#define MIN_COLUMN_WIDTH		DRAG_HEADER_OFF_SIZE
#define MIN_ROW_HEIGHT			DRAG_ROW_OFF_SIZE


	enum GridItemType
	{
		GIT_String = 0,
		GIT_Int,
		GIT_Double,
		GIT_Combo,
		GIT_Date,
	};
	
	class GridItem
	{
	public:
		std::wstring text;
		int row_index;
		int col_index;
		std::wstring text_color;
		std::wstring bk_color;
		UINT text_style;
		int flag;
		
		GridItemType type;
		int iValue;
		double dValue;
		std::vector<std::wstring> combo_list;
		std::wstring date;

		GridItem(std::wstring txt = L"", int row = -1, int col = -1, GridItemType item_type = GIT_String) : text(txt), row_index(row), col_index(col)\
			, type(item_type), iValue(0), dValue(0.0), text_style(0), flag(0){
			
		};
		virtual ~GridItem(){};

		//virtual bool IsValid(){ return true; }
		bool IsSelected(){
			return (flag & 0x00000001) > 0;
		};
		void SetSelected(bool selected){
			if (selected)
				flag |= 0x00000001;
			else
				flag &= ~0x00000001;
		};

		void Clear()
		{
			text = L"";
			text_color = L"";
			bk_color = L"";
			text_style = 0;
			flag = 0;
			_ClearType();
		}

		void InitTypeInt(int value){
			_ClearType();
			type = GIT_Int;
			iValue = value;
		}

		void InitTypeDouble(double value){
			_ClearType();
			type = GIT_Double;
			dValue = value;
		}

		void InitTypeCombo(const std::vector<std::wstring>& value){
			_ClearType();
			type = GIT_Combo;
			combo_list = value;
		}

		void InitTypeDate(std::wstring value){
			_ClearType();
			type = GIT_Date;
			date = value;
		}

		bool CopyType(GridItem *from){
			bool ret = false;
			if (from && type != from->type)
			{
				type = from->type;
				if (type == GIT_Combo)
					combo_list = from->combo_list;
				ret = true;
			}
			return ret;
		}

	protected:
		void _ClearType(){
			type = GIT_String;
			iValue = 0;
			dValue = 0.0;
			combo_list.clear();
			date = L"";
		}
	};

	class GridHeaderItem :public GridItem{
	public:
		GridHeaderItem(std::wstring txt = L"", int row = -1, int col = -1, GridItemType item_type = GIT_String) : GridItem(txt, row, col){
		};
		virtual ~GridHeaderItem(){};
		//virtual bool IsValid() override { return type == GIT_String && text.empty() && iValue == 0 && dValue == 0.0 && combo_list.empty() && date.empty(); }
	};

	/*class GridRowHeaderItem :public GridItem{
	public:
		GridRowHeaderItem(std::wstring txt = L"", int row = -1, int col = -1, GridItemType item_type = GIT_String) : GridItem(txt, row, col){
		};
		virtual ~GridRowHeaderItem(){};
	};*/

	class GridSelRange{
		friend class GridBody;
	public:
		GridSelRange(GridBody *pBody) : m_pBody(pBody){};
		~GridSelRange(){};
		void Clear();
		void ClearContent();

		void SetSelAll();
		void SetSelItem(int row_index, int col_index, bool ctrl = false, bool shift = false);
		void SetSelItemRange(UiRect rc, bool ctrl = false, bool shift = false);
		void SetSelRow(int row_index, bool ctrl = false, bool shift = false);
		void SetSelRowRange(int row_index, int row_index_end, bool ctrl = false, bool shift = false);
		void SetSelCol(int col_index, bool ctrl = false, bool shift = false);
		void SetSelColRange(int col_index, int col_index_end, bool ctrl = false, bool shift = false);
		void MergeRange();
		//bool IsItemSelected(int row_index, int col_index);
		bool IsRowSelected(int row_index);
		bool IsColSelected(int col_index);
		bool GetSelRange(UiRect &rc);

		bool MoveSelItem(TCHAR move_key, bool ctrl = false, bool shift = false);

		void CtrlCorX(bool cut);
		void CtrlV();


	protected:
		GridBody *m_pBody;
		std::map<int, void*> m_mapSelRow;
		std::map<int, void*> m_mapSelCol;
		std::vector<UiRect> m_vecRange;			//用UiRect代表一块选中的区域的四个角

		bool m_bSelAll = false;
	};

	//typedef std::vector<GridItem*> GridRow;
	typedef std::vector<int> GridLayout;

	class GridRow
	{
	public:
		std::vector<GridItem*> items;

		GridRow(){};
		~GridRow(){};

		GridItem* operator[](int index){
			assert(index >= 0 && index < items.size());
			return items[index];
		};

		GridItem* at(int index){
			assert(index >= 0 && index < items.size());
			return items[index];
		};

		void push_back(GridItem* item){
			items.push_back(item);
		}

		size_t size(){
			return items.size();
		}
	};
}