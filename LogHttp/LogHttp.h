#ifndef LOGHTTP_H
#define LOGHTTP_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>

class LogHttp
{
public:
    // Ctors
    LogHttp(); // empty constructor, must set host/port later
    LogHttp(const String &host, uint16_t port = 80);

    // Setup methods for chaining
    LogHttp &setHost(const String &host);
    LogHttp &setPort(uint16_t port);
    LogHttp &setPath(const String &path);
    LogHttp &setContentType(const String &contentType);

    // Initialize client (optional, called in log() if needed)
    void begin();

    // Send log message, returns true if HTTP status 2xx
    bool log(const String &message);

private:
    String _host;
    uint16_t _port = 80;
    String _path = "/";
    String _contentType = "application/json";

    WiFiClient _wifiClient;
    HttpClient *_httpClient = nullptr;

    void cleanup();
};

#endif
