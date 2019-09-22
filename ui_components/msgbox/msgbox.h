#pragma once
#include "duilib/UIlib.h"

namespace nim_comp {

enum MsgBoxRet
{
	MB_YES,
	MB_NO
};

typedef std::function<void(MsgBoxRet)> MsgboxCallback;

void ShowMsgBox(HWND hwnd, MsgboxCallback cb,
    const std::wstring &content = L"", bool content_is_id = true,
    const std::wstring &title = L"STRING_TIPS", bool title_is_id = true,
    const std::wstring &yes = L"STRING_OK", bool btn_yes_is_id = true,
    const std::wstring &no = L"", bool btn_no_is_id = false);

class MsgBox : public ui::WindowImplBase
{
public:
	friend void ShowMsgBox(HWND hwnd, MsgboxCallback cb,
        const std::wstring &content, bool content_is_id,
        const std::wstring &title, bool title_is_id,
        const std::wstring &yes, bool btn_yes_is_id,
        const std::wstring &no, bool btn_no_is_id);
public:
	MsgBox();
	virtual ~MsgBox();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnEsc(BOOL &bHandled);
	virtual void Close(UINT nRet = IDOK) override;

	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const /*override*/;
	virtual UINT GetClassStyle() const override;
	virtual void InitWindow() override;
private:
	bool OnClicked(ui::EventArgs* msg);

	void SetTitle(const std::wstring &str);
	void SetContent(const std::wstring &str);
	void SetButton(const std::wstring &yes, const std::wstring &no);
	void Show(HWND hwnd, MsgboxCallback cb);

	void EndMsgBox(MsgBoxRet ret);
public:
	static const LPCTSTR kClassName;
private:
	ui::Label*		title_;
	ui::RichEdit*	content_;
	ui::Button*		btn_yes_;
	ui::Button*		btn_no_;

	MsgboxCallback	 msgbox_callback_;
};

}
