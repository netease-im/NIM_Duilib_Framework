#ifndef BASE_UTIL_CALLBACK_H_
#define BASE_UTIL_CALLBACK_H_
#pragma once

#include <memory>
#include <functional>
#include "base/base_export.h"

typedef std::function<void(void)> StdClosure;

namespace nbase
{

class WeakFlag
{

};

template<typename T>
class WeakCallback
{
public:
	WeakCallback(const std::weak_ptr<WeakFlag>& weak_flag, const T& t) :
		weak_flag_(weak_flag),
		t_(t)
	{

	}

	WeakCallback(const std::weak_ptr<WeakFlag>& weak_flag, T&& t) :
		weak_flag_(weak_flag),
		t_(std::move(t))
	{

	}

	template<class WeakType>
	WeakCallback(const WeakType& weak_callback) :
		weak_flag_(weak_callback.weak_flag_),
		t_(weak_callback.t_)
	{

	}

	template<class... Args>
	auto operator ()(Args && ... args) const
#if _MSC_VER > 1900
#else
		->decltype(t_(std::forward<Args>(args)...))
#endif
	{
		if (!weak_flag_.expired()) {
			return t_(std::forward<Args>(args)...);
		}
		return decltype(t_(std::forward<Args>(args)...))();
	}

	bool Expired() const
	{
		return weak_flag_.expired();
	}


	std::weak_ptr<WeakFlag> weak_flag_;
	mutable T t_;
};

class BASE_EXPORT SupportWeakCallback
{
public:
	typedef std::weak_ptr<WeakFlag> _TyWeakFlag;
public:
	virtual ~SupportWeakCallback(){};

	template<typename CallbackType>
	auto ToWeakCallback(const CallbackType& closure)
		->WeakCallback<CallbackType>
	{
		return WeakCallback<CallbackType>(GetWeakFlag(), closure);
	}

	std::weak_ptr<WeakFlag> GetWeakFlag()
	{
		if (m_weakFlag.use_count() == 0) {
			m_weakFlag.reset((WeakFlag*)NULL);
		}
		return m_weakFlag;
	}

private:
	template<typename ReturnValue, typename... Param, typename WeakFlag>
	static std::function<ReturnValue(Param...)> ConvertToWeakCallback(
		const std::function<ReturnValue(Param...)>& callback, std::weak_ptr<WeakFlag> expiredFlag)
	{
		auto weakCallback = [expiredFlag, callback](Param... p) {
			if (!expiredFlag.expired()) {
				return callback(p...);
			}
			return ReturnValue();
		};

		return weakCallback;
	}

protected:
	std::shared_ptr<WeakFlag> m_weakFlag;
};

// WeakCallbackFlag 一般作为类成员变量使用，要继承，可使用不带 Cancel() 函数的 SupportWeakCallback
// 这里禁止继承，主要担心误用。当使用这个类的功能，打包出多个支持 Weak 语义的 Callback 时，一旦错误的调用了 Cancel，
// 将会取消所有 Callback，这种情况可能不是用户希望的。此时，应该使用多个带有 Cancel() 函数的 WeakCallbackFlag 类型的成员变量，
// 每个对应一个 Callback，一一对应的控制每个支持 Weak 语义的 Callback。
class BASE_EXPORT WeakCallbackFlag final : public SupportWeakCallback
{
public:
	void Cancel()
	{
		m_weakFlag.reset();
	}

	bool HasUsed()
	{
		return m_weakFlag.use_count() != 0;
	}
};


// global function 
template<class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
auto Bind(F && f, Args && ... args)
	->decltype(std::bind(f, args...))
{
	return std::bind(f, args...);
}

// const class member function 
template<class R, class C, class... DArgs, class P, class... Args>
auto Bind(R(C::*f)(DArgs...) const, P && p, Args && ... args)
	->WeakCallback<decltype(std::bind(f, p, args...))>
{
	std::weak_ptr<WeakFlag> weak_flag = ((SupportWeakCallback*)p)->GetWeakFlag();
	auto bind_obj = std::bind(f, p, args...);
	static_assert(std::is_base_of<nbase::SupportWeakCallback, C>::value, "nbase::SupportWeakCallback should be base of C");
	WeakCallback<decltype(bind_obj)> weak_callback(weak_flag, std::move(bind_obj));
	return weak_callback;
}

// non-const class member function 
template<class R, class C, class... DArgs, class P, class... Args>
auto Bind(R(C::*f)(DArgs...), P && p, Args && ... args) 
	->WeakCallback<decltype(std::bind(f, p, args...))>
{
	std::weak_ptr<WeakFlag> weak_flag = ((SupportWeakCallback*)p)->GetWeakFlag();
	auto bind_obj = std::bind(f, p, args...);
	static_assert(std::is_base_of<nbase::SupportWeakCallback, C>::value, "nbase::SupportWeakCallback should be base of C");
	WeakCallback<decltype(bind_obj)> weak_callback(weak_flag, std::move(bind_obj));
	return weak_callback;
}

}


#endif  // BASE_UTIL_CALLBACK_H_
