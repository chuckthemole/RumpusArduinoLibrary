#pragma once
#include <functional>

/**
 * @brief Wraps a callable for a UI action.
 *
 * Allows storing lambdas, free functions, or member functions
 * as action handlers. Provides a simple interface to execute them.
 */
class ActionHandler
{
public:
    using HandlerFn = std::function<void()>;

    /** Default constructor: no handler assigned */
    ActionHandler() = default;

    /**
     * @brief Construct from any callable compatible with void().
     * Non-explicit to allow convenient implicit conversion from function/lambda.
     */
    ActionHandler(HandlerFn handler) : handler_(std::move(handler)) {}

    /** Execute the stored handler if assigned */
    void execute() const
    {
        if (handler_)
        {
            handler_();
        }
    }

    /**
     * @brief Helper to create an ActionHandler from a member function pointer
     *
     * This allows binding a class member function directly as an action handler,
     * so you don't need to write lambdas each time.
     *
     * @tparam T Class type
     * @param obj Pointer to the instance
     * @param method Pointer to the member function
     * @return ActionHandler wrapping the member function
     *
     * @code
     * // Example usage:
     * class MyUI {
     * public:
     *     void handleDown() { ui->println(F("DOWN!")); }
     * };
     *
     * MyUI uiObj;
     * ui->setActionHandler(
     *     Action("DOWN"),
     *     ActionHandler::fromMember(&uiObj, &MyUI::handleDown)
     * );
     *
     * // Now, when the "DOWN" action is triggered by the UI system (e.g., WASD keys),
     * // uiObj.handleDown() will be automatically executed.
     * @endcode
     */
    template <typename T>
    static ActionHandler fromMember(T *obj, void (T::*method)())
    {
        return ActionHandler([obj, method]()
                             { (obj->*method)(); });
    }

private:
    HandlerFn handler_;
};
