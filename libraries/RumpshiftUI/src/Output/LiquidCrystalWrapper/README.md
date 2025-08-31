# LiquidCrystalWrapper Arduino Library

A simple and convenient wrapper around the Arduino `LiquidCrystal` library for easy management of common LCD operations with default pin configuration (16x2 LCD).

---

## Features

- Default pin mapping for common 16x2 LCDs
- Easy initialization with `begin()`
- Cursor positioning and printing helpers
- Scroll display left or right
- Clear screen function

---

## Installation

Copy `LiquidCrystalWrapper.h` and `LiquidCrystalWrapper.cpp` into your Arduino project or library folder.

Include it in your sketch:

#include "LiquidCrystalWrapper.h"

---

## Usage Example

#include <Arduino.h>
#include "LiquidCrystalWrapper.h"

LiquidCrystalWrapper lcd;  // Uses default pins: RS=8, Enable=9, D4=4, D5=5, D6=6, D7=7, 16x2 display

void setup() {
  lcd.begin();              // Initialize the LCD
  lcd.print("Hello, LCD!"); // Print message at current cursor (0,0)
  
  delay(2000);
  
  lcd.clear();
  lcd.printAt(0, 1, "Line 2 text");  // Print at column 0, row 1
}

void loop() {
  lcd.scrollDisplayLeft();  // Scroll display left
  delay(300);
}

---

## API

| Method                 | Description                                          |
|------------------------|------------------------------------------------------|
| `LiquidCrystalWrapper()`         | Constructor, optionally specify pins and size        |
| `begin()`              | Initializes LCD, call in `setup()`                    |
| `clear()`              | Clears the LCD screen                                 |
| `print(String msg)`    | Prints a message at the current cursor                |
| `setCursor(col, row)`  | Moves cursor to specified column and row              |
| `printAt(col,row,msg)` | Moves cursor and prints message at specified position |
| `scrollDisplayLeft()`  | Scrolls display one position left                      |
| `scrollDisplayRight()` | Scrolls display one position right                     |

---

## Customization

You can specify custom pins and LCD size when creating the object:

LiquidCrystalWrapper lcd(12, 11, 10, 5, 4, 3, 20, 4);  // For 20x4 LCD with custom pins

---
