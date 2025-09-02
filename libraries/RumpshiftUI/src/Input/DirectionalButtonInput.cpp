#include "DirectionalButtonInput.h"

/**
 * @brief Construct a DirectionalButtonInput with the given button and optional pin.
 */
DirectionalButtonInput::DirectionalButtonInput(Button button, int pin)
    : InputType(pin), _button(button)
{
}

/**
 * @brief Get the integer ID of this button.
 */
int DirectionalButtonInput::getId() const
{
    return static_cast<int>(_button);
}

/**
 * @brief Get a human-readable name for this button.
 */
String DirectionalButtonInput::getName() const
{
    switch (_button)
    {
    case LEFT:
        return "LEFT";
    case RIGHT:
        return "RIGHT";
    case UP:
        return "UP";
    case DOWN:
        return "DOWN";
    case SELECT:
        return "SELECT";
    default:
        return "UNKNOWN";
    }
}
