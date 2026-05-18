#ifndef MELONENGINE_MATH_UTIL_H
#define MELONENGINE_MATH_UTIL_H

#include "MathDefines.h"
#include <cmath>
#include "vec3f.h"


MELON_INLINE static bool isNearlyEqual(const float a, const float b, const float epsilon = 1e-5f)
{
	return std::abs(a - b) <= epsilon;
}

MELON_INLINE static vec3f normalize(const vec3f& v) noexcept
{
    const float length_sq = v.x * v.x + v.y * v.y + v.z * v.z;

    // Avoid division by zero
    if (length_sq <= 0.00001f)
        return vec3f(0.0f, 0.0f, 0.0f);

    const float inv_len = 1.0f / std::sqrt(length_sq);

    return vec3f(
        v.x * inv_len,
        v.y * inv_len,
        v.z * inv_len
    );
}

MELON_INLINE static vec3f cross(const vec3f& a, const vec3f& b) noexcept
{
    return vec3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

MELON_INLINE static float dot(const vec3f& a, const vec3f& b) noexcept
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

#endif // MELONENGINE_MATH_UTIL_H



