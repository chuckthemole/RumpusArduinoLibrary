#pragma once
#include "DrawEngine.h"
#include "platform/arduino/LEDMatrixWrapper.h"

/**
 * @brief Arduino-specific implementation of DrawEngine.
 *
 * This class uses LEDMatrixWrapper to perform actual pixel-level
 * operations while DrawEngine handles higher-level drawing logic.
 */
class ArduinoDrawEngine : public DrawEngine
{
public:
    /**
     * @brief Construct a new ArduinoDrawEngine
     * @param matrix Pointer to an LEDMatrixWrapper instance
     * @param logger Optional logger
     */
    ArduinoDrawEngine(LEDMatrixWrapper *matrix, RumpshiftLogger *logger = nullptr);

    /**
     * @brief Initialize hardware (via LEDMatrixWrapper) and clear display
     */
    void begin() override;

    /**
     * @brief Draw a text string using the active font
     */
    void drawText(const char *text, int colOffset = 0) override;

    /**
     * @brief Render the current framebuffer to the Arduino hardware
     */
    void render() override;
};
