#ifndef MELON_DEFINES_H
#define MELON_DEFINES_H

#include <cstddef>

#ifdef WIN32
#define PLATFORM_WINDOWS
#define PLATFORM_USE_WIN32
#endif 

#if defined(WIN32) && defined(USE_GLFW)
#define PLATFORM_WINDOWS
#define PLATFORM_USE_GLFW
#define 
#endif

#ifdef __linux__
#define PLATFORM_LINUX
#define PLATFORM_USE_GLFW
#endif

#ifdef __APPLE__
#define PLATFORM_APPLE
#define PLATFORM_USE_GLFW
#endif

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

// constant expressions

constexpr int operator""_MB(unsigned long long int x) {
	return x * 1024 * 1024;
}

#endif 