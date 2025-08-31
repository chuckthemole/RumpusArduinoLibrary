#ifndef LCD_INTERACTIVE_H
#define LCD_INTERACTIVE_H

#include <Arduino.h>
#include <map>
#include "../InteractiveUI.h"
#include "../../Output/LiquidCrystalWrapper/LiquidCrystalWrapper.h"

/**
 * @brief InteractiveUI implementation for LCD shields with analog buttons.
 *
 * Wraps a LiquidCrystalWrapper object and supports 5-button analog input.
 * Designed for Adafruit-style LCD shields or similar, where buttons share
 * a single analog pin. Button presses are mapped to UIBase::Action values.
 *
 * Supports two usage patterns:
 *   1. Provide an existing LiquidCrystalWrapper reference (full control).
 *   2. Let LCDInteractive create and own a LiquidCrystalWrapper internally
 *      with default pins and size.
 */
class LCDInteractive : public InteractiveUI
{
public:
    /**
     * @brief Construct using an existing LCD wrapper.
     *
     * @param lcdRef Reference to a LiquidCrystalWrapper object
     * @param buttonsPin Analog pin where buttons are connected
     */
    LCDInteractive(
        LiquidCrystalWrapper &lcdRef,
        uint8_t buttonsPin)
        : _lcdRef(lcdRef),
          _ownsLcd(false),
          _buttonsPin(buttonsPin) {}

    /**
     * @brief Construct with default LCD pins and size (owns LCD internally).
     *
     * @param buttonsPin Analog pin for buttons (default: A0)
     * @param rs LCD RS pin (default: 8)
     * @param enable LCD Enable pin (default: 9)
     * @param d4 LCD D4 pin (default: 4)
     * @param d5 LCD D5 pin (default: 5)
     * @param d6 LCD D6 pin (default: 6)
     * @param d7 LCD D7 pin (default: 7)
     * @param cols Number of columns (default: 16)
     * @param rows Number of rows (default: 2)
     */
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
          _buttonsPin(buttonsPin) {}

    virtual ~LCDInteractive() {}

    /** @brief Initialize button pin and LCD (if owned). Call in setup(). */
    void begin() override
    {
        pinMode(_buttonsPin, INPUT);

        if (_ownsLcd)
            _lcdRef.begin();
    }

    /** @brief Get current action based on analog button read. */
    Action getAction() override
    {
        int value = analogRead(_buttonsPin);
        Button btn = NONE;

        if (value < 50)
            btn = RIGHT;
        else if (value < 300)
            btn = UP;
        else if (value < 500)
            btn = DOWN;
        else if (value < 700)
            btn = LEFT;
        else if (value < 850)
            btn = SELECT;

        auto it = _buttonMap.find(btn);
        return (it != _buttonMap.end()) ? it->second : Action::NONE;
    }

    /**
     * @brief Map a button (raw input) to a specific UI action.
     *
     * @param rawInput Button enum value
     * @param action UIBase::Action to assign
     */
    void setInputMapping(uint16_t rawInput, Action action) override
    {
        _buttonMap[(Button)rawInput] = action;
    }

protected:
    /** @brief Button identifiers for analog input mapping. */
    enum Button
    {
        NONE = 0,
        RIGHT,
        UP,
        DOWN,
        LEFT,
        SELECT
    };

    LiquidCrystalWrapper &_lcdRef;       ///< Reference to LCD wrapper
    bool _ownsLcd;                       ///< True if LCDInteractive owns the LCD
    uint8_t _buttonsPin;                 ///< Analog pin for button input
    std::map<Button, Action> _buttonMap; ///< Mapping from Button -> UI Action

private:
    LiquidCrystalWrapper _internalLcd; ///< Internal LCD object (owned if _ownsLcd)
};

#endif // LCD_INTERACTIVE_H
