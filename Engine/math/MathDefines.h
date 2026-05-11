#ifndef MELONENGINE_MATH_DEFINES_H
#define MELONENGINE_MATH_DEFINES_H

#if defined(_MSC_VER)
    #define MELON_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define MELON_INLINE inline __attribute__((always_inline))
#else
    #define MELON_INLINE inline
#endif

#endif // MELONENGINE_MATH_DEFINES_H