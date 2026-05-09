#ifndef MELONENGINE_VEC4F_H
#define MELONENGINE_VEC4F_H

#include "MathDefines.h"
#include <cmath>
#include <algorithm>
#include "Mat4.h"
#include <cfloat>


struct Vec4f {

	float x, y, z, w;

	constexpr Vec4f(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	constexpr Vec4f()
		: x(0), y(0), z(0), w(0)
	{
	}
};

#endif // MELONENGINE_VEC4F_H


