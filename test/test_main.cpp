#include <Arduino.h>
#include <unity.h>

#include "RumpshiftLogger_unit/test_logger.cpp"
#include "WiFiNetworkManager_unit/test_wifi.cpp"

void setup()
{
    UNITY_BEGIN();
    run_logger_tests();
    run_wifi_tests();
    UNITY_END();
}

void loop() {}
