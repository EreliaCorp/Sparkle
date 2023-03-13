#pragma once

#include <iostream>
#include <stdint.h>
#include <map>
#include <string>
#include "spk_input_status.h"
#include "Structure/Mathematic/spk_vector2.h"
#include "Structure/Utils/spk_singleton.h"

namespace spk
{
	class Mouse : public spk::Singleton<Mouse>
	{
	public:
		enum Button
		{
			Right = 0,
			Left = 1,
			Center = 2
		};

	private:
		static const size_t C_NB_BUTTON = 3;

		spk::InputStatus _buttons[C_NB_BUTTON];
		spk::Vector2Int _pos;
		float _wheel;

		void _updateState()
		{
			_wheel = 0;
			for (size_t i = 0; i < C_NB_BUTTON; i++)
			{
				if (_buttons[i] == spk::InputStatus::Pressed)
				{
					_buttons[i] = spk::InputStatus::Down;
				}
				else if (_buttons[i] == spk::InputStatus::Released)
				{
					_buttons[i] = spk::InputStatus::Up;
				}
			}
		}

		Mouse()
		{
			_wheel = 0;
			for (size_t i = 0; i < C_NB_BUTTON; i++)
				_buttons[i] = spk::InputStatus::Up;
		}

	public:
		
		float wheel() const
		{
			return (_wheel);
		}

		const spk::Vector2Int& pos() const
		{
			return (_pos);
		}

		const spk::InputStatus& getButton(Mouse::Button p_button) const
		{
			return (_buttons[static_cast<size_t>(p_button)]);
		}

		friend std::ostream& operator<<(std::ostream& p_os, const Mouse::Button& p_button)
		{
			switch (p_button)
			{
			case (Mouse::Button::Center):
			{
				p_os << ("Center");
				break;
			}
			case (Mouse::Button::Right):
			{
				p_os << ("Right");
				break;
			}
			case (Mouse::Button::Left):
			{
				p_os << ("Left");
				break;
			}
			default:
			{
				p_os << ("Unknow");
				break;
			}
			}

			return p_os;
		}
	};
}