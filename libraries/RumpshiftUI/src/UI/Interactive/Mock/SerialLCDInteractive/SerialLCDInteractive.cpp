#include "SerialLCDInteractive.h"

//-------------------------------------------------------------------
// Constructors / Destructor
//-------------------------------------------------------------------
SerialLCDInteractive::SerialLCDInteractive(uint8_t cols, uint8_t rows, unsigned long baud)
    : LCDInteractive(A0, 1, 1, 1, 1, 1, 1, cols, rows),
      _serialUI(baud),
      COLS(cols),
      ROWS(rows)
{
    _initBuffer();
}

SerialLCDInteractive::SerialLCDInteractive(LiquidCrystalWrapper &lcdRef, uint8_t buttonsPin,
                                           uint8_t cols, uint8_t rows, unsigned long baud)
    : LCDInteractive(lcdRef, buttonsPin),
      _serialUI(baud),
      COLS(cols),
      ROWS(rows)
{
    _initBuffer();
}

SerialLCDInteractive::~SerialLCDInteractive()
{
    for (int r = 0; r < ROWS; ++r)
        delete[] _buffer[r];
    delete[] _buffer;
}

//-------------------------------------------------------------------
// Public methods
//-------------------------------------------------------------------
void SerialLCDInteractive::begin()
{
    _serialUI.begin();
    _serialUI.println(F("=== SerialLCDInteractive (LCD mock) initialized ==="));

    // Map inputs to Actions
    setInputMapping(&_serialUI.left(), Action::LEFT);
    setInputMapping(&_serialUI.right(), Action::RIGHT);
    setInputMapping(&_serialUI.up(), Action::UP);
    setInputMapping(&_serialUI.down(), Action::DOWN);
    setInputMapping(&_serialUI.select(), Action::SELECT);

    // Bind Actions to handlers
    setActionHandler(Action::LEFT, ActionHandler::fromMember(this, &SerialLCDInteractive::buttonLeft));
    setActionHandler(Action::RIGHT, ActionHandler::fromMember(this, &SerialLCDInteractive::buttonRight));
    setActionHandler(Action::UP, ActionHandler::fromMember(this, &SerialLCDInteractive::buttonUp));
    setActionHandler(Action::DOWN, ActionHandler::fromMember(this, &SerialLCDInteractive::buttonDown));
    setActionHandler(Action::SELECT, ActionHandler::fromMember(this, &SerialLCDInteractive::buttonSelect));

    redrawLCD();
}

void SerialLCDInteractive::clear()
{
    initBuffer();
    _cursorCol = 0;
    _cursorRow = 0;
    redrawLCD();
}

void SerialLCDInteractive::print(const String &msg)
{
    appendToBuffer(msg, false);
    redrawLCD();
}

void SerialLCDInteractive::println(const String &msg)
{
    appendToBuffer(msg, true);
    redrawLCD();
}

InputType *SerialLCDInteractive::readRaw()
{
    return _serialUI.readRaw();
}

//-------------------------------------------------------------------
// Virtual button handlers
//-------------------------------------------------------------------
void SerialLCDInteractive::buttonLeft() { Serial.println(F("LEFT pressed")); }
void SerialLCDInteractive::buttonRight() { Serial.println(F("RIGHT pressed")); }
void SerialLCDInteractive::buttonUp() { Serial.println(F("UP pressed")); }
void SerialLCDInteractive::buttonDown() { Serial.println(F("DOWN pressed")); }
void SerialLCDInteractive::buttonSelect() { Serial.println(F("SELECT pressed")); }

//-------------------------------------------------------------------
// Private buffer methods
//-------------------------------------------------------------------
void SerialLCDInteractive::_initBuffer()
{
    _buffer = new char *[ROWS];
    for (int r = 0; r < ROWS; ++r)
        _buffer[r] = new char[COLS]; // only allocate COLS, but we’ll allow printing past it
    initBuffer();
}

void SerialLCDInteractive::initBuffer()
{
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            _buffer[r][c] = ' ';
}

void SerialLCDInteractive::clearRow(int row)
{
    for (int c = 0; c < COLS; ++c)
        _buffer[row][c] = ' ';
}

//-------------------------------------------------------------------
// Append text to buffer
//-------------------------------------------------------------------
void SerialLCDInteractive::appendToBuffer(const String &msg, bool newLine)
{
    for (size_t i = 0; i < msg.length(); ++i)
    {
        // spill past COLS if needed
        int colIndex = _cursorCol;

        // expand row if printing beyond current COLS
        if (colIndex >= COLS)
        {
            // resize row buffer to fit new colIndex+1
            char *newRow = new char[colIndex + 1];
            memcpy(newRow, _buffer[_cursorRow], COLS);
            for (int j = COLS; j <= colIndex; ++j)
                newRow[j] = ' ';
            delete[] _buffer[_cursorRow];
            _buffer[_cursorRow] = newRow;
            COLS = colIndex + 1; // expand "virtual" width
        }

        _buffer[_cursorRow][colIndex] = msg[i];
        _cursorCol++;
    }

    if (newLine)
    {
        _cursorCol = 0;
        _cursorRow = (_cursorRow + 1) % ROWS;
    }
}

//-------------------------------------------------------------------
// Redraw LCD to Serial (borders + full row contents, spill past COLS)
//-------------------------------------------------------------------
void SerialLCDInteractive::redrawLCD()
{
    _serialUI.println();

    const String title = "LCD*SHIELD";
    int totalWidth = COLS + 6; // 3 '*' per side
    int titleStart = (totalWidth - title.length()) / 2;

    // Top 3 rows border with title in the middle row
    for (int row = 0; row < 3; ++row)
    {
        for (int i = 0; i < totalWidth; ++i)
        {
            if (row == 1 && i >= titleStart && i < titleStart + title.length())
                _serialUI.print(String(title[i - titleStart]));
            else
                _serialUI.print("*");
        }
        _serialUI.println();
    }

    // LCD content rows
    for (int r = 0; r < ROWS; ++r)
    {
        // Left border
        _serialUI.print("***");

        // Row contents
        int rowLength = _cursorCol;
        if (rowLength < COLS)
            rowLength = COLS; // ensure at least full width

        for (int c = 0; c < rowLength; ++c)
        {
            char ch = (c < COLS) ? _buffer[r][c] : ' ';
            _serialUI.print(String(ch));
        }

        // Right border
        _serialUI.println("***");
    }

    // Bottom 3 rows border
    for (int row = 0; row < 3; ++row)
    {
        for (int i = 0; i < totalWidth; ++i)
            _serialUI.print("*");
        _serialUI.println();
    }
}

void SerialLCDInteractive::showMenu(const MenuTemplate &menu, size_t selectedIndex)
{
    _serialUI.println();
    _serialUI.println(F("=== Menu ==="));

    for (size_t i = 0; i < menu.numItems && i < ROWS - 1; ++i)
    {
        String line = (i == selectedIndex ? ">" : " ");
        line += menu.items[i];

        if (line.length() > COLS)
            line = line.substring(0, COLS - 3) + "...";

        _serialUI.println(line);
    }

    _serialUI.println(F("================"));
}
