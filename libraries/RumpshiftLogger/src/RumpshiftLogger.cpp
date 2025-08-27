#include "RumpshiftLogger.h"

RumpshiftLogger::RumpshiftLogger(uint32_t baudRate, LogLevel level, bool color)
    : baudRate(baudRate), logLevel(level), inColor(color) {}

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
        if (inColor)
        {
            const char *color = "";

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
                color = COLOR_RESET;
                break;
            }

            Serial.print(color);
        }
        Serial.print("[");
        Serial.print(prefix);
        Serial.print("] ");
        Serial.println(msg);
        Serial.print(COLOR_RESET); // Reset after each message
    }
}
