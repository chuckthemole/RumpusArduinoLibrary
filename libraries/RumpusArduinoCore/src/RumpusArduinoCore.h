#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

// Custom libs
#include <RumpshiftLogger.h>
#include <NetworkManager.h>
#include <WiFi/WiFiNetworkManager.h>
#include <HTTP/PostLogHttp.h>
#include <RumpusHttpClient.h>
#include <TimeHelper.h>
#include <PinManager.h>
#include <Boards/UnoR4WiFi.h>

/**
 * Configuration flags for enabling/disabling subsystems.
 * Each flag indicates whether the corresponding service
 * should be initialized during `begin()`.
 */
struct RumpusConfig
{
    bool useWiFi = false;
    bool useHttpClient = false;
    bool useHttpLogger = false;
    bool useTimeHelper = false;
    bool usePinManager = false;
};

/**
 * Core wrapper class for managing Rumpus Arduino subsystems.
 * Provides lifecycle control for logger, networking, HTTP, time sync, and pins.
 */
class RumpusArduinoCore
{
public:
    // Core logger (always available)
    RumpshiftLogger logger;

    // Optional subsystems (initialized conditionally)
    NetworkManager *network = nullptr;
    PostLogHttp *postHttpLogger = nullptr;
    RumpusHttpClient *httpClient = nullptr;
    PinManager *pinManager = nullptr;
    TimeHelper *timeHelper = nullptr;

    // Shared JSON buffer for structured logs
    StaticJsonDocument<512> logDoc;

    RumpusArduinoCore();
    ~RumpusArduinoCore();

    /**
     * Initialize enabled subsystems based on provided configuration.
     *
     * @param cfg Configuration flags for enabling features.
     * @param ssid Optional WiFi SSID (required if useWiFi = true).
     * @param password Optional WiFi password (required if useWiFi = true).
     * @param server Optional server hostname/IP (for HTTP subsystems).
     * @param port Optional server port (for HTTP subsystems).
     */
    void begin(const RumpusConfig &cfg,
               const char *ssid = nullptr,
               const char *password = nullptr,
               const char *server = nullptr,
               int port = 0);

    /**
     * Periodic maintenance call. Should be run in loop().
     * Keeps subsystems alive (e.g. WiFi reconnect, time sync).
     */
    void maintain();

private:
    RumpusConfig config; // Store last applied config for internal use

    // Helpers to safely delete and reset pointers
    template <typename T>
    void safeDelete(T *&ptr)
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
};
