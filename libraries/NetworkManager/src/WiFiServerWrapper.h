#ifndef WIFI_SERVER_WRAPPER_H
#define WIFI_SERVER_WRAPPER_H

#include <WiFiServer.h>
#include "NetworkServer.h"
#include "WiFiClientWrapper.h"

/**
 * @class WiFiServerWrapper
 * @brief Wraps WiFiServer to conform to the NetworkServer interface.
 */
class WiFiServerWrapper : public NetworkServer
{
public:
    /**
     * @brief Construct a WiFiServerWrapper with a specific port.
     */
    WiFiServerWrapper(uint16_t port) : _server(port) {}

    /**
     * @brief Begin listening on the server.
     */
    void begin() override { _server.begin(); }

    /**
     * @brief Return a pointer to a wrapped NetworkClient if a new connection is available.
     */
    NetworkClient *available() override
    {
        WiFiClient client = _server.available();
        if (client)
        {
            _clientWrapper.setClient(client);
            return &_clientWrapper;
        }
        return nullptr;
    }

private:
    WiFiServer _server;
    WiFiClientWrapper _clientWrapper; // Reused for each available client
};

#endif // WIFI_SERVER_WRAPPER_H
