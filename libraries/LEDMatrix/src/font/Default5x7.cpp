#include "Default5x7.h"
#include <avr/pgmspace.h>

// Each glyph = 5 columns, each column 7 bits (LSB = top pixel).
// Store by columns for fast blit; you can also store by rows if you prefer.

static const uint8_t PROGMEM A_5x7[5] = {
    0b0111110, // col 0 (7 bits used; MSB ignored)
    0b1001001,
    0b1001001,
    0b1001001,
    0b0111110};

// ... define more glyph arrays (B, C, …, a, b, …, 0–9) in PROGMEM …

// Minimal example set just to get you running:
static const uint8_t PROGMEM H_5x7[5] = {
    0b1111111, 0b0010000, 0b0010000, 0b0010000, 0b1111111};
static const uint8_t PROGMEM E_5x7[5] = {
    0b1111111, 0b1001001, 0b1001001, 0b1001001, 0b1000001};
static const uint8_t PROGMEM L_5x7[5] = {
    0b1111111, 0b0000001, 0b0000001, 0b0000001, 0b0000001};
static const uint8_t PROGMEM O_5x7[5] = {
    0b0111110, 0b1000001, 0b1000001, 0b1000001, 0b0111110};
static const uint8_t PROGMEM ZERO_5x7[5] = {
    0b1111111, 0b1000001, 0b1000001, 0b1000001, 0b1111111};
static const uint8_t PROGMEM ONE_5x7[5] = {
    0b0000000, 0b1000010, 0b1111111, 0b1000000, 0b0000000};

// You’ll flesh out the rest (A–Z, a–z, 0–9). Keep all glyphs 5x7 to start.

bool Default5x7::glyphMetrics(char c, Glyph &out)
{
    // Simple monospaced metrics
    out.width = 5;
    out.height = 7;
    out.xAdvance = 5 + spacing();
    out.xOffset = 0;
    out.yOffset = 0;
    out.bitmap = glyphBitmap(c);
    return out.bitmap != nullptr;
}

const uint8_t *Default5x7::glyphBitmap(char c)
{
    switch (c)
    {
    case 'H':
        return H_5x7;
    case 'E':
        return E_5x7;
    case 'L':
        return L_5x7;
    case 'O':
        return O_5x7;
    case '0':
        return ZERO_5x7;
    case '1':
        return ONE_5x7;
    // TODO: add the rest of A–Z, a–z, 0–9 here
    default:
        return nullptr;
    }
}

bool Default5x7::getGlyph(char c, Glyph &out) const
{
    return glyphMetrics(c, out);
}
