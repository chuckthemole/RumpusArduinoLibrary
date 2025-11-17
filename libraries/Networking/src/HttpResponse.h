#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <Arduino.h>
#include <map>

/**
 * @class HttpResponse
 * @brief Represents a parsed HTTP response, encapsulating status code, headers, and body.
 */
class HttpResponse
{
public:
    HttpResponse() : _statusCode(0) {}

    // ---------------------
    // Setters
    // ---------------------
    void setStatus(int status) { _statusCode = status; }
    void setBody(const String &body) { _body = body; }
    void addHeader(const String &key, const String &value) { _headers[key] = value; }

    // ---------------------
    // Getters
    // ---------------------
    int status() const { return _statusCode; }
    const String &body() const { return _body; }

    /**
     * @brief Returns all headers as a single formatted string.
     */
    String headersAsString() const
    {
        String result;
        for (const auto &kv : _headers)
        {
            result += kv.first + ": " + kv.second + "\n";
        }
        return result;
    }

    /**
     * @brief Returns a specific header value by key (case-sensitive).
     */
    String header(const String &key) const
    {
        auto it = _headers.find(key);
        if (it != _headers.end())
        {
            return it->second;
        }
        return "";
    }

    /**
     * @brief Returns a formatted representation of the full HTTP response.
     */
    String full() const
    {
        return "HTTP Status: " + String(_statusCode) + "\n" +
               headersAsString() + "\n" + _body;
    }

    // ---------------------
    // Convenience helpers
    // ---------------------

    /**
     * @brief Returns true if the response indicates success (2xx).
     */
    bool ok() const { return _statusCode >= 200 && _statusCode < 300; }

    /**
     * @brief Returns true if the response indicates redirection (3xx).
     */
    bool isRedirect() const { return _statusCode >= 300 && _statusCode < 400; }

    /**
     * @brief Returns true if the response indicates client error (4xx).
     */
    bool isClientError() const { return _statusCode >= 400 && _statusCode < 500; }

    /**
     * @brief Returns true if the response indicates server error (5xx).
     */
    bool isServerError() const { return _statusCode >= 500 && _statusCode < 600; }

    /**
     * @brief Returns the Content-Type header value if present.
     */
    String contentType() const { return header("Content-Type"); }

    /**
     * @brief Clears all data from the response object.
     */
    void clear()
    {
        _statusCode = 0;
        _body = "";
        _headers.clear();
    }

private:
    int _statusCode;
    String _body;
    std::map<String, String> _headers;
};

#endif // HTTP_RESPONSE_H
