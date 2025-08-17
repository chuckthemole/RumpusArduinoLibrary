#pragma once
#include <Arduino.h>

/**
 * @brief Abstract interface for controlling an LED Matrix display.
 *
 * This class defines the core operations for any LED matrix implementation.
 * It is hardware-agnostic and does not handle text formatting, scrolling, or layout.
 */
class LEDMatrix
{
public:
    virtual ~LEDMatrix() = default;

    /**
     * @brief Matrix row counts.
     * Used for defining the physical display size or text scaling.
     */
    struct Rows
    {
        static constexpr uint8_t TWO = 2;
        static constexpr uint8_t FOUR = 4;
        static constexpr uint8_t SIX = 6;
        static constexpr uint8_t EIGHT = 8;
    };

    /**
     * @brief Matrix column counts.
     * Used for defining the physical display width or text scaling.
     */
    struct Columns
    {
        static constexpr uint8_t SIX = 6;
        static constexpr uint8_t EIGHT = 8;
        static constexpr uint8_t TEN = 10;
        static constexpr uint8_t TWELVE = 12;
    };

    /**
     * @brief Text size scaling.
     */
    struct TextSize
    {
        static constexpr uint8_t SMALL = 2;
        static constexpr uint8_t MEDIUM = 4;
        static constexpr uint8_t LARGE = 8;
    };

    /**
     * @brief Initialize the LED matrix hardware.
     */
    virtual void begin() = 0;

    /**
     * @brief Clear the display (turn off all LEDs).
     */
    virtual void clear() = 0;

    /**
     * @brief Render a prebuilt frame on the display.
     *
     * @param frame 2D array representing the LED states.
     *              Each element should be 0 (off) or 1 (on).
     *              Dimensions are implementation-dependent.
     */
    virtual void renderFrame(const uint8_t frame[8][12]) = 0;

    /**
     * @brief Set the brightness of the LED matrix.
     *
     * @param level Brightness level, typically 0-255.
     */
    virtual void setBrightness(uint8_t level) = 0;

    /**
     * @brief Set the default text size for display operations.
     *
     * @param size Text size enum.
     */
    virtual void setTextSize(uint8_t size) = 0;
};
