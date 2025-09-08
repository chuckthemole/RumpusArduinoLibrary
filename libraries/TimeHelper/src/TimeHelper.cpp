#include "TimeHelper.h"
#include <time.h>

TimeHelper::TimeHelper(const char *ntpServer, long utcOffset, unsigned long updateInterval)
    : timeClient(ntpUDP, ntpServer, utcOffset, updateInterval)
{
}

void TimeHelper::begin()
{
    timeClient.begin();
    timeClient.update();
    captureStartTime();
}

void TimeHelper::update()
{
    timeClient.update();
}

String TimeHelper::getUTCTimeISO()
{
    time_t rawTime = timeClient.getEpochTime();
    struct tm *timeInfo = gmtime(&rawTime);

    char buffer[25];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%02d:%02d:%02dZ",
             timeInfo->tm_year + 1900,
             timeInfo->tm_mon + 1,
             timeInfo->tm_mday,
             timeInfo->tm_hour,
             timeInfo->tm_min,
             timeInfo->tm_sec);

    return String(buffer);
}

String TimeHelper::getStartTimeISO()
{
    return startTimeISO;
}

void TimeHelper::captureStartTime()
{
    startTimeISO = getUTCTimeISO();
}
