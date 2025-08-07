j #include "RelayTester.h"

    RelayTester::RelayTester(uint8_t startPin, uint8_t endPin, unsigned long delayMillis)
    : _startPin(startPin), _endPin(endPin), _delayMillis(delayMillis)
{
}

void RelayTester::begin()
{
    for (uint8_t pin = _startPin; pin <= _endPin; pin++)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    Serial.println("RelayTester initialized.");
}

void RelayTester::testCycle()
{
    for (uint8_t pin = _startPin; pin <= _endPin; pin++)
    {
        testSingle(pin);
    }
    Serial.println("Completed full relay test cycle.\n");
}

void RelayTester::testSingle(uint8_t pin)
{
    Serial.print("Testing pin ");
    Serial.println(pin);

    digitalWrite(pin, HIGH);
    delay(_delayMillis);
    digitalWrite(pin, LOW);
    delay(_delayMillis);
}
