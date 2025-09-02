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
    setActionHandler(Action::LEFT, ActionHandler([this]()
                                                 { buttonLeft(); }));
    setActionHandler(Action::RIGHT, ActionHandler([this]()
                                                  { buttonRight(); }));
    setActionHandler(Action::UP, ActionHandler([this]()
                                               { buttonUp(); }));
    setActionHandler(Action::DOWN, ActionHandler([this]()
                                                 { buttonDown(); }));
    setActionHandler(Action::SELECT, ActionHandler([this]()
                                                   { buttonSelect(); }));

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
        _buffer[r] = new char[COLS];
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

void SerialLCDInteractive::appendToBuffer(const String &msg, bool newLine)
{
    for (size_t i = 0; i < msg.length(); ++i)
    {
        _buffer[_cursorRow][_cursorCol] = msg[i];
        _cursorCol++;
        if (_cursorCol >= COLS)
        {
            _cursorCol = 0;
            _cursorRow = (_cursorRow + 1) % ROWS;
            clearRow(_cursorRow);
        }
    }
    if (newLine)
    {
        _cursorCol = 0;
        _cursorRow = (_cursorRow + 1) % ROWS;
        clearRow(_cursorRow);
    }
}

void SerialLCDInteractive::redrawLCD()
{
    String border;
    border.reserve(COLS + 2);
    for (int i = 0; i < COLS + 2; ++i)
        border += '*';

    _serialUI.println(border);
    for (int r = 0; r < ROWS; ++r)
    {
        _serialUI.print("*");
        for (int c = 0; c < COLS; ++c)
            _serialUI.print(String(_buffer[r][c]));
        _serialUI.println("*");
    }
    _serialUI.println(border);
}
