#pragma once

#include <Arduino.h>
#include "../../LCDInteractive.h"
#include "../../SerialWASDInteractive.h"
#include "../../../../Input/DirectionalButtonInput.h"

/**
 * @brief Hybrid UI: behaves like LCDInteractive but echoes to Serial as a mock LCD.
 *
 * - Inherits from LCDInteractive so client code can treat it as a drop-in.
 * - Composes a SerialWASDInteractive for serial I/O.
 * - Simulates an LCD with a 2D buffer and draws it to Serial with a border.
 * - Maps keyboard:
 *      W -> UP, A -> LEFT, S -> DOWN, D -> RIGHT, Enter -> SELECT
 */
class SerialLCDInteractive : public LCDInteractive
{
public:
    /**
     * @brief Construct with internally owned "LCD" (pins are irrelevant for Serial mock).
     *
     * @param cols Number of columns in mock LCD
     * @param rows Number of rows in mock LCD
     * @param baud Serial baud rate
     */
    SerialLCDInteractive(uint8_t cols = 16, uint8_t rows = 2, unsigned long baud = 9600);

    /**
     * @brief Construct using an external LCD wrapper (display still goes to Serial)
     *
     * @param lcdRef Reference to external LCD wrapper
     * @param buttonsPin Analog pin (unused in serial mock)
     * @param cols Number of columns in mock LCD
     * @param rows Number of rows in mock LCD
     * @param baud Serial baud rate
     */
    SerialLCDInteractive(LiquidCrystalWrapper &lcdRef, uint8_t buttonsPin,
                         uint8_t cols, uint8_t rows, unsigned long baud = 9600);

    ~SerialLCDInteractive() override;

    /** Initialize Serial; do not init the real LCD. */
    void begin() override;

    /** Clear the mock LCD buffer and redraw. */
    void clear() override;

    /** Write text into mock LCD buffer, no newline. */
    void print(const String &msg) override;

    /** Write text into mock LCD buffer, then move to new line. */
    void println(const String &msg) override;

    /** Read input from Serial through SerialWASDInteractive. */
    InputType *readRaw() override;

protected:
    SerialWASDInteractive _serialUI; ///< Serial I/O handler

    const int COLS;
    const int ROWS;
    char **_buffer = nullptr; ///< 2D character buffer representing the LCD
    int _cursorRow = 0;
    int _cursorCol = 0;

    /** Virtual handlers for directional and select buttons */
    virtual void buttonLeft();
    virtual void buttonRight();
    virtual void buttonUp();
    virtual void buttonDown();
    virtual void buttonSelect();

private:
    /** Allocate and initialize the mock LCD buffer */
    void _initBuffer();

    /** Reset buffer content to spaces */
    void initBuffer();

    /** Clear a specific row in the buffer */
    void clearRow(int row);

    /** Append string to buffer; optionally add a newline */
    void appendToBuffer(const String &msg, bool newLine);

    /** Redraw the buffer to Serial with borders */
    void redrawLCD();
};
