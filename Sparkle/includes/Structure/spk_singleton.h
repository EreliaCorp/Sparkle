#pragma once

namespace spk
{
	template <typename TType>
	class Singleton
	{
	protected:
		static inline TType* _instance = nullptr;

	public:
		template <typename... Args>
		static TType* instanciate(Args&&... p_args)
		{
			if (_instance == nullptr)
			{
				_instance = new TType(std::forward<Args>(p_args)...);
			}
			return (_instance);
		}

		static TType* instance()
		{
			return (_instance);
		}

		static void release()
		{
			if (_instance != nullptr)
			{
				delete _instance;
				_instance = nullptr;
			}
		}
	};
}