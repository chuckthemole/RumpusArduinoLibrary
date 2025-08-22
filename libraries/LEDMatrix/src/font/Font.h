// fonts/Font.h
#pragma once
#include <Arduino.h>
#include "FontCommon.h"

class Font
{
public:
    virtual ~Font() = default;

    // Overall metrics (for layout/scrolling)
    virtual uint8_t lineHeight() const = 0; // e.g., 7 for a 5x7 font
    virtual uint8_t baseline() const = 0;   // where letters sit (optional)
    static uint8_t spacing() { return 1; }  // col gap between glyphs (e.g., 1)

    // Returns true if glyph exists; fills out 'out' with metrics + bitmap ptr
    virtual bool getGlyph(char c, Glyph &out) const = 0;
};
