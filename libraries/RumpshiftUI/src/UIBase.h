#ifndef UI_BASE_H
#define UI_BASE_H

#include <Arduino.h>

/**
 * @brief Abstract base class for all user interfaces.
 *
 * This class defines the common interface for output (print/println/clear)
 * and input (getAction, prompt) across different UI implementations.
 */
class UIBase
{
public:
    // Possible actions returned by getAction()
    enum class Action : int
    {
        NONE = -1,   ///< No action available
        ACTION0 = 0, ///< Generic action 0
        ACTION1,     ///< Generic action 1
        ACTION2,     ///< Generic action 2
        ACTION3,     ///< Generic action 3
        ACTION4,     ///< Generic action 4
        // Extend with more actions as needed
    };

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
     * @brief Return the current action, or Action::NONE if no action.
     *
     * @return Action enum value
     */
    virtual Action getAction() { return Action::NONE; }

    /**
     * @brief A blocking return action, or Action::NONE if no action.
     *
     * @return Action enum value
     */
    virtual Action waitForAction(const String &msg = "Waiting for input") { return Action::NONE; }

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

    // ---------------------------
    // Convert Action enum to String
    // ---------------------------
    static String actionToString(Action action)
    {
        switch (action)
        {
        case Action::NONE:
            return "NONE";
        case Action::ACTION0:
            return "ACTION0";
        case Action::ACTION1:
            return "ACTION1";
        case Action::ACTION2:
            return "ACTION2";
        case Action::ACTION3:
            return "ACTION3";
        case Action::ACTION4:
            return "ACTION4";
        default:
            return "UNKNOWN";
        }
    }
};

#endif // UI_BASE_H
