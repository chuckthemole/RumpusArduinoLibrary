#include "DrawEngine.h"

DrawEngine::DrawEngine(LEDMatrixWrapper *matrix, RumpshiftLogger *logger)
    : _matrix(matrix), _logger(logger) {}

void DrawEngine::begin()
{
    if (_matrix)
    {
        _matrix->begin();
        _matrix->clear();
        if (_logger)
            _logger->info("DrawEngine: initialized and display cleared");
    }
}

void DrawEngine::drawText(const char *text, int colOffset)
{
    if (!_matrix || !_font)
        return;

    int cursorX = colOffset;

    while (*text)
    {
        Glyph g;
        if (_font->getGlyph(*text, g))
        {
            _matrix->drawChar(*text, cursorX); // delegate pixel drawing
            cursorX += g.xAdvance;             // move cursor by glyph width
        }
        text++;
    }

    if (_logger)
        _logger->debug("DrawEngine: text drawn");
}

void DrawEngine::render()
{
    if (_matrix)
    {
        _matrix->renderFrame(nullptr); // TODO: fix this, maybe something like _matrix->getFrame() as the param
        if (_logger)
            _logger->debug("DrawEngine: frame rendered");
    }
}

void DrawEngine::setFont(const Font *font)
{
    _font = font;
    if (_logger)
        _logger->info("DrawEngine: font set");
}
