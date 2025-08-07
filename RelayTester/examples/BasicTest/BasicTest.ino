#include <RelayTester.h>

RelayTester tester(2, 13, 1000); // pins 2–13, 1 second delay

void setup()
{
    Serial.begin(9600);
    tester.begin();
}

void loop()
{
    tester.testCycle();
    delay(5000); // pause between test cycles
}
