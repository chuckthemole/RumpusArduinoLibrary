#include "RumpshiftLogger.h"

RumpshiftLogger::RumpshiftLogger(uint32_t baudRate, LogLevel level)
    : baudRate(baudRate), logLevel(level) {}

void RumpshiftLogger::begin()
{
    Serial.begin(baudRate);
    while (!Serial)
    {
        ; // Wait for Serial connection (optional, useful for boards with native USB)
    }
}

void RumpshiftLogger::setLevel(LogLevel level)
{
    logLevel = level;
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

void RumpshiftLogger::log(LogLevel level, const char *prefix, const String &msg)
{
    if (level <= logLevel && logLevel != LOG_LEVEL_NONE)
    {
        Serial.print("[");
        Serial.print(prefix);
        Serial.print("] ");
        Serial.println(msg);
    }
}
