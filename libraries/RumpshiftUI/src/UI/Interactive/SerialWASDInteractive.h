#pragma once

#include <Arduino.h>
#include "SerialInteractiveUI.h"
#include "../../Input/DirectionalButtonInput.h"

/**
 * @brief WASD + Enter interaction style over Serial.
 */
class SerialWASDInteractive : public SerialInteractiveUI
{
public:
    explicit SerialWASDInteractive(unsigned long baud = 9600)
        : SerialInteractiveUI(baud) {}

    InputType *readRaw() override
    {
        if (!Serial.available())
            return nullptr;

        const char c = Serial.read();
        while (Serial.available())
            Serial.read(); // flush

        switch (c)
        {
        case 'w':
        case 'W':
            return &_up;
        case 'a':
        case 'A':
            return &_left;
        case 's':
        case 'S':
            return &_down;
        case 'd':
        case 'D':
            return &_right;
        case '\r':
        case '\n':
            return &_select;
        default:
            return nullptr;
        }
    }

    // Accessors so SerialLCDInteractive can wire them up
    DirectionalButtonInput &left() { return _left; }
    DirectionalButtonInput &right() { return _right; }
    DirectionalButtonInput &up() { return _up; }
    DirectionalButtonInput &down() { return _down; }
    DirectionalButtonInput &select() { return _select; }

private:
    DirectionalButtonInput _up{DirectionalButtonInput::UP};
    DirectionalButtonInput _down{DirectionalButtonInput::DOWN};
    DirectionalButtonInput _left{DirectionalButtonInput::LEFT};
    DirectionalButtonInput _right{DirectionalButtonInput::RIGHT};
    DirectionalButtonInput _select{DirectionalButtonInput::SELECT};
};