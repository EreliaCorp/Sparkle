#pragma once

#include <map>
#include <Windows.h>
#include "Structure/Utils/spk_singleton.h"
#include "Structure/Utils/spk_timer.h"
#include "Structure/Threading/spk_workers.h"

namespace spk
{
	class Application : public spk::Singleton<Application>
	{
		friend class spk::Singleton<Application>;
	public:
		enum class Configuration
		{
			Graphical,
			Console
		};

	private:
		int _errorCode;
		bool _running;
		unsigned long _time;
		size_t _renderTicks;
		size_t _updateTicks;
		spk::Timer _timeTimer = spk::Timer(1000);

		spk::JobPool _jobPool;
		std::map<std::string, spk::PersistentTaskWorker*> _taskWorkers;

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

		Application(Configuration p_configuration)
		{
			switch (p_configuration)
			{
			case Configuration::Console:
			{

				break;
			}
			case Configuration::Graphical:
			{
				_addJob([&]() {
						_time = static_cast<unsigned long>(GetTickCount64());
						_renderTicks++;
					});
				_addJob([&]() {
					if (_timeTimer.isRunning() == false)
					{
						_timeTimer.start();

						spk::cout << "Render : " << _renderTicks << std::endl;
						spk::cout << "Update : " << _updateTicks << std::endl;
						_updateTicks = 0;
						_renderTicks = 0;

					}
					});

				_addJob("UpdaterThread", [&]() {
					_updateTicks++;
					});

				break;
			}
			}
		}

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

	public:
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

		unsigned long time() { return (_time); }
	};
}