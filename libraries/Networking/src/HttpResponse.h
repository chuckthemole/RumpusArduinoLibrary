#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <Arduino.h>
#include <map>

/**
 * @class HttpResponse
 * @brief Represents a parsed HTTP response.
 *
 * Encapsulates status code, headers, and body.
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
    String body() const { return _body; }
    String headers() const
    {
        String result;
        for (auto const &kv : _headers)
        {
            result += kv.first + ": " + kv.second + "\n";
        }
        return result;
    }
    String full() const
    {
        return "HTTP Status: " + String(_statusCode) + "\n" + headers() + "\n" + _body;
    }

    // Optional: fetch specific header
    String header(const String &key) const
    {
        auto it = _headers.find(key);
        if (it != _headers.end())
            return it->second;
        return "";
    }

private:
    int _statusCode;
    String _body;
    std::map<String, String> _headers;
};

#endif // HTTP_RESPONSE_H
