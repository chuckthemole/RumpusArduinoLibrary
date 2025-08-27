#include "WiFiNetworkManager.h"

/**
 * @brief Construct a WiFiNetworkManager with SSID, password, and optional logger.
 * @param ssid WiFi network SSID.
 * @param password WiFi network password.
 * @param logger Optional logger instance.
 *
 * Note: TCP/UDP wrappers must be set externally using the setters from
 * NetworkManager before calling begin().
 */
WiFiNetworkManager::WiFiNetworkManager(
    const char *ssid,
    const char *password,
    RumpshiftLogger *logger)
    : _ssid(ssid),
      _password(password),
      _logger(logger),
      _lastStatusCheck(0)
{
}

/**
 * @brief Initialize WiFi connection and optionally start servers.
 *
 * Must be called after setting externally owned wrappers.
 */
void WiFiNetworkManager::begin()
{
    if (_logger)
        _logger->info("[WiFiNetworkManager] Starting WiFi setup...");

    scanNetworks();
    connectWiFi();

    if (WiFi.status() == WL_CONNECTED)
    {
        delay(3000); // Allow DHCP assignment
        printIPDetails();

        if (_server)
            _server->begin();
        // UDP wrapper is ready to send/receive packets
    }
    else if (_logger)
    {
        _logger->info("[WiFiNetworkManager] ERROR: Failed to connect.");
    }
}

/**
 * @brief Maintain WiFi connection and handle client updates.
 *
 * Should be called periodically in loop() to maintain connectivity.
 */
void WiFiNetworkManager::maintainConnection()
{
    unsigned long now = millis();

    // Check WiFi status every 30 seconds
    if (now - _lastStatusCheck > 30000)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if (_logger)
                _logger->info("[WiFiNetworkManager] WiFi disconnected. Reconnecting...");
            reconnectWiFi();
        }
        _lastStatusCheck = now;
    }

    // Check for new TCP client
    if (_server)
    {
        NetworkClient *newClient = _server->available();
        if (!_client || !_client->connected())
        {
            _client.reset(newClient);
            if (_client && _logger)
                _logger->info("[WiFiNetworkManager] New client connected.");
        }
    }
}

/**
 * @brief Check if WiFi is currently connected.
 * @return true if connected, false otherwise.
 */
bool WiFiNetworkManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief Scan available WiFi networks for debugging purposes.
 */
void WiFiNetworkManager::scanNetworks()
{
    if (_logger)
        _logger->info("[WiFiNetworkManager] Scanning for networks...");

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
        if (_logger)
        {
            String msg = "  SSID: " + String(WiFi.SSID(i)) + ", RSSI: " + String(WiFi.RSSI(i));
            _logger->info(msg.c_str());
        }
    }
}

void WiFiNetworkManager::setRemote(const char *host, uint16_t port)
{
    if (!_client)
    {
        _client = std::make_unique<WiFiClientWrapper>();
    }
    _client->setRemote(host, port);
}

void WiFiNetworkManager::setRemote(IPAddress ip, uint16_t port)
{
    if (!_client)
    {
        _client = std::make_unique<WiFiClientWrapper>();
    }
    _client->setRemote(ip, port);
}

/**
 * @brief Connect to configured WiFi network.
 *
 * Retries a limited number of times before returning.
 */
void WiFiNetworkManager::connectWiFi()
{
    if (_logger)
        _logger->info("[WiFiNetworkManager] Connecting to SSID: " + String(_ssid));

    WiFi.begin(_ssid, _password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(1000);
        if (_logger)
            _logger->info(".");
        attempts++;
    }

    if (_logger)
        _logger->info("[WiFiNetworkManager] Connected!");
}

/**
 * @brief Reconnect WiFi if the connection is lost.
 */
void WiFiNetworkManager::reconnectWiFi()
{
    WiFi.disconnect();
    delay(1000);
    connectWiFi();
}

/**
 * @brief Print detailed WiFi connection status.
 */
void WiFiNetworkManager::printStatus()
{
    const char *statusStr = "Unknown WiFi status";
    switch (WiFi.status())
    {
    case WL_IDLE_STATUS:
        statusStr = "WL_IDLE_STATUS";
        break;
    case WL_NO_SSID_AVAIL:
        statusStr = "WL_NO_SSID_AVAIL";
        break;
    case WL_CONNECTED:
        statusStr = "WL_CONNECTED";
        break;
    case WL_CONNECT_FAILED:
        statusStr = "WL_CONNECT_FAILED";
        break;
    case WL_CONNECTION_LOST:
        statusStr = "WL_CONNECTION_LOST";
        break;
    case WL_DISCONNECTED:
        statusStr = "WL_DISCONNECTED";
        break;
    }
    if (_logger)
        _logger->info(String("[WiFiNetworkManager] ") + statusStr);
}

/**
 * @brief Print IP address, RSSI, gateway, and subnet information.
 */
void WiFiNetworkManager::printIPDetails()
{
    if (_logger)
    {
        _logger->info(("[WiFiNetworkManager] IP: " + WiFi.localIP().toString()).c_str());
        _logger->info(("[WiFiNetworkManager] RSSI: " + String(WiFi.RSSI())).c_str());
        _logger->info(("[WiFiNetworkManager] Gateway: " + WiFi.gatewayIP().toString()).c_str());
        _logger->info(("[WiFiNetworkManager] Subnet: " + WiFi.subnetMask().toString()).c_str());
    }
    else
    {
        Serial.print("[WiFiNetworkManager] IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("RSSI: ");
        Serial.println(WiFi.RSSI());
        Serial.print("Gateway: ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("Subnet: ");
        Serial.println(WiFi.subnetMask());
    }
}
