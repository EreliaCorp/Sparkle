#pragma once

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
		unsigned long _startingTime;
		unsigned long _endingTime;
		unsigned long _timerDuration;

	public:
		Timer(unsigned long p_timer_period);
		void setDuration(unsigned long p_timer_period);
		unsigned long elapsedTime();
		unsigned long duration() { return (_timerDuration); }
		float percent();
		void start();
		void stop();
		State state();
		bool isRunning();

	};
}