#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 
#endif
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <algorithm>

namespace spk
{
	template <typename TType>
	struct IVector2
	{
		TType x;
		TType y;

		IVector2() : x(0), y(0) { }

		IVector2(const TType& p_value) : x(p_value), y(p_value) {}

		IVector2(const TType& p_x, const TType& p_y) : x(p_x), y(p_y) {}

		template <typename TOtherType>
		operator IVector2<TOtherType>()
		{
			return (IVector2<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y)));
		}

		friend std::ostream& operator << (std::ostream& p_os, const IVector2& p_self)
		{
			p_os << p_self.x << " / " << p_self.y;
			return (p_os);
		}

		IVector2<TType> operator + (const IVector2& p_other) const
		{
			return (IVector2<TType>(x + p_other.x, y + p_other.y));
		}
		IVector2<TType> operator - (const IVector2& p_other) const
		{
			return (IVector2<TType>(x - p_other.x, y - p_other.y));
		}
		IVector2<TType> operator * (const IVector2& p_other) const
		{
			return (IVector2<TType>(x * p_other.x, y * p_other.y));
		}
		IVector2<TType> operator / (const IVector2& p_other) const
		{
			if (p_other.x == 0 || p_other.y == 0)
				throw std::runtime_error("Trying to divide by 0");
			return (IVector2<TType>(x / p_other.x, y / p_other.y));
		}
		void operator += (const IVector2& p_other)
		{
			x += p_other.x;
			y += p_other.y;
		}
		void operator -= (const IVector2& p_other)
		{
			x -= p_other.x;
			y -= p_other.y;
		}
		void operator *= (const IVector2& p_other)
		{
			x *= p_other.x;
			y *= p_other.y;
		}
		void operator /= (const IVector2& p_other)
		{
			if (p_other.x == 0 || p_other.y == 0)
				throw std::runtime_error("Trying to divide by 0");
			x /= p_other.x;
			y /= p_other.y;
		}

		float length() const
		{
			return (sqrt(pow(x, 2.0f)
				+ pow(y, 2.0f)));
		}

		float distance(const IVector2& p_other) const
		{
			return (std::sqrt(pow(p_other.x - x, 2) + pow(p_other.y - y, 2)));
		}

		IVector2<float> normalize() const
		{
			float tmp_length;

			tmp_length = length();

			if (tmp_length == 0)
				std::runtime_error("Trying to divide by 0");
			return (IVector2<float>(static_cast<float>(x) / tmp_length, static_cast<float>(y) / tmp_length));
		}

		IVector2 cross(const IVector2& p_other = IVector2(0, 0)) const
		{
			IVector2 result;

			result = IVector2(-(p_other.y - y), p_other.x - x);

			return (result);
		}

		float dot(const IVector2& p_other, const IVector2& p_center = IVector2(0, 0)) const
		{
			float result;

			result = (x - p_center.x) * (p_other.x - p_center.x) + (y - p_center.y) * (p_other.y - p_center.y);

			return (result);
		}

		float angle(const IVector2& p_other, const IVector2& p_center = IVector2(0, 0)) const
		{
			float rdot = dot(p_other, p_center);

			rdot = (rdot < -1.0f ? -1.0f : (rdot > 1.0f ? 1.0f : rdot));

			float angle = std::acos(rdot);

			return (angle * (180 / M_PI));
		}

		float angle() const
		{
			float degree = std::atan((static_cast<float>(y) / static_cast<float>(x)) * (180 / M_PI));

			if (x >= 0 && y >= 0)
				return (degree);
			else if (x >= 0 && y < 0)
				return (360 + degree);
			else if (x < 0 && y >= 0)
				return (180 + degree);
			else
				return (180 + degree);
		}

		IVector2 invert() const
		{
			return (IVector2(x * -1, y * -1));
		}

		IVector2 rotate(IVector2 p_center, float p_angle) const
		{
			float theta = p_angle * (M_PI / 180);

			float pcos = cos(theta);
			float psin = sin(theta);

			IVector2 result;
			result.x = (x - p_center.x) * pcos - (y - p_center.y) * psin;
			result.y = (x - p_center.x) * psin + (y - p_center.y) * pcos;

			return (result + p_center);
		}

		bool operator == (const IVector2& delta) const
		{
			return (x == delta.x && y == delta.y);
		}

		bool operator != (const IVector2& delta) const
		{
			return (x == delta.x && y == delta.y);
		}

		bool operator < (const IVector2& p_other) const
		{
			if (y < p_other.y)
				return (true);
			if (y == p_other.y && x < p_other.x)
				return (true);
			return (false);
		}

		bool operator > (const IVector2& p_other) const
		{
			if (y > p_other.y)
				return (true);
			if (y == p_other.y && x > p_other.x)
				return (true);
			return (false);
		}

		bool operator <= (const IVector2& p_other) const
		{
			if (x <= p_other.x && y <= p_other.y)
				return (true);
			return (false);
		}

		bool operator >= (const IVector2& p_other) const
		{
			if (x >= p_other.x && y >= p_other.y)
				return (true);
			return (false);
		}

		IVector2 floor()
		{
			IVector2 result;

			result.x = std::floorf(x);
			result.y = std::floorf(y);

			return (result);
		}
		IVector2 ceiling()
		{
			IVector2 result;

			result.x = std::ceilf(x);
			result.y = std::ceilf(y);

			return (result);
		}
		IVector2 round()
		{
			IVector2 result;

			result.x = std::roundf(x);
			result.y = std::roundf(y);

			return (result);
		}

		bool isBetween(IVector2 p_min, IVector2 p_max) const
		{
			if (x < p_min.x || y < p_min.y ||
				x > p_max.x || y > p_max.y)
				return (false);
			return (true);
		}
	};

	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator+(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) + p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator-(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) - p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator*(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) * p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector2<TType> operator/(TValueType p_value, const IVector2<TType>& p_point)
	{
		return (IVector2<TType>(p_value, p_value) / p_point);
	};


	using Vector2 = IVector2<float>;
	using Vector2Int = IVector2<int>;
	using Vector2UInt = IVector2<unsigned int>;
}