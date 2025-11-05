#include "RumpusArduinoCore.h"

RumpusArduinoCore::RumpusArduinoCore()
{
    // Default logger initialization
    logger.log("RumpusArduinoCore constructed.");
}

RumpusArduinoCore::~RumpusArduinoCore()
{
    // Clean up dynamically allocated subsystems
    safeDelete(network);
    safeDelete(postHttpLogger);
    safeDelete(httpClient);
    safeDelete(pinManager);
    safeDelete(timeHelper);

    logger.log("RumpusArduinoCore destroyed.");
}

void RumpusArduinoCore::begin(const RumpusConfig &cfg,
                              const char *ssid,
                              const char *password,
                              const char *server,
                              int port)
{
    config = cfg; // store applied config

    logger.log("RumpusArduinoCore: begin() called");

    if (config.useWiFi)
    {
        if (ssid && password)
        {
            logger.log("WiFi enabled. Initializing...");
            network = new WiFiNetworkManager(ssid, password);
            network->begin();
        }
        else
        {
            logger.error("WiFi config enabled but no SSID/password provided.");
        }
    }

    if (config.useHttpClient)
    {
        if (server && port > 0)
        {
            logger.log("HTTP Client enabled. Initializing...");
            httpClient = new RumpusHttpClient(server, port);
        }
        else
        {
            logger.error("HTTP Client enabled but server/port missing.");
        }
    }

    if (config.useHttpLogger)
    {
        if (httpClient)
        {
            logger.log("HTTP Logger enabled. Initializing...");
            postHttpLogger = new PostLogHttp(*httpClient);
        }
        else
        {
            logger.error("HTTP Logger requires HttpClient to be enabled first.");
        }
    }

    if (config.useTimeHelper)
    {
        logger.log("TimeHelper enabled. Initializing...");
        timeHelper = new TimeHelper();
        timeHelper->begin();
    }

    if (config.usePinManager)
    {
        logger.log("PinManager enabled. Initializing...");
        pinManager = new PinManager();
        pinManager->begin();
    }

    logger.log("RumpusArduinoCore initialization complete.");
}

void RumpusArduinoCore::maintain()
{
    if (config.useWiFi && network)
    {
        network->maintain(); // Keep WiFi alive
    }

    if (config.useTimeHelper && timeHelper)
    {
        timeHelper->maintain(); // Sync NTP if needed
    }

    if (config.useHttpLogger && postHttpLogger)
    {
        // Could flush queued logs if implemented
    }

    if (config.usePinManager && pinManager)
    {
        pinManager->maintain(); // Optional: handle pin state updates
    }
}
