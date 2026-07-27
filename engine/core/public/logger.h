#ifndef MELON_LOGGER_H
#define MELON_LOGGER_H

#include <string_view>
#include <string>
#include <iostream>
#include <format>
#include <utility>
#include <cstdlib>
#include <exception>
#include <source_location>

#include "time_util.h"

constexpr std::string_view get_file_name(std::string_view path) {
    auto pos = path.find_last_of("/\\");
    return (pos == std::string_view::npos) ? path : path.substr(pos + 1);
}

enum LogLevel { Info, Warning, Error };

inline constexpr auto* LOG_COLOR_RED    = "\033[31m";
inline constexpr auto* LOG_COLOR_GREEN  = "\033[32m";
inline constexpr auto* LOG_COLOR_YELLOW = "\033[33m";
inline constexpr auto* LOG_COLOR_CYAN   = "\033[36m";
inline constexpr auto* LOG_COLOR_GREY = "\033[38;5;244m";
inline constexpr auto* LOG_COLOR_RESET  = "\033[0m";

class Logger {
public:
    template<typename... Args>
    static void Log(LogLevel level, const std::source_location loc, std::format_string<Args...> fmt, Args&&... args) noexcept
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);

        const char *log_color = LOG_COLOR_RESET;
        const char *level_str = "LOG";

        switch (level) {
            case LogLevel::Info:    log_color = LOG_COLOR_GREEN;   level_str = "INFO";    break;
            case LogLevel::Warning: log_color = LOG_COLOR_YELLOW; level_str = "WARNING"; break;
            case LogLevel::Error:   log_color = LOG_COLOR_RED;    level_str = "ERROR";   break;
        }

        TimeUtil::SysClock time{};
        TimeUtil::get_system_clock(time);

        auto filename = get_file_name(loc.file_name());

        std::string stamp = std::format("{:04}-{:02}-{:02}T{:02}:{:02}:{:02}",time.year, time.month, time.day, time.hour, time.minute, time.second);

        std::cout << LOG_COLOR_GREY
                  << stamp << " "
                  << log_color
                  << level_str
                  << LOG_COLOR_GREY
                  << " [" << filename << ":" << loc.line() << "]"
                  << LOG_COLOR_RESET << " "
                  << msg << "\n"
                  << std::flush;

    }

#if !defined(NDEBUG)
    template<typename... Args>
    static void Info(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warning(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::Warning, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }
#else
    template<typename... Args> static void Info(std::format_string<Args...>, Args&&...) {}
    template<typename... Args> static void Warning(std::format_string<Args...>, Args&&...) {}
    template<typename... Args> static void Error(std::format_string<Args...>, Args&&...) {}
#endif

    template<typename... Args>
    [[noreturn]] static void Abort(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::Error, fmt, std::forward<Args>(args)...);
        std::abort();
    }
};

#if defined(NDEBUG)
#define ME_LOG(level, format, ...) ((void)0)
#else
#define ME_LOG(level, format, ...) \
    Logger::Log(LogLevel::level, std::source_location::current(), format, ##__VA_ARGS__)
#endif

#endif // MELON_LOGGER_H
