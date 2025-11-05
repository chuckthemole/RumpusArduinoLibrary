// EEPROMStorage.h
#pragma once
#include "Storage.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512

class EEPROMStorage : public Storage
{
public:
    void begin() override
    {
        EEPROM.begin(EEPROM_SIZE);
    }

    void clear() override
    {
        for (int i = 0; i < EEPROM_SIZE; i++)
            EEPROM.write(i, 0);
        EEPROM.commit();
    }

    void save(const String &data) override
    {
        int len = min((int)data.length(), EEPROM_SIZE - 1);
        for (int i = 0; i < len; i++)
            EEPROM.write(i, data[i]);
        EEPROM.write(len, '\0');
        EEPROM.commit();
    }

    String load() override
    {
        String result;
        for (int i = 0; i < EEPROM_SIZE; i++)
        {
            char c = EEPROM.read(i);
            if (c == '\0' || c == 0xFF)
                break;
            result += c;
        }
        return result;
    }
};
