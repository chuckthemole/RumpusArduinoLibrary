# WiFiHelper

**WiFiHelper** is a lightweight Arduino library that simplifies connecting to WiFi networks using Arduino-compatible boards with WiFi support (e.g., using the WiFiS3 library). It wraps common boilerplate and connection logic into a simple reusable class, helping you get connected with less code and better clarity.

---

## Features

- Connects to a WiFi network using SSID and password
- Prints connection status and useful debug info to Serial
- Designed to work with boards using the `WiFiS3` library

---

## Dependencies

Make sure your PlatformIO `platformio.ini` or `library.json` includes:

```ini
lib_deps =
    arduino-libraries/WiFiS3
```

## Getting Started
Here's a basic example of how to use the `WiFiHelper` class:
```cpp
#include <Arduino.h>
#include "WiFiHelper.h"
#include "config.h" // Needed for WIFI_SSID and WIFI_PASS

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;

// Create a WiFiHelper instance
WiFiHelper wifiHelper(ssid, password);

void setup()
{
    Serial.begin(BAUD_RATE);
    while (!Serial)
        ;

    Serial.println("Booting WiFi helper...");

    // Connect to WiFi
    wifiHelper.begin();

    // Print connection details
    wifiHelper.printStatus();
}

void loop()
{
    wifiHelper.maintainConnection();
    delay(1000);
}
```

Example config.h:
```cpp
// config.h
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "yourssid"
#define WIFI_PASS "yourpassword"
#define IS_SIMULATION true // other vars
#define USE_WIFI true // another var

#endif
```
