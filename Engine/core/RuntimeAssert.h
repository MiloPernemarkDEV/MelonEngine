#ifndef MELONENGINE_MELONASSERT_H
#define MELONENGINE_MELONASSERT_H

#pragma once

void HandleAssert(const char* expr, const char* msg, const char* file, int line);

#define MELON_ASSERT(x, msg) \
if (!(x)) HandleAssert(#x, msg, __FILE__, __LINE__)

#define NULL_PTR_ERROR_MSG "ERROR::NULL_PTR::DEREFERENCE"

#endif //MELONENGINE_MELONASSERT_H
