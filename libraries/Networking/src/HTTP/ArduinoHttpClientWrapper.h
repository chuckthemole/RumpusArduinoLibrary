#ifdef RUMPUS_USE_ARDUINO_HTTP_CLIENT
#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include "HttpClient.h"

class ArduinoHttpClientWrapper : public HttpClient
{
public:
    ArduinoHttpClientWrapper(NetworkClient &client, const char *host, uint16_t port)
        : _client(client), _http(client, host, port) {}

    void beginRequest() override { _http.beginRequest(); }
    void endRequest() override { _http.endRequest(); }
    void get(const String &path) override { _http.get(path); }
    void post(const String &path) override { _http.post(path); }
    void put(const String &path) override { _http.put(path); }
    void del(const String &path) override { _http.del(path); }

    void sendHeader(const char *name, const String &value) override { _http.sendHeader(name, value); }
    void sendHeader(const char *name, int value) override { _http.sendHeader(name, value); }
    void beginBody() override { _http.beginBody(); }
    void print(const String &data) override { _http.print(data); }

    int responseStatusCode() override { return _http.responseStatusCode(); }
    String responseBody() override { return _http.responseBody(); }
    bool connected() const override { return _http.connected(); }

private:
    NetworkClient &_client;
    ::HttpClient _http;
};
#endif
