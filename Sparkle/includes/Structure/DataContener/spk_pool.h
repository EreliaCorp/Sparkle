#pragma once

#include <shared_mutex>
#include <deque>

namespace spk
{
	template <typename TElement>
	class Pool
	{
	public:
		class Object
		{
		private:
			Pool<TElement>& _pool;
			TElement* _element;

		public:
			template <typename... Args>
			Object(Pool<TElement>& p_pool, Args&&... p_args) : _pool(p_pool), _element(p_pool._obtain(std::forward<Args>(p_args)...)) {}
			~Object() { _pool._release(_element); }
			TElement* operator->() const { return _element; }
			TElement& operator*() const { return *_element; }
		};

	private:
		std::shared_mutex _mutex;
		std::deque<TElement*> _content;

		template <typename... Args>
		TElement* _obtain(Args&&... p_args)
		{
			std::lock_guard<std::shared_mutex> lock(_mutex);

			TElement* result = nullptr;

			if (_content.empty() == false)
			{
				result = _content.back();
				*result = TElement(std::forward<Args>(p_args)...);
				_content.pop_back();
			}
			else
			{
				result = new TElement(std::forward<Args>(p_args)...);
			}

			return result;
		}

		void _release(TElement* p_newContent)
		{
			std::lock_guard<std::shared_mutex> lock(_mutex);
			_content.push_back(p_newContent);
		}

	public:
		Pool() {}

		Pool(const Pool&) = delete;
		Pool& operator=(const Pool&) = delete;

		Pool(Pool&&) = delete;
		Pool& operator=(Pool&&) = delete;

		size_t size() const
		{
			std::lock_guard<std::shared_mutex> lock(_mutex);
			return _content.size();
		}

		template <typename... Args>
		void reserve(size_t p_nbReservedObject, Args&&... p_args)
		{
			std::lock_guard<std::shared_mutex> lock(_mutex);

			size_t currentSize = _content.size();
			_content.resize(currentSize + p_nbReservedObject);

			for (size_t i = currentSize; i < _content.size(); i++)
			{
				_content[i] = new TElement(std::forward<Args>(p_args)...);
			}
		}

		void clear()
		{
			std::lock_guard<std::shared_mutex> lock(_mutex);

			for (auto element : _content)
			{
				delete element;
			}

			_content.clear();
		}

		template <typename... Args>
		Object obtain(Args&&... p_args)
		{
			return (Object(*this, std::forward<Args>(p_args)...));
		}
	};
}