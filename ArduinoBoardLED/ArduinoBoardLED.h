#ifndef ARDUINO_BOARD_LED_H
#define ARDUINO_BOARD_LED_H

#include <Arduino.h>

class ArduinoBoardLED
{
public:
    ArduinoBoardLED();
    void begin();
    void displayMessage(const char *message, uint16_t speed = 100);
};

#endif
