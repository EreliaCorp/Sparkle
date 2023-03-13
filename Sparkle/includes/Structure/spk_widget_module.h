#pragma once

#include "spk_iostream.h"
#include "spk_iwidget.h"

namespace spk
{
	class WidgetModule
	{
	private:

	public:
		WidgetModule()
		{

		}

		void render()
		{
			spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl;
		}

		void update()
		{
			spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl;
		}
	};
}