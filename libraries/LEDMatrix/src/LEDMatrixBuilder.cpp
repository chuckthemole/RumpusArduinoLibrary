#include "LEDMatrixBuilder.h"

/**
 * @brief Construct a new LEDMatrixBuilder with default values
 */
LEDMatrixBuilder::LEDMatrixBuilder()
    : _textSize(LEDMatrix::TextSize::MEDIUM),
      _brightness(255),
      _stopMs(0),
      _logger(nullptr),
      _drawEngine(nullptr)
{
}

LEDMatrixBuilder &LEDMatrixBuilder::setTextSize(uint8_t size)
{
    _textSize = size;
    return *this;
}

LEDMatrixBuilder &LEDMatrixBuilder::setBrightness(uint8_t level)
{
    _brightness = level;
    return *this;
}

LEDMatrixBuilder &LEDMatrixBuilder::stop(uint32_t ms)
{
    _stopMs = ms;
    return *this;
}

LEDMatrixBuilder &LEDMatrixBuilder::withLogger(RumpshiftLogger *logger)
{
    _logger = logger;
    return *this;
}

LEDMatrixBuilder &LEDMatrixBuilder::withDrawEngine(DrawEngine *engine)
{
    _drawEngine = engine;
    return *this;
}

LEDMatrix *LEDMatrixBuilder::build()
{
    // Allocate the platform-specific wrapper on the heap
    LEDMatrixWrapper *display = new LEDMatrixWrapper(_logger);

    // Configure properties
    display->setTextSize(_textSize);
    display->setBrightness(_brightness);

    // Optionally assign a draw engine if provided
    if (_drawEngine)
    {
        display->setDrawEngine(_drawEngine);
    }

    // Stop time can be stored in wrapper or draw engine
    // display->setStopTime(_stopMs);

    return display;
}