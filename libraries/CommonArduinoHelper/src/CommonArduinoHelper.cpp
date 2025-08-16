#include "CommonArduinoHelper.h"

// Instantiate the global lcd object
LiquidCrystal LCD::lcd(LCD::rs, LCD::en, LCD::d4, LCD::d5, LCD::d6, LCD::d7);

// Reads analog value and returns button enum (for LCD Keypad Shield)
Button CommonHelper::readButton(int analogValue)
{
    if (analogValue < 50)
        return btnRIGHT;
    if (analogValue < 200)
        return btnUP;
    if (analogValue < 400)
        return btnDOWN;
    if (analogValue < 600)
        return btnLEFT;
    if (analogValue < 800)
        return btnSELECT;
    return btnNONE;
}

// A simple non-blocking delay using millis()
void CommonHelper::nonBlockingDelay(unsigned long ms)
{
    unsigned long start = millis();
    while (millis() - start < ms)
    {
        // You could add yield() or check other flags here
    }
}
