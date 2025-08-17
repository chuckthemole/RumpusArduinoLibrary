#include "LEDMatrixWrapper.h"

/**
 * @brief Construct a new LEDMatrixWrapper without logger
 */
LEDMatrixWrapper::LEDMatrixWrapper()
    : _logger(nullptr)
{
}

/**
 * @brief Construct a new LEDMatrixWrapper with logger
 * @param logger Pointer to RumpshiftLogger instance
 */
LEDMatrixWrapper::LEDMatrixWrapper(RumpshiftLogger *logger)
    : _logger(logger)
{
}

/**
 * @brief Initialize hardware and clear the display
 */
void LEDMatrixWrapper::begin()
{
    _matrix.begin();
    clear();

    if (_logger)
        _logger->info("LEDMatrixWrapper: initialized");
}

/**
 * @brief Clear both the hardware and internal frame buffer
 */
void LEDMatrixWrapper::clear()
{
    clearFrame();
    _matrix.renderBitmap(_frame, Rows::EIGHT, Columns::TWELVE);

    if (_logger)
        _logger->debug("LEDMatrixWrapper: display cleared");
}

/**
 * @brief Render a prebuilt frame on the hardware
 */
void LEDMatrixWrapper::renderFrame(const uint8_t frame[Rows::EIGHT][Columns::TWELVE])
{
    // Copy to internal buffer
    for (int r = 0; r < Rows::EIGHT; r++)
        for (int c = 0; c < Columns::TWELVE; c++)
            _frame[r][c] = frame[r][c];

    // Render to hardware
    _matrix.renderBitmap(_frame, Rows::EIGHT, Columns::TWELVE);

    if (_logger)
        _logger->debug("LEDMatrixWrapper: frame rendered");
}

/**
 * @brief Set brightness on the underlying hardware
 */
void LEDMatrixWrapper::setBrightness(uint8_t level)
{
    // _matrix.setBrightness(level);

    if (_logger)
        _logger->info(String("LEDMatrixWrapper: brightness set to ") + level);

    if (_logger)
        _logger->info("TODO: this is not implemented!!");
}

/**
 * @brief Clear internal frame buffer
 */
void LEDMatrixWrapper::clearFrame()
{
    for (int r = 0; r < Rows::EIGHT; r++)
        for (int c = 0; c < Columns::TWELVE; c++)
            _frame[r][c] = 0;

    if (_logger)
        _logger->debug("LEDMatrixWrapper: frame buffer cleared");
}

void LEDMatrixWrapper::setTextSize(uint8_t size)
{
    _textSize = size; // store locally, can be used by DrawEngine
    if (_logger)
        _logger->info("LEDMatrixWrapper: text size set");
}

void LEDMatrixWrapper::setDrawEngine(DrawEngine *engine)
{
    _engine = engine;
    if (_logger)
        _logger->info("LEDMatrixWrapper: engine set");
}

/**
 * @brief Convert internal frame buffer to a string for debugging
 */
String LEDMatrixWrapper::toString() const
{
    String result = "";
    for (int r = 0; r < Rows::EIGHT; r++)
    {
        for (int c = 0; c < Columns::TWELVE; c++)
        {
            result += (_frame[r][c] ? "#" : " ");
        }
        result += "\n";
    }

    if (_logger)
        _logger->debug("LEDMatrixWrapper: toString() called");

    return result;
}
