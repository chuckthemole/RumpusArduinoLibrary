#include <Arduino.h>

// ---------------------------------------------------------------------------
// Setup function - runs once when the board powers up or resets
// ---------------------------------------------------------------------------
void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Small delay to let Serial connect
    delay(1000);

    Serial.println("Arduino PlatformIO project started!");
}

// ---------------------------------------------------------------------------
// Loop function - runs continuously after setup()
// ---------------------------------------------------------------------------
void loop() {
    // Example: Blink the built-in LED
    static bool ledState = false;

    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
    ledState = !ledState;

    Serial.println("Toggling LED...");

    delay(1000); // 1 second delay
}
