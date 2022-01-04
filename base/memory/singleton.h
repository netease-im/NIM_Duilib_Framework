#ifndef BASE_MEMORY_SINGLETON_H_
#define BASE_MEMORY_SINGLETON_H_

#include <memory>
#include <mutex>
#include "../util/at_exit.h"
namespace nbase
{
//饿汉模式
#define SINGLETON_DEFINE(TypeName)				\
static TypeName* GetInstance()					\
{												\
	static TypeName type_instance;				\
	return &type_instance;						\
}												\
												\
TypeName(const TypeName&) = delete;				\
TypeName& operator=(const TypeName&) = delete

// 懒汉模式
//release_atexitmanager 是否由atexitmanager来释放
template <typename TSingleton,bool release_atexitmanager = true>
class Singleton
{
	/* 如果TSingleton的构造函数是private / protected需要在类定义时引入此宏, 如下所示
	class Test : public Singleton<Test>
	{
		SingletonHideConstructor(Test)
	private:
		Test() = default;
		~Test() = default;
	}
	*/
#define SingletonHideConstructor(TSingleton) \
	friend Singleton<typename TSingleton>::TSingletonDefaultDelete;\
	template<class _Ty,\
	class... _Types> inline\
	friend	typename std::enable_if<!std::is_array<_Ty>::value,\
		std::unique_ptr<_Ty> >::type std::make_unique(_Types&&... _Args);
private:
	friend TSingleton;
	using TSingletonDefaultDelete = std::default_delete<TSingleton>;
	using TSingletonPtr = std::unique_ptr<TSingleton, TSingletonDefaultDelete>;
private:
	Singleton(void) = default;
	virtual ~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
public:
	static const TSingletonPtr& GetInstance()
	{
		static TSingletonPtr instance = nullptr;
		static std::once_flag oc;
		std::call_once(oc, [&] {
			instance = std::make_unique<TSingleton>();
			if (release_atexitmanager)
			{
				AtExitManager::RegisterCallback([&](void*) {
					instance.reset(nullptr);
#if _MSC_VER > 1900
					oc._Opaque = 0;
#else
					oc._Flag = 0;
#endif

				}, nullptr);
			}
		});
		return instance;
	}
};

}

#endif // BASE_MEMORY_SINGLETON_H_
