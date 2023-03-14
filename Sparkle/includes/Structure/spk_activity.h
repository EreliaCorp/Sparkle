#pragma once

namespace spk
{
	class Activity
	{
	public:
		enum class Status
		{
			OnEnter,
			Running,
			OnExit,
			Complete
		};
		Status _state;

	protected:
		virtual void _onEnter() = 0;
		virtual void _execute() = 0;
		virtual void _onExit() = 0;

		void _moveNextStep()
		{
			switch (_state)
			{
			case Status::OnEnter:
				_state = Status::Running;
				break;
			case Status::Running:
				_state = Status::OnExit;
				break;
			case Status::OnExit:
				_state = Status::Complete;
				break;
			}
		}
		void _moveNextStepAndExecute()
		{
			_moveNextStep();
			update();
		}
	public:
		Activity()
		{
			_state = Status::Complete;
		}
		virtual ~Activity()
		{

		}
		const bool& isCompleted() const { return (_state == Status::Complete); }
		void start()
		{
			_state = Status::OnEnter;
		}
		void update()
		{
			switch (_state)
			{
			case Status::OnEnter:
				_onEnter();
				break;
			case Status::Running:
				_execute();
				break;
			case Status::OnExit:
				_onExit();
				break;
			}
		}
	};
}