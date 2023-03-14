#pragma once

#include "Structure/spk_vector2.h"
#include "spk_basic_functions.h"
#include "Structure/spk_window.h"

namespace spk
{
	class Viewport
	{
	private:
		const Viewport* _parent;

		Vector2Int _origin = Vector2Int(0, 0);
		Vector2Int _anchor = Vector2Int(0, 0);
		Vector2Int _cumulatedAnchor = Vector2Int(0, 0);
		Vector2Int _size = Vector2Int(0, 0);
		Vector2Int _cumulatedSize = Vector2Int(0, 0);
		Vector2Int _anchorOffset = Vector2Int(0, 0);
		Vector2Int _sizeOffset = Vector2Int(0, 0);

	public:
		Viewport()
		{

		}

		const Vector2Int& anchor() const { return (_anchor); }
		const Vector2Int& size() const { return (_size); }
		const Vector2Int& anchorOffset() const { return (_anchorOffset); }
		const Vector2Int& sizeOffset() const { return (_sizeOffset); }

		void setOffset(Vector2Int p_anchorOffset, Vector2Int p_sizeOffset)
		{
			_anchorOffset = p_anchorOffset;
			_sizeOffset = p_sizeOffset;
		}
		void setParentViewport(const Viewport* p_parent)
		{
			_parent = p_parent;
		}
		void configure(Vector2Int p_anchor, Vector2Int p_size)
		{
			_anchor = p_anchor;
			_size = p_size;

			if (_parent == nullptr)
			{
				_origin = _anchor + _anchorOffset;
				_cumulatedAnchor = _anchor + _anchorOffset;
				_cumulatedSize = _size - _sizeOffset;
			}
			else
			{
				_origin = _parent->_origin + _anchor + _anchorOffset;

				_cumulatedAnchor = spk::max(_parent->_cumulatedAnchor, _parent->_cumulatedAnchor + _anchor + _anchorOffset);
				_cumulatedSize = spk::min(_parent->_cumulatedAnchor + _parent->_size, _origin + _size - _sizeOffset) - _cumulatedAnchor;
			}
		}
		void use() const
		{
			Window::instance()->setOrigin(_origin);
			Window::instance()->setScissorViewport(_cumulatedAnchor, _cumulatedSize);
		}
	};
}