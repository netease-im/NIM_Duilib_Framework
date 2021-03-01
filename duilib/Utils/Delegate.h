#ifndef UI_UTILS_DELEGATE_H_
#define UI_UTILS_DELEGATE_H_

#pragma once

#include "Core/Define.h"

namespace ui 
{

typedef std::function<bool(ui::EventArgs*)> EventCallback;

class CEventSource : public std::vector<EventCallback>
{
public:
	CEventSource& operator += (const EventCallback& item) 
	{
		push_back(item);
		return *this;
	}

	bool operator() (ui::EventArgs* param) const 
	{
		for (auto it = this->begin(); it != this->end(); it++) {
			if(!(*it)(param)) return false;
		}
		return true;
	}

};

typedef std::map<EventType, CEventSource> EventMap;

}

#endif // UI_UTILS_DELEGATE_H_