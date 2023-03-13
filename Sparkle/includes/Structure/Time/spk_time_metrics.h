#pragma once

#include <chrono>
#include "Structure/Utils/spk_singleton.h"
#include "Structure/Time/spk_timer.h"

namespace spk
{
	class TimeMetrics : public spk::Singleton<TimeMetrics>
	{
	private:
		long long _time;
		long long _deltaTime;

		spk::Timer _timer;
		long long _actualRenderTicks = 0;
		long long _actualUpdateTicks = 0;

		float _IPSRatio;
		long long _renderIPS = 0;
		long long _updateIPS = 0;

		void _updateMetrics()
		{
			auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch();
			long long tmpTime = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();

			_deltaTime = tmpTime - _time;
			_time = tmpTime;
		
			if (_timer.isRunning() == false)
			{
				_renderIPS = static_cast<long long>(_actualRenderTicks * _IPSRatio);
				_updateIPS = static_cast<long long>(_actualUpdateTicks * _IPSRatio);
			}
		}

		TimeMetrics(long long p_tickUpdateDelay = 1000) :
			_timer(p_tickUpdateDelay),
			_IPSRatio(1000.0f / p_tickUpdateDelay),
			_deltaTime(0ll),
			_time(0ll)
		{

		}

	public:
		long long time() { return (_time); }
		long long deltaTime() { return (_deltaTime); }

		long long renderIPS() { return (_renderIPS); }
		long long updateIPS() {return (_updateIPS); }
	};
}