#pragma once
#include <Arduino.h>

/**
 * @file RumpshiftLogger.h
 * @brief A simple logging utility for Arduino projects with configurable log levels.
 *
 * This logger allows you to control log verbosity (e.g., DEBUG, INFO, WARN, ERROR)
 * and centralize your Serial printing. It is designed for easy production/development
 * environment switching.
 *
 * Usage:
 *   RumpshiftLogger logger(115200, LOG_LEVEL_DEBUG);
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
     * @param baudRate Baud rate for Serial communication (default: 9600).
     * @param level Initial logging level (default: LOG_LEVEL_INFO).
     */
    RumpshiftLogger(uint32_t baudRate = 9600, LogLevel level = LOG_LEVEL_INFO);

    /**
     * @brief Initialize Serial communication for logging.
     * Call this in `setup()` before using log functions.
     */
    void begin();

    /**
     * @brief Change the log level at runtime.
     * @param level New log level.
     */
    void setLevel(LogLevel level);

    /// Log an error message.
    void error(const String &msg);

    /// Log a warning message.
    void warn(const String &msg);

    /// Log an informational message.
    void info(const String &msg);

    /// Log a debug message.
    void debug(const String &msg);

private:
    uint32_t baudRate; ///< Serial baud rate.
    LogLevel logLevel; ///< Current logging level.

    /**
     * @brief Internal log function to handle message printing.
     * @param level The severity level of the message.
     * @param prefix Text prefix for the message (e.g., "ERROR").
     * @param msg The message content.
     */
    void log(LogLevel level, const char *prefix, const String &msg);
};
