#include "PostLogHttp.h"

PostLogHttp::PostLogHttp(
    NetworkManager &network,
    RumpshiftLogger *logger,
    const String &path,
    bool queueFailedRequests,
    Storage *storage)
    : _httpClient(network, logger),
      _logger(logger),
      _path(path),
      _queue(QUEUE_INITIAL_CAPACITY),
      _queueFailedRequests(queueFailedRequests),
      _storage(storage)
{
}

void PostLogHttp::begin()
{
    _httpClient.begin();
    if (_queueFailedRequests && _storage)
    {
        _storage->begin();
        loadFromStorage();
    }
}

void PostLogHttp::log(const String &message)
{
    if (_logger)
        _logger->info("[PostLogHttp::log] called with message: " + message);

    bool sent = sendHttp(message);

    if (!sent && _queueFailedRequests)
    {
        if (_logger)
            _logger->warn("[PostLogHttp::log] network unavailable, queuing message");

        _queue.push(message);

        if (_logger)
            _logger->info("[PostLogHttp::log] saving message to storage");

        saveToStorage(message);
    }
    else if (sent && _logger)
    {
        _logger->info("[PostLogHttp::log] message sent successfully");
    }

    if (_logger)
        _logger->info("[PostLogHttp::log] processing queue");

    if (_queueFailedRequests)
        processQueue();

    if (_logger)
        _logger->info("[PostLogHttp::log] log() completed");
}

void PostLogHttp::processQueue()
{
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
}

bool PostLogHttp::sendHttp(const String &message)
{
    if (_logger)
        _logger->info("[PostLogHttp::sendHttp] called with message: " + message);

    if (!_httpClient.isConnected())
    {
        if (_logger)
            _logger->warn("[PostLogHttp::sendHttp] network disconnected, cannot send message");
        return false;
    }

    if (_logger)
        _logger->info("[PostLogHttp::sendHttp] network connected, attempting POST to path: " + _path);

    _httpClient.post(_path, message);
    int status = _httpClient.lastStatusCode();

    if (_logger)
        _logger->info("[PostLogHttp::sendHttp] HTTP POST completed, status code: " + String(status));

    return (status >= 200 && status < 300);
}

void PostLogHttp::clearQueue()
{
    while (!_queue.isEmpty())
        _queue.pop();

    if (_storage)
        _storage->clear();
}

void PostLogHttp::saveToStorage(const String &message)
{
    if (!_queueFailedRequests || !_storage)
        return;

    // Load current stored messages
    String stored = _storage->load();

    if (stored.length() > 0)
        stored += '\n';
    stored += message;

    // Trim if storage has a limit
    constexpr int STORAGE_LIMIT = 512;
    if (stored.length() > STORAGE_LIMIT - 1)
        stored = stored.substring(stored.length() - (STORAGE_LIMIT - 1));

    _storage->save(stored);

    if (_logger)
        _logger->debug("[PostLogHttp] saved message to storage");
}

void PostLogHttp::loadFromStorage()
{
    if (!_queueFailedRequests || !_storage)
        return;

    String stored = _storage->load();
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

    _storage->clear();

    if (_logger)
        _logger->debug("[PostLogHttp] loaded queued messages from storage");
}

void PostLogHttp::setPath(const String &path)
{
    _path = path;
}
