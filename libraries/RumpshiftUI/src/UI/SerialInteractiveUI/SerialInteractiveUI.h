#ifndef SERIAL_INTERACTIVE_UI_H
#define SERIAL_INTERACTIVE_UI_H

#include <Arduino.h>
#include <map>
#include "../InteractiveUI.h"

/**
 * @brief Abstract base class for interactive UIs that use Serial input.
 *
 * Provides a framework for mapping typed characters or strings from Serial
 * to UIBase::Action values. Subclasses can implement hardware-specific behavior
 * or additional input parsing if needed.
 */
class SerialInteractiveUI : public InteractiveUI
{
public:
    SerialInteractiveUI(unsigned long baud = 9600)
        : _baudRate(baud) {}

    virtual ~SerialInteractiveUI() {}

    /**
     * @brief Subclasses must implement this to return raw input values.
     *
     * @return Raw input identifier (or -1 if none).
     */
    virtual int readRaw() = 0;

    /** Initialize Serial communication. Must be called in setup(). */
    virtual void begin() override
    {
        Serial.begin(_baudRate);
        Serial.println("=== SerialInteractiveUI initialized ===");
        delay(100); // Give Serial a moment to initialize
    }

    /** Clear output (Serial monitor). */
    virtual void clear() override
    {
        // Serial monitor doesn't support clearing; print newlines instead
        for (int i = 0; i < 20; ++i)
            Serial.println();
    }

    /** Print without newline. */
    virtual void print(const String &msg) override
    {
        Serial.print(msg);
    }

    /** Print with newline. */
    virtual void println(const String &msg) override
    {
        Serial.println(msg);
    }

    // /**
    //  * @brief Return current Action based on Serial input.
    //  *
    //  * Non-blocking: returns Action::NONE if no input available.
    //  */
    // Action getAction() override
    // {
    //     if (!Serial.available())
    //         return Action::NONE;

    //     String input = "";
    //     while (Serial.available())
    //     {
    //         char c = Serial.read();
    //         if (c == '\n' || c == '\r')
    //             break;
    //         input += c;
    //     }

    //     return parseAction(input);
    // }

    // /**
    //  * @brief Blocking version of getAction: waits until input is received.
    //  */
    // Action waitForAction(const String &msg = "Waiting for input (SerialInteractiveUI)") override
    // {
    //     println(msg);
    //     while (true)
    //     {
    //         Action act = getAction();
    //         if (act != Action::NONE)
    //             return act;
    //         delay(10); // small debounce/delay
    //     }
    // }

    /**
     * @brief Optional text input prompt.
     */
    String prompt(const String &msg) override
    {
        println(msg);
        while (!Serial.available())
        {
            delay(10);
        }
        String input = "";
        while (Serial.available())
        {
            char c = Serial.read();
            if (c == '\n' || c == '\r')
                break;
            input += c;
        }
        return input;
    }

protected:
    unsigned long _baudRate;
};

#endif // SERIAL_INTERACTIVE_UI_H
