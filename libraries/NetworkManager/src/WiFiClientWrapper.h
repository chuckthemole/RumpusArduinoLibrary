#ifndef WIFI_CLIENT_WRAPPER_H
#define WIFI_CLIENT_WRAPPER_H

#include <WiFiClient.h>
#include "NetworkClient.h"

/**
 * @class WiFiClientWrapper
 * @brief Wraps WiFiClient to conform to the NetworkClient interface.
 */
class WiFiClientWrapper : public NetworkClient
{
public:
    WiFiClientWrapper() = default;
    WiFiClientWrapper(WiFiClient client) : _client(client) {}

    WiFiClientWrapper &operator=(WiFiClient client)
    {
        _client = client;
        return *this;
    }

    /**
     * @brief Set the internal WiFiClient instance (e.g., from server.available()).
     */
    void setClient(WiFiClient client) { _client = client; }

    /**
     * @brief Return the number of bytes available for reading.
     */
    int available() override { return _client.available(); }

    /**
     * @brief Read one byte from the client.
     */
    int read() override { return _client.read(); }

    /**
     * @brief Write a null-terminated string to the client.
     */
    void write(const char *data) override
    {
        if (data)
        {
            _client.write((const uint8_t *)data, strlen(data));
        }
    }

    /**
     * @brief Check if the client is still connected.
     */
    bool connected() override { return _client.connected(); }

    bool exists() override
    {
        if (_client)
        {
            return true;
        }
        return false;
    }

    // bool equals(const NetworkClient &other) const override
    // {
    //     auto *otherWrapper = dynamic_cast<const WiFiClientWrapper *>(&other);
    //     if (!otherWrapper)
    //         return false;
    //     return _client == otherWrapper->_client; // compare underlying WiFiClient
    // }

    bool operator==(const NetworkClient &other) const
    {
        // Make sure other is also a WiFiClientWrapper
        auto otherWrapper = static_cast<const WiFiClientWrapper *>(&other);
        if (!otherWrapper)
        {
            return false;
        }

        // Compare underlying clients (cast away const because WiFiClient::operator== is not const)
        return const_cast<WiFiClient &>(_client) == const_cast<WiFiClient &>(otherWrapper->_client);
    }

private:
    WiFiClient _client;
};

#endif // WIFI_CLIENT_WRAPPER_H
