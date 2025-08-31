#ifndef INPUT_BASE_H
#define INPUT_BASE_H

#include <Arduino.h>

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
     * @brief Poll the input source and return its raw value.
     *
     * Each subclass decides what type of value makes sense:
     *   - For buttons: could be an integer button ID
     *   - For touchscreen: could be coordinates or region ID
     *   - For sensors: could be an analog value
     *
     * @return int Raw input identifier/value
     */
    virtual int readRaw() = 0;

    /**
     * @brief Clear or reset the input state (optional).
     *
     * Default implementation does nothing.
     */
    virtual void clear() {}
};

#endif // INPUT_BASE_H
