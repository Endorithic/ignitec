#ifndef ENDO_LOGGING_HPP
#define ENDO_LOGGING_HPP

#include <fstream>
#include <iostream>
#include <optional>
#include <format>
#include <print>

namespace Endo {

/// Used for printing log messages
/// `Info` and `Warn` both send their messages to stdout, while `Error` sends to stderr.
/// Can optionally be constructed using a filepath for outputting messaages to a log path.
class Logger {
private:
    static constexpr std::string_view ERROR = "\x1b[31m[ERROR]\x1b[0m";
    static constexpr std::string_view WARN = "\x1b[33m[ WARN]\x1b[0m";
    static constexpr std::string_view INFO = "\x1b[32m[ INFO]\x1b[0m";

    std::optional<std::ofstream> log_file;

public:
    /// Creates a new `Logger` instance that only logs to stdout and stderr.
    Logger();

    /// Creates a new `Logger` instance that logs to stdout, stderr, and the provided log file.
    Logger(const std::string& path);

    /// Delete unwanted CTORs, mainly to ensure the ofstream is not attempted to be copied
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    /// Logs an info message to stdout, and the log file if one is provided to the logger.
    template <typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args) {
        std::println("{}: {}", Logger::INFO, std::format(fmt, std::forward<Args>(args)...));
        if (this->log_file) {
            std::println(*log_file, "{}: {}", Logger::INFO, std::format(fmt, std::forward<Args>(args)...));
        }
    }

    /// Logs a warning to stdout, and the log file if one is provided to the logger.
    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args) {
        std::println("{}: {}", Logger::WARN, std::format(fmt, std::forward<Args>(args)...));
        if (this->log_file) {
            std::println(*log_file, "{}: {}", Logger::WARN, std::format(fmt, std::forward<Args>(args)...));
        }
    }

    /// Logs an error message to stderr, and the log file if one is provided to the logger.
    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args) {
        std::println(std::cerr, "{}: {}", Logger::ERROR, std::format(fmt, std::forward<Args>(args)...));
        if (this->log_file) {
            std::println(*log_file, "{}: {}", Logger::ERROR, std::format(fmt, std::forward<Args>(args)...));
        }
    }
};

}

#endif
