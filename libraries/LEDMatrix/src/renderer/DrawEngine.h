#pragma once
#include <Arduino.h>
#include "LEDMatrix.h"
#include "RumpshiftLogger.h"

class DrawEngine
{
public:
    virtual ~DrawEngine() = default;

    /**
     * @brief Construct a new DrawEngine instance
     * @param logger Optional pointer to RumpshiftLogger
     */
    DrawEngine(RumpshiftLogger *logger = nullptr) : _logger(logger) {}

    /**
     * @brief Initialize the hardware. Default does nothing.
     */
    virtual void begin();

    /**
     * @brief Render a prebuilt frame to the hardware.
     * Must be overridden by subclasses.
     */
    virtual void renderFrame(const uint8_t frame[LEDMatrix::Rows::EIGHT][LEDMatrix::Columns::TWELVE]) = 0;

    /**
     * @brief Set the brightness for the hardware. Default does nothing.
     */
    virtual void setBrightness(uint8_t level);

    /**
     * @brief Optional: provide a logger after construction
     */
    virtual void setLogger(RumpshiftLogger *logger) { _logger = logger; }

protected:
    RumpshiftLogger *_logger = nullptr;
};
