#include "Structure/Utils/spk_timer.h"
#include "Structure/Application/spk_application.h"

namespace spk
{
	Timer::Timer(unsigned long p_timer_period)
	{
		_state = State::Stopped;
		_timerDuration = p_timer_period;
	}

	void Timer::setDuration(unsigned long p_timer_period)
	{
		_state = State::Stopped;
		_timerDuration = p_timer_period;
	}

	void Timer::start()
	{
		_startingTime = spk::Application::instance()->time();
		_endingTime = _startingTime + _timerDuration;
		_state = State::Running;
	}

	void Timer::stop()
	{
		_state = State::Stopped;
	}

	Timer::State Timer::state()
	{
		if (_state == State::Running && _endingTime <= spk::Application::instance()->time())
		{
			_state = State::Timeout;
		}

		return (_state);
	}

	bool Timer::isRunning()
	{
		Timer::State p_state = state();

		if (p_state == Timer::State::Running)
			return (true);
		return (false);
	}

	unsigned long Timer::elapsedTime()
	{
		return (spk::Application::instance()->time() - _startingTime);
	}

	float Timer::percent()
	{
		return (static_cast<float>(elapsedTime()) / _timerDuration);
	}
}