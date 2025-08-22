#pragma once
#include <Arduino.h>
#include "LEDMatrix.h"
#include "platform/arduino/LEDMatrixWrapper.h"
#include "RumpshiftLogger.h"
#include "font/Font.h"

/**
 * @brief Base class for rendering text and shapes onto an LEDMatrix.
 *
 * DrawEngine handles higher-level concepts such as text rendering,
 * while delegating pixel-level drawing to the platform-specific
 * LEDMatrix implementation.
 */
class DrawEngine
{
public:
    /**
     * @brief Construct a new DrawEngine instance
     * @param matrix Pointer to an LEDMatrix implementation
     * @param logger Optional pointer to RumpshiftLogger
     */
    DrawEngine(LEDMatrixWrapper *matrix, RumpshiftLogger *logger = nullptr);

    virtual ~DrawEngine() = default;

    /**
     * @brief Initialize hardware and clear display
     */
    virtual void begin();

    /**
     * @brief Draw a string using the active font
     * @param text      C-string to draw
     * @param colOffset Starting column position
     */
    virtual void drawText(const char *text, int colOffset = 0);

    /**
     * @brief Render the current framebuffer to hardware
     */
    virtual void render();

    /**
     * @brief Assign a font for text rendering
     * @param font Pointer to a font implementation
     */
    void setFont(const Font *font);

protected:
    LEDMatrixWrapper *_matrix = nullptr;       ///< Hardware interface
    const Font *_font = nullptr;        ///< Active font
    RumpshiftLogger *_logger = nullptr; ///< Optional logger
};
