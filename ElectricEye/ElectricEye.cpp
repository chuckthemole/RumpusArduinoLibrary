#include "ElectricEye.h"

ElectricEye::ElectricEye(uint8_t pin, int threshold)
    : _pin(pin), _threshold(threshold), _lastState(false),
      _beamBrokenCallback(nullptr), _beamRestoredCallback(nullptr) {}

void ElectricEye::begin()
{
    pinMode(_pin, INPUT);
    _lastState = analogRead(_pin) > _threshold;
}

bool ElectricEye::isBlocked()
{
    int value = analogRead(_pin);
    return value > _threshold;
}

void ElectricEye::update()
{
    bool currentState = isBlocked();
    if (currentState != _lastState)
    {
        if (currentState && _beamBrokenCallback)
        {
            _beamBrokenCallback();
        }
        else if (!currentState && _beamRestoredCallback)
        {
            _beamRestoredCallback();
        }
        _lastState = currentState;
    }
}

void ElectricEye::onBeamBroken(void (*callback)())
{
    _beamBrokenCallback = callback;
}

void ElectricEye::onBeamRestored(void (*callback)())
{
    _beamRestoredCallback = callback;
}
