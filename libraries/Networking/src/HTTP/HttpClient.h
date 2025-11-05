#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "NetworkClient.h"
#include <Arduino.h>

class HttpClient
{
public:
    virtual ~HttpClient() {}

    virtual void beginRequest() = 0;
    virtual void endRequest() = 0;
    virtual void get(const String &path) = 0;
    virtual void post(const String &path) = 0;
    virtual void put(const String &path) = 0;
    virtual void del(const String &path) = 0;

    virtual void sendHeader(const char *name, const String &value) = 0;
    virtual void sendHeader(const char *name, int value) = 0;
    virtual void beginBody() = 0;
    virtual void print(const String &data) = 0;

    virtual int responseStatusCode() = 0;
    virtual String responseBody() = 0;
    virtual bool connected() const = 0;
};

#endif // HTTP_CLIENT_H
