#pragma once

#include <mutex>
#include <deque>

namespace spk
{
	template<typename T>
	class LockedQueue
	{
	protected:
		std::mutex muxQueue;
		std::deque<T> _content;
		std::condition_variable cvBlocking;
		std::mutex muxBlocking;

	public:
		LockedQueue() = default;

		LockedQueue(const LockedQueue<T>&) = delete;

		virtual ~LockedQueue() { clear(); }

		const T& operator [] (const size_t& p_index) const
		{
			return (_content[p_index]);
		}

		const T& front()
		{
			std::scoped_lock lock(muxQueue);
			return _content.front();
		}

		const T& back()
		{
			std::scoped_lock lock(muxQueue);
			return _content.back();
		}

		T pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(_content.front());
			_content.pop_front();
			return t;
		}

		T pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(_content.back());
			_content.pop_back();
			return t;
		}

		void push_back(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			_content.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		void push_front(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			_content.emplace_front(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		bool empty()
		{
			return _content.empty();
		}

		size_t size()
		{
			return _content.size();
		}

		void clear()
		{
			std::scoped_lock lock(muxQueue);
			_content.clear();
		}

		void wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.wait(ul);
			}
		}
	};
}