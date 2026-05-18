#ifndef MELONENGINE_VEC3F_H
#define MELONENGINE_VEC3F_H

#include "MathDefines.h"
#include <cmath>
#include <algorithm>
#include <cfloat>


struct vec3f {

	float x, y, z;

	constexpr vec3f(float x, float y, float z)
		: x(x), y(y), z(z)
	{
	}

	constexpr vec3f()
		: x(0), y(0), z(0)
	{
	}

	MELON_INLINE static vec3f zero()
	{
		return vec3f(0.0f, 0.0f, 0.0f);
	}
	MELON_INLINE static vec3f one()
	{
		return vec3f(1.0f, 1.0f, 1.0f);
	}
	MELON_INLINE static vec3f up()
	{
		return vec3f(0.0f, 1.0f, 0.0f);
	}
	MELON_INLINE static vec3f down()
	{
		return vec3f(0.0f, -1.0f, 0.0f);
	}
	MELON_INLINE static vec3f right()
	{
		return vec3f(1.0f, 0.0f, 0.0f);
	}
	MELON_INLINE static vec3f left()
	{
		return vec3f(-1.0f, 0.0f, 0.0f);
	}
	MELON_INLINE static vec3f forward()
	{
		return vec3f(0.0f, 0.0f, 1.0f);
	}
	MELON_INLINE static vec3f back()
	{
		return vec3f(0.0f, 0.0f, -1.0f);
	}
	MELON_INLINE static vec3f unit()
	{
		return vec3f(0.57735f, 0.57735f, 0.57735f);
	}

	MELON_INLINE constexpr vec3f cross(const vec3f& other) const
	{
		return vec3f(
			(y * other.z) - (z * other.y),
			(z * other.x) - (x * other.z),
			(x * other.y) - (y * other.x)
		);
	}

	MELON_INLINE constexpr float dot(const vec3f& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	MELON_INLINE constexpr float distance_squared(const vec3f& other) const
	{
		const float distanceX = x - other.x;
		const float distanceY = y - other.y;
		const float distanceZ = z - other.z;

		return distanceX * distanceX +
			distanceY * distanceY +
			distanceZ * distanceZ;
	}

	MELON_INLINE  float distance(const vec3f& other) const
	{
		return std::sqrt(distance_squared(other));
	}
	MELON_INLINE float magnitude_squared() const
	{
		return (dot(*this));
	}
	MELON_INLINE float magnitude() const
	{
		return std::sqrt(magnitude_squared());
	}

	MELON_INLINE vec3f normalize() const
	{
		const float m = magnitude();
		if (m <= FLT_EPSILON) return vec3f::zero();
		return vec3f(
			x / m,
			y / m,
			z / m
		);
	}

	MELON_INLINE bool is_normalized() const
	{
		return is_float_close_enough(magnitude_squared(), 1.0f);
	}

	MELON_INLINE vec3f lerp(const vec3f& other, const float t) const
	{
		const float clampedT = std::clamp(t, 0.0f, 1.0f);
		return vec3f(
			x + (clampedT * (other.x - x)),
			y + (clampedT * (other.y - y)),
			z + (clampedT * (other.z - z))
		);
	}

	MELON_INLINE float theta(const vec3f& other) const
	{
		const float magProduct = magnitude() * other.magnitude();
		if (magProduct <= FLT_EPSILON) return 0.0f;
		const float dotProd = dot(other) / magProduct;
		return std::acos(std::clamp(dotProd, -1.0f, 1.0f));
	}

	MELON_INLINE vec3f reflect(const vec3f& other) const
	{
		// reflection of zero vector is zero vector
		if (std::abs(magnitude()) < FLT_EPSILON || std::abs(other.magnitude()) < FLT_EPSILON) return *this;
		const vec3f n = other.is_normalized() ? other : other.normalize();
		// Clamp to avoid NaN from precision errors in acos
		const float s = 2.0f * dot(n);
		return *this - (n * s);
	}

	MELON_INLINE constexpr vec3f operator+(const vec3f& other) const
	{
		return vec3f(
			x + other.x,
			y + other.y,
			z + other.z
		);
	}


	MELON_INLINE constexpr vec3f operator-(const vec3f& other) const
	{
		return vec3f(
			x - other.x,
			y - other.y,
			z - other.z
		);
	}
	MELON_INLINE constexpr vec3f operator-() const
	{
		return vec3f(-x, -y, -z);
	}


	MELON_INLINE vec3f operator*(const vec3f& other) const
	{
		return vec3f(
			x * other.x,
			y * other.y,
			z * other.z
		);
	}

	MELON_INLINE vec3f operator*(const float s) const
	{
		return vec3f(
			x * s,
			y * s,
			z * s
		);
	}

	MELON_INLINE vec3f operator/(const float s) const
	{
		if (std::abs(s) <= FLT_EPSILON) return vec3f::zero();
		const float inv = 1.0f / s;
		return vec3f(x * inv, y * inv, z * inv);
	}

	MELON_INLINE bool operator ==(const vec3f& other) const
	{
		return (is_float_close_enough(x, other.x) && is_float_close_enough(y, other.y) && is_float_close_enough(z, other.z));
	}

	MELON_INLINE  static bool is_float_close_enough(const float a, const float b, const float precision = 1e-4f)
	{
		return std::abs(b - a) <= precision;
	}
};

MELON_INLINE vec3f operator*(const float s, const vec3f& v)
{
	return v * s;
}

#endif // MELONENGINE_VEC3F_H


