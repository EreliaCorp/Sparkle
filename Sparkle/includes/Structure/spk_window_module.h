#pragma once

#include <Windows.h>
#include "spk_imessage_consumer.h"

#include "spk_windows_api_module.h"

#include "spk_window.h"

namespace spk
{
	class WindowModule : public IMessageConsumerModule
	{
	private:
		void* _api_module;

		void _treatMessage(spk::DataBuffer* p_message)
		{
			unsigned int messageId;
			*p_message >> messageId;

			switch (messageId)
			{
			case WM_SIZE:
				unsigned int width;
				unsigned int height;

				*p_message >> width;
				*p_message >> height;

				spk::Window::instance()->resize(width, height);
				break;
			}
		}

		void _onUpdate()
		{
			spk::Window::instance()->render();
			spk::Window::instance()->clear();
		}

	public:
		WindowModule(WindowsAPI* p_windowsAPI, std::string p_title, spk::Vector2Int p_size, spk::Color p_backgroundColor) :
			IMessageConsumerModule(p_windowsAPI->windowMessages()),
			_api_module(p_windowsAPI)
		{
			Window::instanciate(p_windowsAPI, p_title, p_size, p_backgroundColor);
		}

		void reset()
		{
			Window::instance()->reset();
		}

		void render()
		{
			Window::instance()->render();
		}
	};
}