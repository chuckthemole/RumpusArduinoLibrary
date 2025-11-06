#pragma once
#include <Arduino.h>
#include <deque>

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

/**
 * @file RumpshiftLogger.h
 * @brief A simple logging utility for Arduino projects with configurable log levels.
 *
 * Supports Serial output, color coding, and storing a fixed number of recent log lines.
 * Only logs that meet the current log level are stored in the internal buffer.
 *
 * Usage:
 *   RumpshiftLogger logger(115200, LOG_LEVEL_DEBUG, true);
 *   logger.begin();
 *   logger.info("System initialized");
 */

/// Log levels in increasing verbosity
enum LogLevel
{
    LOG_LEVEL_NONE = 0, ///< No logging
    LOG_LEVEL_ERROR,    ///< Only errors
    LOG_LEVEL_WARN,     ///< Errors and warnings
    LOG_LEVEL_INFO,     ///< Errors, warnings, and info messages
    LOG_LEVEL_DEBUG     ///< All messages, including debug details
};

class RumpshiftLogger
{
public:
    /**
     * @brief Construct a new RumpshiftLogger instance.
     * @param baudRate Serial baud rate (default: 9600)
     * @param level Initial log level (default: LOG_LEVEL_INFO)
     * @param inColor Use ANSI color codes in Serial output (default: false)
     */
    RumpshiftLogger(uint32_t baudRate = 9600, LogLevel level = LOG_LEVEL_INFO, bool inColor = false);

    /// Initialize Serial; call in setup()
    void begin();

    /// Set a new log level at runtime
    void setLevel(LogLevel level);

    /// Log an error message
    void error(const String &msg);
    template <typename T>
    void error(const T &value) { error(String(value)); }

    /// Log a warning message
    void warn(const String &msg);
    template <typename T>
    void warn(const T &value) { warn(String(value)); }

    /// Log an informational message
    void info(const String &msg);
    template <typename T>
    void info(const T &value) { info(String(value)); }

    /// Log a debug message
    void debug(const String &msg);
    template <typename T>
    void debug(const T &value) { debug(String(value)); }

    /// Get all stored log lines concatenated into a single String (for LVGL display, etc.)
    String getLogText() const
    {
        String buffer;
        for (const auto &line : _logLines)
            buffer += line + "\n";
        return buffer;
    }

private:
    uint32_t _baudRate;                  ///< Serial baud rate
    LogLevel _logLevel;                  ///< Current logging level
    bool _inColor;                       ///< Enable color in Serial output
    static const int MAX_LOG_LINES = 50; ///< Maximum stored log lines
    std::deque<String> _logLines;        ///< Stores recent logs

    /**
     * @brief Core log function. Prints to Serial and stores in buffer if allowed by log level.
     * @param level The message severity level
     * @param prefix Text prefix (e.g., "INFO", "ERROR")
     * @param msg The message content
     */
    void log(LogLevel level, const char *prefix, const String &msg);

    /// Add a log line to the internal buffer (maintains MAX_LOG_LINES)
    void logLine(const String &msg)
    {
        if (_logLines.size() >= MAX_LOG_LINES)
            _logLines.pop_front();
        _logLines.push_back(msg);
    }
};
