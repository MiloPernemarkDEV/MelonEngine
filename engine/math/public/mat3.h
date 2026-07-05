#ifndef MELON_MAT3X3_H
#define MELON_MAT3X3_H

#include "../math_defines.h"
#include "../Mat4.h"

struct mat3 {
	float m11, m12, m13,
		  m21, m22, m23,
		  m31, m32, m33;

	MELON_INLINE mat3()
		:m11(1.0f), m12(0.0f), m13(0.0f),
		 m21(0.0f), m22(1.0f), m23(0.0f),
		 m31(0.0f), m32(0.0f), m33(1.0f) {

	}

	// Todo:
	// Find the row or column with most 0 and use it
	MELON_INLINE static float determinant(const mat3& m) {
		return m.m11 * (m.m22 * m.m33 - m.m32 * m.m23)
			 - m.m12 * (m.m21 * m.m33 - m.m31 * m.m23)
			 + m.m13 * (m.m21 * m.m32 - m.m31 * m.m22);
	}

	MELON_INLINE static float determinant(const Mat4& m) {
		return m.m11 * (m.m22 * m.m33 - m.m32 * m.m23)
			- m.m12 * (m.m21 * m.m33 - m.m31 * m.m23)
			+ m.m13 * (m.m21 * m.m32 - m.m31 * m.m22);
	}
};

#endif // MELON_MAT3X3_H


