#include "WiFiHelper.h"

WiFiHelper::WiFiHelper(
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
      _server(portTCP) {}

void WiFiHelper::begin()
{
    if (_logger)
        _logger->info("[WiFiHelper] Starting WiFi setup...");

    scanNetworks();
    connectWiFi();

    if (WiFi.status() == WL_CONNECTED)
    {
        delay(3000); // DHCP time
        printIPDetails();
        _server.begin();
        _udp.begin(_portUDP);
    }
    else
    {
        if (_logger)
            _logger->info("[WiFiHelper] ERROR: Failed to connect.");
    }
}

void WiFiHelper::maintainConnection()
{
    unsigned long now = millis();

    if (now - _lastStatusCheck > 30000)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if (_logger)
                _logger->info("[WiFiHelper] WiFi disconnected. Reconnecting...");
            reconnectWiFi();
        }
        _lastStatusCheck = now;
    }

    if (!_client || !_client.connected())
    {
        _client = _server.available();
        if (_client)
        {
            if (_logger)
                _logger->info("[WiFiHelper] New client connected.");
        }
    }

    if (now - _lastBroadcast > 5000)
    {
        IPAddress broadcastIp = ~WiFi.subnetMask() | WiFi.localIP();
        _udp.beginPacket(broadcastIp, _portUDP);
        _udp.print("ARDUINO_ID=1;IP=");
        _udp.print(WiFi.localIP());
        _udp.endPacket();
        _lastBroadcast = now;
    }
}

void WiFiHelper::scanNetworks()
{
    if (_logger)
        _logger->info("[WiFiHelper] Scanning for networks...");
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
        Serial.print("  ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        if (_logger)
            _logger->info(" dBm)");
    }
}

void WiFiHelper::connectWiFi()
{
    WiFi.begin(_ssid, _password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(1000);
        Serial.print(".");
        attempts++;
    }
    if (_logger)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            _logger->info("Connected!");
        }
        else
        {
            _logger->info("Failed to connect.");
        }
    }
}

void WiFiHelper::reconnectWiFi()
{
    WiFi.disconnect();
    delay(1000);
    connectWiFi();
}

void WiFiHelper::printStatus()
{
    switch (WiFi.status())
    {
    case WL_IDLE_STATUS:
        if (_logger)
            _logger->info("WL_IDLE_STATUS");
        break;
    case WL_NO_SSID_AVAIL:
        if (_logger)
            _logger->info("WL_NO_SSID_AVAIL");
        break;
    case WL_CONNECTED:
        if (_logger)
            _logger->info("WL_CONNECTED");
        break;
    case WL_CONNECT_FAILED:
        if (_logger)
            _logger->info("WL_CONNECT_FAILED");
        break;
    case WL_CONNECTION_LOST:
        if (_logger)
            _logger->info("WL_CONNECTION_LOST");
        break;
    case WL_DISCONNECTED:
        if (_logger)
            _logger->info("WL_DISCONNECTED");
        break;
    default:
        if (_logger)
            _logger->info("Unknown WiFi status");
    }
}

void WiFiHelper::printIPDetails()
{
    Serial.print("[WiFiHelper] IP: ");
    if (_logger)
        _logger->info(WiFi.localIP());
    Serial.print("  RSSI: ");
    if (_logger)
        _logger->info(WiFi.RSSI());
    Serial.print("  Gateway: ");
    if (_logger)
        _logger->info(WiFi.gatewayIP());
    Serial.print("  Subnet: ");
    if (_logger)
        _logger->info(WiFi.subnetMask());
}

WiFiClient WiFiHelper::getClient()
{
    return _client;
}

WiFiUDP *WiFiHelper::getUDP()
{
    return &_udp;
}

WiFiServer *WiFiHelper::getServer()
{
    return &_server;
}
