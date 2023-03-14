#pragma once

#include "Structure/spk_vector2.h"
#include "spk_basic_functions.h"

namespace spk
{
	class Perlin
	{
	private:
		unsigned long _seed;
		float _min;
		float _max;
		float _range;
		size_t _octaveValue;
		float _frequency;
		float _persistance;
		float _lacunarity;

		float _computeWaveLength(float p_x, float p_y, float p_frequency)
		{
			p_x /= p_frequency;
			p_y /= p_frequency;

			Vector2 directions[8] = {
				Vector2(0.707f, 0.707f),
				Vector2(-0.707f, 0.707f),
				Vector2(0.707f, -0.707f),
				Vector2(-0.707f, -0.707f),

				Vector2(1.0f, 0.0f),
				Vector2(-1.0f, 0.0f),
				Vector2(0.0f, 1.0f),
				Vector2(0.0f, -1.0f),
			};

			Vector2 targetPoint = Vector2(p_x, p_y);
			Vector2 pointA = Vector2((int)(p_x), (int)(p_y));
			Vector2 pointB = Vector2((int)(p_x + 1), (int)(p_y));
			Vector2 pointC = Vector2((int)(p_x), (int)(p_y + 1));
			Vector2 pointD = Vector2((int)(p_x + 1), (int)(p_y + 1));
			Vector2 targetDelta = targetPoint - pointA;

			int ii = static_cast<int>(pointA.x) & 255;
			int jj = static_cast<int>(pointA.y) & 255;

			int directionIndexA = spk::generateNumberFromSeed(_seed, (ii + spk::generateNumberFromSeed(_seed, jj))) % 8;
			int directionIndexB = spk::generateNumberFromSeed(_seed, (ii + 1 + spk::generateNumberFromSeed(_seed, jj))) % 8;
			int directionIndexC = spk::generateNumberFromSeed(_seed, (ii + spk::generateNumberFromSeed(_seed, jj + 1))) % 8;
			int directionIndexD = spk::generateNumberFromSeed(_seed, (ii + 1 + spk::generateNumberFromSeed(_seed, jj + 1))) % 8;

			float dotA = directions[directionIndexA].dot(targetPoint - pointA);
			float dotB = directions[directionIndexB].dot(targetPoint - pointB);
			float dotC = directions[directionIndexC].dot(targetPoint - pointC);
			float dotD = directions[directionIndexD].dot(targetPoint - pointD);

			float smoothX = 3 * targetDelta.x * targetDelta.x - 2 * targetDelta.x * targetDelta.x * targetDelta.x;
			float smoothY = 3 * targetDelta.y * targetDelta.y - 2 * targetDelta.y * targetDelta.y * targetDelta.y;

			float dotInterpolate1 = dotA + smoothX * (dotB - dotA);
			float dotInterpolate2 = dotC + smoothX * (dotD - dotC);

			float percent = dotInterpolate1 + smoothY * (dotInterpolate2 - dotInterpolate1);

			return (percent);
		}

	public:
		Perlin(unsigned long p_seed = 12500) :
			_seed(p_seed),
			_min(),
			_max(),
			_range(),
			_octaveValue(3),
			_frequency(50),
			_persistance(0.5f),
			_lacunarity(2.0f)
		{

		}

		const unsigned long& seed() const { return (_seed); }
		void configureSeed(unsigned long p_seed)
		{
			_seed = p_seed;
		}
		void configureFrequency(float p_frequency)
		{
			_frequency = p_frequency;
		}
		void configurePersistance(float p_persistance)
		{
			_persistance = p_persistance;
		}
		void configureLacunarity(float p_lacunarity)
		{
			_lacunarity = p_lacunarity;
		}
		void configureOctave(size_t p_octaveValue)
		{
			_octaveValue = p_octaveValue;
		}
		void configureRange(float p_min, float p_max)
		{
			_min = p_min;
			_max = p_max;
			_range = _max - _min;
		}
		float sample2D(float p_x, float p_y)
		{
			p_x = std::fabs(p_x + _seed);
			p_y = std::fabs(p_y + _seed);

			float result = 0;
			float amplitude = 1.0f;
			float frequency = _frequency;

			for (size_t i = 0; i < _octaveValue; i++)
			{
				result += _computeWaveLength(p_x, p_y, frequency) * amplitude;
				amplitude *= _persistance;
				frequency /= _lacunarity;
			}

			result += 1;
			result /= 2;
			result *= _range;
			result += _min;

			return (result);
		}
	};
}