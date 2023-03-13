#pragma once

#include "Structure/Input/spk_mouse.h"
#include "spk_imessage_consumer.h"
#include "spk_windows_api_module.h"

namespace spk
{
	class MouseModule : public spk::IMessageConsumerModule
	{
	private:
		spk::Mouse* _mouse;

		void _treatMessage(spk::DataBuffer* p_message)
		{
			unsigned int messageId;
			*p_message >> messageId;

			switch (messageId)
			{
			case WM_LBUTTONDOWN:
			{
				if (_mouse->_buttons[spk::Mouse::Button::Left] == spk::InputStatus::Up)
					_mouse->_buttons[spk::Mouse::Button::Left] = spk::InputStatus::Pressed;
				break;
			}
			case WM_MBUTTONDOWN:
			{
				if (_mouse->_buttons[spk::Mouse::Button::Center] == spk::InputStatus::Up)
					_mouse->_buttons[spk::Mouse::Button::Center] = spk::InputStatus::Pressed;
				break;
			}
			case WM_RBUTTONDOWN:
			{
				if (_mouse->_buttons[spk::Mouse::Button::Right] == spk::InputStatus::Up)
					_mouse->_buttons[spk::Mouse::Button::Right] = spk::InputStatus::Pressed;
				break;
			}
			case WM_LBUTTONUP:
			{
				_mouse->_buttons[spk::Mouse::Button::Left] = spk::InputStatus::Released;
				break;
			}
			case WM_MBUTTONUP:
			{
				_mouse->_buttons[spk::Mouse::Button::Center] = spk::InputStatus::Released;
				break;
			}
			case WM_RBUTTONUP:
			{
				_mouse->_buttons[spk::Mouse::Button::Right] = spk::InputStatus::Released;
				break;
			}
			case WM_MOUSEWHEEL:
			{
				short value;

				*p_message >> value;
				float delta = value / 100.0f;

				_mouse->_wheel += delta;
				break;
			}
			case WM_MOUSEMOVE:
			{
				*p_message >> _mouse->_pos.x;
				*p_message >> _mouse->_pos.y;

				break;
			}
			}
		}
		void _onUpdate()
		{
			_mouse->_updateState();
		}

	public:
		MouseModule(WindowsAPI* p_windowAPI) :
			IMessageConsumerModule(p_windowAPI->mouseMessages())
		{
			_mouse = spk::Mouse::instanciate();
		}

		const spk::Mouse* mouse() const { return (_mouse); }
	};
}