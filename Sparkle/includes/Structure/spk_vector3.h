#pragma once

#include "Structure/spk_vector2.h"

namespace spk
{
	template <typename TType>
	struct IVector3
	{
		TType x;
		TType y;
		TType z;

		IVector3() : x(0), y(0), z(0) { }

		IVector3(const TType& p_value) : x(p_value), y(p_value), z(p_value) {}

		IVector3(const IVector2<TType>& p_other, const TType& p_value) : x(p_other.x), y(p_other.y), z(p_value) {}

		IVector3(const TType& p_x, const TType& p_y, const TType& p_z) : x(p_x), y(p_y), z(p_z) {}

		template <typename TOtherType>
		operator IVector3<TOtherType>()
		{
			return (IVector3<TOtherType>(static_cast<TOtherType>(x), static_cast<TOtherType>(y), static_cast<TOtherType>(z)));
		}

		friend std::ostream& operator << (std::ostream& p_os, const IVector3& p_self)
		{
			p_os << p_self.x << " / " << p_self.y << " / " << p_self.z;
			return (p_os);
		}

		IVector3<TType> operator + (const IVector3& other) const
		{
			return (IVector3<TType>(x + other.x, y + other.y, z + other.z));
		}
		IVector3<TType> operator - (const IVector3& other) const
		{
			return (IVector3<TType>(x - other.x, y - other.y, z - other.z));
		}
		IVector3<TType> operator * (const IVector3& other) const
		{
			return (IVector3<TType>(x * other.x, y * other.y, z * other.z));
		}
		IVector3<TType> operator / (const IVector3& other) const
		{
			if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0)
				throw std::runtime_error("Trying to divide by 0");
			return (IVector3<TType>(x / other.x, y / other.y, z / other.z));
		}
		void operator += (const IVector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}
		void operator -= (const IVector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}
		void operator *= (const IVector3& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}
		void operator /= (const IVector3& other)
		{
			if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0)
				throw std::runtime_error("Trying to divide by 0");
			x /= other.x;
			y /= other.y;
			z /= other.z;
		}

		bool operator == (const IVector3& delta) const
		{
			return ((x == delta.x && y == delta.y && z == delta.z) ? true : false);
		}
		bool operator != (const IVector3& delta) const
		{
			return ((x == delta.x && y == delta.y && z == delta.z) ? false : true);
		}
		bool operator < (const IVector3& other) const
		{
			if (z < other.z)
				return (true);
			if (z == other.z && y < other.y)
				return (true);
			if (z == other.z && y == other.y && x < other.x)
				return (true);
			return (false);
		}
		bool operator > (const IVector3& other) const
		{
			if (z > other.z)
				return (true);
			if (z == other.z && y > other.y)
				return (true);
			if (z == other.z && y == other.y && x > other.x)
				return (true);
			return (false);
		}
		bool operator <= (const IVector3& other) const
		{
			if (this->operator == (other) == true || this->operator < (other) == true)
				return (true);
			return (false);
		}
		bool operator >= (const IVector3& other) const
		{
			if (this->operator == (other) == true || this->operator > (other) == true)
				return (true);
			return (false);
		}
	};

	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator+(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) + p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator-(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) - p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator*(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) * p_point);
	};
	template<typename TType, typename TValueType, typename = std::enable_if_t<std::is_arithmetic<TValueType>::value>>
	IVector3<TType> operator/(TValueType p_value, const IVector3<TType>& p_point)
	{
		return (IVector3<TType>(p_value) / p_point);
	};

	using Vector3 = IVector3<float>;
	using Vector3Int = IVector3<int>;
	using Vector3UInt = IVector3<unsigned int>;
};