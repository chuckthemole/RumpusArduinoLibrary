## Getting Started
Here's a basic example of how to use `CommonArduinoHelper`:
```cpp
#include "CommonArduinoHelper.h"

void setup() {
    LCD::lcd.begin(16, 2);
    LCD::lcd.print("Hello!");
}

void loop() {
    int analogValue = analogRead(A0);
    Button b = CommonHelper::readButton(analogValue);

    if (b == btnRIGHT) {
        LCD::lcd.setCursor(0, 1);
        LCD::lcd.print("Right pressed   ");
    }

    CommonHelper::nonBlockingDelay(100); // Delay without delay()
}
```