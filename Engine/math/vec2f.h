#ifndef MELONENGINE_VEC2F_H
#define MELONENGINE_VEC2F_H

#include <cmath>
#include <algorithm>
#include <cfloat>

#include "MathDefines.h"


struct vec2f
{
	float x, y;
	constexpr vec2f(float x, float y) : x(x), y(y)
	{
	}
	constexpr vec2f() : x(0), y(0)
	{
	}

	MELON_INLINE static vec2f zero()
	{
		return vec2f(0.0f, 0.0f);
	}
	MELON_INLINE static vec2f one()
	{
		return vec2f(1.0f, 1.0f);
	}
	MELON_INLINE static vec2f up()
	{
		return vec2f(0.0f, 1.0f);
	}
	MELON_INLINE static vec2f down()
	{
		return vec2f(0.0f, -1.0f);
	}
	MELON_INLINE static vec2f right()
	{
		return vec2f(1.0f, 0.0f);
	}
	MELON_INLINE static vec2f left()
	{
		return vec2f(-1.0f, 0.0f);
	}
	MELON_INLINE static vec2f unit()
	{
		return vec2f(0.707107f, 0.707107f);
	}
	MELON_INLINE constexpr float dot(const vec2f& other) const
	{
		return x * other.x + y * other.y;
	}
	MELON_INLINE float distanceSquared(const vec2f& other) const
	{
		const float distanceX = x - other.x;
		const float distanceY = y - other.y;
		return distanceX * distanceX + distanceY * distanceY;
	}
	MELON_INLINE float distance(const vec2f& other) const
	{
		return std::sqrt(distanceSquared(other));
	}
	MELON_INLINE float magnitudeSquared() const
	{
		return dot(*this);
	}
	MELON_INLINE float magnitude() const
	{
		return std::sqrt(magnitudeSquared());
	}
	MELON_INLINE vec2f normalize() const
	{
		const float m = magnitude();
		if (m <= FLT_EPSILON) return vec2f::zero();
		return vec2f(
			x / m,
			y / m
		);
	}
	MELON_INLINE bool isNormalized() const
	{
		return isFloatCloseEnough(magnitudeSquared(), 1.0f);
	}
	MELON_INLINE vec2f lerp(const vec2f& other, const float& t) const
	{
		float clampedT = std::clamp(t, 0.0f, 1.0f);
		return vec2f(
			x + (clampedT * (other.x - x)),
			y + (clampedT * (other.y - y))
		);
	}
	MELON_INLINE float theta(const vec2f& other) const
	{
		const float magProduct = magnitude() * other.magnitude();
		if (magProduct <= FLT_EPSILON) return 0.0f;

		// Clamp to avoid NaN from precision errors in acos
		const float dotProd = dot(other) / magProduct;
		return std::acos(std::clamp(dotProd, -1.0f, 1.0f));
	}
	MELON_INLINE vec2f reflection(const vec2f& other) const
	{
		// other is surface normal
		if (std::abs(magnitude()) < FLT_EPSILON || std::abs(other.magnitude()) < FLT_EPSILON) return *this;

		vec2f n = other.isNormalized() ? other : other.normalize();
		float s = 2.0f * dot(n);
		return *this - (n * s);
	}
	MELON_INLINE constexpr vec2f operator+(const vec2f& other) const
	{
		return vec2f(
			x + other.x,
			y + other.y
		);
	}
	MELON_INLINE constexpr vec2f operator-(const vec2f& other) const
	{
		return vec2f(
			x - other.x,
			y - other.y
		);
	}
	MELON_INLINE vec2f operator-() const
	{
		return vec2f(-x, -y);
	}
	MELON_INLINE vec2f operator*(const vec2f& other) const
	{
		return vec2f(
			x * other.x,
			y * other.y
		);
	}
	MELON_INLINE vec2f operator*(const float& s) const
	{
		return vec2f(
			x * s,
			y * s
		);
	}
	MELON_INLINE vec2f operator/(const float& s) const
	{
		if (std::abs(s) <= FLT_EPSILON) return vec2f::zero();
		const float inv = 1.0f / s;
		return vec2f(x * inv, y * inv);
	}

	MELON_INLINE bool operator ==(const vec2f& other) const
	{
		return (isFloatCloseEnough(x, other.x) && isFloatCloseEnough(y, other.y));
	}
	MELON_INLINE static bool isFloatCloseEnough(float a, float b, float precision = 1e-4f)
	{
		return std::abs(b - a) <= precision;
	}
};
MELON_INLINE vec2f operator*(const float& s, const vec2f& v)
{
	return v * s;
}

#endif // MELONENGINE_VEC2F_H


