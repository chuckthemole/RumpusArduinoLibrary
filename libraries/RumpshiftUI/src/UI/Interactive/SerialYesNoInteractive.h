#pragma once

#include <Arduino.h>
#include "SerialInteractiveUI.h"
#include "../../Input/DirectionalButtonInput.h"

/**
 * @brief Yes/No interaction style (Y/N + Enter).
 */
class SerialYesNoInteractive : public SerialInteractiveUI
{
public:
    explicit SerialYesNoInteractive(unsigned long baud = 9600)
        : SerialInteractiveUI(baud) {}

    InputType* readRaw() override
    {
        if (!Serial.available()) return nullptr;

        const char c = Serial.read();
        while (Serial.available()) Serial.read();

        switch (c)
        {
            case 'y': case 'Y': return &_yes;
            case 'n': case 'N': return &_no;
            case '\r': case '\n': return &_select;
            default: return nullptr;
        }
    }

private:
    DirectionalButtonInput _yes{DirectionalButtonInput::UP};     // could map however you want
    DirectionalButtonInput _no{DirectionalButtonInput::DOWN};
    DirectionalButtonInput _select{DirectionalButtonInput::SELECT};
};