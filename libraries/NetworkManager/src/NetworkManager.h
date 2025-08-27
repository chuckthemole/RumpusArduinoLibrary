#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>
#include "NetworkClient.h"
#include "NetworkUDP.h"
#include "NetworkServer.h"
#include <memory>

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
     * @brief Check if the network is currently connected.
     * @return true if connected, false otherwise.
     */
    virtual bool isConnected() = 0;

    /**
     * @brief Set the remote target using hostname and port.
     */
    virtual void setRemote(const char *host, uint16_t port) = 0;

    /**
     * @brief Set the remote target using IP and port.
     */
    virtual void setRemote(IPAddress ip, uint16_t port) = 0;

    /**
     * @brief Get a TCP client instance.
     */
    virtual NetworkClient *getClient() { return _client.get(); };

    /**
     * @brief Get a UDP instance.
     */
    virtual NetworkUDP *getUDP() { return _udp.get(); };

    /**
     * @brief Get a TCP server instance.
     */
    virtual NetworkServer *getServer() { return _server.get(); };

    /**
     * @brief Set the TCP client object.
     * @param client Pointer to a NetworkClient (ownership remains external)
     */
    virtual void setClient(NetworkClient *client) { _client.reset(client); }

    /**
     * @brief Set the UDP object.
     * @param udp Pointer to a NetworkUDP (ownership remains external)
     */
    virtual void setUDP(NetworkUDP *udp) { _udp.reset(udp); }

    /**
     * @brief Set the TCP server object.
     * @param server Pointer to a NetworkServer (ownership remains external)
     */
    virtual void setServer(NetworkServer *server) { _server.reset(server); }

protected:
    // Internally owned
    std::unique_ptr<NetworkClient> _client = nullptr;
    std::unique_ptr<NetworkUDP> _udp = nullptr;
    std::unique_ptr<NetworkServer> _server = nullptr;
};

#endif // NETWORK_MANAGER_H
