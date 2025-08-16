#ifndef MESSAGE_SCROLLER_H
#define MESSAGE_SCROLLER_H

#include <Arduino.h>
#include <LedControl.h>

class MessageScroller
{
public:
    MessageScroller(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices = 1);
    void begin();
    void scrollMessage(const String &msg, int delayMs = 100);

private:
    LedControl *lc;
    uint8_t numDevices;
    void clearDisplay();
    void displayChar(char c, int offset);
    byte getCharBitmap(char c, int row);
};

#endif
