#ifndef MELON_VEC4F_H
#define MELON_VEC4F_H

#include "math_defines.h"
#include <cmath>
#include <algorithm>
#include "mat4.h"
#include <cfloat>


struct vec4f {

	float x, y, z, w;

	constexpr vec4f(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	constexpr vec4f()
		: x(0), y(0), z(0), w(0)
	{
	}
};

#endif // MELON_VEC4F_H


