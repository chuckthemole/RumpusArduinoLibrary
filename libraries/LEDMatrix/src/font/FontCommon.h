// fonts/FontCommon.h
#pragma once
#include <Arduino.h>

// A single glyph’s metrics + bitmap pointer (bit-packed)
struct Glyph
{
    uint8_t width;         // glyph visible width in columns
    uint8_t height;        // glyph height in rows
    int8_t xAdvance;       // how many columns to advance cursor (often width + spacing)
    int8_t xOffset;        // draw offset (usually 0 for simple fonts)
    int8_t yOffset;        // baseline offset (0 = top aligned; positive moves down)
    const uint8_t *bitmap; // PROGMEM pointer, bit-packed columns or rows
};
