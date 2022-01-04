#pragma once
#include "window_ex.h"

namespace nim_comp
{
//map<窗口类名，map<窗口id，窗口指针>>, 如果同一类只有一个窗口，使用类名作为id
typedef std::map<std::wstring, std::map<std::wstring, WindowEx*>> WindowsMap;
typedef std::list<WindowEx *> WindowList;

/** @class WindowsManager
 * @brief 所有窗体的控制接
 * @copyright (c) 2015, NetEase Inc. All rights reserved
 * @author Redrain
 * @date 2015/9/16
 */
class WindowsManager
{
public:
	SINGLETON_DEFINE(WindowsManager);
	WindowsManager();
	virtual ~WindowsManager();

	/**
	 * 根据窗口类名和id注册窗口
	 * @param[in] wnd_class_name 窗口类名
	 * @param[in] wnd_id 窗口id
	 * @param[in] wnd 窗口指针
	 * @return bool true 成功，false 失败
	 */
	bool RegisterWindow(const std::wstring wnd_class_name, const std::wstring wnd_id, WindowEx *wnd);

	/**
	 * 根据窗口类名和id注销窗口
	 * @param[in] wnd_class_name 窗口类名
	 * @param[in] wnd_id 窗口id
	 * @param[in] wnd 窗口指针
	 * @return void 无返回值
	 */
	void UnRegisterWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id, WindowEx *wnd);

	/**
	 * 根据窗口类名和id获取窗口
	 * @param[in] wnd_class_name 窗口类名
	 * @param[in] wnd_id 窗口id
	 * @return WindowEx* 窗口指针
	 */
	WindowEx* GetWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id);

	/**
	 * 获取所有窗口获取窗口
	 * @return WindowList 窗口列表
	 */
	WindowList GetAllWindows();

	/**
	 * 根据窗口类名获取对应的所有窗口
	 * @param[in] classname 窗口类名
	 * @return WindowList 窗口列表
	 */
	WindowList GetWindowsByClassName(LPCTSTR classname);

	/**
	 * 销毁所有窗口
	 * @return void 无返回值
	 */
	void DestroyAllWindows();

	/**
	 * 设置禁止窗口创建
	 * @param[in] stop 是否禁止窗口创建
	 * @return void 无返回值
	 */
	void SetStopRegister(bool stop = true) { stop_register_ = stop; }

	/**
	 * 是否禁止窗口创建
	 * @return bool true 禁止，false 不禁止
	 */
	bool IsStopRegister() { return stop_register_; }

	/**
	 * 根据窗口id创建一个唯一存在的窗口
	 * @param[in] window_id 窗口id
	 * @return WindowType* 窗口指针
	 */
	template<typename WindowType, typename... TInstanceParams>
	static WindowType* SingletonShow(const std::wstring& window_id, const TInstanceParams&... params)
	{
		WindowType *window = (WindowType*)(WindowsManager::GetInstance()->GetWindow(WindowType::kClassName, window_id));
		if (!window)
		{
			window = new WindowType(params...);
			window->Create(NULL, WindowType::kClassName, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
			window->CenterWindow();
			window->ShowWindow();
		}
		else
		{
			window->ActiveWindow();
		}

		return window;
	}

private:
	WindowsMap					windows_map_;	//所有窗口
	std::string					user_id_;
	bool						stop_register_;	//禁止窗口创建
};
}