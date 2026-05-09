#ifndef MELONENGINE_MATH_UTIL_H
#define MELONENGINE_MATH_UTIL_H

#include "MathDefines.h"
#include <cmath>
#include "Vec3f.h"


MELON_INLINE static bool isNearlyEqual(const float a, const float b, const float epsilon = 1e-5f)
{
	return std::abs(a - b) <= epsilon;
}

MELON_INLINE static Vec3f normalize(const Vec3f& v) noexcept
{
    const float length_sq = v.x * v.x + v.y * v.y + v.z * v.z;

    // Avoid division by zero
    if (length_sq <= 0.00001f)
        return Vec3f(0.0f, 0.0f, 0.0f);

    const float inv_len = 1.0f / std::sqrt(length_sq);

    return Vec3f(
        v.x * inv_len,
        v.y * inv_len,
        v.z * inv_len
    );
}

MELON_INLINE static Vec3f cross(const Vec3f& a, const Vec3f& b) noexcept
{
    return Vec3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

MELON_INLINE static float dot(const Vec3f& a, const Vec3f& b) noexcept
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

#endif // MELONENGINE_MATH_UTIL_H



