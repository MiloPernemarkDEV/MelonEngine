#ifndef MELON_LOGGER_H
#define MELON_LOGGER_H

#include <string_view>
#include <string>
#include <iostream>
#include <format>
#include <utility>
#include <cstdlib>

enum LogLevel { Info, Warning, Error };

// Fixed ODR violation by making these constexpr
inline constexpr const char* LOG_COLOR_RED = "\033[31m";
inline constexpr const char* LOG_COLOR_YELLOW = "\033[33m";
inline constexpr const char* LOG_COLOR_CYAN = "\033[36m";
inline constexpr const char* LOG_COLOR_RESET = "\033[0m";

class Logger
{
public:
    template<typename... Args>
    static void Log(LogLevel level, std::string_view fmt, Args&&... args)
    {
        std::string msg = std::vformat(fmt, std::make_format_args(args...));

        const char* log_color{ nullptr };
        const char* level_str{ nullptr };

        switch (level)
        {
        case LogLevel::Info:    log_color = LOG_COLOR_CYAN;   level_str = "INFO";    break;
        case LogLevel::Warning: log_color = LOG_COLOR_YELLOW; level_str = "WARNING"; break;
        case LogLevel::Error:   log_color = LOG_COLOR_RED;    level_str = "ERROR";   break;
        default:                log_color = LOG_COLOR_RESET;  level_str = "LOG";     break;
        }

        std::cout << log_color << "[" << level_str << "] " << msg << LOG_COLOR_RESET << "\n";
    }

#if !defined(NDEBUG) 
    template<typename... Args>
    static void Info(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(LogLevel::Info, fmt.get(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warning(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::Warning, fmt.get(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::Error, fmt.get(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Abort(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::Error, fmt.get(), std::forward<Args>(args)...);
        std::abort();
    }
#else 
    template<typename... Args> static void Info(std::format_string<Args...>, Args&&...) {}
    template<typename... Args> static void Warning(std::format_string<Args...>, Args&&...) {}
    template<typename... Args> static void Error(std::format_string<Args...>, Args&&...) {}

    template<typename... Args>
    [[noreturn]] static void Abort(std::format_string<Args...> fmt, Args&&... args) {
        std::abort(); 
    }
#endif
};

#if defined(NDEBUG)
#define ME_LOG(LogLevel, format, ...) ((void)0)
#else 
#define ME_LOG(level, format, ...) \
    Logger::Log(level, format, ##__VA_ARGS__)
#endif

#endif // MELON_LOGGER_H