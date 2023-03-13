#pragma once

#include <Windows.h>

namespace spk
{
	class Timer
	{
	public:
		enum class State {
			Stopped,
			Timeout,
			Running
		};

	private:
		State _state;
		long long _startingTime;
		long long _endingTime;
		long long _timerDuration;

	public:
		Timer(long long p_timer_period) :
			_state(State::Stopped),
			_startingTime(0),
			_endingTime(0),
			_timerDuration(p_timer_period)
		{

		}
		void setDuration(long long p_timer_period)
		{
			_state = State::Stopped;
			_timerDuration = p_timer_period;
		}
		long long elapsedTime()
		{
			return (static_cast<long long>(GetTickCount64()) - _startingTime);
		}

		long long duration() { return (_timerDuration); }
		float percent()
		{
			return (static_cast<float>(elapsedTime()) / _timerDuration);
		}
		void start()
		{
			_startingTime = static_cast<long long>(GetTickCount64());
			_endingTime = _startingTime + _timerDuration;
			_state = State::Running;
		}
		void stop()
		{
			_state = State::Stopped;
		}
		State state()
		{
			if (_state == State::Running && _endingTime <= static_cast<long long>(GetTickCount64()))
			{
				_state = State::Timeout;
			}

			return (_state);
		}
		bool isRunning()
		{
			Timer::State p_state = state();

			if (p_state == Timer::State::Running)
				return (true);
			return (false);
		}

	};
}