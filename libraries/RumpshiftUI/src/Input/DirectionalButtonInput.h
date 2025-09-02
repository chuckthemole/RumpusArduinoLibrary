#pragma once
#include <Arduino.h>
#include "InputType.h"

/**
 * @brief Concrete class representing directional button inputs.
 *
 * Each instance corresponds to a single button (LEFT, RIGHT, UP, DOWN, SELECT),
 * and can optionally be associated with a hardware pin.
 */
class DirectionalButtonInput : public InputType
{
public:
    /**
     * @brief Enum listing supported directional buttons.
     */
    enum Button
    {
        LEFT = 0,
        RIGHT,
        UP,
        DOWN,
        SELECT
    };

    /**
     * @brief Construct a DirectionalButtonInput with the given button.
     * @param button Button enum value.
     * @param pin Optional hardware pin associated with this button (default -1)
     */
    explicit DirectionalButtonInput(Button button, int pin = -1);

    /**
     * @brief Get the integer ID of this button.
     * @return int Button enum value.
     */
    int getId() const override;

    /**
     * @brief Get a human-readable name for this button.
     * @return String Button name (e.g., "LEFT").
     */
    String getName() const override;

private:
    Button _button; ///< The specific button represented by this instance.
};
