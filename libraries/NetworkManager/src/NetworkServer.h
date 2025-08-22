#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include "NetworkClient.h"

/**
 * @class NetworkServer
 * @brief Abstract interface for TCP server functionality.
 */
class NetworkServer
{
public:
    virtual ~NetworkServer() {}

    /**
     * @brief Begin listening on the server.
     */
    virtual void begin() = 0;

    /**
     * @brief Check for and return a new client connection.
     * @return Pointer to a NetworkClient for the new connection, or nullptr if none.
     */
    virtual NetworkClient *available() = 0;
};

#endif // NETWORK_SERVER_H
