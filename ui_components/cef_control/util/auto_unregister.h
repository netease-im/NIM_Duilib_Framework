#pragma once
#include <unordered_map>
typedef std::function<void()> UnregisterCallback;

namespace nim_comp {

class AutoUnregister 
{
public:
	AutoUnregister()
	{

	}

	void Add(const UnregisterCallback& cb)
	{
		cb_list_.emplace_back(cb);
	}
	void Add(UnregisterCallback&& cb)
	{
		cb_list_.emplace_back(std::forward<UnregisterCallback>(cb));
	}

	~AutoUnregister()
	{
		for (auto iter = cb_list_.begin(); iter != cb_list_.end(); iter ++)
			(*iter)();
	}

private:
	std::list<UnregisterCallback> cb_list_;
};
template<typename TCallback>
class UnregistedCallbackList : public virtual nbase::SupportWeakCallback
{
public:
	UnregistedCallbackList() {}
	~UnregistedCallbackList(){ element_list_.clear(); };
	void Clear() { element_list_.clear(); }
	UnregisterCallback AddCallback(const TCallback& cb)
	{
		auto new_cb = std::make_shared<TCallback>(cb);
		int cb_id = (int)(new_cb.get());
		element_list_.insert(std::make_pair(cb_id, new_cb));
		return ToWeakCallback([this, cb_id]() {
			element_list_.erase(cb_id);
		});
	}
	UnregisterCallback AddCallback(TCallback&& cb)
	{
		auto new_cb = std::make_shared<TCallback>(std::forward<TCallback>(cb));
		int cb_id = (int)(new_cb.get());
		element_list_.insert(std::make_pair(cb_id, new_cb));
		return ToWeakCallback([this, cb_id]() {
			element_list_.erase(cb_id);
		});
	}
	template<typename... TParams>
	void operator ()(const TParams&... params)
	{
		for (auto& it : element_list_)
		{
			(*it.second)(params...);
		}
	}	
private:
	std::unordered_map<int, std::shared_ptr<TCallback>> element_list_;
};

}
