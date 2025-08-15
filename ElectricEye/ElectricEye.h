#pragma once
#include <Arduino.h>

class ElectricEye
{
public:
    // Constructor: pin number, threshold value for detecting beam break
    ElectricEye(uint8_t pin, int threshold = 500);

    // Initialize the sensor (call in setup)
    void begin();

    // Returns true if beam is currently blocked
    bool isBlocked();

    // Call this in loop() to detect changes
    void update();

    // Optional: set callbacks
    void onBeamBroken(void (*callback)());
    void onBeamRestored(void (*callback)());

private:
    uint8_t _pin;
    int _threshold;
    bool _lastState;
    void (*_beamBrokenCallback)();
    void (*_beamRestoredCallback)();
};
