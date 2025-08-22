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
 * scroll speed, loop count, draw engine, optional logging, and text content.
 * Handles platform-specific wrapper creation internally.
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
     * @brief Set the scroll speed in milliseconds per column shift
     * @param ms Scroll speed in milliseconds
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &speed(uint32_t ms);

    /**
     * @brief Set the number of times the message will loop
     * @param count Number of loops (-1 = infinite)
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &loop(int count);

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
     * @brief Assign the text to display on the LED matrix.
     * @param text Message to display
     * @return Reference to builder for chaining
     */
    LEDMatrixBuilder &withText(const String &text);

    /**
     * @brief Build and return a fully-configured LEDMatrixWrapper instance.
     *
     * The caller is responsible for managing the lifetime of the object
     * (i.e., deleting it when no longer needed).
     *
     * @return Pointer to a fully-configured LEDMatrixWrapper
     */
    LEDMatrixWrapper *build();

private:
    uint8_t _textSize = LEDMatrix::TextSize::MEDIUM;
    uint8_t _brightness = 255;
    uint32_t _stopMs = 0;
    uint32_t _speed = 200; // default scroll speed
    int _loopCount = -1;   // default infinite
    String _text = "";     // message to display
    RumpshiftLogger *_logger = nullptr;
    DrawEngine *_drawEngine = nullptr;
};
