#ifndef ENDO_LOGGING_HPP
#define ENDO_LOGGING_HPP

#include <fstream>
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
    static constexpr std::string_view ERROR_RAW = "[ERROR]";

    static constexpr std::string_view WARN = "\x1b[33m[ WARN]\x1b[0m";
    static constexpr std::string_view WARN_RAW = "[ WARN]";

    static constexpr std::string_view INFO = "\x1b[32m[ INFO]\x1b[0m";
    static constexpr std::string_view INFO_RAW = "[ INFO]";

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
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        std::println("{}: {}", Logger::INFO, msg);
        if (this->log_file) {
            std::println(*log_file, "{}: {}", Logger::INFO_RAW, msg);
        }
    }

    /// Logs a warning to stdout, and the log file if one is provided to the logger.
    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args) {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        std::println("{}: {}", Logger::WARN, msg);
        if (this->log_file) {
            std::println(*log_file, "{}: {}", Logger::WARN_RAW, msg);
        }
    }

    /// Logs an error message to stderr, and the log file if one is provided to the logger.
    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args) {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        std::println(stderr, "{}: {}", Logger::ERROR, msg);
        if (this->log_file) {
            std::println(*log_file, "{}: {}", Logger::ERROR_RAW, msg);
        }
    }
};

} // namespace Endo

#endif // ENDO_LOGGING_HPP
