# ArduinoLEDMatrixWrapper

**ArduinoLEDMatrixWrapper** is a library for displaying text on the Arduino Uno R4 WiFi built-in LED display.

## Installation

1. Copy the `ArduinoLEDMatrixWrapper` folder into your Arduino `libraries` folder.
2. Restart the Arduino IDE.
3. Include the library in your sketch:

```cpp
#include <ArduinoLEDMatrixWrapper.h>
ArduinoLEDMatrixWrapper display;
```

## Usage
```cpp
void setup() {
  display.begin();
  display.displayText("HELLO");
}

void loop() {
  // Your code here
}
```
