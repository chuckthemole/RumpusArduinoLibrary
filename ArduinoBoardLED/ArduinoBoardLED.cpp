#include "ArduinoBoardLED.h"
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>

ArduinoLEDMatrix matrix;

ArduinoBoardLED::ArduinoBoardLED() {}

void ArduinoBoardLED::begin()
{
    matrix.begin();
}

void ArduinoBoardLED::displayMessage(const char *message, uint16_t speed)
{
    matrix.beginDraw();
    matrix.clear();
    matrix.setTextScrollSpeed(speed);
    matrix.setTextWrap(false); // Disable wrapping
    matrix.setCursor(0, 1);    // Y=1 for vertical centering
    matrix.print(message);
    matrix.endDraw();
}
