#ifndef _TOOL_FILE_DIALOGEX_H_
#define _TOOL_FILE_DIALOGEX_H_

#include "modal_wnd_base.h"

#include <commdlg.h>

namespace nim_comp {

struct __POSITION {};
typedef __POSITION* POSITION;

class CFileDialogEx : public ModalWndBase
{
public:
	typedef std::function<void(BOOL, std::wstring)> FileDialogCallback2;

	enum FileDialogType
	{
		FDT_None,
		FDT_OpenFile,
		FDT_SaveFile,
		FDT_SelectFolder,
	};

	CFileDialogEx(void);
	~CFileDialogEx(void);

public:
	void SetDefExt(LPCTSTR lpszDefExt);
	void SetFileName(LPCTSTR lpszFileName);
	void SetFlags(DWORD dwFlags);
	void SetFilter(LPCTSTR lpszFilter);
	void SetFilter(std::map<LPCTSTR,LPCTSTR>& filters);
	void SetMultiSel(BOOL bMultiSel = TRUE);
	void SetParentWnd(HWND hParentWnd);
	void SetTitle(LPCTSTR lpszTitle);
	void SetFileNameBufferSize(DWORD dwSize);

	void AyncShowOpenFileDlg(FileDialogCallback2 file_dialog_callback1);
	void AyncShowSaveFileDlg(FileDialogCallback2 file_dialog_callback2);
	void AsyncShowSelectFolderDlg(FileDialogCallback2 file_dialog_callback2);
	virtual void SyncShowModal() override;
	std::wstring GetPathName();
	std::wstring GetFileName();
	std::wstring GetFileExt();
	std::wstring GetFileTitle();
	std::wstring GetFolderPath();

	POSITION GetStartPosition();
	std::wstring GetNextPathName(POSITION& pos);

public:
	OPENFILENAME m_stOFN;

private:
	wchar_t m_szDefExt[64];
	wchar_t m_szFilter[MAX_PATH];
	wchar_t m_szFileName[MAX_PATH];
	wchar_t * m_lpszFileName;
	FileDialogType file_dialog_type_;
	FileDialogCallback2 file_dialog_callback2_;

	std::wstring m_strTitle;
};

}

#endif //_TOOL_FILE_DIALOGEX_H_