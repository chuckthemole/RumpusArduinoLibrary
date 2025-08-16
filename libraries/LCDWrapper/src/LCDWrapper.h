#ifndef LCDWRAPPER_H
#define LCDWRAPPER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

/**
 * @brief A convenient wrapper around the LiquidCrystal library for managing
 *        common LCD operations with default pin configuration.
 *
 * This class abstracts typical LCD operations like setting cursor position,
 * printing messages, and scrolling the display. It simplifies initialization
 * by providing default pin mappings and common display dimensions (16x2).
 *
 * Example:
 *
 *     LcdWrapper lcd;        // Uses default pin layout and size
 *     lcd.begin();
 *     lcd.print("Hello, LCD");
 */
class LcdWrapper
{
public:
    /**
     * @brief Construct a new LcdWrapper object with optional pin and size configuration.
     *
     * @param rs     Register select pin (default: 8)
     * @param enable Enable pin (default: 9)
     * @param d4     Data pin 4 (default: 4)
     * @param d5     Data pin 5 (default: 5)
     * @param d6     Data pin 6 (default: 6)
     * @param d7     Data pin 7 (default: 7)
     * @param cols   Number of columns (default: 16)
     * @param rows   Number of rows (default: 2)
     */
    LcdWrapper(
        uint8_t rs = 8,
        uint8_t enable = 9,
        uint8_t d4 = 4,
        uint8_t d5 = 5,
        uint8_t d6 = 6,
        uint8_t d7 = 7,
        uint8_t cols = 16,
        uint8_t rows = 2);

    /**
     * @brief Initializes the LCD with the configured number of columns and rows.
     * Must be called in `setup()` before using the LCD.
     */
    void begin();

    /**
     * @brief Clears the LCD screen.
     */
    void clear();

    /**
     * @brief Prints a message at the current cursor position.
     *
     * @param msg The message to print.
     */
    void print(const String &msg);

    void print(const char *msg);
    void print(int value);
    void print(unsigned int value);
    void print(long value);
    void print(unsigned long value);
    void print(float value, int decimals = 2);

    /**
     * @brief Sets the cursor to a specific column and row.
     *
     * @param col Column position (0-based).
     * @param row Row position (0-based).
     */
    void setCursor(uint8_t col, uint8_t row);

    /**
     * @brief Sets the cursor and prints a message at the specified location.
     *
     * @param col Column position (0-based).
     * @param row Row position (0-based).
     * @param msg Message to print.
     */
    void printAt(uint8_t col, uint8_t row, const String &msg);

    /**
     * @brief Scrolls the display one position to the left.
     */
    void scrollDisplayLeft();

    /**
     * @brief Scrolls the display one position to the right.
     */
    void scrollDisplayRight();

private:
    LiquidCrystal lcd; ///< Underlying LiquidCrystal instance.
    uint8_t columns;   ///< Number of LCD columns.
    uint8_t rows;      ///< Number of LCD rows.
};

#endif // LCDWRAPPER_H
