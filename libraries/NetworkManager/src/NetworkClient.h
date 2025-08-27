#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <Arduino.h>
#include <Client.h>

/**
 * @class NetworkClient
 * @brief Abstracted network client interface with remote target management.
 *
 * Extends the Arduino `Client` class to include remote host/IP and port tracking.
 * Designed as a base class for WiFi, Ethernet, or other transport clients.
 *
 * Responsibilities:
 *  - Store remote host/IP and port for connection management.
 *  - Provide getters and setters for remote information.
 *  - Pass through all Client methods to derived classes.
 */
class NetworkClient : public Client
{
public:
    virtual ~NetworkClient() = default;

    // ---------------------
    // Remote target management
    // ---------------------

    /**
     * @brief Set the remote target using hostname and port
     * @param host Remote host as a string
     * @param port Remote TCP port
     */
    void setRemote(const char *host, uint16_t port)
    {
        _remoteHost = host;
        _remotePort = port;
        _remoteIP = IPAddress(); // reset IP if switching to hostname
    }

    /**
     * @brief Set the remote target using IP address and port
     * @param ip Remote IP
     * @param port Remote TCP port
     */
    void setRemote(IPAddress ip, uint16_t port)
    {
        _remoteIP = ip;
        _remotePort = port;
        _remoteHost = nullptr; // reset host if using IP directly
    }

    /**
     * @brief Get the currently configured remote hostname (if any)
     * @return Pointer to host string, nullptr if not set
     */
    const char *getRemoteHost() const { return _remoteHost; }

    /**
     * @brief Get the currently configured remote IP address
     * @return IPAddress object (may be default 0.0.0.0 if not set)
     */
    IPAddress getRemoteIP() const { return _remoteIP; }

    /**
     * @brief Get the currently configured remote port
     * @return TCP port number
     */
    uint16_t getRemotePort() const { return _remotePort; }

    /**
     * @brief Convenience check if a valid client is present
     * @return true if the derived client exists, false otherwise
     */
    virtual bool exists() const { return true; } // optional; can override in derived class

protected:
    const char *_remoteHost = nullptr; ///< Optional hostname for connection
    IPAddress _remoteIP;               ///< Optional IP for connection
    uint16_t _remotePort = 0;          ///< TCP port for connection
};

#endif // NETWORK_CLIENT_H
