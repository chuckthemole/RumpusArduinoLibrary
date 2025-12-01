#include "RumpshiftLogger.h"

RumpshiftLogger::RumpshiftLogger(
    uint32_t baudRate,
    LogLevel level,
    bool inColor)
    : _baudRate(baudRate),
      _logLevel(level),
      _inColor(inColor) {}

void RumpshiftLogger::begin()
{
    Serial.begin(_baudRate);

#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_NRF52) || defined(ARDUINO_ARCH_MBED)
    // Only wait on boards with native USB Serial
    uint32_t timeout = millis() + 5000; // max wait 5 seconds
    while (!Serial && millis() < timeout)
    {
        Serial.println("Waiting for Serial connection...");
        delay(250); // prevent tight loop
    }
#endif
}

void RumpshiftLogger::setLevel(LogLevel level)
{
    _logLevel = level;
}

void RumpshiftLogger::error(const String &msg)
{
    log(LOG_LEVEL_ERROR, "ERROR", msg);
}

void RumpshiftLogger::warn(const String &msg)
{
    log(LOG_LEVEL_WARN, "WARN", msg);
}

void RumpshiftLogger::info(const String &msg)
{
    log(LOG_LEVEL_INFO, "INFO", msg);
}

void RumpshiftLogger::debug(const String &msg)
{
    log(LOG_LEVEL_DEBUG, "DEBUG", msg);
}

String RumpshiftLogger::getTimestamp()
{
    unsigned long ms = millis();

    // Format: seconds.milliseconds
    unsigned long seconds = ms / 1000;
    unsigned long millisPart = ms % 1000;

    // Example output: "12.034s"
    char buf[16];
    snprintf(buf, sizeof(buf), "%lu.%03lus", seconds, millisPart);

    return String(buf);
}

void RumpshiftLogger::log(LogLevel level, const char *prefix, const String &msg)
{
    if (_logLevel == LOG_LEVEL_NONE || level > _logLevel)
        return;

    // Timestamp + prefix
    String fullMsg =
        "[" + String(prefix) + "] " +
        "[" + getTimestamp() + "] " +
        msg;

    // Optional colored output
    if (_inColor)
    {
        const char *color = COLOR_RESET;
        switch (level)
        {
        case LOG_LEVEL_ERROR:
            color = COLOR_RED;
            break;
        case LOG_LEVEL_WARN:
            color = COLOR_YELLOW;
            break;
        case LOG_LEVEL_INFO:
            color = COLOR_GREEN;
            break;
        case LOG_LEVEL_DEBUG:
            color = COLOR_BLUE;
            break;
        default:
            break;
        }
        Serial.print(color);
    }

    Serial.println(fullMsg);

    if (_inColor)
        Serial.print(COLOR_RESET);

    // Add to buffer
    logLine(fullMsg);

    // Callback
    if (_callback)
        _callback(fullMsg);
}
