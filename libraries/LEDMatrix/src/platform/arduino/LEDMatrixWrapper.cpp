#include "LEDMatrixWrapper.h"
#include <avr/pgmspace.h>
#include "renderer/DrawEngine.h"

/**
 * @brief Construct a new LEDMatrixWrapper without logger
 */
LEDMatrixWrapper::LEDMatrixWrapper()
    : _logger(nullptr), _engine(nullptr)
{
}

/**
 * @brief Construct a new LEDMatrixWrapper with logger
 * @param logger Pointer to RumpshiftLogger instance
 */
LEDMatrixWrapper::LEDMatrixWrapper(RumpshiftLogger *logger)
    : _logger(logger), _engine(nullptr)
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

// --------------------------------------------
// Letter Drawing
// --------------------------------------------
void LEDMatrixWrapper::drawLetter(char c, int colOffset)
{
    switch (c)
    {
    case 'H':
        drawH(colOffset);
        break;
    case 'E':
        drawE(colOffset);
        break;
    case 'L':
        drawL(colOffset);
        break;
    case 'O':
        drawO(colOffset);
        break;
    default:
        break; // unsupported chars
    }
}

// --------------------------------------------
// Number Drawing
// --------------------------------------------
void LEDMatrixWrapper::drawNumber(char n, int colOffset)
{
    switch (n)
    {
    case '0':
        draw0(colOffset);
        break;
    case '1':
        draw1(colOffset);
        break;
    case '2':
        draw2(colOffset);
        break;
    case '3':
        draw3(colOffset);
        break;
    case '4':
        draw4(colOffset);
        break;
    case '5':
        draw5(colOffset);
        break;
    case '6':
        draw6(colOffset);
        break;
    case '7':
        draw7(colOffset);
        break;
    case '8':
        draw8(colOffset);
        break;
    case '9':
        draw9(colOffset);
        break;
    default:
        break; // unsupported numbers
    }
}

// --------------------------------------------
// Render a full string
// --------------------------------------------
void LEDMatrixWrapper::renderText(const String &text)
{
    int colOffset = 0;
    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text[i];
        if (c >= '0' && c <= '9')
            drawNumber(c, colOffset);
        else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            drawLetter(toupper(c), colOffset);

        colOffset += 4; // basic spacing; could adjust per char
    }

    // After drawing into frame buffer, optionally render
    renderFrame(_frame);
}

#include "LEDMatrixWrapper.h"
#include <avr/pgmspace.h>

void LEDMatrixWrapper::setFont(const Font *font)
{
    _font = font;
    if (_logger)
        _logger->info("LEDMatrixWrapper: font set");
}

/**
 * @brief Draw a single character from the active font into the framebuffer
 *
 * @param c         ASCII character to draw
 * @param colOffset Column offset (x position) where to start drawing
 */
void LEDMatrixWrapper::drawChar(char c, int colOffset)
{
    if (!_font)
    {
        if (_logger)
            _logger->warn("LEDMatrixWrapper: no font set");
        return;
    }

    Glyph glyph;
    if (!_font->getGlyph(c, glyph) || !glyph.bitmap)
    {
        if (_logger)
            _logger->warn(String("LEDMatrixWrapper: no glyph for '") + c + "'");
        return;
    }

    // Loop through each column of the glyph
    for (int col = 0; col < glyph.width; col++)
    {
        uint8_t colBits = pgm_read_byte(glyph.bitmap + col);

        for (int row = 0; row < glyph.height; row++)
        {
            bool pixelOn = colBits & (1 << row);
            if (pixelOn)
            {
                // Write into internal frame buffer at proper offset
                int x = colOffset + col + glyph.xOffset;
                int y = row + glyph.yOffset;
                if (x >= 0 && x < Columns::TWELVE && y >= 0 && y < Rows::EIGHT)
                {
                    _frame[y][x] = 1;
                }
            }
        }
    }
}
