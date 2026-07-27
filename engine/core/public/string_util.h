#pragma once

#include <string>

class StringUtil {
public:
    static std::string wide_to_multibyte(const std::wstring &wide);

    static std::wstring utf8_to_wide(const std::string &utf8);
};

