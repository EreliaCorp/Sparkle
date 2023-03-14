#pragma once

#include <map>
#include <string>

#include "Structure/spk_image.h"

namespace spk
{
	class TextureAtlas
	{
	private:
		std::map<std::string, Image*> _textureMap;

	public:
		TextureAtlas()
		{

		}
		void addTexture(std::string p_name, Image* p_image)
		{
			_textureMap[p_name] = p_image;
		}
		Image* operator [] (std::string p_name) const
		{
			if (_textureMap.count(p_name) == 0)
				return (nullptr);
			return (_textureMap.at(p_name));
		}
	};
}