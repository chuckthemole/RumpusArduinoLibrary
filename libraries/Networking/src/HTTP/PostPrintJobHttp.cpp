#include "PostPrintJobHttp.h"

PostPrintJobHttp::PostPrintJobHttp(
    NetworkManager &network,
    RumpshiftLogger *logger,
    const String &path,
    bool queueFailedRequests,
    Storage *storage)
    : _httpClient(network, logger),
      _logger(logger),
      _path(path),
      _queue(PRINT_QUEUE_INITIAL_CAPACITY),
      _queueFailedRequests(queueFailedRequests),
      _storage(storage)
{
}

void PostPrintJobHttp::begin()
{
    _httpClient.begin();
    if (_queueFailedRequests && _storage)
    {
        _storage->begin();
        loadFromStorage();
    }
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
            _logger->info("[PostPrintJobHttp::enqueueJob] saving job to storage");

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

    if (_storage)
        _storage->clear();
}

void PostPrintJobHttp::saveToStorage(const String &job)
{
    if (!_queueFailedRequests || !_storage)
        return;

    // Load current data
    String stored = _storage->load();

    if (stored.length() > 0)
        stored += '\n';
    stored += job;

    // Optional trimming if your backend storage has limits
    if (stored.length() > Storage::MAX_SIZE - 1)
        stored = stored.substring(stored.length() - (Storage::MAX_SIZE - 1));

    _storage->save(stored);

    if (_logger)
        _logger->debug("[PostPrintJobHttp] saved job to storage");
}

void PostPrintJobHttp::loadFromStorage()
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
        _logger->debug("[PostPrintJobHttp] loaded queued jobs from storage");
}

void PostPrintJobHttp::setPath(const String &path)
{
    _path = path;
}
