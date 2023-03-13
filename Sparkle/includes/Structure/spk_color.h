#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

namespace spk
{
	struct Color
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;

		Color() {}

		Color(int p_r, int p_g, int p_b, int p_a = 255) :
			r(static_cast<float>(p_r) / 255.0f), g(static_cast<float>(p_g) / 255.0f),
			b(static_cast<float>(p_b) / 255.0f), a(static_cast<float>(p_a) / 255.0f) {}

		Color(float p_r, float p_g, float p_b, float p_a = 1.0f) :
			r(p_r), g(p_g), b(p_b), a(p_a) {}

		Color(const std::string& p_hex)
		{
			std::stringstream ss;
			ss << std::hex << p_hex.substr(1);

			int hex;
			ss >> hex;

			if (p_hex.size() == 7)
			{
				r = static_cast<float>((hex >> 16) & 0xFF) / 255.0f;
				g = static_cast<float>((hex >> 8) & 0xFF) / 255.0f;
				b = static_cast<float>(hex & 0xFF) / 255.0f;
				a = 1.0f;
			}
			else if (p_hex.size() == 9)
			{
				r = static_cast<float>((hex >> 24) & 0xFF) / 255.0f;
				g = static_cast<float>((hex >> 16) & 0xFF) / 255.0f;
				b = static_cast<float>((hex >> 8) & 0xFF) / 255.0f;
				a = static_cast<float>(hex & 0xFF) / 255.0f;
			}
			else
			{
				throw std::invalid_argument("Invalid hex color string");
			}
		}

		friend std::ostream& operator << (std::ostream& p_os, const Color& p_self)
		{
			std::stringstream ss;
			ss << "#" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(p_self.r * 255.0f)
				<< std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(p_self.g * 255.0f)
				<< std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(p_self.b * 255.0f)
				<< std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(p_self.a * 255.0f);
			p_os << ss.str();
			return p_os;
		}

		bool operator == (const Color& p_other) const
		{
			return r == p_other.r && g == p_other.g && b == p_other.b && a == p_other.a;
		}

		bool operator != (const Color& p_other) const
		{
			return !(*this == p_other);
		}

		Color interpolate(const Color& p_target, float p_alpha) const
		{
			float inv_alpha = 1.0f - p_alpha;
			return Color(r * inv_alpha + p_target.r * p_alpha,
				g * inv_alpha + p_target.g * p_alpha,
				b * inv_alpha + p_target.b * p_alpha,
				a * inv_alpha + p_target.a * p_alpha);
		}

		Color linear_interpolation(const Color& p_target, float p_alpha) const
		{
			float r_diff = p_target.r - r;
			float g_diff = p_target.g - g;
			float b_diff = p_target.b - b;
			float a_diff = p_target.a - a;

			return Color(r + r_diff * p_alpha, g + g_diff * p_alpha,
				b + b_diff * p_alpha, a + a_diff * p_alpha);
		}

		void merge(const Color& p_other)
		{
			r = std::clamp(r + p_other.r, 0.0f, 1.0f);
			g = std::clamp(g + p_other.g, 0.0f, 1.0f);
			b = std::clamp(b + p_other.b, 0.0f, 1.0f);
			a = std::clamp(a + p_other.a, 0.0f, 1.0f);
		}

		void overlay(const Color& p_other)
		{
			r = p_other.r * p_other.a + r * (1.0f - p_other.a);
			g = p_other.g * p_other.a + g * (1.0f - p_other.a);
			b = p_other.b * p_other.a + b * (1.0f - p_other.a);
			a = p_other.a + a * (1.0f - p_other.a);
		}

		static const Color transparent;
		static const Color white;
		static const Color black;
		static const Color red;
		static const Color green;
		static const Color blue;
	};
}