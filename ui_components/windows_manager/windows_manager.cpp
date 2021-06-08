#include "stdafx.h"
#include "windows_manager.h"

namespace nim_comp
{
WindowsManager::WindowsManager()
{
	stop_register_ = false;
	windows_map_.clear();
}

WindowsManager::~WindowsManager()
{
	windows_map_.clear();
}

bool WindowsManager::RegisterWindow(const std::wstring wnd_class_name, const std::wstring wnd_id, WindowEx *wnd)
{
	if (IsStopRegister())
	{
		assert(!stop_register_);
		return false;
	}
	WindowsMap::iterator it = windows_map_.find(wnd_class_name);
	if (it != windows_map_.end())
	{
		std::map<std::wstring, WindowEx*>::iterator it2 = it->second.find(wnd_id);
		it->second[wnd_id] = wnd;
	}
	else
	{
		std::map<std::wstring, WindowEx*> id_win;
		id_win[wnd_id] = wnd;
		windows_map_[wnd_class_name] = id_win;
	}

	ui::GlobalManager::AddPreMessage(wnd);

	return true;
}

void WindowsManager::UnRegisterWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id, WindowEx *wnd)
{
	WindowsMap::iterator it = windows_map_.find(wnd_class_name);
	if (it != windows_map_.end())
	{
		ui::GlobalManager::RemovePreMessage(wnd);

		std::map<std::wstring, WindowEx*>::iterator it2 = it->second.find(wnd_id);
		if (it2 != it->second.end())
		{
			it->second.erase(it2);
		}
	}
}

WindowList WindowsManager::GetAllWindows()
{
	WindowList list;
	WindowsMap::iterator it = windows_map_.begin();
	for (; it != windows_map_.end(); ++it)
	{
		std::map<std::wstring, WindowEx*>::iterator it2 = it->second.begin();
		for (; it2 != it->second.end(); ++it2)
		{
			WindowEx *wnd = (WindowEx *)(it2->second);
			if (wnd && ::IsWindow(wnd->GetHWND()))
			{
				list.push_back(wnd);
			}
		}
	}
	return list;
}

WindowEx* WindowsManager::GetWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id)
{
	WindowsMap::iterator it = windows_map_.find(wnd_class_name);
	if (it != windows_map_.end())
	{
		std::map<std::wstring, WindowEx*>::iterator it2 = it->second.find(wnd_id);
		if (it2 != it->second.end())
		{
			WindowEx* wnd = (WindowEx*)(it2->second);
			if (wnd && ::IsWindow(wnd->GetHWND()))
			{
				return wnd;
			}
		}
	}
	return NULL;
}

WindowList WindowsManager::GetWindowsByClassName(LPCTSTR classname)
{
	WindowList wnd_list;
	WindowsMap::iterator it = windows_map_.find(classname);
	if (it != windows_map_.end())
	{
		std::map<std::wstring, WindowEx*>::iterator it2 = it->second.begin();
		for (; it2 != it->second.end(); it2++)
		{
			WindowEx* wnd = (WindowEx*)(it2->second);
			if (wnd && ::IsWindow(wnd->GetHWND()))
			{
				wnd_list.push_back(wnd);
			}
		}
	}
	return wnd_list;
}

void WindowsManager::DestroyAllWindows()
{
	SetStopRegister();

	WindowList lst_wnd = GetAllWindows();
	WindowList::iterator it = lst_wnd.begin();
	for (; it != lst_wnd.end(); ++it)
	{
		WindowEx *wnd = (WindowEx *)*it;
		if (wnd && ::IsWindow(wnd->GetHWND()))
		{
			::DestroyWindow(wnd->GetHWND());
		}
	}
}
}