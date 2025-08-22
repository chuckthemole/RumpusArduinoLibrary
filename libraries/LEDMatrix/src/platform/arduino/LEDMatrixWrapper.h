#pragma once
#include <Arduino.h>
#include <Arduino_LED_Matrix.h>
#include "LEDMatrix.h"
#include "RumpshiftLogger.h"
#include "font/Font.h"

class DrawEngine;

/**
 * @brief Platform-specific implementation of LEDMatrix for Arduino.
 *
 * This wrapper handles hardware interaction: initialization, clearing,
 * brightness, rendering frames, and drawing individual letters/numbers.
 * All scrolling or high-level text logic can be handled by the wrapper
 * or higher-level managers.
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

    /**
     * @brief Set the default text size for drawing characters.
     *
     * @param size Text size enum (SMALL, MEDIUM, LARGE)
     */
    void setTextSize(uint8_t size) override;

    /**
     * @brief Assign a DrawEngine implementation for rendering frames.
     *
     * @param engine Pointer to DrawEngine instance
     */
    void setDrawEngine(DrawEngine *engine);

    /**
     * @brief Optional: Get a string representation of the current frame
     *        for debugging purposes.
     *
     * @return String representation of the visible frame
     */
    String toString() const;

    // --- Character/Number Drawing Methods ---

    /**
     * @brief Draw a single letter into the internal frame buffer.
     *
     * @param c Character to draw (A-Z)
     * @param colOffset Column offset to start drawing
     */
    void drawLetter(char c, int colOffset);

    /**
     * @brief Draw a single number into the internal frame buffer.
     *
     * @param n Character representing number (0-9)
     * @param colOffset Column offset to start drawing
     */
    void drawNumber(char n, int colOffset);

    /**
     * @brief Render a full string of text into the internal frame buffer.
     *
     * @param text Text to render
     */
    void renderText(const String &text);

    void setFont(const Font *font); ///< Set current font for text rendering
    void drawChar(char c, int colOffset);

private:
    ArduinoLEDMatrix _matrix;                     ///< Underlying hardware object
    uint8_t _frame[Rows::EIGHT][Columns::TWELVE]; ///< Local frame buffer for the visible display
    RumpshiftLogger *_logger = nullptr;           ///< Optional logger
    uint8_t _textSize = TextSize::MEDIUM;         ///< Current text size
    DrawEngine *_engine = nullptr;                ///< Optional engine for rendering frames
    const Font *_font = nullptr;                  ///< Active font (default can be set in begin())

    /**
     * @brief Clear the internal frame buffer.
     */
    void clearFrame();

    // --- Internal helpers for drawing specific letters/numbers ---
    void drawA(int colOffset);
    void drawB(int colOffset);
    void drawC(int colOffset);
    void drawD(int colOffset);
    void drawE(int colOffset);
    void drawF(int colOffset);
    void drawG(int colOffset);
    void drawH(int colOffset);
    void drawI(int colOffset);
    void drawJ(int colOffset);
    void drawK(int colOffset);
    void drawL(int colOffset);
    void drawM(int colOffset);
    void drawN(int colOffset);
    void drawO(int colOffset);
    void drawP(int colOffset);
    void drawQ(int colOffset);
    void drawR(int colOffset);
    void drawS(int colOffset);
    void drawT(int colOffset);
    void drawU(int colOffset);
    void drawV(int colOffset);
    void drawW(int colOffset);
    void drawX(int colOffset);
    void drawY(int colOffset);
    void drawZ(int colOffset);

    // Numbers
    void draw0(int colOffset);
    void draw1(int colOffset);
    void draw2(int colOffset);
    void draw3(int colOffset);
    void draw4(int colOffset);
    void draw5(int colOffset);
    void draw6(int colOffset);
    void draw7(int colOffset);
    void draw8(int colOffset);
    void draw9(int colOffset);
};
