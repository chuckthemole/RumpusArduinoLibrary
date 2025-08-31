#pragma once

#include "UIBase.h"
#include "Action.h"
#include "ActionHandler.h"

#include <map>
#include <cstdint>

/**
 * @brief Abstract base class for interactive UIs.
 *
 * InteractiveUI introduces the Action / ActionHandler system,
 * bridging raw input identifiers (button IDs, touch regions,
 * voice tokens, etc.) into higher-level Actions with attached
 * handlers.
 *
 * Subclasses must implement `readRaw()` to provide device-specific
 * input values. The base class manages mapping raw inputs to Actions
 * and executing associated handlers.
 */
class InteractiveUI : public UIBase
{
protected:
    /// Map from raw input identifiers (source-specific) to Actions.
    std::map<uint16_t, Action> _actionMap;

    /// Map from Actions to their associated handlers (optional).
    std::map<Action, ActionHandler> _handlerMap;

public:
    virtual ~InteractiveUI() = default;

    virtual void begin() = 0;

    /**
     * @brief Associate a raw input identifier with an Action.
     *
     * @param rawInput Raw identifier (e.g., button index, voice token).
     * @param action   The Action to trigger for this raw input.
     */
    void setInputMapping(uint16_t rawInput, const Action &action);

    /**
     * @brief Bind a handler (lambda or function) to a given Action.
     *
     * @param action  The Action to handle.
     * @param handler The ActionHandler to execute when triggered.
     */
    void setActionHandler(const Action &action, const ActionHandler &handler);

    /**
     * @brief Validate configuration consistency between _actionMap and _handlerMap.
     *
     * Ensures that every mapped raw input has a corresponding ActionHandler.
     *
     * @return true if all mappings have handlers, false otherwise.
     */
    bool validateConfiguration() const;

    /**
     * @brief Attempt to resolve a raw input to its mapped Action.
     *
     * @param rawInput Raw identifier from `readRaw()`.
     * @return The mapped Action, or a default "NONE" Action if unmapped.
     */
    Action resolveAction(uint16_t rawInput) const;

    /**
     * @brief Execute the handler bound to the given Action (if any).
     *
     * @param action The Action whose handler should be executed.
     */
    void executeHandler(const Action &action) const;

    /**
     * @brief Subclasses must implement this to return raw input values.
     *
     * Example return values:
     *   - Button shield: 0–4 for each button
     *   - Touchscreen: region ID
     *   - Voice: recognized phrase index
     *
     * @return Raw input identifier (or -1 if none).
     */
    virtual int readRaw() = 0;

    /**
     * @brief Return the current Action based on the most recent raw input.
     *
     * This method reads the raw input using readRaw(), resolves it to
     * an Action via _actionMap, and executes the associated handler
     * if one exists.
     *
     * @return Action enum representing the current user input,
     *         or Action::NONE if no action is detected.
     */
    virtual Action getAction() final;

    /**
     * @brief Block until a valid Action is received.
     *
     * Continuously polls getAction() until a non-NONE Action is returned.
     *
     * @param msg Optional message for debugging or logging while waiting.
     * @return The first valid Action detected.
     */
    virtual Action waitForAction(const String &msg = "Waiting for input") final;
};
