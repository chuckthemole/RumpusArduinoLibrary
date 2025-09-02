#pragma once
#include <Arduino.h>

/**
 * @brief Abstract base class for all types of input.
 *
 * InputType represents any input source, including buttons, sensors, or other triggers.
 * Optionally, an input can be associated with a hardware pin.
 */
class InputType
{
public:
    /**
     * @brief Construct an InputType with optional hardware pin.
     * @param pin Hardware pin number, or -1 if unassigned.
     */
    explicit InputType(int pin = -1) : _pin(pin) {} // TODO: think about taking multiple pins. Right now DirectionalButtonInput just needs one analog pin

    virtual ~InputType() = default;

    /**
     * @brief Get the integer ID of this input.
     * Must be implemented by subclasses.
     * @return int Unique identifier for the input.
     */
    virtual int getId() const = 0;

    /**
     * @brief Get a human-readable name for this input.
     * Must be implemented by subclasses.
     * @return String Name of the input.
     */
    virtual String getName() const = 0;

    /**
     * @brief Get the hardware pin associated with this input.
     * @return int Pin number, or -1 if unassigned.
     */
    int getPin() const { return _pin; }

protected:
    int _pin; ///< Hardware pin associated with this input (-1 if none)
};
