#include <Arduino.h>
#include <RumpusArduinoCore.h>
#include "config.h"   // optional if you keep your WiFi creds/server here

// --- Example configuration ---
// Enable WiFi + HTTP client, disable others
RumpusConfig cfg = {
    .useWiFi        = true,
    .useHttpClient  = true,
    .useHttpLogger  = false,
    .useTimeHelper  = false,
    .usePinManager  = false
};

// --- Global core instance ---
RumpusArduinoCore rumpus;

void setup() {
    // Start Serial + initialize selected modules
    rumpus.begin(cfg, WIFI_SSID, WIFI_PASS, LAN_IP, 8000);

    rumpus.logger.info("BasicSetup example starting...");

    // Example: Make a GET request if enabled
    if (rumpus.httpClient != nullptr) {
        String url = "/api/hello";
        String resp = rumpus.httpClient->get(url);
        rumpus.logger.info("GET " + url + " => " + resp);
    }
}

void loop() {
    // Keep connections and services alive
    rumpus.maintain();

    delay(1000);
}
