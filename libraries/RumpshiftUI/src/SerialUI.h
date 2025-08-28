#ifndef SERIAL_UI_H
#define SERIAL_UI_H

#include <Arduino.h>
#include "UIBase.h"

/**
 * @brief Serial implementation of UIBase for prototyping.
 * 
 * Maps typed keys (numbers or letters) to Action enums.
 * Supports printing output and text input prompts via Serial.
 */
class SerialUI : public UIBase {
public:
    /**
     * @brief Construct a new SerialUI object with optional baud rate.
     * @param baud Baud rate for Serial communication (default: 9600)
     */
    SerialUI(unsigned long baud = 9600);

    /** Initialize the Serial port. Must be called in setup(). */
    void begin() override;

    /** Clear Serial buffer. */
    void clear() override;

    /** Print without newline. */
    void print(const String& msg) override;

    /** Print with newline. */
    void println(const String& msg) override;

    /** Return the current action, or Action::NONE if no input. */
    Action getAction() override;

    /** A blocking return action, or Action::NONE if no input. */
    Action waitForAction(const String &msg) override;

    /** Prompt user for text input. */
    String prompt(const String& msg) override;

private:
    unsigned long baudRate;

    /** Helper: map a typed string to an Action enum */
    Action parseAction(const String& input);
};

#endif // SERIAL_UI_H
