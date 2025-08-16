#ifndef LOGHTTP_H
#define LOGHTTP_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>
#include <QueueArray.h> // Add a simple queue library
#include "RumpshiftLogger.h"

class LogHttp
{
public:
    LogHttp();
    LogHttp(const String &host, uint16_t port = 80, RumpshiftLogger *logger = nullptr);

    LogHttp &setHost(const String &host);
    LogHttp &setPort(uint16_t port);
    LogHttp &setPath(const String &path);
    LogHttp &setContentType(const String &contentType);

    void begin();

    // Non-blocking log: enqueue message
    void log(const String &message);

    // Must be called in loop(): processes queue and retries failed messages
    void processQueue();

    // Optional: clear flash storage queue
    void clearStorage();

private:
    String _host;
    uint16_t _port = 80;
    String _path = "/";
    String _contentType = "application/json";

    WiFiClient _wifiClient;
    HttpClient *_httpClient = nullptr;

    QueueArray<String> _queue; // RAM queue for messages

    RumpshiftLogger *_logger = nullptr; // Pointer to your logger

    void cleanup();
    bool sendHttp(const String &message);      // send a single message
    void saveToStorage(const String &message); // save message if offline
    void loadFromStorage();                    // load stored messages into queue
};

#endif
