#ifndef COMMON_ARDUINO_HELPER_H
#define COMMON_ARDUINO_HELPER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD pin definitions (change these if your setup changes)
namespace LCD
{
    constexpr uint8_t rs = 8;
    constexpr uint8_t en = 9;
    constexpr uint8_t d4 = 4;
    constexpr uint8_t d5 = 5;
    constexpr uint8_t d6 = 6;
    constexpr uint8_t d7 = 7;

    // Extern reference to globally defined lcd instance
    extern LiquidCrystal lcd;
}

// Button definitions for analog button input (LCD keypad shield style)
enum Button
{
    btnRIGHT = 0,
    btnUP = 1,
    btnDOWN = 2,
    btnLEFT = 3,
    btnSELECT = 4,
    btnNONE = 5
};

// Helpers
namespace CommonHelper
{
    Button readButton(int analogValue);
    void nonBlockingDelay(unsigned long ms);
}

#endif // COMMON_ARDUINO_HELPER_H
