#include "ArduinoLEDMatrixWrapper.h"

ArduinoLEDMatrixWrapper::ArduinoLEDMatrixWrapper() {}

void ArduinoLEDMatrixWrapper::begin()
{
    _matrix.begin();
    clear();
}

ArduinoLEDMatrixWrapper &ArduinoLEDMatrixWrapper::build(const String &text)
{
    _text = text;
    return *this;
}

ArduinoLEDMatrixWrapper &ArduinoLEDMatrixWrapper::stop(unsigned long ms)
{
    _stopDuration = ms;
    return *this;
}

ArduinoLEDMatrixWrapper &ArduinoLEDMatrixWrapper::speed(unsigned long ms)
{
    _speed = ms;
    return *this;
}

ArduinoLEDMatrixWrapper &ArduinoLEDMatrixWrapper::loop(int count)
{
    _loopCount = count;
    return *this;
}

void ArduinoLEDMatrixWrapper::run()
{
    if (_text.length() == 0)
        return;

    uint8_t fullFrame[8][128];
    int totalCols = 0;

    // Draw the text into fullFrame once
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 128; c++)
            fullFrame[r][c] = 0;

    for (size_t i = 0; i < _text.length(); i++)
    {
        char c = _text[i];
        drawLetterToFullFrame(c, totalCols, fullFrame);
        totalCols += getLetterWidth(c) + 1; // spacing
    }

    int loopsDone = 0;
    while (_loopCount == -1 || loopsDone < _loopCount)
    {
        // Scroll left across display
        for (int startCol = 0; startCol <= totalCols; startCol++)
        {
            for (int r = 0; r < 8; r++)
            {
                for (int c = 0; c < 12; c++)
                {
                    int srcCol = c + startCol;
                    _frame[r][c] = (srcCol < 128) ? fullFrame[r][srcCol] : 0;
                }
            }
            _matrix.renderBitmap(_frame, 8, 12);
            delay(_speed);
        }

        // Pause at the end of one scroll cycle
        if (_stopDuration > 0)
        {
            delay(_stopDuration);
        }

        loopsDone++;
    }
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

// Draw a single letter into the fullFrame
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

int ArduinoLEDMatrixWrapper::getLetterWidth(char c)
{
    switch (c)
    {
    case 'H':
    case 'E':
    case 'L':
    case 'O':
        return 3;
    default:
        return 3;
    }
}

void ArduinoLEDMatrixWrapper::drawHFull(int col, uint8_t fullFrame[8][128])
{
    for (int r = 0; r < 8; r++)
    {
        fullFrame[r][col] = 1;
        fullFrame[r][col + 2] = 1;
    }
    fullFrame[3][col + 1] = 1;
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
            result += (_frame[r][c] ? "#" : " ");
        }
        result += "\n";
    }
    return result;
}
