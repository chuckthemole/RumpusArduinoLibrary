#include "ArduinoDrawEngine.h"

ArduinoDrawEngine::ArduinoDrawEngine(LEDMatrixWrapper *matrix, RumpshiftLogger *logger)
    : DrawEngine(matrix, logger) {}

void ArduinoDrawEngine::begin()
{
    if (_matrix)
        _matrix->begin();
    if (_logger)
        _logger->info("ArduinoDrawEngine: initialized");
}

void ArduinoDrawEngine::drawText(const char *text, int colOffset)
{
    if (!_matrix || !_font)
        return;

    int cursorX = colOffset;
    while (*text)
    {
        Glyph glyph;
        if (_font->getGlyph(*text, glyph))
        {
            // _matrix->drawChar(glyph, cursorX); // delegate pixel plotting
            _matrix->drawChar(*text, cursorX); // delegate pixel plotting
            cursorX += glyph.xAdvance;
        }
        ++text;
    }

    if (_logger)
        _logger->debug("ArduinoDrawEngine: text drawn");
}

void ArduinoDrawEngine::render()
{
    if (_matrix)
        _matrix->renderFrame(nullptr); //TODO: NEED TO FIX THIS
    if (_logger)
        _logger->debug("ArduinoDrawEngine: frame rendered");
}
