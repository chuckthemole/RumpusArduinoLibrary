#pragma once
#include "UserSelector.h"
#include <LiquidCrystal.h>

class LCDUserSelector : public UserSelector
{
private:
    LiquidCrystal &lcd;

public:
    LCDUserSelector(LiquidCrystal &lcdRef) : lcd(lcdRef) {}

    void begin() override
    {
        lcd.begin(16, 2);
        lcd.print("Select User:");
    }

    int selectUser() override
    {
        // Example: read buttons to choose a user
        // This is pseudo-code; replace with your actual button reading logic
        int userId = 0;
        while (true)
        {
            if (digitalRead(2) == HIGH)
                userId++;
            if (digitalRead(3) == HIGH)
                return userId;
        }
    }
};
