```cpp
#include <Arduino.h>
#include <WiFiS3.h>
#include "WiFiHelper.h"

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiHelper wifiHelper;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Booting...");

    wifiHelper.connect(ssid, password);
    wifiHelper.printStatus();
}

void loop() {
    // Test ping to confirm it's still connected
    delay(5000);
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected! Attempting reconnect...");
        wifiHelper.connect(ssid, password);
    } else {
        Serial.println("WiFi still connected.");
    }
}
```