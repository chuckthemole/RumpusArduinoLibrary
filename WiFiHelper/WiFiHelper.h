#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <Arduino.h>
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "RumpshiftLogger.h"

/**
 * @class WiFiHelper
 * @brief Utility class to simplify WiFi and network socket handling for Arduino boards (e.g., UNO R4 WiFi).
 *
 * This class manages WiFi connections and provides access to TCP (via WiFiServer and WiFiClient)
 * and UDP communication. It supports automatic reconnection and regular status updates.
 */
class WiFiHelper
{
public:
    /**
     * @brief Construct a new WiFiHelper object.
     *
     * @param ssid The SSID of the WiFi network.
     * @param password The password of the WiFi network.
     * @param portTCP TCP server listening port. Defaults to 12345.
     * @param portUDP UDP communication port. Defaults to 8888.
     */
    WiFiHelper(
        const char *ssid,
        const char *password,
        RumpshiftLogger *logger = nullptr,
        unsigned int portTCP = 12345,
        unsigned int portUDP = 8888);

    /**
     * @brief Initializes WiFi connection and starts TCP server.
     */
    void begin();

    /**
     * @brief Maintains the WiFi connection, attempting to reconnect if disconnected.
     * Should be called regularly from loop().
     */
    void maintainConnection();

    /**
     * @brief Prints the current connection status to Serial.
     */
    void printStatus();

    /**
     * @brief Returns a WiFiClient object for TCP communication.
     *
     * @return WiFiClient The client instance.
     */
    WiFiClient getClient();

    /**
     * @brief Returns a pointer to the internal WiFiUDP instance.
     *
     * @return WiFiUDP* Pointer to the UDP handler.
     */
    WiFiUDP *getUDP();

    /**
     * @brief Returns a pointer to the internal WiFiServer instance.
     *
     * @return WiFiServer* Pointer to the TCP server.
     */
    WiFiServer *getServer();

private:
    const char *_ssid;     ///< WiFi SSID
    const char *_password; ///< WiFi password
    unsigned int _portTCP; ///< TCP port number
    unsigned int _portUDP; ///< UDP port number

    WiFiUDP _udp;       ///< UDP communication object
    WiFiServer _server; ///< TCP server object
    WiFiClient _client; ///< TCP client connection object

    unsigned long _lastStatusCheck = 0; ///< Last time status was printed
    unsigned long _lastBroadcast = 0;   ///< Last time a broadcast was sent (if applicable)

    RumpshiftLogger *_logger = nullptr; //< Pointer to your logger

    /**
     * @brief Scans for nearby WiFi networks and prints their SSIDs.
     */
    void scanNetworks();

    /**
     * @brief Attempts to connect to the configured WiFi network.
     */
    void connectWiFi();

    /**
     * @brief Reconnects to WiFi if the connection is lost.
     */
    void reconnectWiFi();

    /**
     * @brief Prints IP address, subnet, and gateway info to Serial.
     */
    void printIPDetails();
};

#endif // WIFI_HELPER_H
