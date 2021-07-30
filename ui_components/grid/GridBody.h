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
		* @brief ������
		*/
		int GetColCount() const;
		virtual void SetColCount(int count);

		/**
		* @brief ������
		*/
		int GetRowCount() const;
		void SetRowCount(int count);

		/**
		* @brief �̶�����
		*/
		int GetFixedColCount() const;
		void SetFixedColCount(int fixed);

		/**
		* @brief �̶�����
		*/
		int GetFixedRowCount() const;
		void SetFixedRowCount(int fixed);

		/**
		* @brief �п�;
		* @param[in] col_index: �ڼ���,base on 0;
		*/
		int GetColumnWidth(int col_index) const;
		void SetColumnWidth(int col_index, int width);

		/**
		* @brief �и�;
		* @param[in] row_index: �ڼ���,base on 0;
		*/
		int GetRowHeight(int row_index) const;
		void SetRowHeight(int row_index, int height);

		/**
		* @brief ��ͷ�߶�
		*/
		int GetHeaderHeight() const;
		void SetHeaderHeight(int height);

		/**
		* @brief �̶����еı���ɫ
		*/
		std::wstring GetFixedBkColor() const;
		void SetFixedBkColor(std::wstring color);

		/**
		* @brief ѡ�е�Ԫ���ǰ��ɫ
		*/
		std::wstring GetSelForeColor() const;
		void SetSelForeColor(std::wstring color);

		/**
		* @brief ��������ɫ
		*/
		std::wstring GetGridLineColor() const;
		void SetGridLineColor(std::wstring color);
	
		/**
		*/
		int GetFixedColWidth() const;
		int GetFixedRowHeight() const;

		/**
		* @brief ����/��ȡ��Ԫ���ı�����;
		* @param[in] row_index: �ڼ���,base on 0;
		* @param[in] col_index: �ڼ���,base on 0;
		*/
		std::wstring GetGridItemText(int row_index, int col_index);
		bool SetGridItemText(std::wstring text, int row_index, int col_index);

		/**
		* @brief �жϵ�Ԫ���Ƿ�fixed;
		* @param[in] row_index: �ڼ���,base on 0;
		* @param[in] col_index: �ڼ���,base on 0;
		*/
		bool IsGridItemFixed(int row_index, int col_index);

		/**
		* @brief �������ڱ�����;
		* @param[in] text: ��ͷ�ı�;
		* @param[in] width: �п��;
		* @return �����¼ӵı�ͷ��Ԫ�����ָ��,null��ʾδ�ɹ�;
		*/
		virtual GridHeaderItem* AddCol(std::wstring text, int width = 80);

		/**
		* @brief �������ڱ�����
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		virtual bool AddRow();

		/*
		* @brief ��ȡ��ͷ����Ϣ;
		* @return ��ͷ����Ϣ;
		*/
		GridHeader* GetHeader() const;

		/**
		* @brief ��ȡ��Ԫ�����
		* @param[in] row_index: �ڼ���,base on 0;
		* @param[in] col_index: �ڼ���,base on 0;
		* @return ���ص�Ԫ�����ָ��,null��ʾδ�ҵ�;
		*/
		GridItem *GetGridItem(int row_index, int col_index);

		/**
		* @brief �Ƴ�һ��
		* @param[in] row_index: �ڼ���,base on 0;
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		bool RemoveRow(int row_index);

		/**
		* @brief �Ƴ�һ��
		* @param[in] col_index: �ڼ���,base on 0;
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		bool RemoveCol(int col_index);

		/**
		* @brief �������
		* @param[in] include_header: true��ʾ���header, false��ʾ����header;
		*/
		void Clear(bool include_header);

		/**
		* @brief ������ݱ����Ļص�ע��
		* @param[in] callback
		*/
		void AttachTextChange(const EventCallback& callback)	{ this->OnEvent[kEventTextChange] += callback; }

		/**
		* @brief ����excel
		* @param[in] file:	excel·��;
		* @param[in] sheet_num:	�����excel�ĵڼ�ҳ, base on 0;
		* @param[in] touch_header: true��ʾexcel�ĵ�һ����ΪGrid��Header����
		* @return ���� trueΪ�ɹ���false Ϊʧ��
		*/
		bool LoadExcel(std::wstring file, int sheet_num, bool touch_header);

		/**
		* @brief �и��������Զ��������
		* @param[in] col_index:	Ҫ�����������, base on 0;
		* @param[in] min_width:	��С���;
		* @param[in] max_width: �����, -1����û������;
		* @return ���� trueΪ�ɹ���false Ϊʧ��;
		*/
		bool AutoFixColWidth(int col_index, int min_width = 30, int max_width = -1);

		/**
		* @brief ��ȡѡ����Ϣ
		* @return ���� GridSelRange
		*/
		const GridSelRange& GetSelRange() const;
	protected:
		virtual void HandleMessage(EventArgs& event) override;
		virtual bool ButtonDown(EventArgs& msg) override;
		virtual bool ButtonUp(EventArgs& msg) override;
		virtual bool OnMouseDoubleClick(EventArgs& msg);		//not override
		virtual bool OnMouseMove(EventArgs& msg);				//not override
		virtual bool OnKeyDown(EventArgs& msg);					//not override

		virtual void PaintChild(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;
		virtual void Paint(dui::common::dui_refptr<dui::render::IRenderContext> pRender, const UiRect& rcPaint) override;
		virtual void PaintBody(dui::common::dui_refptr<dui::render::IRenderContext> pRender);		//not override
		virtual void PaintBorder(dui::common::dui_refptr<dui::render::IRenderContext> pRender) override;
	protected:
		/**
		* @brief �༭�ĵ�Ԫ��ΪCombo����ʱ, ѡ����ComboItem����;
		* @return ���ؼ�;
		*/
		bool OnComboEditSelected(EventArgs *args);

		/**
		* @brief SetColumnWidth�е���, �������п�;
		* @param[in] col_index:	Ҫ�����������, base on 0;
		* @param[in] width:	�µ��п�;
		*/
		virtual void OnColumnWidthChanged(int col_index, int width){};

		/**
		* @brief ��/ɾ��;
		* @param[in] col_index:	�����, base on 0, -1�������;
		* @param[in] bRemove:	true:ɾ����; false:������;
		*/
		virtual void OnColumnCountChanged(int col_index, bool bRemove){};

	protected:
		/*
		* @brief ����vector<int>֮��;
		* @return ��;
		*/
		int _SumIntList(const std::vector<int> &vec);

		virtual void _BeginEditGridItem(GridItem *item);
		virtual void _EndEdit();

		/*
		* @brief ��ȡ���λ���µ�GridItem
		* @param[in] pt: �������λ��;
		* @param[out] position:	��õ�GridItem����, base on 0
		* @param[in] fixed:	�Ƿ����fixed����;
		* @return trueΪ�ɹ�, �����position��ȡ, falseΪʧ��;
		*/
		bool _GetGridItemByMouse(CPoint pt, CPoint& position, bool fixed = false);

		/*
		* @brief ��ȡrow_index��GridItem��topλ��;
		* @param[in] row_index:	base on 0;
		* @return: row_index��GridItem��topλ��, û��ȥ������λ��
		*/
		int _GetGridItemTop(int row_index);

		/*
		* @brief ��ȡcol_index��GridItem��leftλ��;
		* @param[in] col_index:	base on 0;
		* @return: col_index��GridItem��leftλ��, û��ȥ������λ��
		*/
		int _GetGridItemLeft(int col_index);

		/*
		* @brief ��ȡGridItem����λ��;
		* @param[in] row_index: �ڼ���,base on 0;
		* @param[in] col_index: �ڼ���,base on 0;
		* @return ��������λ��, ��Grid�ؼ�[left,top]Ϊ0��, û��ȥ������λ��;
		*/
		UiRect _GetGridItemPos(int row_index, int col_index);

		/*
		* @brief ����༭��ѡ��״̬;
		*/
		void _ClearModifyAndSel();

		/* (�ݲ���)
		* @brief ��ȡ��fixed�������ʾGridItem��������;
		* ��ֵ��m_rcPaintRange;
		* return true��ʾ��ȡ�ɹ�, false��ʾ��ȡʧ��,���ü���������
		*/
		bool _GetPaintRangeRect();


	protected:	//��GridSelRange���õĺ���
		void _SelItem(int row_index, int col_index, bool selected);
		void _SelRow(int row_index, bool selected);
		void _SelCol(int col_index, bool selected);
		void _SelRange(const UiRect& rc, bool selected);


	protected:
		Grid *m_pGrid = nullptr;

		/* �����п� */
		GridLayout m_hLayout;

		/* �����и� */
		GridLayout m_vLayout;

		/* ��������Ϣ, ��һ��ΪHeader */
		std::vector<GridRow*> m_vecRow;

		/* �̶���/���� */
		unsigned int m_nFixedCol = 0;
		unsigned int m_nFixedRow = 0;

		/* ����ѡ����Ϣ */
		GridSelRange m_selRange;

		/* �϶���ͷ��� */
		int m_nDragColIndex = -1;					//���϶���col_index
		CPoint m_ptDragColumnStart;					//��갴��ʱmousepoint, ��GridΪzero��
		CPoint m_ptDragColumnMoving;				//�϶��п�״̬������ƶ���λ��, ��GridΪzero��
		int m_nDrawDragColumnMovingOffX = 0;		// = m_nDragColIndex���ұ߽� -  m_ptDragColumnMoving.x; Ϊ�˱�֤�϶��ߵ�λ��׼ȷ��

		/* �϶�ѡ����� */
		bool m_bDragSel = false;
		bool m_bDrageSelChanged = false;
		CPoint m_ptDragSelStart;					//����ѡ���϶���ʼrow_index��col_index
		CPoint m_ptDragSelMoving;					//�϶�ѡ��״̬��row_index��col_index��λ��

		/*	(�ݲ���)
		*  ��fixed�������ʾGridItem��������;
		*  top,bottom����row_index, left, right����col_index;
		*  PaintBkColor��ȡ��ֵ, ����Paint...����ʹ�ø�ֵ
		*/
		UiRect m_rcPaintRange;

		/* һЩ���� */
		int m_defaultRowHeight = 24;
		int m_defaultColWidth = 80;
		std::wstring m_strFixedBkColor = L"splitline_level2";
		std::wstring m_strSelForeColor = L"grid_sel_fore";
		std::wstring m_strGridLineColor = L"grid_line";
		std::wstring m_strGridFont = L"system_12";
		UINT	m_uTextStyle =  DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		
#define GRIDBODY_CHILD_COUNT	2
		/* �༭ʱ���RichEdit/Combo�ؼ� */
		RichEdit *m_pReEdit = nullptr;
		Combo *m_pComboEdit = nullptr;

		GridItem *m_pReEditGridItem = nullptr;
		GridItem *m_pComboEditGridItem = nullptr;
	};
}