```cpp
config.h
```
```h
#pragma once

/**
 * @file config.h
 * @brief Central configuration file for system settings.
 *
 * This is where you define constants that affect the entire project,
 * such as baud rate, log level, feature toggles, etc.
 */

// Global Serial baud rate
#define BAUD_RATE 115200

// Logging level for the entire application
// Options: LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_WARN, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG
#define DEBUG_LEVEL LOG_LEVEL_DEBUG
```

```cpp
#include <Arduino.h>
#include "config.h"
#include <Logger.h>

// Create a global logger instance
Logger logger(BAUD_RATE, DEBUG_LEVEL);

void setup() {
    logger.begin();

    logger.info("System starting...");
    logger.debug("Debugging enabled. Verbose logs will be shown.");
}

void loop() {
    logger.warn("Battery level is low.");
    logger.error("Sensor disconnected!");
    delay(2000);
}

```