#ifndef POST_LOG_HTTP_H
#define POST_LOG_HTTP_H

#include <Arduino.h>
#include <QueueArray.h>       // Simple in-RAM queue for messages
#include "NetworkProtocol.h"  // Base interface for protocols
#include "RumpusHttpClient.h" // Base HTTP client
#include "RumpshiftLogger.h"
#include "Storage.h"

constexpr size_t QUEUE_INITIAL_CAPACITY = 128;

/**
 * @class PostLogHttp
 * @brief Logs JSON messages via HTTP POST using a NetworkManager.
 *
 * Responsibilities:
 *  - Maintains a queue of messages for asynchronous posting.
 *  - Uses a RumpusHttpClient member to perform actual HTTP requests.
 *  - Handles retries if the network is temporarily unavailable.
 *
 * Notes:
 *  - Does not own the NetworkManager; it must remain valid during the lifetime.
 *  - Uses optional RumpshiftLogger for debug/info output.
 */
class PostLogHttp
{
public:
    /**
     * @brief Construct a PostLogHttp instance.
     * @param network Reference to a NetworkManager providing connectivity.
     * @param logger Optional logger for debug/info messages.
     * @param path Optional HTTP path for POST requests (default "/").
     * @param queueFailedRequests Enable or disable queuing of failed messages.
     */
    explicit PostLogHttp(
        NetworkManager &network,
        RumpshiftLogger *logger = nullptr,
        const String &path = "/",
        bool queueFailedRequests = true,
        Storage *storage = nullptr);

    ~PostLogHttp() = default;

    /**
     * @brief Initialize internal HTTP client.
     * Typically called in setup().
     */
    void begin();

    /**
     * @brief Enqueue a message to be posted asynchronously.
     * Non-blocking; processQueue() will attempt sending.
     * @param message JSON payload or string to post
     */
    void log(const String &message);

    /**
     * @brief Process queued messages.
     * Should be called in loop().
     * Retries failed messages if network temporarily unavailable.
     */
    void processQueue();

    /**
     * @brief Clear all queued messages from memory.
     */
    void clearQueue();

    /**
     * @brief Set the HTTP path for POST requests.
     * @param path New HTTP path (e.g., "/api/log")
     */
    void setPath(const String &path);

    void loadFromStorage();
    void saveToStorage(const String &message);

private:
    String _path;                 ///< HTTP path for POST requests
    QueueArray<String> _queue;    ///< Queue of messages to send
    RumpshiftLogger *_logger;     ///< Optional logger
    RumpusHttpClient _httpClient; ///< Internal HTTP client for sending messages
    bool _queueFailedRequests;    ///< Whether failed messages are queued
    Storage *_storage;

    /**
     * @brief Attempt to send a single message via HTTP.
     * @param message Message to send
     * @return true if successful, false if network unavailable
     */
    bool sendHttp(const String &message);
};

#endif // POST_LOG_HTTP_H
