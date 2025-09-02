#ifndef LCD_INTERACTIVE_H
#define LCD_INTERACTIVE_H

#include <Arduino.h>
#include <map>
#include "../../Input/DirectionalButtonInput.h"
#include "InteractiveUI.h"
#include "../../Output/LiquidCrystalWrapper/LiquidCrystalWrapper.h"

/**
 * @brief InteractiveUI for LCD shields with analog buttons.
 *
 * Wraps a LiquidCrystalWrapper object and maps analog button values
 * to DirectionalButtonInput instances, which are then mapped to Actions.
 */
class LCDInteractive : public InteractiveUI
{
public:
    /**
     * @brief Construct using an existing LCD wrapper.
     * @param lcdRef Reference to LiquidCrystalWrapper
     * @param buttonsPin Analog pin for buttons
     */
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

    /**
     * @brief Construct with default LCD pins (internally owned LCD).
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
        : _internalLcd(
              rs,
              enable,
              d4,
              d5,
              d6,
              d7,
              cols,
              rows),
          _lcdRef(_internalLcd),
          _ownsLcd(true),
          _buttonsPin(buttonsPin),
          _left(DirectionalButtonInput::LEFT),
          _right(DirectionalButtonInput::RIGHT),
          _up(DirectionalButtonInput::UP),
          _down(DirectionalButtonInput::DOWN),
          _select(DirectionalButtonInput::SELECT)
    {
    }

    virtual ~LCDInteractive() {}

    /** @brief Initialize LCD and button pin. Call in setup(). */
    void begin() override
    {
        pinMode(_buttonsPin, INPUT);
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

protected:
    LiquidCrystalWrapper &_lcdRef;
    bool _ownsLcd;
    uint8_t _buttonsPin;
    LiquidCrystalWrapper _internalLcd;

    // DirectionalButtonInput instances
    DirectionalButtonInput _left;
    DirectionalButtonInput _right;
    DirectionalButtonInput _up;
    DirectionalButtonInput _down;
    DirectionalButtonInput _select;
};

#endif // LCD_INTERACTIVE_H
