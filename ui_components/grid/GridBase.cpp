#include "StdAfx.h"
#include "GridBase.h"
#include "GridBody.h"

namespace ui
{
	void GridSelRange::Clear() {

		bool bFlash = (m_vecRange.size() > 0 || m_mapSelRow.size() > 0 || m_mapSelCol.size() > 0);

		if (m_vecRange.size() > 0)
		{
			m_pBody->_SelRange(m_vecRange[0], false);
			m_vecRange.clear();
		}

		for (auto it = m_mapSelRow.cbegin(); it != m_mapSelRow.cend(); it++)
		{
			m_pBody->_SelRow(it->first, false);
		}
		m_mapSelRow.clear();  
		
		for (auto it = m_mapSelCol.cbegin(); it != m_mapSelCol.cend(); it++)
		{
			m_pBody->_SelCol(it->first, false);
		}
		m_mapSelCol.clear(); 
		m_bSelAll = false;

		if (bFlash)
			m_pBody->Invalidate();
	}

	void GridSelRange::ClearContent()
	{
		int row_count = m_pBody->GetRowCount();
		int col_count = m_pBody->GetColCount();
		int fixed_row_count = m_pBody->GetFixedRowCount();
		int fixed_col_count = m_pBody->GetFixedColCount();

		if (fixed_row_count == row_count || fixed_col_count == col_count)
			return;

		if (m_bSelAll)
		{
			for (int row = fixed_row_count; row < row_count; row++)
			{
				GridRow *pRow = m_pBody->m_vecRow[row];
				for (int col = fixed_col_count; col < col_count; col++)
				{
					pRow->at(col)->text = L"";
				}
			}
			m_pBody->Invalidate();
			return;
		}
		
		bool bFlash = false;
		for (auto it = m_mapSelRow.cbegin(); it != m_mapSelRow.cend(); it++)
		{
			int row_index = it->first;
			assert(row_index >= fixed_row_count && row_index < row_count);
			if (row_index < fixed_row_count || row_index >= row_count)
				continue;
			GridRow *pRow = m_pBody->m_vecRow[row_index];
			for (int col = fixed_col_count; col < col_count; col++)
			{
				pRow->at(col)->text = L"";
			}
			bFlash = true;
		}

		if (m_mapSelCol.size() > 0)
		{
			for (int row = fixed_row_count; row < row_count; row++)
			{
				GridRow *pRow = m_pBody->m_vecRow[row];
				for (auto it = m_mapSelCol.cbegin(); it != m_mapSelCol.cend(); it++)
				{
					int col_index = it->first;
					assert(col_index >= fixed_col_count && col_index < col_count);
					if (col_index < fixed_col_count || col_index >= col_count)
						continue;
					pRow->at(col_index)->text = L"";
				}
			}
			bFlash = true;
		}

		if (m_vecRange.size() > 0)
		{
			int row_index_start = m_vecRange[0].top;
			int col_index_start = m_vecRange[0].left;
			int row_index_end = m_vecRange[0].bottom;
			int col_index_end = m_vecRange[0].right;
			if (row_index_start < fixed_row_count)
				row_index_start = fixed_row_count;
			if (col_index_start < fixed_col_count)
				col_index_start = fixed_col_count;
			if (row_index_end >=  row_count)
				row_index_end = row_count - 1;
			if (col_index_end >= col_count)
				col_index_end = col_count - 1;

			for (int row = row_index_start; row <= row_index_end; row++)
			{
				GridRow *pRow = m_pBody->m_vecRow[row];
				for (int col = col_index_start; col <= col_index_end; col++)
				{
					GridItem *pItem = pRow->at(col);
					if (pItem->IsSelected())
						pItem->text = L"";
				}
			}
			bFlash = true;
		}
		if (bFlash)
			m_pBody->Invalidate();
	}
	
	void GridSelRange::SetSelAll()
	{
		if (m_bSelAll)
		{
			Clear();
		}
		else
		{
			m_mapSelRow.clear();
			m_mapSelCol.clear();
			m_vecRange.clear();

			int row_count = m_pBody->GetRowCount();
			int col_count = m_pBody->GetColCount();
			int fixed_row_count = m_pBody->GetFixedRowCount();
			int fixed_col_count = m_pBody->GetFixedColCount();

			if (fixed_row_count == row_count || fixed_col_count == col_count)
				return;
			UiRect rc(fixed_col_count, fixed_row_count, col_count - 1, row_count - 1);
			m_vecRange.push_back(rc);
			m_pBody->_SelRange(rc, true);

			m_bSelAll = true;
		}
		m_pBody->Invalidate();
	}

	void GridSelRange::SetSelItem(int row_index, int col_index, bool ctrl, bool shift){
		assert(m_vecRange.size() < 2);

		GridItem *pItem = m_pBody->GetGridItem(row_index, col_index);
		assert(pItem);
		if (!pItem)
			return;
		bool bSelected = pItem->IsSelected();

		if (ctrl && shift)
		{
			ctrl = false;
			shift = false;
		}

		if (ctrl)
		{
			UiRect rc(-1,-1,-1,-1);
			if (m_vecRange.size() > 0)
				rc = m_vecRange[0];
			
			bool bInSelRow = false;
			bool bInSelCol = false;

			if (m_mapSelRow.find(row_index) != m_mapSelRow.end())	//row_index为选中状态
			{
				assert(bSelected);
				bInSelRow = true;
				rc.left = m_pBody->GetFixedColCount();
				rc.right = m_pBody->GetColCount() - 1;
				if (rc.top < row_index)
					rc.top = row_index;
				if (rc.bottom > row_index)
					rc.bottom = row_index;
				m_mapSelRow.erase(row_index);
				//pItem->SetSelected(false);
			}

			if (m_mapSelCol.find(col_index) != m_mapSelCol.end())	//col_index为选中状态
			{
				assert(bSelected);
				bInSelCol = true;
				rc.top = m_pBody->GetFixedRowCount();
				rc.bottom = m_pBody->GetRowCount() - 1;
				if (rc.left < col_index)
					rc.left = col_index;
				if (rc.right > col_index)
					rc.left = col_index;
				m_mapSelCol.erase(col_index);
				//pItem->SetSelected(false);
			}

			if (rc.left > col_index || rc.right < col_index || rc.top > row_index || rc.bottom < row_index)		//不位于rc中
			{
				//assert(!bSelected);
				rc.left = min(rc.left, col_index);
				rc.top = min(rc.top, row_index);
				rc.right = max(rc.right, col_index);
				rc.bottom = max(rc.bottom, row_index);
			}

			if (m_vecRange.empty())
				m_vecRange.push_back(rc);
			else
				m_vecRange[0] = rc;
			pItem->SetSelected(!bSelected);
			m_pBody->Invalidate();
		}
		else if (shift)
		{
			if (m_vecRange.size() == 0)
			{
				assert(!bSelected);
				UiRect rc(col_index, row_index, col_index, row_index);
				m_vecRange.push_back(rc);
				m_pBody->_SelItem(row_index, col_index, true);
				m_pBody->Invalidate();
			}
			else
			{
				UiRect rc = m_vecRange[0];
				rc.left = min(rc.left, col_index);
				rc.top = min(rc.top, row_index);
				rc.right = max(rc.right, col_index);
				rc.bottom = max(rc.bottom, row_index);
				m_vecRange[0] = rc;
				m_pBody->_SelRange(rc, true);
				m_pBody->Invalidate();
			}
		}
		else
		{
			Clear();
			m_vecRange.push_back(UiRect(col_index, row_index, col_index, row_index));
			m_pBody->_SelItem(row_index, col_index, true);
			m_pBody->Invalidate();
		}
		if (m_bSelAll)
			m_bSelAll = false;
	}

	void GridSelRange::SetSelItemRange(UiRect rc, bool ctrl, bool shift)
	{
		assert(m_vecRange.size() < 3);
		if (ctrl && shift)
		{
			ctrl = false;
			shift = false;
		}

		if (!ctrl)
		{
			Clear();
			m_vecRange.push_back(rc);
			m_pBody->_SelRange(rc, true);
			m_pBody->Invalidate();
		}
		else
		{
			assert(m_vecRange.size() > 0);		//ButtonDown时应该调用了SetSelItem, 给m_vecRange赋值
			if (m_vecRange.size() == 0)
			{
				m_vecRange.push_back(rc);
				m_pBody->_SelRange(rc, true);
				m_pBody->Invalidate();
			}
			else
			{
				if (m_vecRange.size() == 1)
				{
					m_pBody->_SelRange(rc, true);
					m_vecRange.push_back(rc);
				}
				else if (m_vecRange.size() == 2)
				{
					m_pBody->_SelRange(m_vecRange[1], false);
					//m_pBody->_SelRange(m_vecRange[0], true);
					m_pBody->_SelRange(rc, true);
					m_vecRange.pop_back();
					m_vecRange.push_back(rc);
				}	
				m_pBody->Invalidate();
			}
		}
		if (m_bSelAll)
			m_bSelAll = false;
	}

	void GridSelRange::SetSelRow(int row_index, bool ctrl, bool shift)
	{
		assert(m_vecRange.size() < 2);
		if (ctrl && shift)
		{
			ctrl = false;
			shift = false;
		}

		if (ctrl)
		{
			if (m_mapSelRow.find(row_index) == m_mapSelRow.end())
			{
				m_pBody->_SelRow(row_index, true);
				m_mapSelRow[row_index] = nullptr;
			}
			else
			{
				m_pBody->_SelRow(row_index, false);
				m_mapSelRow.erase(row_index);
			}
			m_pBody->Invalidate();
		}
		else if (shift)
		{
			if (m_mapSelRow.size() == 0)
			{
				m_pBody->_SelRow(row_index, true);
				m_mapSelRow[row_index] = nullptr;
				m_pBody->Invalidate();
			}
			else
			{
				bool bChange = false;
				int min_row_index = 0, max_row_index = 0;
				min_row_index = m_mapSelRow.begin()->first;
				max_row_index = m_mapSelRow.rbegin()->first;		

				min_row_index = min(min_row_index, row_index);
				max_row_index = max(max_row_index, row_index);

				for (int row = min_row_index; row <= max_row_index; row++)
				{
					if (m_mapSelRow.find(row) == m_mapSelRow.end())
					{
						bChange = true;
						m_pBody->_SelRow(row, true);
						m_mapSelRow[row] = nullptr;
					}
				}
				if (bChange)
					m_pBody->Invalidate();
			}
		}
		else
		{
			Clear();
			m_pBody->_SelRow(row_index, true);
			m_mapSelRow[row_index] = nullptr;
			m_pBody->Invalidate();
		}
		if (m_bSelAll)
			m_bSelAll = false;
	}

	void GridSelRange::SetSelRowRange(int row_index, int row_index_end, bool ctrl, bool shift)
	{

		assert(m_vecRange.size() < 2);
		assert(row_index <= row_index_end);
		if (row_index > row_index_end)
			return;
		if (ctrl || shift)		//直接返回不处理了, 搞不清逻辑了
			return;
#if 0
		if (ctrl && shift)
		{
			ctrl = false;
			shift = false;
		}

		if (ctrl)
		{
			bool bChange = false;
			for (int row = row_index; row <= row_index_end; row++)
			{
				if (m_mapSelRow.find(row) == m_mapSelRow.end())
				{
					bChange = true;
					m_pBody->_SelRow(row, true);
					m_mapSelRow[row] = nullptr;
				}
			}
			if (bChange)
				m_pBody->Invalidate();
		}
		else if (shift)
		{
			bool bChange = false;
			int min_row_index = 0, max_row_index = 0;
			if (m_mapSelRow.size() > 0){
				min_row_index = m_mapSelRow.begin()->first;
				max_row_index = m_mapSelRow.rbegin()->first;
			}
				
			min_row_index = min(min_row_index, row_index);
			max_row_index = max(max_row_index, row_index_end);

			for (int row = min_row_index; row <= max_row_index; row++)
			{
				if (m_mapSelRow.find(row) == m_mapSelRow.end())
				{
					bChange = true;
					m_pBody->_SelRow(row, true);
					m_mapSelRow[row] = nullptr;
				}
			}
			if (bChange)
				m_pBody->Invalidate();
		}
		else
#endif
		{
			Clear();
			for (int row = row_index; row <= row_index_end; row++)
			{
				if (m_mapSelRow.find(row) == m_mapSelRow.end())
				{
					m_pBody->_SelRow(row, true);
					m_mapSelRow[row] = nullptr;
				}
			}
			m_pBody->Invalidate();
		}
		if (m_bSelAll)
			m_bSelAll = false;
	}

	void GridSelRange::SetSelCol(int col_index, bool ctrl, bool shift)
	{
		assert(m_vecRange.size() < 2);
		if (ctrl && shift)
		{
			ctrl = false;
			shift = false;
		}

		if (ctrl)
		{
			if (m_mapSelCol.find(col_index) == m_mapSelCol.end())
			{
				m_pBody->_SelCol(col_index, true);
				m_mapSelCol[col_index] = nullptr;
			}
			else
			{
				m_pBody->_SelCol(col_index, false);
				m_mapSelCol.erase(col_index);
			}
			m_pBody->Invalidate();
		}
		else if (shift)
		{
			if (m_mapSelCol.size() == 0)
			{
				m_pBody->_SelCol(col_index, true);
				m_mapSelCol[col_index] = nullptr;
				m_pBody->Invalidate();
			}
			else
			{
				bool bChange = false;
				int min_col_index = 0, max_col_index = 0;
				min_col_index = m_mapSelCol.begin()->first;
				max_col_index = m_mapSelCol.rbegin()->first;

				min_col_index = min(min_col_index, col_index);
				max_col_index = max(max_col_index, col_index);

				for (int col = min_col_index; col <= max_col_index; col++)
				{
					if (m_mapSelCol.find(col) == m_mapSelCol.end())
					{
						bChange = true;
						m_pBody->_SelCol(col, true);
						m_mapSelCol[col] = nullptr;
					}
				}
				if (bChange)
					m_pBody->Invalidate();
			}
		}
		else
		{
			Clear();
			m_pBody->_SelCol(col_index, true);
			m_mapSelCol[col_index] = nullptr;
			m_pBody->Invalidate();
		}
		if (m_bSelAll)
			m_bSelAll = false;
	}

	void GridSelRange::SetSelColRange(int col_index, int col_index_end, bool ctrl, bool shift)
	{
		assert(m_vecRange.size() < 2);
		assert(col_index <= col_index_end);
		if (col_index > col_index_end)
			return;
		if (ctrl || shift)		//直接返回不处理了, 搞不清逻辑了
			return;
#if 0
		if (ctrl && shift)
		{
			ctrl = false;
			shift = false;
		}

		if (ctrl)
		{
			bool bChange = false;
			for (int col = col_index; col <= col_index_end; col++)
			{
				if (m_mapSelCol.find(col) == m_mapSelCol.end())
				{
					bChange = true;
					m_pBody->_SelCol(col, true);
					m_mapSelCol[col] = nullptr;
				}
			}
			if (bChange)
				m_pBody->Invalidate();
		}
		else if (shift)
		{
			bool bChange = false;
			int min_col_index = 0, max_col_index = 0;
			if (m_mapSelCol.size() > 0){
				min_col_index = m_mapSelCol.begin()->first;
				max_col_index = m_mapSelCol.rbegin()->first;
			}

			min_col_index = min(min_col_index, col_index);
			max_col_index = max(max_col_index, col_index_end);

			for (int col = min_col_index; col <= max_col_index; col++)
			{
				if (m_mapSelCol.find(col) == m_mapSelCol.end())
				{
					bChange = true;
					m_pBody->_SelCol(col, true);
					m_mapSelCol[col] = nullptr;
				}
			}
			if (bChange)
				m_pBody->Invalidate();
		}
		else
#endif
		{
			Clear();
			for (int col = col_index; col <= col_index_end; col++)
			{
				if (m_mapSelCol.find(col) == m_mapSelCol.end())
				{
					m_pBody->_SelCol(col, true);
					m_mapSelCol[col] = nullptr;
				}
			}
			m_pBody->Invalidate();
		}
		if (m_bSelAll)
			m_bSelAll = false;
	}

	void GridSelRange::MergeRange()
	{
		assert(m_vecRange.size() < 3);
		if (m_vecRange.size() == 2)
		{
			UiRect rc = m_vecRange[0];
			UiRect rc1 = m_vecRange[1];
			rc.left = min(rc.left, rc1.left);
			rc.top = min(rc.top, rc1.top);
			rc.right = max(rc.right, rc1.right);
			rc.bottom = max(rc.bottom, rc1.bottom);
			m_vecRange.pop_back();
			m_vecRange[0] = rc;
		}
	}
	
	/*bool GridSelRange::IsItemSelected(int row_index, int col_index)
	{
		if (IsRowSelected(row_index) || IsColSelected(col_index))
			return true;
		bool ret = false;
		CPoint pt(col_index, row_index);
		for (auto it = m_vecRange.cbegin(); it != m_vecRange.cend(); it++)
		{
			UiRect rc = *it;
			if (rc.IsPointIn(pt))
			{
				ret = true;
				break;
			}
		}
		return ret;
	}*/

	bool GridSelRange::IsRowSelected(int row_index)
	{
		return m_mapSelRow.find(row_index) != m_mapSelRow.cend();
	}

	bool GridSelRange::IsColSelected(int col_index)
	{
		return m_mapSelCol.find(col_index) != m_mapSelCol.cend();
	}

	bool GridSelRange::GetSelRange(UiRect &rc)
	{
		if (m_vecRange.size() != 1)
			return false;
		rc = m_vecRange[0];
		return true;
	}

	std::vector<int> GridSelRange::GetSelRows()
	{
		std::vector<int> vec;
		for (auto it = m_mapSelRow.cbegin(); it != m_mapSelRow.cend(); it++)
		{
			vec.push_back(it->first);
		}

		return vec;
	}

	std::vector<int> GridSelRange::GetSelCols()
	{
		std::vector<int> vec;
		for (auto it = m_mapSelCol.cbegin(); it != m_mapSelCol.cend(); it++)
		{
			vec.push_back(it->first);
		}

		return vec;
	}

	bool GridSelRange::MoveSelItem(TCHAR move_key, bool ctrl, bool shift)
	{
		if (shift)
			return false;

		int row_count = m_pBody->GetRowCount();
		int col_count = m_pBody->GetColCount();
		int fixed_row_count = m_pBody->GetFixedRowCount();
		int fixed_col_count = m_pBody->GetFixedColCount();
		int row_index = fixed_row_count;
		int col_index = fixed_col_count;

		if (fixed_row_count == row_count || fixed_col_count == col_count)
			return false;

		if (ctrl)
		{

		}
		else
		{
			UiRect rc(row_index, col_index, row_index, col_index);
			if (m_vecRange.size() > 0)
				rc = m_vecRange[0];

			switch (move_key)
			{
			case VK_LEFT:
				row_index = rc.top;
				col_index = rc.left - 1;
				break;
			case VK_UP:
				row_index = rc.top - 1;
				col_index = rc.left;
				break;
			case VK_RIGHT:
				row_index = rc.bottom;
				col_index = rc.right + 1;
				break;
			case VK_DOWN:
				row_index = rc.bottom + 1;
				col_index = rc.right;
				break;
			default:
				break;
			}

			if (row_index >= row_count)
				row_index = row_count - 1;
			if (col_index >= col_count)
				col_index = col_count - 1;
			if (row_index < fixed_row_count)
				row_index = fixed_row_count;
			if (col_index < fixed_col_count)
				col_index = fixed_col_count;

			SetSelItem(row_index, col_index);
		}
		if (m_bSelAll)
			m_bSelAll = false;
	}

	void GridSelRange::CtrlCorX(bool cut)
	{
		if (m_vecRange.size() == 0)
			return;

		int row_count = m_pBody->GetRowCount();
		int col_count = m_pBody->GetColCount();
		int fixed_row_count = m_pBody->GetFixedRowCount();
		int fixed_col_count = m_pBody->GetFixedColCount();

		if (fixed_row_count == row_count || fixed_col_count == col_count)
			return;

		UiRect rc = m_vecRange[0];
		if (rc.left < fixed_col_count)
			rc.left = fixed_col_count;
		if (rc.top < fixed_row_count)
			rc.top = fixed_row_count;
		if (rc.right >= col_count)
			rc.right = col_count - 1;
		if (rc.bottom >= row_count)
			rc.bottom = row_count - 1;

		std::wstring strContent;
		GridItem *pItem = nullptr;
		for (int row_index = rc.top; row_index <= rc.bottom; row_index++)
		{
			GridRow *pRow = m_pBody->m_vecRow[row_index];
			for (int col_index = rc.left; col_index <= rc.right; col_index++)
			{
				pItem = pRow->at(col_index);
				assert(pItem);
				if (pItem && pItem->IsSelected())
					strContent += pItem->text;
				if (col_index < rc.right)
					strContent += L"\t";
			}
			if (row_index < rc.bottom)
				strContent += L"\r\n";
		}
		if (strContent.empty())
			return;
		strContent += L"\0";

		if (::OpenClipboard(NULL))
		{
			if (FALSE == ::EmptyClipboard())
			{
				::CloseClipboard();
				return;
			}
			HGLOBAL hClip = ::GlobalAlloc(GMEM_MOVEABLE, (strContent.length() + 1) * sizeof(wchar_t));
			if (NULL == hClip)
			{
				::CloseClipboard();
				return;
			}
			wchar_t *pBuf = (wchar_t*)::GlobalLock(hClip);
			if (NULL == pBuf)
			{
				::CloseHandle(hClip);
				::CloseClipboard();
				return;
			}
			wcscpy(pBuf, strContent.c_str());
			::GlobalUnlock(hClip);

			if (NULL == ::SetClipboardData(CF_UNICODETEXT, hClip))
			{
				::CloseHandle(hClip);
				::CloseClipboard();
				return;
			}

			::CloseClipboard();

			if (cut)
			{
				for (int row_index = rc.top; row_index <= rc.bottom; row_index++)
				{
					GridRow *pRow = m_pBody->m_vecRow[row_index];
					for (int col_index = rc.left; col_index <= rc.right; col_index++)
					{
						pItem = pRow->at(col_index);
						assert(pItem);
						if (pItem && pItem->IsSelected())
							pItem->text = L"";
					}
				}
				m_pBody->Invalidate();
			}
		}
	}
	void GridSelRange::CtrlV()
	{
		int row_count = m_pBody->GetRowCount();
		int col_count = m_pBody->GetColCount();
		int fixed_row_count = m_pBody->GetFixedRowCount();
		int fixed_col_count = m_pBody->GetFixedColCount();
		int row_index_start = fixed_row_count;
		int col_index_start = fixed_col_count;

		if (fixed_row_count == row_count || fixed_col_count == col_count)
			return;

		std::wstring strClipboard;
		if (::OpenClipboard(NULL))
		{
			if (::IsClipboardFormatAvailable(CF_UNICODETEXT))
			{
				HGLOBAL hClip = GetClipboardData(CF_UNICODETEXT);
				if (NULL == hClip)
				{
					CloseClipboard();
					return;
				}
				wchar_t *pStr = (wchar_t*)GlobalLock(hClip);
				strClipboard = pStr;
				GlobalUnlock(hClip);
				CloseClipboard();
			}
			else if (::IsClipboardFormatAvailable(CF_TEXT))
			{
				HGLOBAL hClip = GetClipboardData(CF_TEXT);
				if (NULL == hClip)
				{
					CloseClipboard();
					return;
				}
				char *pStr = (char*)GlobalLock(hClip);
				StringHelper::MBCSToUnicode(pStr, strClipboard);
				GlobalUnlock(hClip);
				CloseClipboard();
			}
		}
		if (strClipboard.empty())
			return;
		if (m_vecRange.size() > 0)
		{
			row_index_start = m_vecRange[0].top;
			col_index_start = m_vecRange[0].left;
		}

		bool bFlash = false;
		std::wstring strLine;
		std::wstring strItem;
		std::wstring strLineNext = strClipboard;
		//std::wstring strItemNext;
		int pos_rn = -1;
		int pos_t = -1;
		for (int row = row_index_start; row < row_count; row++)
		{
			if (strLineNext.empty())
				break;
			pos_rn = strLineNext.find_first_of(L"\r\n");
			if (pos_rn == std::wstring::npos)
			{
				strLine = strLineNext;
				strLineNext = L"";
			}
			else
			{
				strLine = strLineNext.substr(0, pos_rn);
				strLineNext = strLineNext.substr(pos_rn + 2);
			}
			GridRow *pRow = m_pBody->m_vecRow[row];
			for (int col = col_index_start; col < col_count; col++)
			{
				if (strLine.empty())
					break;
				pos_t = strLine.find_first_of(L"\t");
				if (pos_t == std::wstring::npos)
				{
					strItem = strLine;
					strLine = L"";
				}
				else
				{
					strItem = strLine.substr(0, pos_t);
					strLine = strLine.substr(pos_t + 1);
				}
				pRow->at(col)->text = strItem;
				bFlash = true;
				wprintf(L"GridSelRange::CtrlV row:%d, col:%d, [%s]\n", row, col, strItem.c_str());
			}
		}
		if (bFlash)
			m_pBody->Invalidate();
	}

}

