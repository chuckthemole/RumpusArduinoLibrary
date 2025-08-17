// Define a default LED pin if not already defined
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#pragma once
#include <Arduino.h>
#include "DrawEngine.h"

/**
 * @brief Default concrete implementation of DrawEngine for Arduino LED matrices.
 *
 * This engine handles rendering a frame buffer to the hardware.
 * All text formatting, scrolling, or layout is handled outside (e.g., in DrawEngine clients).
 */
class DefaultDrawEngine : public DrawEngine
{
public:
    /**
     * @brief Construct a new DefaultDrawEngine instance.
     * @param pin Optional pin used for testing or built-in LED feedback
     */
    DefaultDrawEngine(uint8_t pin = LED_BUILTIN);

    /**
     * @brief Initialize the engine hardware.
     */
    void begin() override;

    /**
     * @brief Render a prebuilt frame to the hardware.
     * @param frame 2D array of size ROWS x COLUMNS
     */
    void renderFrame(const uint8_t frame[LEDMatrix::Rows::EIGHT][LEDMatrix::Columns::TWELVE]) override;

    /**
     * @brief Set the brightness of the display (dummy in this default implementation)
     * @param level Brightness level 0-255
     */
    void setBrightness(uint8_t level) override;

private:
    uint8_t _pin; ///< Optional built-in LED for testing
};
