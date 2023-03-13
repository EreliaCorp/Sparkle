#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include <sstream>

namespace spk
{
	class JSONParser {
	private:
		using JSONData = std::variant<bool, int, std::string, double>;
		struct ParsingData
		{
			std::string name;
			JSONData data;
			std::string arrayContent;
		};

		struct Element
		{
			std::string name;
			JSONData data;

			Element(std::string p_name = "Undefined", JSONData p_data = JSONData()) :
				name(p_name),
				data(p_data)
			{

			}
		};

		struct Block
		{
			std::string name;
			std::vector<Element> elements;
		};

		std::vector<Block> blocks;

		std::map<std::string, JSONData> datas;

		std::string _readFileContent(std::string p_path)
		{
			std::fstream file;
			file.open(p_path, std::ios_base::in);

			std::stringstream buffer;
			buffer << file.rdbuf();

			file.close();

			return (buffer.str());
		}

		void _extractElementName(std::string& p_str, size_t& p_index, ParsingData& p_element)
		{
			p_index++;
			size_t start = p_index;
			for (; p_index < p_str.size() && (p_index == 0 || (p_str[p_index] != '\"' && p_str[p_index - 1] != '\\')); p_index++);

			p_element.name = (p_str.substr(start, p_index - start));
			p_index++;
		}
		void _skipDelimiter(std::string& p_str, size_t& p_index)
		{
			p_index = p_str.find_first_not_of("\n\t ");
			if (p_str[p_index] != ':')
				throw std::runtime_error("Error while parsing a JSON file : invalid delimiter");
			p_index = p_str.find_first_not_of("\n\t ", p_index + 1);
		}

		void _extractElementDataArray(std::string& p_str, size_t& p_index, ParsingData& p_element)
		{
			size_t curlyBracketCount = 0;
			size_t bracketCount = 0;
			size_t start = p_index;
			if (p_str[p_index] == '[')
			{
				p_index++;
				bracketCount++;
			}
			if (p_str[p_index] == '{')
			{
				p_index++;
				curlyBracketCount++;
			}

			for (; curlyBracketCount != 0 || bracketCount != 0; p_index++)
			{
				switch (p_str[p_index])
				{
				case '[':
					bracketCount++;
					break;
				case '{':
					curlyBracketCount++;
					break;
				case ']':
					bracketCount--;
					break;
				case '}':
					curlyBracketCount--;
					break;
				}
			}

			p_element.arrayContent = (p_str.substr(start, p_index - start));
		}
		void _extractElementDataString(std::string& p_str, size_t& p_index, ParsingData& p_element)
		{
			p_index++;
			size_t start = p_index;
			for (; p_index < p_str.size() && (p_index == 0 || (p_str[p_index] != '\"' && p_str[p_index - 1] != '\\')); p_index++);
			p_index++;

			p_element.data = (p_str.substr(start, p_index - start - 1));
		}
		void _extractElementDataValue(std::string& p_str, size_t& p_index, ParsingData& p_element)
		{
			size_t start = p_index;
			for (; p_index < p_str.size() && p_str[p_index] != ','; p_index++);

			std::string valueInput = (p_str.substr(start, p_index - start));

			if (valueInput == "true")
			{
				p_element.data = true;
			}
			else if (valueInput == "false")
			{
				p_element.data = false;
			}
			else if (valueInput.find(".") != valueInput.npos)
			{
				p_element.data = std::stof(valueInput);
			}
			else
			{
				p_element.data = std::stoi(valueInput);
			}
		}

		void _extractElementData(std::string& p_str, size_t& p_index, ParsingData& p_element)
		{
			if (p_str[p_index] == '[' || p_str[p_index] == '{')
				_extractElementDataArray(p_str, p_index, p_element);
			else if (p_str[p_index] == '"')
				_extractElementDataString(p_str, p_index, p_element);
			else
				_extractElementDataValue(p_str, p_index, p_element);
		}

		ParsingData _composeElement(std::string& p_str, size_t& p_index)
		{
			JSONParser::ParsingData result;

			_extractElementName(p_str, p_index, result);
			_skipDelimiter(p_str, p_index);
			_extractElementData(p_str, p_index, result);

			return (result);
		}

		Block _parseBlock(std::string p_name, std::string& p_str, size_t& p_index)
		{
			JSONParser::Block result;

			result.name = p_name;
			for (; p_index < p_str.size() && p_str[p_index] != '}'; p_index++)
			{
				if (p_str[p_index] == '\"')
				{
					JSONParser::ParsingData newElement = _composeElement(p_str, p_index);

					if (newElement.arrayContent == "")
						result.elements.push_back(Element(newElement.name, newElement.data));
					else
					{
						std::string baseName = (result.name != "" ? result.name + "." : "") + newElement.name;
						size_t subBlockIndex = 0;
						for (size_t tmpIndex = 0; tmpIndex < newElement.arrayContent.size(); tmpIndex++)
						{
							if (newElement.arrayContent[tmpIndex] == '{')
							{
								std::string name = baseName + "[" + std::to_string(subBlockIndex) + "]";
								if (tmpIndex == 0)
									name = baseName;

								blocks.push_back(_parseBlock(name, newElement.arrayContent, tmpIndex));
								subBlockIndex++;
							}
						}

						result.elements.push_back(Element(baseName + ".size", static_cast<int>(subBlockIndex)));
					}
				}
			}

			return (result);
		}

		void _exportBlockComposition()
		{
			for (size_t i = 0; i < blocks.size(); i++)
			{
				for (size_t j = 0; j < blocks[i].elements.size(); j++)
				{
					datas[(blocks[i].name != "" ? blocks[i].name + "." : "") + blocks[i].elements[j].name] = blocks[i].elements[j].data;
				}
			}
		}

	public:
		JSONParser()
		{

		}

		JSONParser(std::string p_filePath)
		{
			load(p_filePath);
		}
		void load(std::string p_path)
		{
			std::string fileContent = _readFileContent(p_path);

			size_t index = 0;
			blocks.push_back(_parseBlock("", fileContent, index));

			_exportBlockComposition();

			blocks.clear();
		}
		JSONParser::JSONData get(std::string key)
		{
			if (datas.find(key) != datas.end())
				return datas[key];
			else
				return (JSONParser::JSONData());
		}

		template<typename TType>
		TType smartGet(std::string key)
		{
			JSONParser::JSONData baseValue = get(key);

			return (std::get<TType>(baseValue));
		}

		bool getBool(std::string key)
		{
			return (std::get<bool>(get(key)));
		}
		int getInteger(std::string key)
		{
			return (std::get<int>(get(key)));
		}
		std::string getString(std::string key)
		{
			return (std::get<std::string>(get(key)));
		}
		double getFloatingPoint(std::string key)
		{
			return (std::get<double>(get(key)));
		}
	};
}