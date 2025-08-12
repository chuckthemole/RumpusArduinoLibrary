#include "LcdWrapper.h"

LcdWrapper::LcdWrapper(
    uint8_t rs,
    uint8_t enable,
    uint8_t d4,
    uint8_t d5,
    uint8_t d6,
    uint8_t d7,
    uint8_t cols,
    uint8_t rows)
    : lcd(rs, enable, d4, d5, d6, d7), columns(cols), rows(rows) {}

void LcdWrapper::begin()
{
    lcd.begin(columns, rows);
    lcd.clear();
}

void LcdWrapper::clear()
{
    lcd.clear();
}

void LcdWrapper::print(const String &msg)
{
    lcd.print(msg);
}

void LcdWrapper::print(const char *msg)
{
    lcd.print(msg);
}

void LcdWrapper::print(int value)
{
    lcd.print(value);
}

void LcdWrapper::print(unsigned int value)
{
    lcd.print(value);
}

void LcdWrapper::print(long value)
{
    lcd.print(value);
}

void LcdWrapper::print(unsigned long value)
{
    lcd.print(value);
}

void LcdWrapper::print(float value, int decimals)
{
    lcd.print(value, decimals);
}

void LcdWrapper::setCursor(uint8_t col, uint8_t row)
{
    lcd.setCursor(col, row);
}

void LcdWrapper::printAt(uint8_t col, uint8_t row, const String &msg)
{
    lcd.setCursor(col, row);
    lcd.print(msg);
}

void LcdWrapper::scrollDisplayLeft()
{
    lcd.scrollDisplayLeft();
}

void LcdWrapper::scrollDisplayRight()
{
    lcd.scrollDisplayRight();
}
