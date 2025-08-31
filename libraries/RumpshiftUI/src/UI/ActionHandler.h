#pragma once
#include <functional>

class ActionHandler
{
public:
    using HandlerFn = std::function<void()>;

    ActionHandler() = default;
    explicit ActionHandler(HandlerFn handler) : handler_(std::move(handler)) {}

    void execute() const
    {
        if (handler_)
        {
            handler_();
        }
    }

private:
    HandlerFn handler_;
};
