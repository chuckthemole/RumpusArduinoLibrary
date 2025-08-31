#ifndef UI_BASE_H
#define UI_BASE_H

#include <Arduino.h>
#include "../Input/InputBase.h"
#include "../Output/OutputBase.h"

/**
 * @brief Abstract base class for all user interfaces.
 *
 * This class defines the common interface for output (print/println/clear)
 * and input (getAction, prompt) across different UI implementations.
 */
class UIBase : public InputBase, public OutputBase
{
public:
    virtual ~UIBase() {}

    /** @brief Initialize the interface. Must be called in setup(). */
    virtual void begin() = 0;

    /** @brief Clear the display or console output. */
    virtual void clear() = 0;

    /** @brief Print a message without a newline. */
    virtual void print(const String &msg) = 0;

    /** @brief Print a message followed by a newline. */
    virtual void println(const String &msg) = 0;


    /**
     * @brief Optional text input prompt.
     *
     * Default implementation just prints the message and returns empty string.
     * Override in implementations that support text input.
     */
    virtual String prompt(const String &msg)
    {
        println(msg);
        return "";
    }
};

#endif // UI_BASE_H
