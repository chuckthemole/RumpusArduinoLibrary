#pragma once
#include <Arduino.h>

// ----------------------------
// Abstract Board Base (Static Interface)
// ----------------------------
// This base class does not enforce runtime polymorphism,
// it just acts as a tag and shared namespace.
// ----------------------------
class Board
{
public:
    virtual ~Board() = default;

    // Each board defines its own nested struct `PIN`
    // with static constexpr integers.
    // Example: UnoR4WiFi::PIN::D2
};
