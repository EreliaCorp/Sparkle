#pragma once

#include "Structure/Input/spk_keyboard.h"
#include "spk_imessage_consumer.h"
#include "spk_windows_api_module.h"

namespace spk
{
	class KeyboardModule : public spk::IMessageConsumerModule
	{
	private:
		spk::Keyboard* _keyboard;

		void _treatMessage(spk::DataBuffer* p_message)
		{
			unsigned int messageId;
			*p_message >> messageId;

			switch (messageId)
			{
			case WM_CHAR:
			{
				unsigned int value;

				*p_message >> value;

				_keyboard->_entry = static_cast<uint8_t>(value);
				break;
			}
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				unsigned int value;

				*p_message >> value;

				_keyboard->_keys[value] = spk::InputStatus::Pressed;

				break;
			}
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				unsigned int value;

				*p_message >> value;

				_keyboard->_keys[value] = spk::InputStatus::Released;
				break;
			}
			}
		}
		void _onUpdate()
		{
			_keyboard->_updateState();
		}

	public:
		KeyboardModule(WindowsAPI* p_windowAPI) :
			IMessageConsumerModule(p_windowAPI->keyboardMessages())
		{
			_keyboard = spk::Keyboard::instanciate();
		}
		const spk::Keyboard* keyboard() const { return (_keyboard); }
	};
}