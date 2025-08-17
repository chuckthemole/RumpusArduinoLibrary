#include "ArduinoLEDMatrixWrapper.h"

ArduinoLEDMatrixWrapper::ArduinoLEDMatrixWrapper() {}

void ArduinoLEDMatrixWrapper::begin()
{
    _matrix.begin();
    clear();
}

void ArduinoLEDMatrixWrapper::clear()
{
    clearFrame();
    _matrix.renderBitmap(_frame, 8, 12);
}

void ArduinoLEDMatrixWrapper::clearFrame()
{
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 12; c++)
            _frame[r][c] = 0;
}

// Draw text with automatic scrolling if it doesn't fit
void ArduinoLEDMatrixWrapper::displayText(const String &text)
{
    // Build the full frame for the text
    uint8_t fullFrame[8][128]; // supports up to 128 columns
    int totalCols = 0;

    // Clear fullFrame
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 128; c++)
            fullFrame[r][c] = 0;

    // Draw each letter
    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text[i];
        drawLetterToFullFrame(c, totalCols, fullFrame);
        totalCols += getLetterWidth(c) + 1; // letter width + 1 col spacing
    }

    // Scroll frame if totalCols > 12
    if (totalCols <= 12)
    {
        // Just copy to visible frame
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < totalCols; c++)
                _frame[r][c] = fullFrame[r][c];
        _matrix.renderBitmap(_frame, 8, 12);
    }
    else
    {
        // Scroll across the display
        for (int startCol = 0; startCol <= totalCols - 12; startCol++)
        {
            for (int r = 0; r < 8; r++)
                for (int c = 0; c < 12; c++)
                    _frame[r][c] = fullFrame[r][c + startCol];
            _matrix.renderBitmap(_frame, 8, 12);
            delay(200); // adjust scroll speed
        }
    }
}

// Draw a single letter into the fullFrame at column offset
void ArduinoLEDMatrixWrapper::drawLetterToFullFrame(char c, int colOffset, uint8_t fullFrame[8][128])
{
    switch (c)
    {
    case 'H':
        drawHFull(colOffset, fullFrame);
        break;
    case 'E':
        drawEFull(colOffset, fullFrame);
        break;
    case 'L':
        drawLFull(colOffset, fullFrame);
        break;
    case 'O':
        drawOFull(colOffset, fullFrame);
        break;
    default:
        break;
    }
}

// Widths for each letter
int ArduinoLEDMatrixWrapper::getLetterWidth(char c)
{
    switch (c)
    {
    case 'H':
        return 3;
    case 'E':
        return 3;
    case 'L':
        return 3;
    case 'O':
        return 3;
    default:
        return 3;
    }
}

// Letter drawing functions for fullFrame
void ArduinoLEDMatrixWrapper::drawHFull(int col, uint8_t fullFrame[8][128])
{
    for (int r = 0; r < 8; r++)
    {
        fullFrame[r][col] = 1;
        fullFrame[r][col + 2] = 1; // right vertical
    }
    fullFrame[3][col + 1] = 1; // middle horizontal
}

void ArduinoLEDMatrixWrapper::drawEFull(int col, uint8_t fullFrame[8][128])
{
    for (int r = 0; r < 8; r++)
        fullFrame[r][col] = 1;
    fullFrame[0][col + 1] = 1;
    fullFrame[3][col + 1] = 1;
    fullFrame[7][col + 1] = 1;
}

void ArduinoLEDMatrixWrapper::drawLFull(int col, uint8_t fullFrame[8][128])
{
    for (int r = 0; r < 8; r++)
        fullFrame[r][col] = 1;
    fullFrame[7][col + 1] = 1;
}

void ArduinoLEDMatrixWrapper::drawOFull(int col, uint8_t fullFrame[8][128])
{
    for (int r = 1; r < 7; r++)
    {
        fullFrame[r][col] = 1;
        fullFrame[r][col + 2] = 1;
    }
    fullFrame[0][col + 1] = 1;
    fullFrame[7][col + 1] = 1;
}

String ArduinoLEDMatrixWrapper::toString()
{
    String result = "";
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 12; c++)
        {
            result += (_frame[r][c] ? "#" : " "); // "#" = lit, " " = off
        }
        result += "\n"; // new line for each row
    }
    return result;
}
