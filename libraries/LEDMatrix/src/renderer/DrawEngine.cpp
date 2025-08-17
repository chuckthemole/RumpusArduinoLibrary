#include "DrawEngine.h"

// No need for a separate .cpp for constructors or inline methods
// Everything simple is already in the header

// If you really want to provide a separate implementation file:
void DrawEngine::begin()
{
    if (_logger)
        _logger->info("DrawEngine: begin() called");
}

void DrawEngine::setBrightness(uint8_t level)
{
    if (_logger)
        _logger->info("DrawEngine: setBrightness() called with level " + String(level));
}
