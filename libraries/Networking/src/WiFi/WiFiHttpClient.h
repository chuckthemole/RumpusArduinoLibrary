#ifndef WIFI_HTTP_CLIENT_H
#define WIFI_HTTP_CLIENT_H

#include <Arduino.h>
#include <WiFiClient.h>
#include "RumpshiftLogger.h"
#include "HttpResponse.h" // include your Response class

/**
 * @class WiFiHttpClient
 * @brief Simple HTTP client wrapper using WiFiClient with logging.
 */
class WiFiHttpClient
{
public:
    WiFiHttpClient() = default;

    explicit WiFiHttpClient(WiFiClient &client, RumpshiftLogger *logger = nullptr)
        : _client(client), _logger(logger) {}

    void setLogger(RumpshiftLogger *logger) { _logger = logger; }

    // --------------------
    // HTTP Methods
    // --------------------
    HttpResponse get(const String &host, uint16_t port, const String &path)
    {
        return sendRequest("GET", host, port, path, "");
    }

    HttpResponse post(const String &host, uint16_t port, const String &path, const String &body)
    {
        return sendRequest("POST", host, port, path, body);
    }

    HttpResponse put(const String &host, uint16_t port, const String &path, const String &body)
    {
        return sendRequest("PUT", host, port, path, body);
    }

    HttpResponse del(const String &host, uint16_t port, const String &path)
    {
        return sendRequest("DELETE", host, port, path, "");
    }

private:
    WiFiClient _client;
    RumpshiftLogger *_logger = nullptr;

    HttpResponse sendRequest(
        const String &method,
        const String &host,
        uint16_t port,
        const String &path,
        const String &body)
    {
        HttpResponse response;

        if (!_client.connect(host.c_str(), port))
        {
            if (_logger)
                _logger->error("[WiFiHttpClient] Failed to connect to " + host + ":" + String(port));
            response.setStatus(0);
            return response;
        }

        // Build HTTP request
        String req = method + " " + path + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n";
        if (body.length() > 0)
        {
            req += "Content-Type: application/json\r\n";
            req += "Content-Length: " + String(body.length()) + "\r\n";
        }
        req += "\r\n" + body;

        _client.print(req);
        if (_logger)
            _logger->info("[WiFiHttpClient] Request sent:\n" + req);

        // -----------------------------
        // Parse status line
        // -----------------------------
        String statusLine = _client.readStringUntil('\n');
        statusLine.trim();
        if (statusLine.length() > 0)
            response.setStatus(parseStatusCode(statusLine));

        // -----------------------------
        // Parse headers
        // -----------------------------
        int contentLength = -1;
        while (_client.connected())
        {
            String headerLine = _client.readStringUntil('\n');
            headerLine.trim();
            if (headerLine.length() == 0) // blank line = end of headers
                break;

            int sep = headerLine.indexOf(':');
            if (sep > 0)
            {
                String key = headerLine.substring(0, sep);
                String value = headerLine.substring(sep + 1);
                value.trim();
                response.addHeader(key, value);

                if (key.equalsIgnoreCase("Content-Length"))
                    contentLength = value.toInt();
            }
        }

        // -----------------------------
        // Read body based on Content-Length (if available)
        // -----------------------------
        String bodyStr;
        unsigned long start = millis();
        if (contentLength > 0)
        {
            while (bodyStr.length() < contentLength)
            {
                while (_client.available())
                    bodyStr += (char)_client.read();

                if (millis() - start > 5000) // timeout
                    break;
            }
        }
        else
        {
            // fallback if no content-length
            while (_client.connected() || _client.available())
            {
                while (_client.available())
                    bodyStr += (char)_client.read();

                if (millis() - start > 5000)
                    break;
            }
        }

        response.setBody(bodyStr);

        if (_logger)
        {
            _logger->info("[WiFiHttpClient] Response - STATUS: " + String(response.status()));
            _logger->info("[WiFiHttpClient] Response - HEADERS:\n" + response.headers());
            _logger->info("[WiFiHttpClient] Response - BODY:\n" + response.body());
        }

        _client.stop();
        return response;
    }

    int parseStatusCode(const String &statusLine)
    {
        // Example: HTTP/1.1 200 OK
        int firstSpace = statusLine.indexOf(' ');
        int secondSpace = statusLine.indexOf(' ', firstSpace + 1);
        if (firstSpace > 0 && secondSpace > firstSpace)
        {
            String codeStr = statusLine.substring(firstSpace + 1, secondSpace);
            return codeStr.toInt();
        }
        return 0;
    }
};

#endif // WIFI_HTTP_CLIENT_H
