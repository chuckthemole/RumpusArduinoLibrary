#ifndef NETWORK_UDP_H
#define NETWORK_UDP_H

#include <stdint.h>

/**
 * @class NetworkUDP
 * @brief Abstract interface for UDP communication.
 *
 * Implementations should provide methods to begin a packet,
 * write data, and end/send the packet.
 */
class NetworkUDP
{
public:
    virtual ~NetworkUDP() {}

    /**
     * @brief Begin a UDP packet to the specified host and port.
     * @param host Destination IP or hostname as a null-terminated string.
     * @param port Destination port number.
     */
    virtual void beginPacket(const char *host, uint16_t port) = 0;

    /**
     * @brief Begin a UDP packet to the specified host and port.
     * @param IPAddress Destination IP or hostname as a null-terminated string.
     * @param port Destination port number.
     */
    virtual void beginPacket(IPAddress ip, uint16_t port) = 0;

    /**
     * @brief Write a null-terminated string to the current UDP packet.
     * @param data Null-terminated string to send.
     */
    virtual void write(const char *data) = 0;

    /**
     * @brief End and send the current UDP packet.
     */
    virtual void endPacket() = 0;
};

#endif // NETWORK_UDP_H
