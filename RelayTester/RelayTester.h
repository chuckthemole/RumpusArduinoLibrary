#ifndef RELAY_TESTER_H
#define RELAY_TESTER_H

#include <Arduino.h>

class RelayTester
{
public:
    RelayTester(uint8_t startPin = 2, uint8_t endPin = 13, unsigned long delayMillis = 1000);

    void begin();
    void testCycle();
    void testSingle(uint8_t pin);

private:
    uint8_t _startPin;
    uint8_t _endPin;
    unsigned long _delayMillis;
};

#endif
