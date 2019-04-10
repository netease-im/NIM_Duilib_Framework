#pragma once

/** @class DragForm
  * @brief 模仿系统拖拽功能的半透明窗体，因为使用了低级键盘钩子所以一些安全软件会拦截
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Redrain
  * @date 2019/3/20
  */
class BitmapControl;
class DragForm : public ui::WindowImplBase
{
public:
	/**
	* 低级键盘钩子的回调函数
	* @param[in] nCode 操作码
	* @param[in] wParam 附加参数
	* @param[in] lParam 附加参数
	* @return LRESULT	操作结果
	*/
	static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

	/**
	* 创建自定义拖拽窗口
	* @param[in] bitmap 拖拽效果位图
	* @param[in] pt_offset 拖拽图像相对于鼠标的偏移
	* @return void	无返回值
	*/
	static void CreateCustomDragImage(HBITMAP bitmap, POINT pt_offset);

	/**
	* 关闭自定义拖拽窗口
	* @return void	无返回值
	*/
	static void CloseCustomDragImage();

public:	
	// 覆盖虚函数
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual UINT GetClassStyle() const override;

	/**
	* 根据控件类名创建自定义控件
	* @param[in] pstrClass 控件类名
	* @return Control* 创建的控件的指针
	*/
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;

	/**
	* 窗口初始化函数
	* @return void	无返回值
	*/
	virtual void InitWindow() override;

private:
	DragForm();
	~DragForm(){};

	/**
	* 创建一个拖拽效果的窗口
	* @param[in] bitmap 拖拽效果位图对象
	* @param[in] pt_offset 拖拽图像相对于鼠标的偏移
	* @return void	无返回值
	*/
	static DragForm* CreateDragForm(HBITMAP bitmap, POINT pt_offset);

	/**
	* 设置拖拽效果的位图
	* @param[in] bitmap 拖拽效果位图对象
	* @return void	无返回值
	*/
	void SetDragImage(HBITMAP bitmap);
public:
	static const LPCTSTR kClassName;

private:
	static HHOOK	s_mouse_hook;
	static DragForm *s_drag_form;
	static POINT	s_point_offset;
	BitmapControl	*bitmap_control_;
};