#pragma once
namespace shared
{
	class TemplatedObjectFactoryWrapper;
	/*
	类名：TemplatedObjectFactory
	功能：根据注册类型的信息创建一个TOBJFLG的实例，类型TBase应是TOBJFLG的父类
	*/
	template<typename TBase, typename TOBJFLG>
	class TemplatedObjectFactory : public nbase::Singleton<TemplatedObjectFactory<typename TBase, typename TOBJFLG>>
	{
	private:
		using _ParentType = nbase::Singleton<TemplatedObjectFactory<typename TBase, typename TOBJFLG>>;
		using _MyType = TemplatedObjectFactory<typename TBase, typename TOBJFLG>;
		friend class TemplatedObjectFactoryWrapper;
		SingletonHideConstructor(_MyType);
	private:
		TemplatedObjectFactory() = default;
		~TemplatedObjectFactory() = default;
	private:		
		/*
		创建TClass实例的方法, 存在TClass : public TBase的关系
		params TClass的构造参数
		返回TBase类型的指针对像 
		*/
		template<typename TClass, typename... TParam>
		TBase* Create(const TParam&... params)
		{
			return dynamic_cast<TBase*>(new TClass(params...));
		}
		template<typename TClass, typename... TParam>
		void AddCreateFunction(TOBJFLG flg, const TParam&... params)
		{			
			auto it = std::find_if(crate_function_list_.begin(), crate_function_list_.end(), [&](const decltype(*crate_function_list_.begin()) & item){
				return flg == item.first;
			});
			if (it == crate_function_list_.end())
				crate_function_list_.emplace_back(std::make_pair(flg, std::bind(&TemplatedObjectFactory::Create<TClass, TParam...>, this, params...)));
		}
		auto CreateSharedObject(TOBJFLG flg)->std::shared_ptr<TBase>
		{
			auto it = std::find_if(crate_function_list_.begin(), crate_function_list_.end(), [&](const decltype(*crate_function_list_.begin()) & item){
				return flg == item.first;
			});
			if (it != crate_function_list_.end())
				return std::shared_ptr<TBase>((*it).second());
			return nullptr;
		}
		TBase* CreateObject(TOBJFLG flg)
		{
			TBase* ret = nullptr;
			auto it = std::find_if(crate_function_list_.begin(), crate_function_list_.end(), [&](const decltype(*crate_function_list_.begin()) & item){
				return flg == item.first;
			});
			if (it != crate_function_list_.end())
				ret = (*it).second();
			return ret;
		}
		void CreateAllSharedObject(std::list<std::shared_ptr<TBase>>& objects)
		{
			for (auto it : crate_function_list_)
				objects.emplace_back(std::shared_ptr<TBase>(it.second()));		
		}
	private:		
		std::list<std::pair<TOBJFLG, std::function<TBase*()>>> crate_function_list_;
	};	
	class TemplatedObjectFactoryWrapper
	{
	public:
		//注册类型
		template<typename TBase, typename TObject, typename TOBJFLG, typename... TParam>
		static void RegisteredOjbect(const TOBJFLG& flg, const TParam&... params)
		{
			using TDecayType = std::decay<TOBJFLG>::type;
			if (std::is_base_of<TBase, TObject>::value)
			{
				auto&& manager = TemplatedObjectFactory<TBase, TDecayType>::GetInstance();
				if (manager != nullptr)
					manager->AddCreateFunction<TObject>(flg, params...);
			}				
		}
		//创建含引用计数的实例
		template<typename TBase, typename TFLG>
		static auto InstantiateSharedRegisteredOjbect(const TFLG& flag)->std::shared_ptr<TBase>
		{
			using TDecayType = std::decay<TFLG>::type;
			auto&& manager = TemplatedObjectFactory<TBase, TDecayType>::GetInstance();
			if (manager != nullptr)
				return manager->CreateSharedObject(flag);
			return nullptr;
		}
		//创建实例
		template<typename TBase, typename TFLG>
		static auto InstantiateRegisteredOjbect(const TFLG& flag)->TBase*
		{
			using TDecayType = std::decay<TFLG>::type;
			auto&& manager = TemplatedObjectFactory<TBase, TDecayType>::GetInstance();
			if (manager != nullptr)
				return manager->CreateObject(flag);
			return nullptr;
		}
		//创建所有已注册为 TBase 与 TFLG 为标识的的实例
		template<typename TBase, typename TFLG>
		static auto InstantiateAllRegisteredSharedOjbect()->std::list<std::shared_ptr<TBase>>
		{
			using TDecayType = std::decay<TFLG>::type;
			std::list<std::shared_ptr<TBase>> ret;
			auto&& manager = TemplatedObjectFactory<TBase, TDecayType>::GetInstance();
			if (manager != nullptr)
				manager->CreateAllSharedObject(ret);			
			return ret;
		}
	};	
}
