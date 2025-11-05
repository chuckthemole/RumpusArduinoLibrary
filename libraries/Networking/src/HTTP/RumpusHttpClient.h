#ifndef RUMPUS_HTTP_CLIENT_H
#define RUMPUS_HTTP_CLIENT_H

#include "NetworkProtocol.h"
#include "NetworkManager.h"
#include "NetworkClient.h"
#include "RumpshiftLogger.h"
#include "HttpClient.h"
#include <memory>

#ifdef RUMPUS_USE_ARDUINO_HTTP_CLIENT
#include "ArduinoHttpClientWrapper.h"
#else
#include "SimpleHttpClient.h"
#endif

class RumpusHttpClient : public NetworkProtocol
{
public:
    explicit RumpusHttpClient(NetworkManager &net, RumpshiftLogger *logger = nullptr)
        : _network(net), _logger(logger), _lastStatusCode(-1) {}

    ~RumpusHttpClient() {}

    void begin() override
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient] begin() called.");

        NetworkClient *client = _getValidClient("INITIALIZE");
        if (!client)
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] No valid client returned.");
            return;
        }
        _lazyInit(client);
    }

    void maintain() override
    {
        _network.maintainConnection();
    }

    void post(const String &path, const String &payload)
    {
        NetworkClient *client = _getValidClient("POST");
        if (!client)
            return;
        _lazyInit(client);
        _sendRequest("POST", path, payload);
    }

    String get(const String &path)
    {
        NetworkClient *client = _getValidClient("GET");
        if (!client)
            return "";
        _lazyInit(client);
        _sendRequest("GET", path);
        return _httpClient ? _httpClient->responseBody() : "";
    }

    void put(const String &path, const String &payload)
    {
        NetworkClient *client = _getValidClient("PUT");
        if (!client)
            return;
        _lazyInit(client);
        _sendRequest("PUT", path, payload);
    }

    void del(const String &path)
    {
        NetworkClient *client = _getValidClient("DELETE");
        if (!client)
            return;
        _lazyInit(client);
        _sendRequest("DELETE", path);
    }

    bool isConnected() const
    {
        if (!_network.isConnected())
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] Network is not connected");
            return false;
        }

        if (!_httpClient)
        {
            if (_logger)
                _logger->warn("[RumpusHttpClient] HTTP client not initialized");
            return false;
        }

        if (!_httpClient->connected())
        {
            if (_logger)
                _logger->warn("[RumpusHttpClient] HTTP client not connected to host");
            return false;
        }

        if (_logger)
            _logger->debug("[RumpusHttpClient] Network and HTTP client connected");

        return true;
    }

    int lastStatusCode() const { return _lastStatusCode; }

private:
    NetworkManager &_network;
    RumpshiftLogger *_logger;

    std::unique_ptr<HttpClient> _httpClient;
    int _lastStatusCode;

    void _lazyInit(NetworkClient *client)
    {
        if (!_httpClient && client)
        {
            const char *host = client->getRemoteHost();
            IPAddress ip = client->getRemoteIP();
            uint16_t port = client->getRemotePort();
            String ipStr = ip.toString();

#ifdef RUMPUS_USE_ARDUINO_HTTP_CLIENT
            if (host)
                _httpClient = std::make_unique<ArduinoHttpClientWrapper>(*client, host, port);
            else
                _httpClient = std::make_unique<ArduinoHttpClientWrapper>(*client, ipStr.c_str(), port);
#else
            if (host)
                _httpClient = std::make_unique<SimpleHttpClient>(*client, host, port, _logger);
            else
                _httpClient = std::make_unique<SimpleHttpClient>(*client, ipStr.c_str(), port, _logger);
#endif

            // Ensure TCP connection is open
            if (!client->connected())
            {
                if (host)
                    client->connect(host, port);
                else
                    client->connect(ip, port);

                if (_logger)
                    _logger->info("[RumpusHttpClient] TCP client connected to " + String(host ? host : ipStr.c_str()));
            }
        }
    }

    void _sendRequest(const String &method, const String &path, const String &payload = "")
    {
        if (!_httpClient)
            return;

        _httpClient->beginRequest();
        if (method == "POST")
            _httpClient->post(path);
        else if (method == "PUT")
            _httpClient->put(path);
        else if (method == "DELETE")
            _httpClient->del(path);
        else
            _httpClient->get(path);

        if (!payload.isEmpty())
        {
            _httpClient->sendHeader("Content-Type", "application/json");
            _httpClient->sendHeader("Content-Length", payload.length());
            _httpClient->beginBody();
            _httpClient->print(payload);
        }

        _httpClient->endRequest();
        _lastStatusCode = _httpClient->responseStatusCode();
    }

    NetworkClient *_getValidClient(const String &action)
    {
        NetworkClient *client = _network.getClient();
        if (!client && _logger)
            _logger->error("[RumpusHttpClient] _getValidClient returned nullptr for " + action);
        return client;
    }
};

#endif // RUMPUS_HTTP_CLIENT_H
