#ifndef RUMPUS_HTTP_CLIENT_H
#define RUMPUS_HTTP_CLIENT_H

#include "NetworkProtocol.h"
#include "NetworkManager.h"
#include "NetworkClient.h"
#include "RumpshiftLogger.h"
#include <ArduinoHttpClient.h>
#include <memory>

/**
 * @class RumpusHttpClient
 * @brief Lightweight HTTP client wrapper for Arduino, supporting GET/POST/PUT/DELETE.
 *
 * Responsibilities:
 *  - Perform HTTP requests over TCP using ArduinoHttpClient.
 *  - Integrate with NetworkManager to ensure connectivity.
 *  - Provide optional logging with RumpshiftLogger.
 *  - Track connection state and last HTTP status code.
 *
 * Notes:
 *  - Does not own the NetworkClient; caller must manage validity.
 *  - HttpClient instance is lazily initialized on first request.
 */
class RumpusHttpClient : public NetworkProtocol
{
public:
    /**
     * @brief Construct the HTTP client wrapper.
     * @param net Reference to NetworkManager for connectivity maintenance.
     * @param logger Optional logger for debug/info messages.
     */
    explicit RumpusHttpClient(
        NetworkManager &net,
        RumpshiftLogger *logger = nullptr)
        : _network(net),
          _logger(logger),
          _lastStatusCode(-1) {}

    /**
     * @brief Destructor cleans up allocated HttpClient.
     */
    ~RumpusHttpClient()
    {
    }

    /**
     * @brief Initialize the client. Currently optional.
     */
    void begin() override
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient] begin() called.");

        NetworkClient *client = _getValidClient("INITIALIZE");

        if (!client)
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] No valid client returned from _getValidClient().");
            return;
        }

        if (_logger)
            _logger->debug("[RumpusHttpClient] Valid client retrieved, proceeding with _lazyInit().");

        _lazyInit(client);

        if (_logger)
            _logger->debug("[RumpusHttpClient] _lazyInit() completed successfully.");
    }

    /**
     * @brief Maintain network connectivity. Should be called in main loop.
     */
    void maintain() override
    {
        _network.maintainConnection();
    }

    // ---------------------
    // HTTP convenience methods
    // ---------------------

    /**
     * @brief Send an HTTP POST request with a JSON payload.
     * @param path Request path.
     * @param payload JSON string to send.
     */
    void post(const String &path, const String &payload)
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient] post() called. Path: " + path + ", Payload length: " + String(payload.length()));

        NetworkClient *client = _getValidClient("POST");
        if (!client)
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] post() aborted. No valid client.");
            return;
        }

        if (_logger)
            _logger->debug("[RumpusHttpClient] post() got a valid client. Initializing...");
        _lazyInit(client);

        _sendRequest("POST", path, payload);

        if (_logger)
            _logger->debug("[RumpusHttpClient] post() finished sending request.");
    }

    /**
     * @brief Send an HTTP GET request.
     * @param path Request path.
     */
    void get(const String &path)
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient] get() called. Path: " + path);

        NetworkClient *client = _getValidClient("GET");
        if (!client)
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] get() aborted. No valid client.");
            return;
        }

        if (_logger)
            _logger->debug("[RumpusHttpClient] get() got a valid client. Initializing...");
        _lazyInit(client);

        _sendRequest("GET", path);

        if (_logger)
            _logger->debug("[RumpusHttpClient] get() finished sending request.");
    }

    /**
     * @brief Send an HTTP PUT request with a JSON payload.
     * @param path Request path.
     * @param payload JSON string to send.
     */
    void put(const String &path, const String &payload)
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient] put() called. Path: " + path + ", Payload length: " + String(payload.length()));

        NetworkClient *client = _getValidClient("PUT");
        if (!client)
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] put() aborted. No valid client.");
            return;
        }

        if (_logger)
            _logger->debug("[RumpusHttpClient] put() got a valid client. Initializing...");
        _lazyInit(client);

        _sendRequest("PUT", path, payload);

        if (_logger)
            _logger->debug("[RumpusHttpClient] put() finished sending request.");
    }

    /**
     * @brief Send an HTTP DELETE request.
     * @param path Request path.
     */
    void del(const String &path)
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient] del() called. Path: " + path);
        NetworkClient *client = _getValidClient("DELETE");
        if (!client)
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] del() aborted. No valid client.");
            return;
        }
        if (_logger)
            _logger->debug("[RumpusHttpClient] del() got a valid client. Initializing...");

        _lazyInit(client);
        _sendRequest("DELETE", path);

        if (_logger)
            _logger->debug("[RumpusHttpClient] del() finished sending request.");
    }

    // ---------------------
    // Status helpers
    // ---------------------

    /**
     * @brief Check if the network is connected and  underlying client is connected.
     * @return true if network is connected, false otherwise.
     */
    bool isConnected() const
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient] isConnected() check starting.");

        // First check if the device is online at all
        if (!_network.isConnected())
        {
            if (_logger)
                _logger->warn("[RumpusHttpClient] NetworkManager reports not connected.");
            return false;
        }

        // Optionally check HttpClient only if you *expect* a persistent connection
        if (_httpClient && _httpClient->connected())
        {
            if (_logger)
                _logger->debug("[RumpusHttpClient] HttpClient is connected to remote.");
            return true;
        }

        // Otherwise, network is up but no active HttpClient session
        return true; // treat as "connected enough" for next request
    }

    /**
     * @brief Get the last HTTP status code received.
     * @return Integer HTTP status code, or -1 if none yet.
     */
    int lastStatusCode() const
    {
        return _lastStatusCode;
    }

private:
    NetworkManager &_network; ///< Reference to NetworkManager for connectivity
    RumpshiftLogger *_logger; ///< Optional logger for debug/info
    std::unique_ptr<HttpClient> _httpClient;
    int _lastStatusCode; ///< Last received HTTP status code (-1 if none)

    /**
     * @brief Initialize the HttpClient if not already set up.
     * @param client Valid NetworkClient.
     */
    void _lazyInit(NetworkClient *client)
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient::_lazyInit] called.");

        if (!_httpClient && client)
        {
            if (_logger)
                _logger->debug("[RumpusHttpClient::_lazyInit] No HttpClient yet, attempting to initialize.");

            const char *host = client->getRemoteHost();
            IPAddress ip = client->getRemoteIP();
            uint16_t port = client->getRemotePort();

            if (_logger)
            {
                _logger->debug(String("[RumpusHttpClient::_lazyInit] Remote host: ") + (host ? host : "null"));
                _logger->debug(String("[RumpusHttpClient::_lazyInit] Remote IP: ") + ip.toString());
                _logger->debug(String("[RumpusHttpClient::_lazyInit] Remote port: ") + String(port));
            }

            if (host || (ip != IPAddress(0, 0, 0, 0)))
            {
                if (host)
                {
                    if (_logger)
                        _logger->info("[RumpusHttpClient] Initializing HttpClient with hostname.");
                    _httpClient = std::make_unique<HttpClient>(*client, host, port);
                }
                else
                {
                    if (_logger)
                        _logger->info("[RumpusHttpClient] Initializing HttpClient with IP address.");
                    _httpClient = std::make_unique<HttpClient>(*client, ip, port);
                }

                if (_httpClient)
                {
                    if (_logger)
                        _logger->info("[RumpusHttpClient] HttpClient successfully initialized.");
                }
                else
                {
                    if (_logger)
                        _logger->error("[RumpusHttpClient] Failed to allocate HttpClient.");
                }
            }
            else
            {
                if (_logger)
                    _logger->error("[RumpusHttpClient] Cannot initialize HttpClient: no host or valid IP set.");
            }
        }
        else
        {
            if (!client)
            {
                if (_logger)
                    _logger->error("[RumpusHttpClient::_lazyInit] Provided NetworkClient is null!");
            }
            else if (_httpClient)
            {
                if (_logger)
                    _logger->debug("[RumpusHttpClient::_lazyInit] HttpClient already initialized, skipping.");
            }
        }
    }

    /**
     * @brief Internal helper to send an HTTP request.
     * @param method HTTP verb ("GET", "POST", "PUT", "DELETE").
     * @param path Request path.
     * @param payload Optional body for POST/PUT.
     *
     * TODO: Error handling in _sendRequest():
     * You only log the status code. Might also be worth logging the body (for debugging 400/500s) or exposing a method to readResponse().
     */
    void _sendRequest(const String &method, const String &path, const String &payload = "")
    {
        if (!_httpClient)
        {
            if (_logger)
                _logger->error("[RumpusHttpClient] HttpClient not initialized.");
            return;
        }

        if (_logger)
            _logger->debug("[RumpusHttpClient] Preparing to send HTTP " + method + " to path: " + path);

        _httpClient->beginRequest();
        if (_logger)
            _logger->debug("[RumpusHttpClient] beginRequest() called");

        if (method == "POST")
        {
            _httpClient->post(path);
            if (_logger)
                _logger->debug("[RumpusHttpClient] POST request prepared");
        }
        else if (method == "PUT")
        {
            _httpClient->put(path);
            if (_logger)
                _logger->debug("[RumpusHttpClient] PUT request prepared");
        }
        else if (method == "DELETE")
        {
            _httpClient->del(path);
            if (_logger)
                _logger->debug("[RumpusHttpClient] DELETE request prepared");
        }
        else
        {
            _httpClient->get(path);
            if (_logger)
                _logger->debug("[RumpusHttpClient] GET request prepared");
        }

        if (payload.length() > 0)
        {
            if (_logger)
                _logger->debug("[RumpusHttpClient] Sending payload: " + payload);

            _httpClient->sendHeader("Content-Type", "application/json");
            _httpClient->sendHeader("Content-Length", payload.length());
            _httpClient->beginBody();
            _httpClient->print(payload);

            if (_logger)
                _logger->debug("[RumpusHttpClient] Payload sent");
        }

        _httpClient->endRequest();
        if (_logger)
            _logger->debug("[RumpusHttpClient] endRequest() called");

        _lastStatusCode = _httpClient->responseStatusCode();
        _debugHttpClientStatusCode(); // TODO: this should not execute if not in DEBUG mode. 

        if (_logger)
            _logger->info("[RumpusHttpClient] HTTP " + method + " " + path +
                          " => Status: " + String(_lastStatusCode));
    }

    /**
     * @brief Acquire a valid client from the NetworkManager.
     * @param action Context string for logging.
     * @return NetworkClient* or nullptr if unavailable.
     */
    NetworkClient *_getValidClient(const String &action)
    {
        NetworkClient *client = _network.getClient();
        if (!client && _logger)
        {
            _logger->error("[RumpusHttpClient] No client available for " + action + ".");
        }
        return client;
    }

    void _debugHttpClientStatusCode()
    {
        if (_logger)
            _logger->debug("[RumpusHttpClient::debugHttpClientStatusCode] debug client status code start.");
        String reason;
        switch (_lastStatusCode)
        {
        case HTTP_ERROR_API:
            reason = "API not ready or request not sent";
            break;
        case HTTP_ERROR_INVALID_RESPONSE:
            reason = "Invalid HTTP response";
            break;
        case HTTP_ERROR_TIMED_OUT:
            reason = "HTTP response timed out";
            break;
        default:
            if (_lastStatusCode >= 100 && _lastStatusCode < 200)
                reason = "Informational response";
            else if (_lastStatusCode >= 200 && _lastStatusCode < 300)
                reason = "Success";
            else if (_lastStatusCode >= 300 && _lastStatusCode < 400)
                reason = "Redirection";
            else if (_lastStatusCode >= 400 && _lastStatusCode < 500)
                reason = "Client error";
            else if (_lastStatusCode >= 500 && _lastStatusCode < 600)
                reason = "Server error";
            else
                reason = "Unknown status";
            break;
        }

        if (_logger)
            _logger->debug("[RumpusHttpClient::debugHttpClientStatusCode] Reason: " + reason );
    }
};

#endif // RUMPUS_HTTP_CLIENT_H
