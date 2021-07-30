#pragma once

#include <memory>

namespace dui {
namespace common {

enum class RefCountReleaseStatus { kDroppedLastRef, kOtherRefsRemained };

class RefCountInterface {
protected:
	virtual ~RefCountInterface() = default;

public:
	virtual void AddRef() const = 0;

	virtual RefCountReleaseStatus Release() const = 0;

	virtual bool HasOneRef() const = 0;
};

template<class T>
class dui_refptr {
public:
	dui_refptr() : ptr_(nullptr) {}

	dui_refptr(T* p) : ptr_(p) { if (ptr_) ptr_->AddRef(); }

	template<typename U>
	dui_refptr(U* p) : ptr_(p) { if (ptr_) ptr_->AddRef(); }

	dui_refptr(const dui_refptr<T>& r) : dui_refptr(r.get()) {}

	template <typename U>
	dui_refptr(const dui_refptr<U>& r) : dui_refptr(r.get()) {}

	dui_refptr(dui_refptr<T>&& r) : ptr_(r.move()) {}

	template <typename U>
	dui_refptr(dui_refptr<U>&& r) : ptr_(r.move()) {}

	~dui_refptr() { reset(); }

	T* get() const { return ptr_; }

	// Allow dui_refptr<C> to be used in boolean expression
	// and comparison operations.
	operator bool() const { return (ptr_ != nullptr); }

	T* operator->() const { return ptr_; }

	// Returns the (possibly null) raw pointer, and makes the dui_refptr hold a
	// null pointer, all without touching the reference count of the underlying
	// pointed-to object. The object is still reference counted, and the caller of
	// move() is now the proud owner of one reference, so it is responsible for
	// calling Release() once on the object when no longer using it.
	T* move() {
		T* retVal = ptr_;
		ptr_ = nullptr;
		return retVal;
	}

	dui_refptr<T>& operator=(T* p) {
		if (ptr_ == p) return *this;

		if (p) p->AddRef();
		if (ptr_) ptr_->Release();
		ptr_ = p;
		return *this;
	}

	dui_refptr<T>& operator=(const dui_refptr<T>& r) {
		return *this = r.ptr_;
	}

	dui_refptr<T>& operator=(dui_refptr<T>&& r) {
		dui_refptr<T>(std::move(r)).swap(*this);
		return *this;
	}

	template <typename U>
	dui_refptr<T>& operator=(dui_refptr<U>&& r) {
		dui_refptr<T>(std::move(r)).swap(*this);
		return *this;
	}

	// For working with std::find()
	bool operator==(const dui_refptr<T>& r) const { return ptr_ == r.ptr_; }

	// For working with std::set
	bool operator<(const dui_refptr<T>& r) const { return ptr_ < r.ptr_; }

	void swap(T** pp) {
		T* p = ptr_;
		ptr_ = *pp;
		*pp = p;
	}

	void swap(dui_refptr<T>& r) { swap(&r.ptr_); }

	void reset() {
		if (ptr_) {
			ptr_->Release();
			ptr_ = nullptr;
		}
	}

protected:
	T* ptr_;
};

} // namespace common
} // namespace dui
