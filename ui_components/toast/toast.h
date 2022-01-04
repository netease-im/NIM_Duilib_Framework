#pragma once

namespace nim_comp {
/** @class Toast
 * @brief 提供一个简易的提示窗体，相当于MessageBox的简化版
 * @copyright (c) 2016, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2016/8/24
 */
class Toast : public ui::WindowImplBase
{
public:
	/**
	 * 在屏幕中间显示一个提示框
	 * @param[in] content 提示内容
	 * @param[in] duration 持续时间(毫秒),0代表一直显示
	 * @param[in] parent 父窗口句柄，如果提供的话toast会显示在父窗口的中心
	 * @return void 无返回值
	 */
	static void ShowToast(const std::wstring &content, int duration = 0, HWND parent = NULL);
public:
	Toast(){};
	~Toast(){};

	//覆盖虚函数
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const /*override*/;
	virtual UINT GetClassStyle() const override;

	/**
	 * 窗口初始化函数
	 * @return void	无返回值
	 */
	virtual void InitWindow() override;

	/**
	 * 拦截并处理底层窗体消息
	 * @param[in] uMsg 消息类型
	 * @param[in] wParam 附加参数
	 * @param[in] lParam 附加参数
	 * @return LRESULT 处理结果
	 */
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	/**
	 * 设置窗体的显示时长
	 * @param[in] duration 持续时间，单位毫秒
	 * @return void	无返回值
	 */
	void SetDuration(int duration);

private:
	/**
	 * 处理所有控件单击消息
	 * @param[in] msg 消息的相关信息
	 * @return bool true 继续传递控件消息，false 停止传递控件消息
	 */
	bool OnClicked(ui::EventArgs* msg);

	/**
	 * 设置提示内容
	 * @param[in] str 提示内容
	 * @return void	无返回值
	 */
	void SetContent(const std::wstring &str);

	public:
		static const LPCTSTR kClassName;
	private:
		ui::RichEdit	*content_;
		ui::Button		*close_button_;
	};
}