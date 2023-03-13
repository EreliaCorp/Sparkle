#pragma once

#include "Structure/spk_vector2.h"
#include "Structure/spk_inheritance_system.h"
#include "Structure/spk_value.h"

namespace spk
{
	class IWidget
	{
		friend class WidgetModule;
	private:
		spk::InheritanceSystem<IWidget> _inheritanceSystem;

		spk::Vector2Int _anchor;
		spk::Vector2Int _size;

		virtual bool _onUpdate() = 0;
		virtual void _onGeometryChange() = 0;
		virtual void _onRender() = 0;

		void _render()
		{
			_onRender();
			for (size_t i = 0; i < _inheritanceSystem.childrens().size(); i++)
			{
				_inheritanceSystem.childrens()[i]->_render();
			}
		}
		bool _update()
		{
			for (size_t i = 0; i < _inheritanceSystem.childrens().size(); i++)
			{
				if (_inheritanceSystem.childrens()[i]->_update() == true)
					return (true);
			}
			return (_onUpdate());
		}

	public:
		IWidget()
		{

		}

		template <typename TChildrenType, typename ... Args>
		TChildrenType* addChildren(Args&& ...p_args)
		{
			TChildrenType* result = new TChildrenType(std::forward<Args>(p_args)...);

			_inheritanceSystem.addChild(result);

			return (result);
		}

		void setGeometry(spk::Vector2Int p_anchor, spk::Vector2Int p_size)
		{
			_anchor = p_anchor;
			_size = p_size;
			_onGeometryChange();
		}

		void place(spk::Vector2Int p_anchor)
		{
			_anchor = p_anchor;
			_onGeometryChange();
		}

		void move(spk::Vector2Int p_delta)
		{
			_anchor += p_delta;
			_onGeometryChange();
		}

		const spk::Vector2Int& anchor() const { return (_anchor); }
		const spk::Vector2Int& size() const { return (_size); }

	};
}