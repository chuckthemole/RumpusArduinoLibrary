#ifndef APIFETCHER_H
#define APIFETCHER_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

// Base abstract interface
class ApiClient
{
public:
    virtual bool fetch() = 0;                     // fetch data from server
    virtual void process(const String &json) = 0; // parse JSON
    virtual void display() = 0;                   // display results
    virtual ~ApiClient() {}
};

// HTTP fetcher for making requests
class HttpFetcher
{
public:
    HttpFetcher(const String &url);
    void setUrl(const String &url);
    String get();

private:
    String _url;
    String _host;
    String _path;
    uint16_t _port;
};

#endif
