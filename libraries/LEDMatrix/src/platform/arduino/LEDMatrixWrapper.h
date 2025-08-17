#pragma once
#include <Arduino.h>
#include <Arduino_LED_Matrix.h>
#include "LEDMatrix.h"
#include "RumpshiftLogger.h"

/**
 * @brief Platform-specific implementation of LEDMatrix for Arduino.
 *
 * This wrapper only handles hardware interaction: initialization,
 * clearing, brightness, and rendering a given frame.
 * All drawing, scrolling, or text logic is handled by the DrawEngine.
 */
class LEDMatrixWrapper : public LEDMatrix
{
public:
    /**
     * @brief Construct without logger.
     */
    LEDMatrixWrapper();

    /**
     * @brief Construct with logger dependency.
     * @param logger Pointer to an existing RumpshiftLogger instance.
     */
    LEDMatrixWrapper(RumpshiftLogger *logger = nullptr);

    /**
     * @brief Initialize the hardware and clear the display.
     */
    void begin() override;

    /**
     * @brief Clear the visible display.
     */
    void clear() override;

    /**
     * @brief Render a prebuilt frame on the hardware.
     *
     * @param frame 2D array of size ROWS x COLUMNS representing LED states
     */
    void renderFrame(const uint8_t frame[Rows::EIGHT][Columns::TWELVE]) override;

    /**
     * @brief Set the brightness of the LED matrix.
     *
     * @param level Brightness level (0-255)
     */
    void setBrightness(uint8_t level) override;

    void setTextSize(uint8_t size) override;

    void setDrawEngine(DrawEngine *engine);

    /**
     * @brief Optional: Get a string representation of the current frame
     *        for debugging purposes.
     *
     * @return String representation of the visible frame
     */
    String toString() const;

private:
    ArduinoLEDMatrix _matrix;                     ///< Underlying hardware object
    uint8_t _frame[Rows::EIGHT][Columns::TWELVE]; ///< Local frame buffer for the visible display
    RumpshiftLogger *_logger = nullptr;           ///< Optional logger
    uint8_t _textSize = TextSize::MEDIUM;
    DrawEngine *_engine;

    /**
     * @brief Clear the internal frame buffer.
     */
    void clearFrame();
};
