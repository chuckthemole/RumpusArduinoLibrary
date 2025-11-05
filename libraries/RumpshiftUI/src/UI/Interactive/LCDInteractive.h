#ifndef LCD_INTERACTIVE_H
#define LCD_INTERACTIVE_H

#include <Arduino.h>
#include <vector>
#include <map>
#include "../../Input/DirectionalButtonInput.h"
#include "InteractiveUI.h"
#include "../../Output/LiquidCrystalWrapper/LiquidCrystalWrapper.h"
#include "Menu/MenuTemplate.h"

/**
 * @brief InteractiveUI for LCD shields with analog buttons.
 *
 * Wraps a LiquidCrystalWrapper object and maps analog button values
 * to DirectionalButtonInput instances, which are then mapped to Actions.
 *
 * Provides utilities for:
 * - Displaying scrollable messages
 * - Displaying menus with selection highlighting
 * - Mapping button inputs to Actions
 */
class LCDInteractive : public InteractiveUI
{
public:
    /** @brief Construct using an existing LCD wrapper. */
    LCDInteractive(LiquidCrystalWrapper &lcdRef, uint8_t buttonsPin)
        : _lcdRef(lcdRef),
          _ownsLcd(false),
          _buttonsPin(buttonsPin),
          _left(DirectionalButtonInput::LEFT),
          _right(DirectionalButtonInput::RIGHT),
          _up(DirectionalButtonInput::UP),
          _down(DirectionalButtonInput::DOWN),
          _select(DirectionalButtonInput::SELECT)
    {
    }

    /** @brief Construct with default LCD pins (internally owned LCD). */
    LCDInteractive(
        uint8_t buttonsPin = A0,
        uint8_t rs = 8,
        uint8_t enable = 9,
        uint8_t d4 = 4,
        uint8_t d5 = 5,
        uint8_t d6 = 6,
        uint8_t d7 = 7,
        uint8_t cols = 16,
        uint8_t rows = 2)
        : _internalLcd(rs, enable, d4, d5, d6, d7, cols, rows),
          _lcdRef(_internalLcd),
          _ownsLcd(true),
          _buttonsPin(buttonsPin),
          _left(DirectionalButtonInput::LEFT),
          _right(DirectionalButtonInput::RIGHT),
          _up(DirectionalButtonInput::UP),
          _down(DirectionalButtonInput::DOWN),
          _select(DirectionalButtonInput::SELECT),
          _cols(cols),
          _rows(rows)
    {
    }

    virtual ~LCDInteractive() {}

    /** @brief Initialize LCD and button pin. Call in setup(). */
    void begin() override
    {
        pinMode(_buttonsPin, PinMode::INPUT);
        if (_ownsLcd)
            _lcdRef.begin();
    }

    /** @brief Read analog value and return pointer to corresponding DirectionalButtonInput. */
    InputType *readRaw() override
    {
        int value = analogRead(_buttonsPin);
        if (value < 50)
            return &_right;
        else if (value < 300)
            return &_up;
        else if (value < 500)
            return &_down;
        else if (value < 700)
            return &_left;
        else if (value < 850)
            return &_select;
        return nullptr;
    }

    /** @brief Display a message on the LCD, scrolls if longer than screen width. */
    void showMessage(const String &msg, uint8_t row = 0, uint16_t delayMs = 300)
    {
        if (msg.length() <= _cols)
        {
            _lcdRef.setCursor(0, row);
            _lcdRef.print(msg);
        }
        else
        {
            // Scroll message
            for (size_t offset = 0; offset <= msg.length() - _cols; ++offset)
            {
                _lcdRef.setCursor(0, row);
                _lcdRef.print(msg.substring(offset, offset + _cols));
                delay(delayMs);
            }
        }
    }

    /** @brief Display a menu with the selected item highlighted. */
    virtual void showMenu(const MenuTemplate &menu, size_t selectedIndex = 0) override
    {
        _lcdRef.clear();

        // Display items (fit within available rows, reserve row 0 for first item)
        for (size_t i = 0; i < menu.numItems && i < _rows; ++i)
        {
            _lcdRef.setCursor(0, i);
            String line = (i == selectedIndex ? ">" : " ");
            line += menu.items[i]; // menu.items is const char**
            _lcdRef.print(line.substring(0, _cols));
        }
    }

protected:
    LiquidCrystalWrapper &_lcdRef;     ///< Reference to LCD wrapper
    bool _ownsLcd;                     ///< True if LCD is internally owned
    uint8_t _buttonsPin;               ///< Analog pin for shield buttons
    LiquidCrystalWrapper _internalLcd; ///< Internally owned LCD instance

    const uint8_t _cols = 16; ///< LCD columns
    const uint8_t _rows = 2;  ///< LCD rows

    // DirectionalButtonInput instances
    DirectionalButtonInput _left;
    DirectionalButtonInput _right;
    DirectionalButtonInput _up;
    DirectionalButtonInput _down;
    DirectionalButtonInput _select;
};

#endif // LCD_INTERACTIVE_H
