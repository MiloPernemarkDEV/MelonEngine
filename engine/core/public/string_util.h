#ifndef MELON_STRING_UTIL_H
#define MELON_STRING_UTIL_H

#include <string>

class StringUtil {
public:
    static std::string wide_to_utf8(const std::wstring& wide);
    static std::wstring utf8_to_wide(const std::string& utf8);
};

#endif // MELON_STRING_UTIL_H