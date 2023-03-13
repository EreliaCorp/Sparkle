#pragma once

#include "spk_imessage_consumer.h"
#include "spk_windows_api_module.h"

#include "Structure/spk_application.h"

namespace spk
{
	class SystemModule : public IMessageConsumerModule
	{
	private:
		Application* _application;

		void _treatMessage(spk::DataBuffer* p_message)
		{
			unsigned int messageId;
			*p_message >> messageId;

			switch (messageId)
			{
			case WM_DESTROY:
			{
				_application->quit(0);
				break;
			}
			}
		}

		void _onUpdate()
		{

		}

	public:
		SystemModule(WindowsAPI* p_windowsAPI, Application* p_application) :
			IMessageConsumerModule(p_windowsAPI->systemMessages()),
			_application(p_application)
		{

		}
	};
}