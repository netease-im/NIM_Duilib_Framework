#ifndef BASE_FRAMEWORK_OBSERVER_LIST_H_
#define BASE_FRAMEWORK_OBSERVER_LIST_H_

#include <assert.h>
#include <vector>
#include <algorithm>

/*
 * 这个类主要解决一个问题：
 * 当MessagePump在遍历ObserverList的时候，
 * 允许从一个Observer内部调用RemoveObserver来移除自己或者别的Observer而不会导致接下来的遍历崩溃（iterator被erase）
 *
 * Google Chrome的做法是重载iterator，这里采用lazy erase技术。下面是示例代码：
 *
 * ObserverList<Observer> observers;
 * ...
 * observers.AquireLazyErase();		// aquire lazy erase
 * size_t index = 0;
 * for (;;)
 * {
 *     Observer *observer = observers.GetObject(index++);
 *     if (observer == NULL)
 *         break;
 *     // use observer here
 * }
 * observers.ReleaseLazyErase();	// release lazy erase
 * observer.Compact();				// compact the list
 */

class LazyEraser
{
public:
	LazyEraser() : lazy_erase_(0) {}
	/* NOTE: calling 'ApplyLasyErase' increases the reference count, calling 'ReleaseLasyErase' decreases the reference count */
	int AquireLazyErase() { return ++lazy_erase_; }
	int ReleaseLazyErase() { assert(lazy_erase_ > 0); return --lazy_erase_; }
	virtual void Compact() = 0;

protected:
	int lazy_erase_;
};

class AutoLazyEraser
{
public:
	AutoLazyEraser(LazyEraser* lazy_eraser) : ptr_(lazy_eraser)
	{
		if (ptr_)
			ptr_->AquireLazyErase();
	}

	AutoLazyEraser(AutoLazyEraser &lazy_eraser)
	{
		if (lazy_eraser.ptr_)
			lazy_eraser.ptr_->AquireLazyErase();
		ptr_ = lazy_eraser.ptr_;
	}

	void operator=(AutoLazyEraser &lazy_eraser)
	{
		if (lazy_eraser.ptr_)
			lazy_eraser.ptr_->AquireLazyErase();
		ptr_ = lazy_eraser.ptr_;
	}

	~AutoLazyEraser()
	{
		if (ptr_)
		{
			if (ptr_->ReleaseLazyErase() < 1)
				ptr_->Compact();
		}
	}

private:
	LazyEraser *ptr_;
};

template<typename ObserverType>
class ObserverList : public LazyEraser
{
public:
	void AddObserver(ObserverType *observer)
	{
		assert(observer);
		if (std::find(observers_.begin(), observers_.end(), observer) == observers_.end())
			observers_.push_back(observer);
	}

	void RemoveObserver(ObserverType *observer)
	{
		typename std::vector<ObserverType *>::iterator pos = std::find(observers_.begin(), observers_.end(), observer);
		if (pos == observers_.end())
			return;
		/* this method may be called within a traversal of the list */
		if (lazy_erase_ > 0)
			*pos = NULL;
		else
			observers_.erase(pos);
	}

	void Compact()
	{
		if (lazy_erase_ > 0 || observers_.empty())
			return;
		ObserverType **src = &observers_[0];
		ObserverType **target = src;
		ObserverType **end = src + observers_.size();

		/* fast compacting algorithm */
		while (src < end)
		{
			if (*src != NULL)
			{
				if (target != src)
					*target = *src;
				target++;
			}
			src++;
		}

		observers_.resize(target - &observers_[0]);
	}

	ObserverType* GetObserver(size_t index) const
	{
		if (index >= GetObserverCount())
			return NULL;
		return observers_[index];
	}

	size_t GetObserverCount() const { return observers_.size(); };

private:
	std::vector<ObserverType *> observers_;
};

#endif // BASE_FRAMEWORK_OBSERVER_LIST_H_
