#include "PostPrintJobHttp.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512

PostPrintJobHttp::PostPrintJobHttp(
    NetworkManager &network,
    RumpshiftLogger *logger,
    const String &path,
    bool queueFailedRequests)
    : _httpClient(network, logger),
      _logger(logger),
      _path(path),
      _queue(PRINT_QUEUE_INITIAL_CAPACITY),
      _queueFailedRequests(queueFailedRequests)
{
}

void PostPrintJobHttp::begin()
{
    _httpClient.begin();
    if (_queueFailedRequests)
        loadFromStorage();
}

void PostPrintJobHttp::enqueueJob(const String &job)
{
    if (_logger)
        _logger->info("[PostPrintJobHttp::enqueueJob] called with job: " + job);

    bool sent = sendHttp(job);

    if (!sent && _queueFailedRequests)
    {
        if (_logger)
            _logger->warn("[PostPrintJobHttp::enqueueJob] network unavailable, queuing job");

        String copy = job;
        _queue.push(copy);

        if (_logger)
            _logger->info("[PostPrintJobHttp::enqueueJob] saving job to EEPROM");

        saveToStorage(copy);
    }
    else if (sent && _logger)
    {
        _logger->info("[PostPrintJobHttp::enqueueJob] job sent successfully");
    }

    if (_logger)
        _logger->info("[PostPrintJobHttp::enqueueJob] processing queue");

    if (_queueFailedRequests)
        processQueue();

    if (_logger)
        _logger->info("[PostPrintJobHttp::enqueueJob] enqueueJob() completed");
}

void PostPrintJobHttp::processQueue()
{
    while (!_queue.isEmpty())
    {
        String job = _queue.peek();
        if (!sendHttp(job))
        {
            saveToStorage(job);
            break;
        }
        _queue.pop();
    }
}

bool PostPrintJobHttp::sendHttp(const String &job)
{
    if (_logger)
        _logger->info("[PostPrintJobHttp::sendHttp] called with job: " + job);

    if (!_httpClient.isConnected())
    {
        if (_logger)
            _logger->warn("[PostPrintJobHttp::sendHttp] network disconnected, cannot send job");
        return false;
    }

    if (_logger)
        _logger->info("[PostPrintJobHttp::sendHttp] network connected, attempting POST to path: " + _path);

    _httpClient.post(_path, job);
    int status = _httpClient.lastStatusCode();

    if (_logger)
        _logger->info("[PostPrintJobHttp::sendHttp] HTTP POST completed, status code: " + String(status));

    return (status >= 200 && status < 300);
}

void PostPrintJobHttp::clearQueue()
{
    while (!_queue.isEmpty())
        _queue.pop();
}

void PostPrintJobHttp::saveToStorage(const String &job)
{
    if (!_queueFailedRequests)
        return;

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
    stored += job;

    if (stored.length() > EEPROM_SIZE - 1)
        stored = stored.substring(stored.length() - (EEPROM_SIZE - 1));

    for (int i = 0; i < stored.length(); i++)
        EEPROM.update(i, stored[i]);
    EEPROM.update(stored.length(), '\0');

    if (_logger)
        _logger->debug("[PostPrintJobHttp] saved job to EEPROM");
}

void PostPrintJobHttp::loadFromStorage()
{
    if (!_queueFailedRequests)
        return;

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
        _logger->debug("[PostPrintJobHttp] loaded queued jobs from EEPROM");
}

void PostPrintJobHttp::setPath(const String &path)
{
    _path = path;
}
