#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include <Arduino.h>
#include "NetworkManager.h"

/**
 * @class NetworkProtocol
 * @brief Abstract base class for any protocol using a NetworkManager.
 */
class NetworkProtocol
{
public:
    virtual ~NetworkProtocol() {}

    /**
     * @brief Initialize the protocol (e.g., connect, handshake, etc.)
     */
    virtual void begin() = 0;

    /**
     * @brief Perform routine tasks for the protocol (keepalive, retries, etc.)
     */
    virtual void maintain() = 0;
};

#endif // NETWORK_PROTOCOL_H
