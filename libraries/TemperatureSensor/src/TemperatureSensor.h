#pragma once
#include <Arduino.h>

/**
 * @brief Abstract interface for any temperature sensor.
 *
 * This defines a standard contract that all temperature sensors must follow,
 * so your application can work with different sensor types interchangeably
 * (Dallas 1-Wire, analog thermistor, simulated, etc.).
 */
class TemperatureSensor
{
public:
    virtual ~TemperatureSensor() {}

    /**
     * @brief Initialize the sensor.
     *
     * Called once in setup(). Use this to start hardware communication,
     * configure pins, etc.
     */
    virtual void begin() = 0;

    /**
     * @brief Update the sensor reading.
     *
     * Some sensors require explicit requests before reading (e.g. DallasTemperature).
     * Call this regularly in loop() before calling getTemperatureC().
     */
    virtual void update() = 0;

    /**
     * @brief Get the last measured temperature in Celsius.
     *
     * @return float Temperature in °C. May be NAN if no valid reading is available.
     */
    virtual float getTemperatureC() const = 0;

    /**
     * @brief Apply an offset to adjust calibration.
     *
     * @param offset Value in °C to add/subtract from the raw reading.
     */
    virtual void setOffset(float offset) = 0;
};
