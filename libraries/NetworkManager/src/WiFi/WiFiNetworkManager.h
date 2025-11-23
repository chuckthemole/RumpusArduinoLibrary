#ifndef WIFI_NETWORK_MANAGER_H
#define WIFI_NETWORK_MANAGER_H

#include "NetworkManager.h"
#include "RumpshiftLogger.h"
#include "WiFiClientWrapper.h"
#include "WiFiUDPWrapper.h"
#include "WiFiServerWrapper.h"

// Forward declarations for WiFi libraries (avoid include issues)
#ifdef USE_WIFI_S3
#include <WiFiS3.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>

/**
 * @enum WiFiImpl
 * @brief Represents the WiFi implementation to use.
 */
enum class WiFiImpl
{
    ARDUINO_WIFI, ///< Use standard Arduino WiFi library
    WIFI_S3       ///< Use WiFiS3 library (UNO R4 WiFi)
};

/**
 * @class WiFiNetworkManager
 * @brief Manages WiFi connections and provides access to TCP/UDP sockets
 *        for Arduino boards with WiFi support.
 *
 * Responsibilities:
 *  - Connect to a WiFi network using provided SSID/password.
 *  - Provide access to TCP (client/server) and UDP communication.
 *  - Maintain connectivity and handle reconnections automatically.
 *  - Print network debug and status information.
 *
 * Ownership and Lifecycle:
 *  - TCP client, TCP server, and UDP wrappers are **externally owned**.
 *    They must be created and set via the setters in NetworkManager or
 *    provided before calling begin().
 *  - This class **does not delete** the wrappers; it only holds pointers.
 *  - This design allows flexibility and dependency injection for testing.
 */
class WiFiNetworkManager : public NetworkManager
{
public:
    /**
     * @brief Default constructor (does not initialize WiFi).
     *
     * Useful for testing or late initialization. Wrappers must be set
     * externally using setClient(), setServer(), or setUDP().
     */
    WiFiNetworkManager() = default;

    /**
     * @brief Construct a manager with SSID, password, WiFi implementation, and optional logger.
     * @param ssid WiFi network SSID.
     * @param password WiFi network password.
     * @param impl Implementation type (Arduino WiFi or WiFiS3).
     * @param logger Optional logger for debug messages.
     *
     * Note: Wrappers must still be set externally before using network
     * operations.
     */
    WiFiNetworkManager(
        const char *ssid,
        const char *password,
        WiFiImpl impl = WiFiImpl::ARDUINO_WIFI,
        RumpshiftLogger *logger = nullptr);

    /**
     * @brief Initialize the WiFi connection.
     *
     * Must be called after construction and after wrappers are set.
     */
    void begin() override;

    /**
     * @brief Maintain the network connection and reconnect if necessary.
     *
     * Should be called periodically in loop() to ensure connectivity.
     */
    void maintainConnection() override;

    /**
     * @brief Print the current network status, IP address, and other info.
     */
    void printStatus() override;

    /**
     * @brief Get the current WiFi connection status.
     * @return Current WiFi status as wl_status_t (e.g., WL_CONNECTED, WL_CONNECT_FAILED, etc.)
     */
    int getStatus() const override;

    /**
     * @brief Check if WiFi is currently connected.
     * @return true if connected, false otherwise.
     */
    virtual bool isConnected() override;

    void setRemote(const char *host, uint16_t port) override;

    void setRemote(IPAddress ip, uint16_t port) override;

private:
    const char *_ssid = nullptr;             ///< WiFi SSID
    const char *_password = nullptr;         ///< WiFi password
    WiFiImpl _impl = WiFiImpl::ARDUINO_WIFI; ///< WiFi implementation
    unsigned long _lastStatusCheck = 0;      ///< Timestamp of last status check
    RumpshiftLogger *_logger = nullptr;      ///< Optional logger

    /**
     * @brief Scan and log available WiFi networks for debugging.
     */
    void scanNetworks();

    /**
     * @brief Connect to the configured WiFi network.
     */
    void connectWiFi();

    /**
     * @brief Attempt reconnection if WiFi is disconnected.
     */
    void reconnectWiFi();

    /**
     * @brief Print detailed IP, subnet mask, and gateway information.
     */
    void printIPDetails();
};

#endif // WIFI_NETWORK_MANAGER_H
