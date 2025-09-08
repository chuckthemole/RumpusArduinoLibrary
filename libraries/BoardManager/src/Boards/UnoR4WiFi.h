#pragma once
#include "../Board.h"

// ----------------------------
// Concrete Board Class for Arduino Uno R4 WiFi
// ----------------------------
// Provides a static mapping of all pins so you can use
// UnoR4WiFi::PIN::D2 directly without instantiating the class.
// ----------------------------
class UnoR4WiFi : public Board
{
public:
    // Nested struct to hold all pin mappings
    struct PIN
    {
        // ----------------------------
        // Digital Pins
        // ----------------------------
        static constexpr int D2 = 2; // Example: Sensor pin
        static constexpr int D3 = 3;
        static constexpr int D4 = 4; // Example: Reset button pin
        static constexpr int D5 = 5;
        static constexpr int D6 = 6;
        static constexpr int D7 = 7;
        static constexpr int D8 = 8;
        static constexpr int D9 = 9;
        static constexpr int D10 = 10;
        static constexpr int D11 = 11;
        static constexpr int D12 = 12;
        static constexpr int D13 = 13; // Built-in LED

        // ----------------------------
        // Analog Pins
        // ----------------------------
        static constexpr int A0 = ::A0;
        static constexpr int A1 = ::A1;
        static constexpr int A2 = ::A2;
        static constexpr int A3 = ::A3;
        static constexpr int A4 = ::A4;
        static constexpr int A5 = ::A5;
    };

    // Optional: override Board's interface (if you want polymorphism later).
    // Since pins are static, we just return the PIN struct type.
    using Pins = PIN;
};
