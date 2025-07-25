#include "WiFiHelper.h"

WiFiHelper::WiFiHelper(
    const char *ssid,
    const char *password,
    unsigned int portTCP,
    unsigned int portUDP)
    : _ssid(ssid),
      _password(password),
      _portTCP(portTCP),
      _portUDP(portUDP),
      _server(portTCP) {}

void WiFiHelper::begin()
{
    Serial.println("[WiFiHelper] Starting WiFi setup...");

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
        Serial.println("[WiFiHelper] ERROR: Failed to connect.");
    }
}

void WiFiHelper::maintainConnection()
{
    unsigned long now = millis();

    if (now - _lastStatusCheck > 30000)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("[WiFiHelper] WiFi disconnected. Reconnecting...");
            reconnectWiFi();
        }
        _lastStatusCheck = now;
    }

    if (!_client || !_client.connected())
    {
        _client = _server.available();
        if (_client)
        {
            Serial.println("[WiFiHelper] New client connected.");
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
    Serial.println("[WiFiHelper] Scanning for networks...");
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
        Serial.print("  ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm)");
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
    Serial.println();
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
        Serial.println("WL_IDLE_STATUS");
        break;
    case WL_NO_SSID_AVAIL:
        Serial.println("WL_NO_SSID_AVAIL");
        break;
    case WL_CONNECTED:
        Serial.println("WL_CONNECTED");
        break;
    case WL_CONNECT_FAILED:
        Serial.println("WL_CONNECT_FAILED");
        break;
    case WL_CONNECTION_LOST:
        Serial.println("WL_CONNECTION_LOST");
        break;
    case WL_DISCONNECTED:
        Serial.println("WL_DISCONNECTED");
        break;
    default:
        Serial.println("Unknown WiFi status");
    }
}

void WiFiHelper::printIPDetails()
{
    Serial.print("[WiFiHelper] IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("  RSSI: ");
    Serial.println(WiFi.RSSI());
    Serial.print("  Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("  Subnet: ");
    Serial.println(WiFi.subnetMask());
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
