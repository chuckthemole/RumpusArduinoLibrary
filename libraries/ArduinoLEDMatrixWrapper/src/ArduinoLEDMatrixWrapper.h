#pragma once
#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

class ArduinoLEDMatrixWrapper
{
public:
    ArduinoLEDMatrixWrapper();

    // Builder entrypoint
    ArduinoLEDMatrixWrapper &build(const String &text);

    // Chainable config
    ArduinoLEDMatrixWrapper &stop(unsigned long ms);
    ArduinoLEDMatrixWrapper &speed(unsigned long ms);
    ArduinoLEDMatrixWrapper &loop(int count); // -1 = infinite

    // Run scrolling
    void run();
    void begin(); // legacy immediate start (no chain)

    // Utility
    void clear();
    String toString();

private:
    ArduinoLEDMatrix _matrix;
    uint8_t _frame[8][12];

    String _text = "";
    unsigned long _stopDuration = 0;
    unsigned long _speed = 200; // default ms per shift
    int _loopCount = -1;        // -1 = infinite

    void clearFrame();

    // For full scroll rendering
    void drawLetterToFullFrame(char c, int colOffset, uint8_t fullFrame[8][128]);
    int getLetterWidth(char c);
    void drawHFull(int col, uint8_t fullFrame[8][128]);
    void drawEFull(int col, uint8_t fullFrame[8][128]);
    void drawLFull(int col, uint8_t fullFrame[8][128]);
    void drawOFull(int col, uint8_t fullFrame[8][128]);

    // Legacy drawing (for direct displayText)
    void drawLetter(char c, int col);
    void drawH(int col);
    void drawE(int col);
    void drawL(int col);
    void drawO(int col);
};
