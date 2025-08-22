#include "LEDMatrixBuilder.h"

/**
 * @brief Construct a new LEDMatrixBuilder with default values
 */
LEDMatrixBuilder::LEDMatrixBuilder()
    : _textSize(LEDMatrix::TextSize::MEDIUM),
      _brightness(255),
      _stopMs(0),
      _speed(200),
      _loopCount(-1),
      _text(""),
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

LEDMatrixBuilder &LEDMatrixBuilder::speed(uint32_t ms)
{
    _speed = ms;
    return *this;
}

LEDMatrixBuilder &LEDMatrixBuilder::loop(int count)
{
    _loopCount = count;
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

LEDMatrixBuilder &LEDMatrixBuilder::withText(const String &text)
{
    _text = text;
    return *this;
}

/**
 * @brief Build and return a fully-configured LEDMatrixWrapper instance
 *
 * The caller is responsible for managing the lifetime of the object.
 */
LEDMatrixWrapper *LEDMatrixBuilder::build()
{
    // Allocate the platform-specific wrapper on the heap
    LEDMatrixWrapper *display = new LEDMatrixWrapper(_logger);

    // Configure basic properties
    display->setTextSize(_textSize);
    display->setBrightness(_brightness);

    // Optionally assign a draw engine if provided
    if (_drawEngine)
    {
        display->setDrawEngine(_drawEngine);
    }

    // Optionally assign text to display
    if (_text.length() > 0)
    {
        display->renderText(_text); // Assumes LEDMatrixWrapper has renderText()
    }

    // Scroll speed and loop count can be stored in wrapper if supported
    // display->setScrollSpeed(_speed);
    // display->setLoopCount(_loopCount);

    // Stop duration can be stored in wrapper if supported
    // display->setStopTime(_stopMs);

    return display;
}
