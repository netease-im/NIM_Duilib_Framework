#include "stdafx.h"
#include <tchar.h>
#include "file_dialog_ex.h"
#include "async_do_modal.h"
#include "base/thread/thread_manager.h"
#include "base/util/string_util.h"
#include "base/file/file_util.h"
#include "shared/threads.h"

CFileDialogEx::CFileDialogEx(void)
{
	memset(&m_szDefExt, 0, sizeof(m_szDefExt));
	memset(&m_szFilter, 0, sizeof(m_szFilter));
	memset(&m_szFileName, 0, sizeof(m_szFileName));
	memset(&m_stOFN, 0, sizeof(OPENFILENAME));

	m_lpszFileName = NULL;
	file_dialog_type_ = FDT_None; 
	m_stOFN.lStructSize = sizeof(OPENFILENAME);
	m_stOFN.Flags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_NOCHANGEDIR|OFN_EXTENSIONDIFFERENT;
	m_stOFN.lpstrDefExt = m_szDefExt;
	m_stOFN.lpstrFilter = m_szFilter;
	m_stOFN.lpstrFile = m_szFileName;
	m_stOFN.nMaxFile = MAX_PATH;
}

CFileDialogEx::~CFileDialogEx(void)
{
	if (m_lpszFileName != NULL)
	{
		delete []m_lpszFileName;
		m_lpszFileName = NULL;
	}
}

void CFileDialogEx::SetDefExt(LPCTSTR lpszDefExt)
{
	wcsncpy_s(m_szDefExt, lpszDefExt, sizeof(m_szDefExt) / sizeof(TCHAR));
}

void CFileDialogEx::SetFileName(LPCTSTR lpszFileName)
{
	wcsncpy_s(m_szFileName, lpszFileName, sizeof(m_szFileName) / sizeof(TCHAR));
}

void CFileDialogEx::SetFlags(DWORD dwFlags)
{
	m_stOFN.Flags = dwFlags;
}

// 示例_T("Text Files(*.txt)\0*.txt\0网页文件\0*.htm;*.html\0All Files(*.*)\0*.*\0\0")
void CFileDialogEx::SetFilter(LPCTSTR lpszFilter)
{
	LPTSTR lpsz = (LPTSTR)lpszFilter;
	LPTSTR lpszOld;
	int i = 0;

	while(1)
	{
		if (i >= MAX_PATH - 1)
			break;

		m_szFilter[i] = *lpsz;
		i++;
		lpszOld = lpsz;
		lpsz = _tcsinc(lpsz);

		if (*lpszOld == '\0' && *lpsz == '\0')
		{
			m_szFilter[i] = *lpsz;
			break;
		}
	}
}

//param:过滤字符串对,key-描述字符串 value-过滤字符串
//参数可以指定多组过滤类型
//example:filers[L"图像文件(*.jpg)"] = L"*.jpg";
void CFileDialogEx::SetFilter(std::map<LPCTSTR,LPCTSTR>& filters)
{
	std::map<LPCTSTR,LPCTSTR>::iterator it = filters.begin();
	const int size = filters.size()*100;
	wchar_t* filterstring= new wchar_t[size];
	int offset = 0;
	for(;it != filters.end();it++)
	{
		_tcscpy(filterstring + offset,it->first);
		offset += _tcslen(it->first) + 1;
		_tcscpy(filterstring + offset,it->second);
		offset += _tcslen(it->second) + 1;
	}
	*(filterstring + offset) = '\0';
	SetFilter(filterstring);
	delete[] filterstring;
}

void CFileDialogEx::SetMultiSel(BOOL bMultiSel/* = TRUE*/)
{
	if (bMultiSel)
	{
		m_stOFN.Flags |= OFN_ALLOWMULTISELECT;
		SetFileNameBufferSize(4096);
	}
	else
		m_stOFN.Flags &= ~OFN_ALLOWMULTISELECT;
}

void CFileDialogEx::SetParentWnd(HWND hParentWnd)
{
	m_stOFN.hwndOwner = hParentWnd;
}

void CFileDialogEx::SetTitle(LPCTSTR lpszTitle)
{
	m_stOFN.lpstrTitle = lpszTitle;
}

void CFileDialogEx::SetFileNameBufferSize(DWORD dwSize)
{
	if (m_lpszFileName != NULL)
	{
		delete []m_lpszFileName;
		m_lpszFileName = NULL;
	}

	if (dwSize <= (sizeof(m_szFileName) / sizeof(TCHAR)))
	{
		m_stOFN.lpstrFile = m_szFileName;
		m_stOFN.nMaxFile = MAX_PATH;
	}
	else
	{
		m_lpszFileName = new TCHAR[dwSize];
		if (m_lpszFileName != NULL)
		{
			memset(m_lpszFileName, 0, dwSize * sizeof(TCHAR));
			m_stOFN.lpstrFile = m_lpszFileName;
			m_stOFN.nMaxFile = dwSize;
		}
		else
		{
			m_stOFN.lpstrFile = m_szFileName;
			m_stOFN.nMaxFile = MAX_PATH;
		}
	}
}

void CFileDialogEx::AyncShowOpenFileDlg(FileDialogCallback2 file_dialog_callback2)
{
	file_dialog_callback2_ = file_dialog_callback2;
	file_dialog_type_ = FDT_OpenFile;
	AsyncDoModal(this);
}

void CFileDialogEx::AyncShowSaveFileDlg(FileDialogCallback2 file_dialog_callback2)
{
	file_dialog_callback2_ = file_dialog_callback2;
	file_dialog_type_ = FDT_SaveFile;
	AsyncDoModal(this);
}

void CFileDialogEx::SyncShowModal()
{
	if (file_dialog_type_ == FDT_OpenFile)
	{
		BOOL ret = ::GetOpenFileName(&m_stOFN);
		if ( (m_stOFN.Flags & OFN_ALLOWMULTISELECT) > 0)
		{
			uint32_t get_length = 0;
			std::wstring file_directory = m_stOFN.lpstrFile;
			get_length = file_directory.size()+1;
			std::wstring file_name = m_stOFN.lpstrFile + get_length;
			if (file_name.size() == 0)
			{
				StdClosure closure = nbase::Bind(file_dialog_callback2_, ret, file_directory);
				nbase::ThreadManager::PostTask(kThreadUI, closure);
			}
			else
			{
				while (file_name.size() > 0)
				{
					std::wstring file_path = file_directory + L"\\" + file_name;
					if (nbase::FilePathIsExist(file_path, false))
					{
						StdClosure closure = nbase::Bind(file_dialog_callback2_, ret, file_path);
						nbase::ThreadManager::PostTask(kThreadUI, closure);
					}
					get_length += file_name.size()+1;
					file_name = m_stOFN.lpstrFile + get_length;
				}
			}
		} 
		else
		{
			StdClosure closure = nbase::Bind(file_dialog_callback2_, ret, GetPathName());
			nbase::ThreadManager::PostTask(kThreadUI, closure);
		}
	}
	else if (file_dialog_type_ == FDT_SaveFile)
	{
		BOOL ret = ::GetSaveFileName(&m_stOFN);
		StdClosure closure = nbase::Bind(file_dialog_callback2_, ret, GetPathName());
		nbase::ThreadManager::PostTask(kThreadUI, closure);
	}
	else
	{
		assert(FALSE);
	}
}

std::wstring CFileDialogEx::GetPathName()
{
	return m_stOFN.lpstrFile;
}

std::wstring CFileDialogEx::GetFileName()
{
	return L"";
}

std::wstring CFileDialogEx::GetFileExt()
{
	return L"";
}

std::wstring CFileDialogEx::GetFileTitle()
{
	return L"";
}

std::wstring CFileDialogEx::GetFolderPath()
{
	return L"";
}


POSITION CFileDialogEx::GetStartPosition()
{
	return (POSITION)m_stOFN.lpstrFile;
}

std::wstring CFileDialogEx::GetNextPathName(POSITION& pos)
{
	LPTSTR lpsz = (LPTSTR)pos;
	if (lpsz == m_stOFN.lpstrFile) // first time
	{
		if ((m_stOFN.Flags & OFN_ALLOWMULTISELECT) == 0)
		{
			pos = NULL;
			return m_stOFN.lpstrFile;
		}

		// find char pos after first Delimiter
		while(*lpsz != '\0')
			lpsz = _tcsinc(lpsz);
		lpsz = _tcsinc(lpsz);

		// if single selection then return only selection
		if (*lpsz == '\0')
		{
			pos = NULL;
			return m_stOFN.lpstrFile;
		}
	}

	std::wstring strBasePath = m_stOFN.lpstrFile;
	std::wstring strFileName = lpsz;
	if (strBasePath.substr(strBasePath.length() - 1) != _T("\\"))
		strBasePath += _T("\\");
	std::wstring strPath = strBasePath + strFileName;

	// find char pos at next Delimiter
	while(*lpsz != '\0')
		lpsz = _tcsinc(lpsz);
	lpsz = _tcsinc(lpsz);
	if (*lpsz == '\0') // if double terminated then done
		pos = NULL;
	else
		pos = (POSITION)lpsz;

	return strPath;
}