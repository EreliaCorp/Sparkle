#pragma once

#include <functional>
#include "Structure/spk_timer.h"

#include "spk_input_status.h"
#include "spk_mouse.h"
#include "spk_keyboard.h"

namespace spk
{
	class IInputController
	{
	protected:
		spk::Timer _timer;
		std::function<void()> _funct;

		virtual bool _needExecution() = 0;
		virtual bool _skipCondition() {
			return (true);
		}

	public:
		template <typename Func, typename... Args>
		IInputController(unsigned long p_inputDelay, Func&& p_func, Args&&... p_args) :
			_timer(p_inputDelay),
			_funct(std::bind(std::forward<Func>(p_func), std::forward<Args>(p_args)...))
		{

		}
		virtual void update()
		{
			if (_skipCondition() == true && _timer.isRunning())
				return;

			if (_needExecution() == true) {
				_funct();
				_timer.start();
			}
		}
	};

	class MouseInputController : public IInputController
	{
	private:
		spk::Mouse::Button _button;
		spk::InputStatus _expectedStatus;

		bool _needExecution() {
			return (spk::Mouse::instance()->getButton(_button) == _expectedStatus);
		}

	public:
		template <typename Func, typename... Args>
		MouseInputController(spk::Mouse::Button p_button, spk::InputStatus p_expectedStatus, unsigned long p_inputDelay, Func&& p_func, Args&&... p_args) :
			IInputController(p_inputDelay, std::forward<Func>(p_func), std::forward<Args>(p_args)...),
			_button(p_button),
			_expectedStatus(p_expectedStatus)
		{

		}
	};

	class KeyInputController : public IInputController
	{
	private:
		spk::Keyboard::Key _key;
		spk::InputStatus _expectedStatus;

		bool _needExecution() {
			return (spk::Keyboard::instance()->getKey(_key) == _expectedStatus);
		}

	public:
		template <typename Func, typename... Args>
		KeyInputController(spk::Keyboard::Key p_key, spk::InputStatus p_expectedStatus, unsigned long p_inputDelay, Func&& p_func, Args&&... p_args) :
			IInputController(p_inputDelay, std::forward<Func>(p_func), std::forward<Args>(p_args)...),
			_key(p_key),
			_expectedStatus(p_expectedStatus)
		{

		}
	};

	class EntryInputController : public IInputController
	{
	private:
		unsigned char _lastEntry = '\0';

		bool _needExecution() {
			return (spk::Keyboard::instance()->getEntry() >= ' ');
		}
		bool _skipCondition() {
			return (_lastEntry == spk::Keyboard::instance()->getEntry());
		}

	public:
		template <typename Func, typename... Args>
		EntryInputController(unsigned long p_inputDelay, Func&& p_func, Args&&... p_args) :
			IInputController(p_inputDelay, std::forward<Func>(p_func), std::forward<Args>(p_args)...)
		{

		}
	};
}