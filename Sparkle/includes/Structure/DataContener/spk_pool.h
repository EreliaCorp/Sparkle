#pragma once

#include <shared_mutex>
#include <mutex>
#include <deque>

#include "Structure/Utils/spk_iostream.h"

namespace spk
{
	template <typename TElement>
	class Pool
	{
	private:
		std::recursive_mutex _mutex;
		std::vector<TElement*> _content;

	public:
		Pool()
		{

		}

		size_t size()
		{
			return (_content.size());
		}

		template <typename... Args>
		void reserve(size_t p_nbReservedObject, Args&&... p_args)
		{
			_mutex.lock();

			_content.resize(p_nbReservedObject);

			for (size_t i = 0; i < p_nbReservedObject; i++)
			{
				_content[i] = new TElement(std::forward<Args>(p_args)...);
			}

			_mutex.unlock();
		}

		template <typename... Args>
		TElement* obtain(Args&&... p_args)
		{
			TElement* result = nullptr;

			if (_content.size() == 0)
			{
				result = new TElement(std::forward<Args>(p_args)...);
			}
			else
			{
				_mutex.lock();

				TElement* result = _content.back();

				*result = TElement(std::forward<Args>(p_args)...);

				_content.pop_back();

				_mutex.unlock();
			}

			return (result);
		}

		void release(TElement* p_newContent)
		{
			_mutex.lock();
			_content.push_back(p_newContent);
			_mutex.unlock();
		}
	};
}