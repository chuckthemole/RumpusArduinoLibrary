#pragma once
#include "Font.h"

class Default5x7 : public Font
{
public:
    uint8_t lineHeight() const override { return 7; }
    uint8_t baseline() const override { return 6; } // row index (0..6)
    static uint8_t spacing() { return 1; }

    bool getGlyph(char c, Glyph &out) const override;

private:
    static const uint8_t *glyphBitmap(char c);
    static bool glyphMetrics(char c, Glyph &out);
};
