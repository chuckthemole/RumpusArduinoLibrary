#ifndef WIFI_UDP_WRAPPER_H
#define WIFI_UDP_WRAPPER_H

#include <WiFiUdp.h>
#include "NetworkUDP.h"

/**
 * @class WiFiUDPWrapper
 * @brief Wraps WiFiUDP to conform to the NetworkUDP interface.
 */
class WiFiUDPWrapper : public NetworkUDP
{
public:
    WiFiUDPWrapper() = default;

    /**
     * @brief Start a UDP packet to the specified host and port.
     *
     * @param host The destination IP as a string.
     * @param port The destination port.
     */
    void beginPacket(const char *host, uint16_t port) override
    {
        if (_udp.beginPacket(host, port) != 1)
        {
            // Optionally log or handle error
        }
    }

    /**
     * @brief Start a UDP packet to the specified ip and port.
     *
     * @param IPAddress The destination IP as a string.
     * @param port The destination port.
     */
    void beginPacket(IPAddress ip, uint16_t port) override
    {
        if (_udp.beginPacket(ip, port) != 1)
        {
            // Optionally log or handle error
        }
    }

    /**
     * @brief Write a null-terminated string to the UDP packet.
     */
    void write(const char *data) override
    {
        if (data)
        {
            _udp.write((const uint8_t *)data, strlen(data));
        }
    }

    /**
     * @brief End and send the UDP packet.
     */
    void endPacket() override
    {
        _udp.endPacket();
    }

    /**
     * @brief Access the internal WiFiUDP instance if needed.
     */
    WiFiUDP &getInternalUDP() { return _udp; }

private:
    WiFiUDP _udp;
};

#endif // WIFI_UDP_WRAPPER_H
