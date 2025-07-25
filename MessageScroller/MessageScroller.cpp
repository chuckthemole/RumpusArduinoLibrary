#include "MessageScroller.h"
#include "font.h" // font data stored as bitmaps

MessageScroller::MessageScroller(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices)
    : numDevices(numDevices)
{
    lc = new LedControl(dataPin, clkPin, csPin, numDevices);
}

void MessageScroller::begin()
{
    for (uint8_t i = 0; i < numDevices; i++)
    {
        lc->shutdown(i, false);
        lc->setIntensity(i, 8);
        lc->clearDisplay(i);
    }
}

void MessageScroller::clearDisplay()
{
    for (uint8_t i = 0; i < numDevices; i++)
    {
        lc->clearDisplay(i);
    }
}

void MessageScroller::scrollMessage(const String &msg, int delayMs)
{
    for (int offset = 0; offset < msg.length() * 8; offset++)
    {
        clearDisplay();
        for (int col = 0; col < 8; col++)
        {
            int charIndex = (offset + col) / 8;
            int colOffset = (offset + col) % 8;
            if (charIndex < msg.length())
            {
                char c = msg.charAt(charIndex);
                byte columnData = getCharBitmap(c, colOffset);
                lc->setColumn(0, 7 - col, columnData); // reverse column order for scrolling
            }
        }
        delay(delayMs);
    }
}

byte MessageScroller::getCharBitmap(char c, int col)
{
    if (c < 32 || c > 127)
        return 0; // unsupported char
    return font[c - 32][col];
}
