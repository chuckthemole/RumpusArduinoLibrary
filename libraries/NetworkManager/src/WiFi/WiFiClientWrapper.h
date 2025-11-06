#ifndef WIFI_CLIENT_WRAPPER_H
#define WIFI_CLIENT_WRAPPER_H

#include <WiFiClient.h>
#include <Arduino.h>
#include "NetworkClient.h"
#include "WiFi/WiFiHttpClient.h"
#include "RumpshiftLogger.h"

/**
 * @class WiFiClientWrapper
 * @brief Wraps WiFiClient to conform to the extended NetworkClient interface,
 *        with optional logging support and integrated HTTP client helper.
 */
class WiFiClientWrapper : public NetworkClient
{
public:
    WiFiClientWrapper() = default;

    explicit WiFiClientWrapper(
        WiFiClient client,
        RumpshiftLogger *logger = nullptr)
        : _client(client),
          _logger(logger) {}

    void setLogger(RumpshiftLogger *logger) { _logger = logger; }

    WiFiClientWrapper &operator=(WiFiClient client)
    {
        _client = client;
        return *this;
    }

    void begin(RumpshiftLogger *logger = nullptr)
    {
        _client = WiFiClient(); // explicitly construct internal client
        _logger = logger;
        if (_logger)
        {
            _logger->info("[WiFiClientWrapper] Initialized internal WiFiClient.");
            _logger->info("[WiFiClientWrapper] Initialized internal WiFiClient. address=" + String((uintptr_t)this, HEX));
        }
    }

    void setClient(WiFiClient client) { _client = client; }

    /**
     * @brief Connect to the currently configured remote host/IP and port.
     * @return true if connection succeeded, false otherwise
     */
    bool connect()
    {
        bool result = false;
        if (_remoteHost)
        {
            result = _client.connect(_remoteHost, _remotePort);
        }
        else if (_remoteIP)
        {
            result = _client.connect(_remoteIP, _remotePort);
        }

        if (_logger)
        {
            if (result)
                _logger->info("[WiFiClientWrapper] Connected to " + String(_remoteHost ? _remoteHost : _remoteIP.toString()) + ":" + String(_remotePort));
            else
                _logger->error("[WiFiClientWrapper] Failed to connect to " + String(_remoteHost ? _remoteHost : _remoteIP.toString()) + ":" + String(_remotePort));
        }

        return result;
    }

    /**
     * @brief Return an HTTP helper object using the internal WiFiClient
     */
    WiFiHttpClient http()
    {
        return WiFiHttpClient(_client, _logger);
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

    operator bool() override { return _client ? true : false; }
    uint8_t connected() override { return _client.connected(); }
    bool exists() const override { return true; } // Underlying _client always exists

private:
    WiFiClient _client;       ///< Actual WiFi client used for TCP communication
    RumpshiftLogger *_logger; ///< Optional logger for debug/info
};

#endif // WIFI_CLIENT_WRAPPER_H
