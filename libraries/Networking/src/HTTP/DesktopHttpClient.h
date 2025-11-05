#ifndef RUMPUS_USE_ARDUINO_HTTP_CLIENT
#include "HttpClient.h"
#include <iostream>

class DesktopHttpClient : public HttpClient
{
public:
    DesktopHttpClient(NetworkClient &, const char *, uint16_t) {}
    DesktopHttpClient(NetworkClient &, IPAddress, uint16_t) {}

    void beginRequest() override {}
    void endRequest() override {}
    void get(const String &path) override { std::cout << "GET " << path.c_str() << "\n"; }
    void post(const String &path) override { std::cout << "POST " << path.c_str() << "\n"; }
    void put(const String &path) override { std::cout << "PUT " << path.c_str() << "\n"; }
    void del(const String &path) override { std::cout << "DELETE " << path.c_str() << "\n"; }

    void sendHeader(const char *, const String &) override {}
    void sendHeader(const char *, int) override {}
    void beginBody() override {}
    void print(const String &) override {}

    int responseStatusCode() override { return 200; }
    String responseBody() override { return "{}"; }
    bool connected() const override { return true; }
};
#endif
