#pragma once

#include "Structure/spk_pool.h"
#include "Structure/spk_data_buffer.h"
#include "Structure/spk_locked_queue.h"

namespace spk
{
	class Application;

	class IMessageConsumerModule
	{
	private:
		spk::LockedQueue<spk::DataBuffer*>& _message;

		virtual void _treatMessage(spk::DataBuffer* p_message) = 0;
		virtual void _onUpdate() = 0;

	public:
		IMessageConsumerModule(spk::LockedQueue<spk::DataBuffer*>& p_message) :
			_message(p_message)
		{

		}
		void update()
		{
			_onUpdate();
			while (_message.empty() == false)
			{
				spk::DataBuffer* message = _message.pop_front();

				_treatMessage(message);
			}
		}
	};
}