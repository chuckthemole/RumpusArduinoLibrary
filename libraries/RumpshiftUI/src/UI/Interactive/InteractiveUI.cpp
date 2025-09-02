#include "InteractiveUI.h"

void InteractiveUI::setInputMapping(InputType *input, const Action &action)
{
    if (!input || Action::isReserved(action))
    {
        // Cannot assign handler to reserved NONE action or null input
        return;
    }
    _actionMap[input] = action;
}

void InteractiveUI::setActionHandler(const Action &action, const ActionHandler &handler)
{
    if (Action::isReserved(action))
    {
        // Cannot assign handler to reserved NONE action
        return;
    }
    _handlerMap[action] = handler;
}

bool InteractiveUI::validateConfiguration() const
{
    for (const auto &entry : _actionMap)
    {
        const Action &action = entry.second;
        if (_handlerMap.find(action) == _handlerMap.end())
        {
            // No handler registered for this action
            return false;
        }
    }
    return true;
}

Action InteractiveUI::resolveAction(InputType *input) const
{
    if (!input)
        return Action::NONE;

    auto it = _actionMap.find(input);
    if (it != _actionMap.end())
    {
        return it->second;
    }
    return Action::NONE;
}

void InteractiveUI::executeHandler(const Action &action) const
{
    auto it = _handlerMap.find(action);
    if (it != _handlerMap.end())
    {
        it->second.execute();
    }
}

Action InteractiveUI::getAction()
{
    InputType *input = readRaw();
    if (!input)
        return Action::NONE;

    Action action = resolveAction(input);
    if (!Action::isReserved(action))
    {
        executeHandler(action);
    }
    return action;
}

Action InteractiveUI::waitForAction(const String &msg)
{
    if (!msg.isEmpty())
    {
        println(msg);
    }

    Action action = Action::NONE;
    while (action == Action::NONE)
    {
        // Poll until a valid action is received
        action = getAction();
    }
    return action;
}
