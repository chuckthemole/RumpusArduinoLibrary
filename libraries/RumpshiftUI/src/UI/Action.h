#pragma once

#include <Arduino.h>

/**
 * @brief Represents a discrete user action in the UI system.
 *
 * Actions are used to identify events such as button presses,
 * menu selections, voice commands, etc. Actions can be mapped
 * to raw inputs and associated handlers.
 *
 * The special Action::NONE is reserved and represents "no action".
 *
 */
class Action
{
public:
    Action() : _name("") {}

    /**
     * @brief Construct an Action with a unique name.
     * @param name The name of the action (e.g., "UP", "SELECT").
     */
    explicit Action(String name) : _name(name) {}

    /**
     * @brief Retrieve the action's name.
     * @return const reference to the action name string.
     */
    const String &getName() const { return _name; }

    /**
     * @brief Comparison operator for use in associative containers.
     * @param other Another Action to compare against.
     * @return true if this Action is "less than" the other Action.
     */
    bool operator<(const Action &other) const { return _name < other._name; }

    /**
     * @brief Equality operator.
     * @param other Another Action to compare against.
     * @return true if the names are equal.
     */
    bool operator==(const Action &other) const { return _name == other._name; }

    /**
     * @brief Inequality operator.
     * @param other Another Action to compare against.
     * @return true if the names are not equal.
     */
    bool operator!=(const Action &other) const { return _name != other._name; }

    /**
     * @brief Reserved action representing "no action".
     *
     * This action is used internally by UI systems to signify
     * unmapped or idle inputs. Clients should not map handlers
     * or raw inputs to Action::NONE.
     */
    static const Action NONE;

    /**
     * @brief Reserved action representing "no action".
     *
     * This action is used internally by UI systems to signify
     * unmapped or idle inputs. Clients should not map handlers
     * or raw inputs to Action::EMPTY.
     */
    static const Action EMPTY;

    static const Action UP;
    static const Action DOWN;
    static const Action LEFT;
    static const Action RIGHT;
    static const Action SELECT;

    /**
     * @brief Return true if the action is a reserved action.
     * TODO: let subclass define more reserved actions?
     */
    static bool isReserved(Action action)
    {
        const String name = action.getName();
        if (name == "NONE" || name == "")
        {
            return true;
        }
        return false;
    }

private:
    String _name; ///< Unique name of the action
};

// Define the static NONE action
inline const Action Action::NONE("NONE");
inline const Action Action::EMPTY("");

// Directional Actions, and select
inline const Action Action::UP("UP");
inline const Action Action::DOWN("DOWN");
inline const Action Action::LEFT("LEFT");
inline const Action Action::RIGHT("RIGHT");
inline const Action Action::SELECT("SELECT");
