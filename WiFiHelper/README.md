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
#include <WiFiS3.h>
#include "WiFiHelper.h"

// Replace with your actual network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Create a WiFiHelper instance
WiFiHelper wifiHelper;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Booting...");

    // Connect to WiFi
    wifiHelper.connect(ssid, password);

    // Print connection details
    wifiHelper.printStatus();
}

void loop() {
    // Check WiFi connection status every 5 seconds
    delay(5000);

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected! Attempting to reconnect...");
        wifiHelper.connect(ssid, password);
    } else {
        Serial.println("WiFi still connected.");
    }
}
```
