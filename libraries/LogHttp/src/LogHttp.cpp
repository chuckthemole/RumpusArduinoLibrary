#include "LogHttp.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512

LogHttp::LogHttp() : _queue(10) {}

LogHttp::LogHttp(const String &host, uint16_t port, RumpshiftLogger *logger)
    : _host(host), _port(port), _queue(10), _logger(logger) {}

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
        delete _httpClient;
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

void LogHttp::log(const String &message)
{
    // if (!_wifiClient.connected() || !sendHttp(message)) TODO: look into this check
    if (!sendHttp(message))
    {
        if (_logger)
            _logger->warn("LogHttp: network unavailable, queuing message");
        _queue.push(message);
        saveToStorage(message);
    }
    processQueue();
}

void LogHttp::processQueue()
{
    // if (!_wifiClient.connected()) TODO: look into this check
    // {
    //     if (_logger)
    //         _logger->warn("LogHttp: skipping queue, WiFi not connected");
    //     return;
    // }

    while (!_queue.isEmpty())
    {
        String msg = _queue.peek();
        if (!sendHttp(msg))
        {
            saveToStorage(msg);
            break;
        }
        _queue.pop();
    }

    loadFromStorage();
    while (!_queue.isEmpty())
    {
        String msg = _queue.peek();
        if (!sendHttp(msg))
            break;
        _queue.pop();
    }
}

bool LogHttp::sendHttp(const String &message)
{
    if (_host.length() == 0)
    {
        if (_logger)
            _logger->error("LogHttp error: Host not set");
        return false;
    }
    if (!_httpClient)
        begin();

    _httpClient->beginRequest();
    _httpClient->post(_path.c_str());
    _httpClient->sendHeader("Content-Type", _contentType.c_str());
    _httpClient->sendHeader("Content-Length", message.length());
    _httpClient->beginBody();
    _httpClient->print(message);
    _httpClient->endRequest();

    int status = _httpClient->responseStatusCode();

    if (status >= 200 && status < 300)
    {
        if (_logger)
            _logger->info("LogHttp: sent successfully, status " + String(status));
        return true;
    }
    else
    {
        if (_logger)
            _logger->warn("LogHttp failed, status code: " + String(status));
        return false;
    }
}

void LogHttp::saveToStorage(const String &message)
{
    String stored;
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        char c = EEPROM.read(i);
        if (c == 0xFF || c == '\0')
            break;
        stored += c;
    }

    if (stored.length() > 0)
        stored += '\n';
    stored += message;

    if (stored.length() > EEPROM_SIZE - 1)
        stored = stored.substring(stored.length() - (EEPROM_SIZE - 1));

    for (int i = 0; i < stored.length(); i++)
        EEPROM.update(i, stored[i]);
    EEPROM.update(stored.length(), '\0');

    if (_logger)
        _logger->debug("LogHttp: saved message to EEPROM");
}

void LogHttp::loadFromStorage()
{
    String stored;
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        char c = EEPROM.read(i);
        if (c == 0xFF || c == '\0')
            break;
        stored += c;
    }

    if (stored.length() == 0)
        return;

    int start = 0;
    int end = stored.indexOf('\n');
    while (end != -1)
    {
        String line = stored.substring(start, end);
        if (line.length() > 0)
            _queue.push(line);
        start = end + 1;
        end = stored.indexOf('\n', start);
    }

    String lastLine = stored.substring(start);
    if (lastLine.length() > 0)
        _queue.push(lastLine);

    for (int i = 0; i < EEPROM_SIZE; i++)
        EEPROM.update(i, '\0');

    if (_logger)
        _logger->debug("LogHttp: loaded queued messages from EEPROM");
}
