#pragma once
#include <Arduino.h>
#include "LEDMatrix.h"
#include "RumpshiftLogger.h"
#include "renderer/DrawEngine.h"
#include "platform/arduino/LEDMatrixWrapper.h"

/**
 * @brief Builder for constructing and configuring LEDMatrix instances.
 *
 * Provides a fluent interface for setting text size, brightness, stop time,
 * draw engine, and optional logging. Handles platform-specific wrapper creation
 * internally.
 */
class LEDMatrixBuilder
{
public:
    LEDMatrixBuilder();

    /**
     * @brief Set the text size for the LED matrix.
     * @param size LEDMatrix::TextSize value
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &setTextSize(uint8_t size);

    /**
     * @brief Set the brightness for the LED matrix (0-255)
     * @param level Brightness level
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &setBrightness(uint8_t level);

    /**
     * @brief Configure the LED matrix to stop for a number of milliseconds
     *        after scrolling the message.
     * @param ms Stop duration in milliseconds
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &stop(uint32_t ms);

    /**
     * @brief Assign a logger to the LED matrix.
     * @param logger Pointer to RumpshiftLogger instance
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &withLogger(RumpshiftLogger *logger);

    /**
     * @brief Assign a custom DrawEngine implementation.
     * @param engine Pointer to DrawEngine instance
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &withDrawEngine(DrawEngine *engine);

    /**
     * @brief Build and return a fully-configured LEDMatrix instance.
     *
     * The LEDMatrixWrapper is allocated on the heap and returned as a pointer.
     * The caller is responsible for managing the lifetime of the object
     * (i.e., deleting it when no longer needed).
     *
     * @return Pointer to a fully-configured LEDMatrix instance
     */
    LEDMatrix *build();

private:
    uint8_t _textSize;
    uint8_t _brightness;
    uint32_t _stopMs;
    RumpshiftLogger *_logger;
    DrawEngine *_drawEngine;
};
