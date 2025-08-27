#ifndef WIFI_CLIENT_WRAPPER_H
#define WIFI_CLIENT_WRAPPER_H

#include <WiFiClient.h>
#include <Arduino.h>
#include "NetworkClient.h"

/**
 * @class WiFiClientWrapper
 * @brief Wraps WiFiClient to conform to the extended NetworkClient interface.
 *
 * Responsibilities:
 *  - Maintains a WiFiClient instance for actual TCP communication.
 *  - Implements NetworkClient abstract methods.
 *  - Connection info (host/IP/port) is inherited from NetworkClient.
 */
class WiFiClientWrapper : public NetworkClient
{
public:
    WiFiClientWrapper() = default;
    explicit WiFiClientWrapper(WiFiClient client) : _client(client) {}

    WiFiClientWrapper &operator=(WiFiClient client)
    {
        _client = client;
        return *this;
    }

    void setClient(WiFiClient client) { _client = client; }

    /**
     * @brief Connect to the currently configured remote host/IP and port.
     * @return true if connection succeeded, false otherwise
     */
    bool connect()
    {
        if (_remoteHost)
            return _client.connect(_remoteHost, _remotePort);
        else if (_remoteIP)
            return _client.connect(_remoteIP, _remotePort);
        return false;
    }

    // ---------------------
    // Client method overrides
    // ---------------------
    int available() override { return _client.available(); }
    int read() override { return _client.read(); }
    size_t write(uint8_t data) override { return _client.write(data); }
    size_t write(const uint8_t *data, size_t size) override { return _client.write(data, size); }
    int peek() override { return _client.peek(); }
    void flush() override { _client.flush(); }
    void stop() override { _client.stop(); }
    int read(uint8_t *buf, size_t size) override { return _client.read(buf, size); }
    int connect(IPAddress ip, uint16_t port) override
    {
        return _client.connect(ip, port);
    }

    int connect(const char *host, uint16_t port) override
    {
        return _client.connect(host, port);
    }

    operator bool() override
    {
        return _client ? true : false;
    }

    uint8_t connected() override { return _client.connected(); }
    bool exists() const override { return true; } // Underlying _client always exists

private:
    WiFiClient _client; ///< Actual WiFi client used for TCP communication
};

#endif // WIFI_CLIENT_WRAPPER_H
