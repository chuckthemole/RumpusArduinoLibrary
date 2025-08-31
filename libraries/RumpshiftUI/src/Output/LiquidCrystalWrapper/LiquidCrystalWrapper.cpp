#include "LiquidCrystalWrapper.h"

LiquidCrystalWrapper::LiquidCrystalWrapper(
    uint8_t rs,
    uint8_t enable,
    uint8_t d4,
    uint8_t d5,
    uint8_t d6,
    uint8_t d7,
    uint8_t cols,
    uint8_t rows)
    : lcd(rs, enable, d4, d5, d6, d7), columns(cols), rows(rows) {}

void LiquidCrystalWrapper::begin()
{
    lcd.begin(columns, rows);
    lcd.clear();
}

void LiquidCrystalWrapper::clear()
{
    lcd.clear();
}

void LiquidCrystalWrapper::setCursor(uint8_t col, uint8_t row)
{
    lcd.setCursor(col, row);
}

void LiquidCrystalWrapper::printAt(uint8_t col, uint8_t row, const String &msg)
{
    lcd.setCursor(col, row);
    lcd.print(msg);
}

void LiquidCrystalWrapper::scrollDisplayLeft()
{
    lcd.scrollDisplayLeft();
}

void LiquidCrystalWrapper::scrollDisplayRight()
{
    lcd.scrollDisplayRight();
}
