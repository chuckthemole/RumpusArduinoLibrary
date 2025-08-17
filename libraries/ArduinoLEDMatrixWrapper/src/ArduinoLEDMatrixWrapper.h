#pragma once
#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

class ArduinoLEDMatrixWrapper
{
public:
    ArduinoLEDMatrixWrapper();

    void begin();
    void clear();
    void displayText(const String &text);
    String toString();

private:
    ArduinoLEDMatrix _matrix;
    uint8_t _frame[8][12];

    void clearFrame();

    // Full-frame helper for scrolling
    void drawLetterToFullFrame(char c, int colOffset, uint8_t fullFrame[8][128]);
    int getLetterWidth(char c);
    void drawHFull(int col, uint8_t fullFrame[8][128]);
    void drawEFull(int col, uint8_t fullFrame[8][128]);
    void drawLFull(int col, uint8_t fullFrame[8][128]);
    void drawOFull(int col, uint8_t fullFrame[8][128]);

    // Legacy letter drawing (optional if you want to keep)
    void drawLetter(char c, int col);
    void drawH(int col);
    void drawE(int col);
    void drawL(int col);
    void drawO(int col);
};
