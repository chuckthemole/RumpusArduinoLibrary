```cpp
#include <WiFiS3.h>
#include "LogHttp.h"

const char* ssid = "yourSSID";
const char* password = "yourPassword";

LogHttp logger("your.api.server.com", 80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");

  // Optional chaining setters to override defaults
  logger.setPath("/log").setContentType("application/json");
  
  logger.log("{\"message\":\"Arduino started\"}");
}

void loop() {
  String logMsg = "{\"uptime\":" + String(millis()) + "}";
  logger.log(logMsg);
  delay(5000);
}

```