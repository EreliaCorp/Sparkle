#pragma once

#include <vector>
#include <functional>

#include "spk_locked_queue.h"
#include "spk_thread.h"

namespace spk
{
	class Worker
	{
	private:
		bool _execute = true;
		const std::function<void()> _threadExecutiveFunct = [&]() {
			while (_execute == true)
			{
				if (_jobQueue.empty() == false)
				{
					auto&& tmp = _jobQueue.pop_front();
					tmp();
				}
			}
		};
		spk::Thread* _thread;
		spk::LockedQueue<std::function<void()>> _jobQueue;

	public:
		Worker(std::string p_workerName)
		{
			_thread = new spk::Thread(spk::Thread::LaunchMethod::Delayed, p_workerName, _threadExecutiveFunct);
		}

		~Worker()
		{
			stop();
			delete _thread;
		}

		void start()
		{
			_execute = true;
			_thread->start();
		}

		void stop()
		{
			_execute = false;
			_thread->join();
		}

		template <typename Funct, typename... Args>
		void addJob(Funct&& p_funct, Args&&... p_args)
		{
			_jobQueue.push_back(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...));
		}
	};

	class JobPool
	{
	private:
		std::vector<std::function<void()>> _jobs;

	public:
		JobPool()
		{

		}

		void execute()
		{
			for (auto& tmp : _jobs)
			{
				tmp();
			}
		}

		template <typename Funct, typename... Args>
		void addJob(Funct&& p_funct, Args&&... p_args)
		{
			spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl;
			_jobs.push_back(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...));
		}
	};

	class PersistentTaskWorker
	{
	private:
		const std::function<void()> _threadExecutiveFunct = [&]() {
			while (_execute == true)
			{
				_jobPool.execute();
			}
		};
		bool _execute = true;
		spk::Thread* _thread;
		JobPool _jobPool;

	public:
		PersistentTaskWorker(std::string p_workerName)
		{
		spk::cout << "Creating PersistantTaskWorker with name " << p_workerName << " with size : " << p_workerName.size() << std::endl;
			_thread = new spk::Thread(spk::Thread::LaunchMethod::Delayed, p_workerName, _threadExecutiveFunct);
		}

		~PersistentTaskWorker()
		{
			stop();
			delete _thread;
		}

		void start()
		{
			_execute = true;
			_thread->start();
		}

		void stop()
		{
			_execute = false;
		}

		void join()
		{
			_thread->join();
		}

		template <typename Funct, typename... Args>
		void addJob(Funct&& p_funct, Args&&... p_args)
		{
			spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl;
			_jobPool.addJob(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...);
		}
	};

	class WorkerPool
	{
	private:
		bool _execute = true;
		const std::function<void()> _threadExecutiveFunct = [&]() {
			while (_execute == true)
			{
				if (_jobQueue.empty() == false)
				{
					auto&& tmp = _jobQueue.pop_front();
					tmp();
				}
			}
		};
		std::vector<spk::Thread*> _threads;
		spk::LockedQueue<std::function<void()>> _jobQueue;

	public:
		WorkerPool(std::string p_poolName, size_t p_nbWorker = 0)
		{
			if (p_nbWorker == 0)
				p_nbWorker = std::thread::hardware_concurrency() - 1;

			for (size_t i = 0; i < p_nbWorker; i++)
			{
				spk::Thread* newThread = new spk::Thread(spk::Thread::LaunchMethod::Delayed, p_poolName + " " + std::to_string(i), _threadExecutiveFunct);
				_threads.push_back(newThread);
			}
		}

		~WorkerPool()
		{
			stop();
			for (size_t i = 0; i < _threads.size(); i++)
			{
				delete _threads[i];
			}
		}

		void start()
		{
			_execute = true;
			for (size_t i = 0; i < _threads.size(); i++)
			{
				_threads[i]->start();
			}
		}

		void stop()
		{
			_execute = false;
			for (size_t i = 0; i < _threads.size(); i++)
			{
				_threads[i]->join();
			}
		}

		template <typename Funct, typename... Args>
		void addJob(Funct&& p_funct, Args&&... p_args)
		{
			_jobQueue.push_back(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...));
		}
	};
}