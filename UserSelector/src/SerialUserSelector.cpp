#include "SerialUserSelector.h"

void SerialUserSelector::begin()
{
    Serial.begin(9600);
    Serial.println("Enter user ID:");
}

// Original numeric input
int SerialUserSelector::selectUser()
{
    while (Serial.available() == 0)
    {
        // wait for user input
    }
    return Serial.parseInt();
}

// Select from an array of IDs
int SerialUserSelector::selectUser(const int *userIds, size_t numUsers)
{
    Serial.println("Select a user ID from the list:");

    for (size_t i = 0; i < numUsers; ++i)
    {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(userIds[i]);
    }

    while (true)
    {
        if (Serial.available() > 0)
        {
            int index = Serial.parseInt();
            if (index >= 0 && index < (int)numUsers)
                return userIds[index];
            else
                Serial.println("Invalid selection. Try again.");
        }
    }
}

// Select from an array of names
int SerialUserSelector::selectUser(const char *const *userNames, size_t numUsers)
{
    Serial.println("Select a user from the list:");

    for (size_t i = 0; i < numUsers; ++i)
    {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(userNames[i]);
    }

    while (true)
    {
        if (Serial.available() > 0)
        {
            int index = Serial.parseInt();
            if (index >= 0 && index < (int)numUsers)
                return index; // Return index of selection
            else
                Serial.println("Invalid selection. Try again.");
        }
    }
}
