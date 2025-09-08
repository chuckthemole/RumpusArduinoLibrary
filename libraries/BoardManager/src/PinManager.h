#pragma once
#include <map>
#include <Arduino.h>
#include "RumpshiftLogger.h" // Your logger library

// ----------------------------
// PinManager
// Tracks pin assignments by name and number
// Prevents duplicate pin assignments
// Uses a logger for error/debug output
// Returns the pin number on assignment so it can be bound to a const
// ----------------------------
class PinManager
{
public:
    // ----------------------------
    // Constructor
    // ----------------------------
    // @param loggerPtr: pointer to a RumpshiftLogger instance for logging
    explicit PinManager(RumpshiftLogger *loggerPtr) : logger(loggerPtr) {}

    // ----------------------------
    // Assign a pin to a logical name
    // ----------------------------
    // @param name: logical pin name, e.g., "SENSOR"
    // @param pin: actual Arduino pin number, e.g., board.PIN.D2
    // @return: the pin number if assignment succeeds, or -1 on failure
    int assignPin(const String &name, int pin)
    {
        // Check if pin number is already assigned
        if (pinInUse(pin))
        {
            if (logger)
                logger->error("Pin " + String(pin) + " already assigned to " + pinsUsed[pin]);
            return -1;
        }

        // Check if name is already assigned
        if (nameInUse(name))
        {
            if (logger)
                logger->error("Pin name '" + name + "' already assigned to pin " + String(nameToPin[name]));
            return -1;
        }

        // Assign pin
        pinsUsed[pin] = name;
        nameToPin[name] = pin;

        if (logger)
            logger->debug("Assigned pin " + String(pin) + " to '" + name + "'");

        return pin;
    }

    // ----------------------------
    // Get pin number by logical name
    // ----------------------------
    // @param name: logical pin name
    // @return: pin number, or -1 if not found
    int getPin(const String &name) const
    {
        auto it = nameToPin.find(name);
        if (it != nameToPin.end())
        {
            return it->second;
        }

        if (logger)
            logger->error("Pin name '" + name + "' not found!");
        return -1;
    }

    // ----------------------------
    // Check if a pin number is already in use
    // ----------------------------
    bool pinInUse(int pin) const
    {
        return pinsUsed.find(pin) != pinsUsed.end();
    }

    // ----------------------------
    // Check if a pin name is already in use
    // ----------------------------
    bool nameInUse(const String &name) const
    {
        return nameToPin.find(name) != nameToPin.end();
    }

private:
    RumpshiftLogger *logger = nullptr; // Logger instance for error/debug messages
    std::map<int, String> pinsUsed;    // pinNumber → pinName
    std::map<String, int> nameToPin;   // pinName → pinNumber
};
