#pragma once

#include "Structure/spk_image.h"

namespace spk
{
	class SpriteSheet : public Image
	{
	private:
		Vector2UInt _size;
		Vector2 _unit;
		std::vector<Vector2> _sprites;

	public:
		SpriteSheet(std::string path, Vector2UInt p_size) : Image(path)
		{
			_size = p_size;
			_unit = Vector2(1.0f, 1.0f) / _size;

			if (p_size == Vector2UInt(1, 1))
				_sprites.push_back(Vector2(0, 0));
			else
			{
				for (size_t j = 0; j < _size.y; j++)
				{
					for (size_t i = 0; i < _size.x; i++)
					{
						_sprites.push_back(Vector2(i, j) * _unit);
					}
				}
			}
		}

		const Vector2UInt& size() const { return (_size); }
		const Vector2& unit() const { return (_unit); }
		const std::vector<Vector2>& sprites() const { return (_sprites); }
		 
		const size_t& spriteID(Vector2UInt p_spriteValue) const
		{
			return ((static_cast<size_t>(_size.x) * static_cast<size_t>(p_spriteValue.y)) + static_cast<size_t>(p_spriteValue.x));
		}
		const Vector2& sprite(size_t index) const
		{
			if (_sprites.size() <= index)
				throw std::runtime_error("Segfault : sprite id out of range");
			return (_sprites[index]);
		}
		const Vector2& sprite(Vector2UInt p_spriteValue) const
		{
			return (sprite(spriteID(p_spriteValue)));
		}
		const Vector2& sprite(Vector2Int p_spriteValue) const
		{
			return (sprite(spriteID(p_spriteValue)));
		}

		void draw(size_t p_spriteId, Vector2Int p_pos, Vector2UInt p_size, float p_depth)
		{
			Image::draw(p_pos, p_size, _sprites[p_spriteId], _unit, p_depth);
		}
		void draw(Vector2Int p_sprite, Vector2Int p_pos, Vector2UInt p_size, float p_depth)
		{
			SpriteSheet::draw(spriteID(p_sprite), p_pos, p_size, p_depth);
		}
	};
}