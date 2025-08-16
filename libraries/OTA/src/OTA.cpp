#include <WiFiS3.h>        // For R4 WiFi
#include <ArduinoOTA.h>
#include "OTA.h"

void setupOTA() {
    ArduinoOTA.begin();
}

void handleOTA() {
    ArduinoOTA.handle();
}
