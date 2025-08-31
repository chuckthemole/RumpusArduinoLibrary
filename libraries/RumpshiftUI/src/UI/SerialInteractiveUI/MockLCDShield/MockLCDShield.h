#ifndef MOCK_LCD_SHIELD_H
#define MOCK_LCD_SHIELD_H

#include <Arduino.h>
#include "../SerialInteractiveUI.h"
#include <map>

/**
 * @brief Mock version of an LCD Interactive UI for prototyping.
 *
 * This class prints output to Serial instead of a physical LCD
 * and reads keyboard input via Serial.
 *
 * Key mapping:
 *   W - UP
 *   A - LEFT
 *   S - DOWN
 *   D - RIGHT
 *   Enter - SELECT
 */
class MockLCDShield : public SerialInteractiveUI
{
public:
    MockLCDShield() = default;

    /**
     * @brief Constructor with optional baud rate for Serial.
     * @param baud Serial baud rate (default 9600).
     */
    explicit MockLCDShield(unsigned long baud = 9600)
        : SerialInteractiveUI(baud) {}

    virtual ~MockLCDShield() = default;

    /**
     * @brief Initialize Serial port and configure input/action mappings.
     * Call in Arduino setup().
     */
    void begin() override
    {
        SerialInteractiveUI::begin();

        // Map buttons to Actions
        setInputMapping(Button::BUTTON_LEFT, Action("LEFT"));
        setInputMapping(Button::BUTTON_RIGHT, Action("RIGHT"));
        setInputMapping(Button::BUTTON_UP, Action("UP"));
        setInputMapping(Button::BUTTON_DOWN, Action("DOWN"));
        setInputMapping(Button::BUTTON_SELECT, Action("SELECT"));

        // Bind Actions to handler methods
        setActionHandler(Action("LEFT"), ActionHandler([this]()
                                                       { buttonLeft(); }));
        setActionHandler(Action("RIGHT"), ActionHandler([this]()
                                                        { buttonRight(); }));
        setActionHandler(Action("UP"), ActionHandler([this]()
                                                     { buttonUp(); }));
        setActionHandler(Action("DOWN"), ActionHandler([this]()
                                                       { buttonDown(); }));
        setActionHandler(Action("SELECT"), ActionHandler([this]()
                                                         { buttonSelect(); }));

        Serial.println(F("=== MockLCDShield initialized ==="));
    }

    /**
     * @brief Read keyboard input from Serial and map to Button enum.
     * @return Corresponding Button enum value, or NONE if no input.
     */
    int readRaw() override
    {
        if (!Serial.available())
            return Button::BUTTON_NONE;

        char c = Serial.read();
        switch (c)
        {
        case 'w':
        case 'W':
            return Button::BUTTON_UP;
        case 'a':
        case 'A':
            return Button::BUTTON_LEFT;
        case 's':
        case 'S':
            return Button::BUTTON_DOWN;
        case 'd':
        case 'D':
            return Button::BUTTON_RIGHT;
        case '\r':
        case '\n':
            return Button::BUTTON_SELECT;
        default:
            return Button::BUTTON_NONE;
        }
    }

    /**
     * @brief Virtual handlers for each button.
     * Subclasses can override to change behavior dynamically.
     */
    virtual void buttonLeft() { Serial.println(F("LEFT pressed")); }
    virtual void buttonRight() { Serial.println(F("RIGHT pressed")); }
    virtual void buttonUp() { Serial.println(F("UP pressed")); }
    virtual void buttonDown() { Serial.println(F("DOWN pressed")); }
    virtual void buttonSelect() { Serial.println(F("SELECT pressed")); }

    /** @brief Public access to predefined Actions */
    static const Action LEFT;
    static const Action RIGHT;
    static const Action UP;
    static const Action DOWN;
    static const Action SELECT;

protected:
    /**
     * @brief Button identifiers matching the physical LCDShield layout.
     */
    enum Button : uint16_t
    {
        BUTTON_NONE = 0,
        BUTTON_LEFT,
        BUTTON_RIGHT,
        BUTTON_UP,
        BUTTON_DOWN,
        BUTTON_SELECT
    };
};

// Define the static Action instances
inline const Action MockLCDShield::LEFT("LEFT");
inline const Action MockLCDShield::RIGHT("RIGHT");
inline const Action MockLCDShield::UP("UP");
inline const Action MockLCDShield::DOWN("DOWN");
inline const Action MockLCDShield::SELECT("SELECT");

#endif // MOCK_LCD_SHIELD_H
