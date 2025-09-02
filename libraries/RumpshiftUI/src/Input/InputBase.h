#ifndef INPUT_BASE_H
#define INPUT_BASE_H

#include <Arduino.h>
#include "InputType.h"

/**
 * @brief Abstract base class for all user input providers.
 *
 * InputBase represents the raw input layer (buttons, sensors, encoders, etc.)
 * without interpreting them as higher-level UI actions.
 *
 * Higher-level mappings (like "LEFT", "CONFIRM", "CANCEL") should be handled
 * by InteractiveUI or Action classes in the UI layer.
 */
class InputBase
{
public:
    virtual ~InputBase() {}

    /**
     * @brief Initialize the input device.
     *
     * Example: setting pinMode for buttons, configuring hardware, etc.
     */
    virtual void begin() = 0;

    /**
     * @brief Subclasses must implement this to return the currently triggered InputType.
     *
     * @return Pointer to triggered InputType, or nullptr if none.
     */
    virtual InputType *readRaw() = 0;

    /**
     * @brief Clear or reset the input state (optional).
     *
     * Default implementation does nothing.
     */
    virtual void clear() {}
};

#endif // INPUT_BASE_H
