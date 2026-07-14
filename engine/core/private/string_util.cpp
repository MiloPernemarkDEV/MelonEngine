#include "string_util.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "types.h"

std::string StringUtil::wide_to_multibyte(const std::wstring& wide) {
    if (wide.empty()) {
        return {};
    }
    const s32 size = WideCharToMultiByte(
        CP_UTF8,
        0,
        wide.c_str(),
        static_cast<int>(wide.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );

    std::string utf8(size, '\0');
    WideCharToMultiByte(
        CP_UTF8,
        0,
        wide.c_str(),
        static_cast<int>(wide.size()),
        utf8.data(),
        size,
        nullptr,
        nullptr
    );

    return utf8;
}

std::wstring StringUtil::utf8_to_wide(const std::string& utf8)
{
    if (utf8.empty())
    {
        return {};
    }

    const s32 size = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.c_str(),
        static_cast<int>(utf8.size()),
        nullptr,
        0
    );

    std::wstring wide(size, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.c_str(),
        static_cast<int>(utf8.size()),
        wide.data(),
        size
    );

    return wide;
}

#endif
