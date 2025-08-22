#include "WiFiNetworkManager.h"

WiFiNetworkManager::WiFiNetworkManager(
    const char *ssid,
    const char *password,
    RumpshiftLogger *logger,
    unsigned int portTCP,
    unsigned int portUDP)
    : _ssid(ssid),
      _password(password),
      _logger(logger),
      _portTCP(portTCP),
      _portUDP(portUDP),
      _lastStatusCheck(0),
      _remoteHost(nullptr),
      _remotePort(0),
      _remoteIP(IPAddress(0, 0, 0, 0))
{
    _server = new WiFiServerWrapper(portTCP);
    _udp = new WiFiUDPWrapper();
    _client = nullptr; // will be assigned when a client connects
}

/**
 * @brief Initialize WiFi connection and start TCP/UDP servers.
 */
void WiFiNetworkManager::begin()
{
    if (_logger)
        _logger->info("[WiFiNetworkManager] Starting WiFi setup...");

    scanNetworks();
    connectWiFi();

    if (WiFi.status() == WL_CONNECTED)
    {
        delay(3000); // DHCP delay
        printIPDetails();
        _server->begin();
        // For UDP, you’ll call beginPacket when you need to send
    }
    else
    {
        if (_logger)
            _logger->info("[WiFiNetworkManager] ERROR: Failed to connect.");
    }
}

/**
 * @brief Maintain WiFi connection and handle client/UDP updates.
 */
void WiFiNetworkManager::maintainConnection()
{
    unsigned long now = millis();

    // Reconnect if WiFi lost, check every 30s
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
    NetworkClient *newClient = _server->available();
    if (!_client || !_client->connected())
    {
        _client = newClient;
        if (_client && _logger)
            _logger->info("[WiFiNetworkManager] New client connected.");
    }
}

/**
 * @brief Scan available WiFi networks.
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

/**
 * @brief Connect to configured WiFi.
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
 * @brief Reconnect WiFi if lost.
 */
void WiFiNetworkManager::reconnectWiFi()
{
    WiFi.disconnect();
    delay(1000);
    connectWiFi();
}

/**
 * @brief Print WiFi connection status.
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
 * @brief Print IP, RSSI, gateway, and subnet information.
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
