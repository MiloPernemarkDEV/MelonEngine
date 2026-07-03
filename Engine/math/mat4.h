#ifndef MELON_MAT4X4_H
#define MELON_MAT4X4_H

#include <cmath>
#include <ostream>
#include "math_defines.h"
#include "math_util.h"
#include "vec2f.h"
#include "vec3f.h"
#include <glm/mat4x4.hpp>


struct mat4 {
	float m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44;

	// Default Constructs to identity matrix
	// The identity matrix is the equivalent of 1 in scalar multiplication
	MELON_INLINE mat4()
		: m11(1.0f), m12(0.0f), m13(0.0f), m14(0.0f),
		m21(0.0f), m22(1.0f), m23(0.0f), m24(0.0f),
		m31(0.0f), m32(0.0f), m33(1.0f), m34(0.0f),
		m41(0.0f), m42(0.0f), m43(0.0f), m44(1.0f)
	{

	}

	// Transposing a matrix flips it diagonally
	// Because we use row major vectors and opengl uses column vectors
	// We need to transpose the matrices before we send them to the GPU
	MELON_INLINE void transpose() noexcept {
		mat4 r;

		r.m11 = m11; r.m12 = m21; r.m13 = m31; r.m14 = m41;
		r.m21 = m12; r.m22 = m22; r.m23 = m32; r.m24 = m42;
		r.m31 = m13; r.m32 = m23; r.m33 = m33; r.m34 = m43;
		r.m41 = m14; r.m42 = m24; r.m43 = m34; r.m44 = m44;

		*this = r;
	}

	// Each element is computed as a dot product between a row of 'this'
	// and a column of the other matrix
	MELON_INLINE mat4 operator*(const mat4& o) const noexcept
	{
		mat4 r{};
		r.m11 = m11 * o.m11 + m12 * o.m21 + m13 * o.m31 + m14 * o.m41;
		r.m12 = m11 * o.m12 + m12 * o.m22 + m13 * o.m32 + m14 * o.m42;
		r.m13 = m11 * o.m13 + m12 * o.m23 + m13 * o.m33 + m14 * o.m43;
		r.m14 = m11 * o.m14 + m12 * o.m24 + m13 * o.m34 + m14 * o.m44;

		r.m21 = m21 * o.m11 + m22 * o.m21 + m23 * o.m31 + m24 * o.m41;
		r.m22 = m21 * o.m12 + m22 * o.m22 + m23 * o.m32 + m24 * o.m42;
		r.m23 = m21 * o.m13 + m22 * o.m23 + m23 * o.m33 + m24 * o.m43;
		r.m24 = m21 * o.m14 + m22 * o.m24 + m23 * o.m34 + m24 * o.m44;

		r.m31 = m31 * o.m11 + m32 * o.m21 + m33 * o.m31 + m34 * o.m41;
		r.m32 = m31 * o.m12 + m32 * o.m22 + m33 * o.m32 + m34 * o.m42;
		r.m33 = m31 * o.m13 + m32 * o.m23 + m33 * o.m33 + m34 * o.m43;
		r.m34 = m31 * o.m14 + m32 * o.m24 + m33 * o.m34 + m34 * o.m44;

		r.m41 = m41 * o.m11 + m42 * o.m21 + m43 * o.m31 + m44 * o.m41;
		r.m42 = m41 * o.m12 + m42 * o.m22 + m43 * o.m32 + m44 * o.m42;
		r.m43 = m41 * o.m13 + m42 * o.m23 + m43 * o.m33 + m44 * o.m43;
		r.m44 = m41 * o.m14 + m42 * o.m24 + m43 * o.m34 + m44 * o.m44;

		return r;
	}

	// Todo: Refactor
	// Have not yet learned how to calculate the determinant of a 4x4 matrix
	// So for now i make use of sweet glm and transpose my row major matrix
	// As glm uses column major
	MELON_INLINE static float determinant(mat4& m) {

	}

	// singular and noninvertable can not be inversed
	// nonsingular and invertible can be inversed,
	MELON_INLINE static bool isInvertible(mat4& m) {
		return determinant(m) != 0.0f;
	}

	MELON_INLINE bool isInvertible() {
		return determinant(*this) != 0.0f;
	}

	MELON_INLINE mat4 cofactor() {

	}

	// Classical adjoint matrix inverse
	MELON_INLINE mat4 inverse() {
		MELON_ASSERT(isInvertible(*this), "Tried to inverse singular matrix");



	}

	MELON_INLINE static mat4 inverse(mat4& m) {
		MELON_ASSERT(isInvertible(m), "Tried to inverse singular matrix");
	}

	MELON_INLINE mat4 operator*(const float s) const noexcept
	{
		mat4 r = *this;
		r.m11 *= s; r.m12 *= s; r.m13 *= s; r.m14 *= s;
		r.m21 *= s; r.m22 *= s; r.m23 *= s; r.m24 *= s;
		r.m31 *= s; r.m32 *= s; r.m33 *= s; r.m34 *= s;
		r.m41 *= s; r.m42 *= s; r.m43 *= s; r.m44 *= s;
		return r;
	}

	MELON_INLINE bool operator==(const mat4& m) const noexcept
	{
		return
			isNearlyEqual(m11, m.m11) && isNearlyEqual(m12, m.m12) &&
			isNearlyEqual(m13, m.m13) && isNearlyEqual(m14, m.m14) &&

			isNearlyEqual(m21, m.m21) && isNearlyEqual(m22, m.m22) &&
			isNearlyEqual(m23, m.m23) && isNearlyEqual(m24, m.m24) &&

			isNearlyEqual(m31, m.m31) && isNearlyEqual(m32, m.m32) &&
			isNearlyEqual(m33, m.m33) && isNearlyEqual(m34, m.m34) &&

			isNearlyEqual(m41, m.m41) && isNearlyEqual(m42, m.m42) &&
			isNearlyEqual(m43, m.m43) && isNearlyEqual(m44, m.m44);
	}

	MELON_INLINE static mat4 createScale2D
	(const vec2f& v) noexcept {
		mat4 r = {};
		r.m11 = v.x;
		r.m22 = v.y;

		return r;
	}

	// uniform and nonuniform scaling
	MELON_INLINE static mat4 scale(const vec3f& v) noexcept {
		mat4 r{};
		r.m11 = v.x;
		r.m22 = v.y;
		r.m33 = v.z;
		return r;
	}

	// Counterclockwise rotation = positive, clockwise rotation = negative
	MELON_INLINE static mat4 rotate2D(const float angle) noexcept {
		const float cosAngle = std::cos(angle);
		const float sinAngle = std::sin(angle);

		mat4 r{};

		r.m11 = cosAngle; r.m12 = -sinAngle;
		r.m21 = sinAngle; r.m22 = cosAngle;

		return r;
	}

	MELON_INLINE static mat4 euler(const vec3f& v) noexcept {
		const float cosX = std::cos(v.x);
		const float sinX = std::sin(v.x);

		const float cosY = std::cos(v.y);
		const float sinY = std::sin(v.y);

		const float cosZ = std::cos(v.z);
		const float sinZ = std::sin(v.z);

		mat4 r{};

		r.m11 = cosY * cosZ + sinY * sinX * sinZ;
		r.m12 = cosZ * sinY * sinX - cosY * sinZ;
		r.m13 = cosX * sinY;

		r.m21 = cosX * sinZ;
		r.m22 = cosX * cosZ;
		r.m23 = -sinX;

		r.m31 = cosY * sinX * sinZ - cosZ * sinY;
		r.m32 = sinY * sinZ + cosY * cosZ * sinX;
		r.m33 = cosX * cosY;

		return r;
	}

	MELON_INLINE static mat4 reflectPlane(const vec3f& n) noexcept {
		mat4 r{};

		r.m11 = 1.0f - 2.0f * n.x * n.x;
		r.m22 = 1.0f - 2.0f * n.y * n.y;
		r.m33 = 1.0f - 2.0f * n.z * n.z;

		r.m12 = -2.0f * n.x * n.y;
		r.m21 = -2.0f * n.y * n.x;

		r.m13 = -2.0f * n.x * n.z;
		r.m31 = -2.0f * n.z * n.x;

		r.m23 = -2.0f * n.y * n.z;
		r.m32 = -2.0f * n.z * n.y;

		return r;
	}

	// reflecting the x-axis can be used for optimizing assets
	// only left side of models can be loaded then we can reflect the right side
	MELON_INLINE static mat4 reflectX() noexcept {
		mat4 r{};

		r.m11 = -1.0f;
		r.m22 = 1.0f;
		r.m33 = 1.0f;

		return r;
	}
	// reflecting the y-axis can be used for water surface reflection but it's limited in usage
	// much more performant than plane reflection though
	MELON_INLINE static mat4 reflectY() noexcept {
		mat4 r{};
		r.m11 = 1.0f;
		r.m22 = -1.0f;
		r.m33 = 1.0f;
		return r;
	}

	MELON_INLINE static mat4 reflectZ() noexcept {
		mat4 r{};
		r.m11 = 1.0f;
		r.m22 = 1.0f;
		r.m33 = -1.0f;
		return r;
	}
};

MELON_INLINE mat4 operator*(const float s, const mat4& m) noexcept
{
	return m * s;
}

std::ostream& operator<<(std::ostream& out_stream, const mat4& a);

#endif // MELON_MAT4X4_H

