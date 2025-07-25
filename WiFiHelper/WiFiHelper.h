#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <Arduino.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>

// Optional: You can keep SSID/PASSWORD definitions in a separate config file
#include "config.h"

class WiFiHelper
{
public:
    WiFiHelper(const char *ssid, const char *password, unsigned int portTCP = 12345, unsigned int portUDP = 8888);

    void begin();
    void maintainConnection();
    void printStatus();

    WiFiClient getClient();
    WiFiUDP *getUDP();
    WiFiServer *getServer();

private:
    const char *_ssid;
    const char *_password;
    unsigned int _portTCP;
    unsigned int _portUDP;

    WiFiUDP _udp;
    WiFiServer _server;
    WiFiClient _client;

    unsigned long _lastStatusCheck = 0;
    unsigned long _lastBroadcast = 0;

    void scanNetworks();
    void connectWiFi();
    void reconnectWiFi();
    void printIPDetails();
};

#endif
