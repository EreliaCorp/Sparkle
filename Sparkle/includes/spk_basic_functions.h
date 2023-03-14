#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#include "Structure/spk_iostream.h"

#include "spk_opengl.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define GET_VARIABLE_NAME(Variable) (#Variable)

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace spk
{
	std::string glGetTypeString(GLenum type);
	void checkOpengl(std::string msg);
	std::string methodName(const std::string& prettyFunction);
	std::string className(const std::string& prettyFunction);
	float degreeToRadian(const float& angle);
	float radianToDegree(const float& radian);
	long generateNumberFromSeed(const long seed, const long x, const long y = 0, const long z = 0);
	long generateNumber(const long min, const long max);
	std::vector<std::string> stringSplit(const std::string& p_string, const std::string& p_delim);
	std::string getStr(std::fstream& p_file);
	std::vector<std::string> getStringSplit(std::fstream& p_file, const std::string& p_delim, const int& p_expectedSize = -1);
	int positive_modulo(int i, int n);
	std::string normalizeString(std::string p_str, size_t p_finalLength, char p_char);
	std::vector<std::string> decomposeIntoParagraph(std::string p_str, size_t p_lineLength, std::string p_delim);
	void editStringContent(std::string& p_str, size_t p_position, size_t p_length, char p_c);
	size_t findCharFromEnd(std::string p_str, size_t p_base, char p_c);
	std::vector<std::string> listFile(std::string p_path, std::string p_extension);
	void drawRectangleColor(Color p_color, Vector2Int p_pos, Vector2Int p_size, float p_depth);

	template <typename TType>
	IVector2<TType> max(const IVector2<TType>& p_lhd, const IVector2<TType>& p_rhd)
	{
		IVector2<TType> result;

		result.x = std::max(p_lhd.x, p_rhd.x);
		result.y = std::max(p_lhd.y, p_rhd.y);

		return (result);
	}

	template <typename TType>
	IVector2<TType> min(const IVector2<TType>& p_lhd, const IVector2<TType>& p_rhd)
	{
		IVector2<TType> result;

		result.x = std::min(p_lhd.x, p_rhd.x);
		result.y = std::min(p_lhd.y, p_rhd.y);

		return (result);
	}

	template <typename TType>
	IVector2<TType> clamp(const IVector2<TType>& p_min, const IVector2<TType>& p_value, const IVector2<TType>& p_max)
	{
		IVector2<TType> result;

		result.x = std::max(p_min.x, std::min(p_max.x, p_value.x));
		result.y = std::max(p_min.y, std::min(p_max.y, p_value.y));

		return (result);
	}
}

#define __METHOD__ spk::methodName(__PRETTY_FUNCTION__)
#define __CLASS__ spk::className(__PRETTY_FUNCTION__)

#define DEBUG_LINE() spk::cout << __CLASS__ << " - " << __METHOD__ << "::" << __LINE__ << std::endl
#define DEBUG_LINE_TEXT(p_text) spk::cout << __CLASS__ << " - " << __METHOD__ << "::" << __LINE__ << " - " << p_text << std::endl
