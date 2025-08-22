#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>
#include "NetworkClient.h"
#include "NetworkUDP.h"
#include "NetworkServer.h"

/**
 * @class NetworkManager
 * @brief Abstract base class for managing network connections.
 *
 * Provides a common interface for network implementations such as WiFi, Ethernet, or Mock.
 */
class NetworkManager
{
public:
    virtual ~NetworkManager() {}

    /**
     * @brief Initialize the network connection.
     */
    virtual void begin() = 0;

    /**
     * @brief Maintain the network connection (e.g., reconnect if disconnected).
     */
    virtual void maintainConnection() = 0;

    /**
     * @brief Print the current connection status (debugging/logging).
     */
    virtual void printStatus() = 0;

    /**
     * @brief Get a TCP client instance.
     */
    virtual NetworkClient *getClient() = 0;

    /**
     * @brief Get a UDP instance.
     */
    virtual NetworkUDP *getUDP() = 0;

    /**
     * @brief Get a TCP server instance.
     */
    virtual NetworkServer *getServer() = 0;
};

#endif // NETWORK_MANAGER_H
