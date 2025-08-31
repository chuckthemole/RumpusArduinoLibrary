#ifndef POST_PRINT_JOB_HTTP_H
#define POST_PRINT_JOB_HTTP_H

#include <Arduino.h>
#include <QueueArray.h>       // In-RAM queue for print jobs
#include "NetworkProtocol.h"  // Base interface for protocols
#include "RumpusHttpClient.h" // Base HTTP client
#include "RumpshiftLogger.h"

constexpr size_t PRINT_QUEUE_INITIAL_CAPACITY = 128;

/**
 * @class PostPrintJobHttp
 * @brief Queues and sends print jobs via HTTP POST using a NetworkManager.
 *
 * Responsibilities:
 *  - Maintains a queue of print jobs for asynchronous posting.
 *  - Uses a RumpusHttpClient member to perform actual HTTP requests.
 *  - Handles retries if the network is temporarily unavailable.
 *
 * Notes:
 *  - Does not own the NetworkManager; it must remain valid during the lifetime.
 *  - Uses optional RumpshiftLogger for debug/info output.
 */
class PostPrintJobHttp
{
public:
    /**
     * @brief Construct a PostPrintJobHttp instance.
     * @param network Reference to a NetworkManager providing connectivity.
     * @param logger Optional logger for debug/info messages.
     * @param path Optional HTTP path for POST requests (default "/print").
     * @param queueFailedRequests Enable or disable queuing of failed jobs.
     */
    explicit PostPrintJobHttp(
        NetworkManager &network,
        RumpshiftLogger *logger = nullptr,
        const String &path = "/print",
        bool queueFailedRequests = true);

    ~PostPrintJobHttp() = default;

    /**
     * @brief Initialize internal HTTP client.
     * Typically called in setup().
     */
    void begin();

    /**
     * @brief Enqueue a print job to be posted asynchronously.
     * Non-blocking; processQueue() will attempt sending.
     * @param job JSON payload or string containing print instructions.
     */
    void enqueueJob(const String &job);

    /**
     * @brief Process queued jobs.
     * Should be called in loop().
     * Retries failed jobs if network temporarily unavailable.
     */
    void processQueue();

    /**
     * @brief Clear all queued jobs from memory.
     */
    void clearQueue();

    /**
     * @brief Set the HTTP path for POST requests.
     * @param path New HTTP path (e.g., "/api/print")
     */
    void setPath(const String &path);

    void loadFromStorage();
    void saveToStorage(const String &job);

private:
    String _path;                 ///< HTTP path for POST requests
    QueueArray<String> _queue;    ///< Queue of print jobs to send
    RumpshiftLogger *_logger;     ///< Optional logger
    RumpusHttpClient _httpClient; ///< Internal HTTP client for sending jobs
    bool _queueFailedRequests;    ///< Whether failed jobs are queued

    /**
     * @brief Attempt to send a single print job via HTTP.
     * @param job Job payload to send
     * @return true if successful, false if network unavailable
     */
    bool sendHttp(const String &job);
};

#endif // POST_PRINT_JOB_HTTP_H
