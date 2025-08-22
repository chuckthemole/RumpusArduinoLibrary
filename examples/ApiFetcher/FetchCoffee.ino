#include <WiFiS3.h>
#include "ApiFetcher.h"
#include "CoffeeTypesFetcher.h"

const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASS";

// Change to your server URL
HttpFetcher fetcher("http://192.168.1.42:8000/api/coffee");
CoffeeTypesFetcher coffee(&fetcher);

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    if (coffee.fetch())
    {
        coffee.display();
    }
}

void loop()
{
    // refresh every 10 seconds
    delay(10000);
    coffee.fetch();
    coffee.display();
}
