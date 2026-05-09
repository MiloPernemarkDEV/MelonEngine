#ifndef MELONENGINE_VEC3F_H
#define MELONENGINE_VEC3F_H

#include "MathDefines.h"
#include <cmath>
#include <algorithm>
#include <cfloat>


struct Vec3f {

	float x, y, z;

	constexpr Vec3f(float x, float y, float z)
		: x(x), y(y), z(z)
	{
	}

	constexpr Vec3f()
		: x(0), y(0), z(0)
	{
	}

	MELON_INLINE static Vec3f zero()
	{
		return Vec3f(0.0f, 0.0f, 0.0f);
	}
	MELON_INLINE static Vec3f one()
	{
		return Vec3f(1.0f, 1.0f, 1.0f);
	}
	MELON_INLINE static Vec3f up()
	{
		return Vec3f(0.0f, 1.0f, 0.0f);
	}
	MELON_INLINE static Vec3f down()
	{
		return Vec3f(0.0f, -1.0f, 0.0f);
	}
	MELON_INLINE static Vec3f right()
	{
		return Vec3f(1.0f, 0.0f, 0.0f);
	}
	MELON_INLINE static Vec3f left()
	{
		return Vec3f(-1.0f, 0.0f, 0.0f);
	}
	MELON_INLINE static Vec3f forward()
	{
		return Vec3f(0.0f, 0.0f, 1.0f);
	}
	MELON_INLINE static Vec3f back()
	{
		return Vec3f(0.0f, 0.0f, -1.0f);
	}
	MELON_INLINE static Vec3f unit()
	{
		return Vec3f(0.57735f, 0.57735f, 0.57735f);
	}

	MELON_INLINE constexpr Vec3f cross(const Vec3f& other) const
	{
		return Vec3f(
			(y * other.z) - (z * other.y),
			(z * other.x) - (x * other.z),
			(x * other.y) - (y * other.x)
		);
	}

	MELON_INLINE constexpr float dot(const Vec3f& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	MELON_INLINE constexpr float distance_squared(const Vec3f& other) const
	{
		const float distanceX = x - other.x;
		const float distanceY = y - other.y;
		const float distanceZ = z - other.z;

		return distanceX * distanceX +
			distanceY * distanceY +
			distanceZ * distanceZ;
	}

	MELON_INLINE  float distance(const Vec3f& other) const
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

	MELON_INLINE Vec3f normalize() const
	{
		const float m = magnitude();
		if (m <= FLT_EPSILON) return Vec3f::zero();
		return Vec3f(
			x / m,
			y / m,
			z / m
		);
	}

	MELON_INLINE bool is_normalized() const
	{
		return is_float_close_enough(magnitude_squared(), 1.0f);
	}

	MELON_INLINE Vec3f lerp(const Vec3f& other, const float t) const
	{
		const float clampedT = std::clamp(t, 0.0f, 1.0f);
		return Vec3f(
			x + (clampedT * (other.x - x)),
			y + (clampedT * (other.y - y)),
			z + (clampedT * (other.z - z))
		);
	}

	MELON_INLINE float theta(const Vec3f& other) const
	{
		const float magProduct = magnitude() * other.magnitude();
		if (magProduct <= FLT_EPSILON) return 0.0f;
		const float dotProd = dot(other) / magProduct;
		return std::acos(std::clamp(dotProd, -1.0f, 1.0f));
	}

	MELON_INLINE Vec3f reflect(const Vec3f& other) const
	{
		// reflection of zero vector is zero vector
		if (std::abs(magnitude()) < FLT_EPSILON || std::abs(other.magnitude()) < FLT_EPSILON) return *this;
		const Vec3f n = other.is_normalized() ? other : other.normalize();
		// Clamp to avoid NaN from precision errors in acos
		const float s = 2.0f * dot(n);
		return *this - (n * s);
	}

	MELON_INLINE constexpr Vec3f operator+(const Vec3f& other) const
	{
		return Vec3f(
			x + other.x,
			y + other.y,
			z + other.z
		);
	}


	MELON_INLINE constexpr Vec3f operator-(const Vec3f& other) const
	{
		return Vec3f(
			x - other.x,
			y - other.y,
			z - other.z
		);
	}
	MELON_INLINE constexpr Vec3f operator-() const
	{
		return Vec3f(-x, -y, -z);
	}


	MELON_INLINE Vec3f operator*(const Vec3f& other) const
	{
		return Vec3f(
			x * other.x,
			y * other.y,
			z * other.z
		);
	}

	MELON_INLINE Vec3f operator*(const float s) const
	{
		return Vec3f(
			x * s,
			y * s,
			z * s
		);
	}

	MELON_INLINE Vec3f operator/(const float s) const
	{
		if (std::abs(s) <= FLT_EPSILON) return Vec3f::zero();
		const float inv = 1.0f / s;
		return Vec3f(x * inv, y * inv, z * inv);
	}

	MELON_INLINE bool operator ==(const Vec3f& other) const
	{
		return (is_float_close_enough(x, other.x) && is_float_close_enough(y, other.y) && is_float_close_enough(z, other.z));
	}

	MELON_INLINE  static bool is_float_close_enough(const float a, const float b, const float precision = 1e-4f)
	{
		return std::abs(b - a) <= precision;
	}
};

MELON_INLINE Vec3f operator*(const float s, const Vec3f& v)
{
	return v * s;
}

#endif // MELONENGINE_VEC3F_H


