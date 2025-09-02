#ifndef SERIAL_INTERACTIVE_UI_H
#define SERIAL_INTERACTIVE_UI_H

#include <Arduino.h>
#include <map>
#include "InteractiveUI.h"

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

protected:
    unsigned long _baudRate;
};

#endif // SERIAL_INTERACTIVE_UI_H
