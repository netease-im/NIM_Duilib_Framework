// The LazyInstance<Type> class manages a single instance of Type,
// which will be lazily created on the first time it's accessed

#ifndef BASE_MEMORY_LAZY_INSTANCE_H_
#define BASE_MEMORY_LAZY_INSTANCE_H_

#include "base/base_config.h"
#include "base/third_party/chrome/atomicops.h"
#include "base/thread/thread.h"
#include "base/util/at_exit.h"
//#include "base/log/log.h"

namespace nbase
{

template<typename Type>
class LazyInstance
{	
public:

	LazyInstance() : instance_(NULL)
	{
		base::subtle::NoBarrier_Store(&state_, kNone);
	}

	~LazyInstance()
	{
		// |instance_| should be deleted by |OnExit|
		//DCHECK(instance_ == 0);
	}

	Type& Get()
	{
		return *Pointer();
	}

	Type* Pointer()
	{
		using namespace base::subtle;

		if (Acquire_Load(&state_) != kCreated)
		{
			Atomic32 state =
				NoBarrier_CompareAndSwap(&state_, kNone, kCreating);
			if (state == kNone)
			{
				// we take the chance to create the instance
				instance_ = reinterpret_cast<AtomicWord>(new Type());
				AtExitManager::RegisterCallback(OnExit, this);
				Release_Store(&state_, kCreated);
			}
			else if (state != kCreated)
			{
				// wait, util another thread created the instance
				while (Acquire_Load(&state_) != kCreated)
					Thread::YieldThread();
			}
		}

		return reinterpret_cast<Type *>(instance_);
	}

	bool operator ==(Type *object) const
	{
		switch (base::subtle::NoBarrier_Load(&state_))
		{
		case kNone:
			return object == NULL;
		case kCreating:
		case kCreated:
			return instance_ == object;
		default:
			return false;
		}
	}

private:
	enum
	{
		kNone = 0,
		kCreating,
		kCreated,
		kDeleting,
	};

	static void OnExit(void *lazy_instance)
	{
		LazyInstance<Type>* me =
			reinterpret_cast<LazyInstance<Type>*>(lazy_instance);
		delete reinterpret_cast<Type*>(me->instance_);
		base::subtle::NoBarrier_Store(&me->instance_, 0);
	}

	base::subtle::Atomic32 state_;
	base::subtle::AtomicWord instance_;

	DISALLOW_COPY_AND_ASSIGN(LazyInstance);
};

}

#endif // BASE_MEMORY_LAZY_INSTANCE_H_
