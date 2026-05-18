#ifndef MELON_PROJECTION_H
#define MELON_PROJECTION_H

#include "MathDefines.h"
#include "mat4.h"

// Projects a 3D point onto the axis by zeroing out the other axes
// m11(x), m22(y), m33(z)
MELON_INLINE static mat4 project_x() noexcept {
	mat4 r{};
	r.m22 = 0.0f;
	r.m33 = 0.0f;
	return r;
}
MELON_INLINE static mat4 project_y() noexcept {
	mat4 r{};
	r.m11 = 0.0f;
	r.m33 = 0.0f;
	return r;
}
MELON_INLINE static mat4 project_z() noexcept {
	mat4 r{};
	r.m11 = 0.0f;
	r.m22 = 0.0f;
	return r;
}
MELON_INLINE static mat4 project_xy() noexcept {
	mat4 r{};
	r.m33 = 0.0f;
	return r;
}

#endif // MELON_PROJECTION_H
