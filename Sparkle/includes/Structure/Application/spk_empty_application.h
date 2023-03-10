#pragma once

#include <Windows.h>
#include <vector>

#include "spk_application_imodule.h"

namespace spk
{

	class EmptyApplication
	{
	private:
		bool _running = false;
		static inline unsigned long long _time;

		std::vector<IModule*> _modules;

		void _updateTime()
		{
			FILETIME ft;
			GetSystemTimeAsFileTime(&ft);

			_time = ((((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime) - 116444736000000000ULL) / 10000;
		}

		void _executeModules()
		{
			for (size_t i = 0; i < _modules.size(); i++)
			{
				_modules[i]->execute();
			}
		}

	public:
		EmptyApplication()
		{

		}

		~EmptyApplication()
		{
			for (size_t i = 0; i < _modules.size(); i++)
			{
				delete _modules[i];
			}
			_modules.clear();
		}

		template <typename TModuleType, typename... Args>
		TModuleType* attach(Args&&... p_args)
		{
			TModuleType* result = new TModuleType(std::forward<Args>(p_args)...);
			_modules.push_back(result);

			return (result);
		}

		int run()
		{
			_running = true;

			while (_running == true)
			{
				_updateTime();
				_executeModules();
			}

			return (0);
		}

		static unsigned long long time() { return (_time); }
	};
}