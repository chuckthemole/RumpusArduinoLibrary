#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

class TimeHelper
{
public:
    // Constructor: specify NTP server, timezone offset in seconds, and update interval
    TimeHelper(const char *ntpServer = "pool.ntp.org", long utcOffset = 0, unsigned long updateInterval = 60000);

    void begin();
    void update();
    String getUTCTimeISO();
    String getStartTimeISO(); // returns the ISO time captured at begin()

private:
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    String startTimeISO;
    void captureStartTime();
};
