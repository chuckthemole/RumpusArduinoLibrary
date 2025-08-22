#ifndef WIFI_NETWORK_MANAGER_H
#define WIFI_NETWORK_MANAGER_H

#include "NetworkManager.h"
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "RumpshiftLogger.h"
#include "WiFiClientWrapper.h"
#include "WiFiUDPWrapper.h"
#include "WiFiServerWrapper.h"

/**
 * @class WiFiNetworkManager
 * @brief Manages WiFi connections and provides access to TCP/UDP sockets
 *        for Arduino boards with WiFi support (e.g., UNO R4 WiFi).
 *
 * Responsibilities:
 *  - Connect to a WiFi network (with SSID/password).
 *  - Provide TCP (client/server) and UDP communication.
 *  - Handle reconnection if disconnected.
 *  - Print debug/status information.
 */
class WiFiNetworkManager : public NetworkManager
{
public:
    WiFiNetworkManager(
        const char *ssid,
        const char *password,
        RumpshiftLogger *logger = nullptr,
        unsigned int portTCP = 12345,
        unsigned int portUDP = 8888);

    void begin() override;
    void maintainConnection() override;
    void printStatus() override;

    void setRemoteTarget(const char *host, unsigned int port)
    {
        _remoteHost = host;
        _remotePort = port;
    }

    void setRemoteTarget(IPAddress ip, unsigned int port)
    {
        _remoteIP = ip;
        _remotePort = port;
    }

    NetworkClient *getClient() override { return _client; }
    WiFiUDPWrapper *getUDP() override { return _udp; }
    WiFiServerWrapper *getServer() override { return _server; }

private:
    const char *_ssid;     ///< WiFi SSID
    const char *_password; ///< WiFi password
    unsigned int _portTCP; ///< TCP port number
    unsigned int _portUDP; ///< UDP port number

    const char *_remoteHost = nullptr; ///< Optional remote hostname for client
    IPAddress _remoteIP;               ///< Optional remote IP for client
    unsigned int _remotePort = 0;      ///< Remote port for client

    WiFiUDPWrapper *_udp;       ///< UDP communication object
    WiFiServerWrapper *_server; ///< TCP server object
    NetworkClient *_client; ///< TCP client connection object

    unsigned long _lastStatusCheck = 0; ///< Timestamp of last status check
    RumpshiftLogger *_logger = nullptr; ///< Optional logger

    void scanNetworks();   ///< Scan and log available WiFi networks
    void connectWiFi();    ///< Connect to the configured WiFi
    void reconnectWiFi();  ///< Attempt reconnection if disconnected
    void printIPDetails(); ///< Print IP/subnet/gateway details
};

#endif // WIFI_NETWORK_MANAGER_H
