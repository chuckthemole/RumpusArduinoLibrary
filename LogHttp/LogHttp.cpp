#include "LogHttp.h"

LogHttp::LogHttp()
{
    // empty
}

LogHttp::LogHttp(const String &host, uint16_t port) : _host(host), _port(port)
{
    // lazy init _httpClient in log()
}

LogHttp &LogHttp::setHost(const String &host)
{
    _host = host;
    cleanup();
    return *this;
}

LogHttp &LogHttp::setPort(uint16_t port)
{
    _port = port;
    cleanup();
    return *this;
}

LogHttp &LogHttp::setPath(const String &path)
{
    _path = path;
    return *this;
}

LogHttp &LogHttp::setContentType(const String &contentType)
{
    _contentType = contentType;
    return *this;
}

void LogHttp::begin()
{
    if (_httpClient)
    {
        delete _httpClient;
    }
    _httpClient = new HttpClient(_wifiClient, _host.c_str(), _port);
}

void LogHttp::cleanup()
{
    if (_httpClient)
    {
        delete _httpClient;
        _httpClient = nullptr;
    }
}

bool LogHttp::log(const String &message)
{
    if (_host.length() == 0)
    {
        Serial.println("LogHttp error: Host not set");
        return false;
    }
    if (!_httpClient)
    {
        begin();
    }

    _httpClient->beginRequest();
    _httpClient->post(_path.c_str());
    _httpClient->sendHeader("Content-Type", _contentType.c_str());
    _httpClient->sendHeader("Content-Length", message.length());
    _httpClient->beginBody();
    _httpClient->print(message);
    _httpClient->endRequest();

    int status = _httpClient->responseStatusCode();
    if (status < 0)
    {
        Serial.println("Something failed during the http exchange. Consider returning smaller message back.");
    }
    if (status == HTTP_ERROR_CONNECTION_FAILED)
    {
        Serial.println("HTTP_ERROR_CONNECTION_FAILED");
    }
    else if (status == HTTP_ERROR_API)
    {

    }
    else if (status == HTTP_ERROR_TIMED_OUT)
    {
        Serial.println("HTTP_ERROR_TIMED_OUT");
    }
    else if (status == HTTP_ERROR_INVALID_RESPONSE)
    {
        Serial.println("HTTP_ERROR_INVALID_RESPONSE");
    }
    else if (status == 0)
    {
        Serial.println("Status return 0");
        return true;
    }
    else if (status >= 200 && status < 300)
    {
        Serial.println("Status return 200");
        return true;
    }
    else
    {
        Serial.print("LogHttp failed, status code: ");
        Serial.println(status);
        return false;
    }
}
