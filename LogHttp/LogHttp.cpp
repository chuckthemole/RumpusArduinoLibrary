#include "LogHttp.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512 // Adjust if your Uno R4 has more EEPROM
static const char LOG_QUEUE_TXT[] = "/logqueue.txt";
static const int LOG_QUEUE_ADDR = 0; // starting address for log queue

LogHttp::LogHttp() : _queue(10)
{
    // empty
}

LogHttp::LogHttp(const String &host, uint16_t port) : _host(host), _port(port), _queue(10)
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

void LogHttp::log(const String &message)
{
    _queue.push(message);
    this->processQueue();
}

void LogHttp::processQueue()
{
    if (!_wifiClient.connected())
    {
        // Could attempt WiFi reconnect here if needed
        return;
    }

    // Send RAM queue first
    while (!_queue.isEmpty())
    {
        String msg = _queue.peek();
        if (!sendHttp(msg))
        {
            saveToStorage(msg); // store if send fails
            break;              // stop trying this cycle
        }
        _queue.pop();
    }

    // Then flush any stored messages
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

void LogHttp::saveToStorage(const String &message)
{
    // Read existing data
    String stored;
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        char c = EEPROM.read(i);
        if (c == 0xFF || c == '\0')
            break; // end of stored data
        stored += c;
    }

    if (stored.length() > 0)
    {
        stored += '\n';
    }
    stored += message;

    // Truncate if too long
    if (stored.length() > EEPROM_SIZE - 1)
    {
        stored = stored.substring(stored.length() - (EEPROM_SIZE - 1));
    }

    // Write back to EEPROM
    for (int i = 0; i < stored.length(); i++)
    {
        EEPROM.update(i, stored[i]);
    }
    EEPROM.update(stored.length(), '\0'); // null-terminate
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
            _queue.push(line); // Changed from enqueue() to push()
        start = end + 1;
        end = stored.indexOf('\n', start);
    }

    String lastLine = stored.substring(start);
    if (lastLine.length() > 0)
        _queue.push(lastLine);

    // Clear EEPROM after loading
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        EEPROM.update(i, '\0');
    }
}
