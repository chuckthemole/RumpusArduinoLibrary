#pragma once
#include "TemperatureSensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

/**
 * @brief Implementation of TemperatureSensor using a Dallas 1-Wire device.
 *
 * Uses the DallasTemperature and OneWire libraries.
 */
class DallasTemperatureSensorWrapper : public TemperatureSensor
{
private:
    OneWire oneWire;           ///< Low-level OneWire bus
    DallasTemperature sensors; ///< DallasTemperature wrapper
    float lastTempC = NAN;     ///< Last temperature reading
    float offset = 0.0f;       ///< Calibration offset in °C

public:
    /**
     * @brief Construct a DallasTemperatureSensorWrapper on a given pin.
     *
     * @param pin GPIO pin connected to the 1-Wire bus.
     */
    DallasTemperatureSensorWrapper(uint8_t pin)
        : oneWire(pin), sensors(&oneWire) {}

    void begin() override
    {
        sensors.begin();
    }

    void update() override
    {
        sensors.requestTemperatures();
        lastTempC = sensors.getTempCByIndex(0) + offset;
    }

    float getTemperatureC() const override
    {
        return lastTempC;
    }

    void setOffset(float o) override
    {
        offset = o;
    }
};
