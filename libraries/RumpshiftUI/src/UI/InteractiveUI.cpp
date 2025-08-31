#include "InteractiveUI.h"

void InteractiveUI::setInputMapping(uint16_t rawInput, const Action &action)
{
    if (Action::isReserved(action))
    {
        // Cannot assign handler to reserved NONE action
        return;
    }
    _actionMap[rawInput] = action;
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

Action InteractiveUI::resolveAction(uint16_t rawInput) const
{
    auto it = _actionMap.find(rawInput);
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
    int raw = readRaw();
    if (raw < 0)
        return Action::NONE;

    Action action = resolveAction(static_cast<uint16_t>(raw));
    if (action.getName() != Action::NONE.getName())
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
        // TODO: add debug here
        action = getAction();
    }
    return action;
}