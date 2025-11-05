#ifndef SIMPLE_HTTP_CLIENT_H
#define SIMPLE_HTTP_CLIENT_H

#include <Arduino.h>
#include "HttpClient.h"
#include <WiFiClient.h>
#include "RumpshiftLogger.h"

class SimpleHttpClient : public HttpClient
{
public:
    SimpleHttpClient(
        NetworkClient &client,
        const char *host,
        uint16_t port,
        RumpshiftLogger *logger = nullptr)
        : _client(client),
          _host(host),
          _port(port),
          _logger(logger),
          _statusCode(-1)
    {
        if (_logger)
            _logger->info("[SimpleHttpClient] Initialized for host: " + String(host) + ":" + String(port));
    }

    void beginRequest() override
    {
        _headers = "";
        _body = "";
        if (_logger)
            _logger->debug("[SimpleHttpClient] beginRequest called");
    }

    void endRequest() override
    {
        if (_client.connected())
        {
            _client.stop();
            if (_logger)
                _logger->debug("[SimpleHttpClient] TCP connection closed");
        }
    }

    void get(const String &path) override { _sendRequest("GET", path); }
    void post(const String &path) override { _sendRequest("POST", path); }
    void put(const String &path) override { _sendRequest("PUT", path); }
    void del(const String &path) override { _sendRequest("DELETE", path); }

    void sendHeader(const char *name, const String &value) override
    {
        _headers += String(name) + ": " + value + "\r\n";
        if (_logger)
            _logger->debug("[SimpleHttpClient] Added header: " + String(name) + " = " + value);
    }

    void sendHeader(const char *name, int value) override
    {
        _headers += String(name) + ": " + value + "\r\n";
        if (_logger)
            _logger->debug("[SimpleHttpClient] Added header: " + String(name) + " = " + String(value));
    }

    void beginBody() override
    {
        if (_logger)
            _logger->debug("[SimpleHttpClient] beginBody called");
    }

    void print(const String &data) override
    {
        _body = data;
        if (_logger)
            _logger->debug("[SimpleHttpClient] Body set: " + data);
    }

    int responseStatusCode() override { return _statusCode; }

    String responseBody() override { return _response; }

    bool connected() const override { return _client.connected(); }

private:
    NetworkClient &_client;
    const char *_host;
    uint16_t _port;
    RumpshiftLogger *_logger;

    String _headers;
    String _body;
    String _response;
    int _statusCode;

    void _sendRequest(const String &method, const String &path)
    {
        if (_logger)
            _logger->info("[SimpleHttpClient] Sending " + method + " request to " + String(_host) + ":" + String(_port) + path);

        if (!_client.connect(_host, _port))
        {
            _statusCode = 0;
            if (_logger)
                _logger->warn("[SimpleHttpClient] Failed to connect to host");
            return;
        }

        // Send request line and Host header
        _client.print(method + " " + path + " HTTP/1.1\r\n");
        _client.print("Host: " + String(_host) + "\r\n");

        if (_logger)
            _logger->debug("[SimpleHttpClient] Request line sent: " + method + " " + path);

        // Send additional headers
        if (_headers.length() > 0)
        {
            _client.print(_headers);
            if (_logger)
                _logger->debug("[SimpleHttpClient] Custom headers sent:\n" + _headers);
        }

        // Send Content-Length header if body exists
        if (_body.length() > 0)
        {
            _client.print("Content-Length: " + String(_body.length()) + "\r\n");
            if (_logger)
                _logger->debug("[SimpleHttpClient] Content-Length: " + String(_body.length()));
        }

        _client.print("\r\n"); // End of headers

        // Send body
        if (_body.length() > 0)
        {
            _client.print(_body);
            if (_logger)
                _logger->debug("[SimpleHttpClient] Request body sent");
        }

        // Read response
        _response = "";
        unsigned long start = millis();
        while (_client.connected() || _client.available())
        {
            while (_client.available())
            {
                _response += (char)_client.read();
            }

            // Safety timeout: 5s
            if (millis() - start > 5000)
            {
                if (_logger)
                    _logger->warn("[SimpleHttpClient] Response read timeout");
                break;
            }
        }

        if (_logger)
            _logger->debug("[SimpleHttpClient] Raw response:\n" + _response);

        // Parse HTTP status code from response
        int space1 = _response.indexOf(' ');
        int space2 = _response.indexOf(' ', space1 + 1);
        if (space1 > 0 && space2 > space1)
        {
            _statusCode = _response.substring(space1 + 1, space2).toInt();
            if (_logger)
                _logger->info("[SimpleHttpClient] Parsed status code: " + String(_statusCode));
        }
        else
        {
            _statusCode = -1;
            if (_logger)
                _logger->warn("[SimpleHttpClient] Failed to parse status code from response");
        }

        // Clear headers and body for next request
        _headers = "";
        _body = "";
    }
};

#endif // SIMPLE_HTTP_CLIENT_H
