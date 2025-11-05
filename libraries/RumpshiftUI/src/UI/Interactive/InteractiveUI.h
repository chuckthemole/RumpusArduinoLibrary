#pragma once

#include "../UIBase.h"
#include "../Action.h"
#include "../ActionHandler.h"
#include "../../Input/InputType.h"
#include "Menu/MenuTemplate.h"
#include <map>

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
    /// Map from InputType pointers to Actions.
    std::map<InputType *, Action> _actionMap;

    /// Map from Actions to their associated handlers (optional).
    std::map<Action, ActionHandler> _handlerMap;

public:
    virtual ~InteractiveUI() = default;

    /**
     * @brief Associate an InputType with an Action.
     *
     * @param input  Pointer to an InputType instance.
     * @param action The Action to trigger for this input.
     */
    void setInputMapping(InputType *input, const Action &action);

    /**
     * @brief Bind a handler (lambda or function) to a given Action.
     *
     * @param action  The Action to handle.
     * @param handler The ActionHandler to execute when triggered.
     */
    void setActionHandler(const Action &action, const ActionHandler &handler);

    /**
     * @brief Validate configuration consistency between _actionMap and _handlerMap.
     */
    bool validateConfiguration() const;

    /**
     * @brief Attempt to resolve an InputType to its mapped Action.
     *
     * @param input InputType pointer.
     * @return The mapped Action, or Action::NONE if unmapped.
     */
    Action resolveAction(InputType *input) const;

    /**
     * @brief Execute the handler bound to the given Action (if any).
     */
    void executeHandler(const Action &action) const;

    /**
     * @brief Return the current Action based on the most recent raw input.
     */
    virtual Action getAction() final;

    /**
     * @brief Block until a valid Action is received.
     */
    virtual Action waitForAction(const String &msg = "Waiting for input") final;

    /**
     * @brief Display a menu to the user.
     *
     * Derived classes should override this method to render the menu
     * according to the specific UI type (LCD, touchscreen, voice, etc.).
     *
     * The provided MenuTemplate contains the menu title, items, and the
     * currently selected index. Rendering may involve highlighting the
     * selected item, scrolling long lists, or other UI-specific behaviors.
     *
     * @param menu The MenuTemplate representing the menu to display.
     *
     * @note This function is intended to be overridden in subclasses.
     *       Calling the base implementation (if any) may not produce
     *       meaningful output.
     */
    virtual void showMenu(const MenuTemplate &menu, size_t selectedIndex = 0) = 0;
};
