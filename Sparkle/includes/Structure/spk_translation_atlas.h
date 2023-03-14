#pragma once

#include <map>
#include <string>
#include <fstream>
#include <vector>

#include "spk_basic_functions.h"

namespace spk
{
	class TranslationAtlas
	{
	private:
		std::map<std::string, std::string> _translationMap;

	public:
		void load(std::string p_path)
		{
			std::fstream file;
			file.open(p_path, std::ios_base::in);

			while (file.eof() == false)
			{
				std::vector<std::string> tab = getStringSplit(file, ":");
				if (tab.size() >= 2)
				{
					std::string key = tab[0];
					std::string value = tab[1];
					for (size_t i = 2; i < tab.size(); i++)
						value += ":" + tab[i];
					_translationMap[key] = value;
				}
			}
		}

		void addTranslation(std::string p_key, std::string p_value)
		{
			_translationMap[p_key] = p_value;
		}

		std::string translate(std::string p_key)
		{
			if (_translationMap.count(p_key) == 0)
				return (p_key);
			return (_translationMap.at(p_key));
		}
	};
}