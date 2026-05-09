#ifndef MELONENGINE_DEBUG_H
#define MELONENGINE_DEBUG_H

#include <iostream>

enum class LogLevel {
    WARNING,
    ERROR,
    INFO
};

constexpr const char* ASCI_RED_ERROR = "\033[31m";
constexpr const char* ASCI_YELLOW_WARNING = "\033[33m";
constexpr const char* ASCI_RESET = "\033[0m";
constexpr const char* ASCI_BLUE_INFO = "\033[34m";


class Debug {
public:
    Debug() = default;
    ~Debug() = default;
    inline static void test_class() { std::cout << "Test Debug!\n";  };
    static auto Log(LogLevel logLevel, const char* msg) -> void;
private:
};


#endif //MELONENGINE_DEBUG_H
