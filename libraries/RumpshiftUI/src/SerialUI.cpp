#include "SerialUI.h"

SerialUI::SerialUI(unsigned long baud)
    : baudRate(baud) {}

/**
 * @brief Initialize the Serial port. Must be called in setup().
 */
void SerialUI::begin()
{
    Serial.begin(baudRate);
    while (!Serial)
    {
        ; // wait for Serial port to connect (only needed for native USB)
    }
}

/**
 * @brief Clear any available Serial input.
 */
void SerialUI::clear()
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}

/**
 * @brief Print without newline.
 */
void SerialUI::print(const String &msg)
{
    Serial.print(msg);
}

/**
 * @brief Print with newline.
 */
void SerialUI::println(const String &msg)
{
    Serial.println(msg);
}

/**
 * @brief Return the current action mapped from user input.
 *
 * If no input is available, returns Action::NONE.
 */
UIBase::Action SerialUI::getAction()
{
    if (Serial.available() == 0)
        return Action::NONE;

    String input = Serial.readStringUntil('\n');
    input.trim();
    return parseAction(input);
}

/**
 * @brief Prompt user for textual input.
 *
 * Displays a message, waits for user input, and returns the entered string.
 */
String SerialUI::prompt(const String &msg)
{
    println(msg);
    while (Serial.available() == 0)
    {
        ; // wait for input
    }
    String input = Serial.readStringUntil('\n');
    input.trim();
    return input;
}

/**
 * @brief Map typed input to Action enum.
 *
 * Accepts both numeric strings ("0"-"4") and optional letter codes ("a"-"e").
 */
UIBase::Action SerialUI::parseAction(const String &input)
{
    if (input.length() == 0)
        return Action::NONE;

    // Map numeric input
    if (input == "0")
        return Action::ACTION0;
    if (input == "1")
        return Action::ACTION1;
    if (input == "2")
        return Action::ACTION2;
    if (input == "3")
        return Action::ACTION3;
    if (input == "4")
        return Action::ACTION4;

    // Optional: map letters
    if (input == "a")
        return Action::ACTION0;
    if (input == "b")
        return Action::ACTION1;
    if (input == "c")
        return Action::ACTION2;
    if (input == "d")
        return Action::ACTION3;
    if (input == "e")
        return Action::ACTION4;

    return Action::NONE;
}
