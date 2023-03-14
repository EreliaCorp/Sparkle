#pragma once

#include "spk_iostream.h"
#include "spk_iwidget.h"

namespace spk
{
	class WidgetModule
	{
	private:
		std::vector<IWidget*> _widgets;

	public:
		WidgetModule()
		{

		}

		void render()
		{
			for (size_t i = 0; i < _widgets.size(); i++)
			{
				if (_widgets[i]->isActive() == true)
					_widgets[i]->_render();
			}
		}

		void update()
		{
			for (size_t i = 0; i < _widgets.size(); i++)
			{
				if (_widgets[i]->isActive() == true)
					_widgets[i]->_update();
			}
		}

		template <typename TChildrenType, typename ... Args>
		TChildrenType* addWidget(Args&& ...p_args)
		{
			TChildrenType* result = new TChildrenType(std::forward<Args>(p_args)...);

			_widgets.push_back(result);

			return (result);
		}
	};
}