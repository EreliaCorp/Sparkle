#pragma once

#include <map>
#include <Windows.h>
#include "Structure/spk_singleton.h"
#include "Structure/spk_timer.h"
#include "Structure/spk_workers.h"

namespace spk
{
	class Application
	{
	public:
		class Module
		{
		private:

		public:
			virtual void update() = 0;
		};

	private:
		int _errorCode;

		bool _running;

		spk::JobPool _jobPool;
		std::map<std::string, spk::PersistentTaskWorker*> _taskWorkers;

		std::vector<Module*> _modules;

		void _startWorker()
		{
			for (auto& tmp : _taskWorkers)
			{
				tmp.second->start();
			}
		}

		void _stopWorker()
		{
			for (auto& tmp : _taskWorkers)
			{
				tmp.second->stop();
			}
			for (auto& tmp : _taskWorkers)
			{
				tmp.second->join();
			}
		}

		void _runMainThread()
		{
			spk::cout.setPrefix("RenderThread");
			while (_running == true)
			{
				_jobPool.execute();
			}
		}

	protected:
		spk::PersistentTaskWorker* _getWorker(std::string p_workerName)
		{
			if (_taskWorkers.count(p_workerName) == 0)
				_taskWorkers[p_workerName] = new spk::PersistentTaskWorker(p_workerName);

			return (_taskWorkers[p_workerName]);
		}

		void _addJob(std::function<void()> p_funct)
		{
			_jobPool.addJob(p_funct);
		}

		void _addJob(std::string p_workerName, std::function<void()> p_funct)
		{
			_getWorker(p_workerName)->addJob(p_funct);
		}

	public:
		Application() :
			_errorCode(0),
			_running(false)
		{

		}

		~Application()
		{
			for (size_t i = 0; i < _modules.size(); i++)
			{
				delete _modules[i];
			}
		}

		int run()
		{
			_errorCode = 0;
			_running = true;

			_startWorker();

			_runMainThread();

			_stopWorker();

			return (_errorCode);
		}

		void quit(int p_errorCode)
		{
			_errorCode = p_errorCode;
			_running = false;
		}
	};
}