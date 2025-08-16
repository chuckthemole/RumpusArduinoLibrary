#pragma once
#include <Arduino.h>

class UserSelector
{
public:
    virtual void begin() = 0;

    // Select by numeric ID (default behavior)
    virtual int selectUser() = 0;

    // Optional: select from a list of users (array of IDs)
    // Returns the index in the array, or -1 if canceled
    virtual int selectUser(const int *userIds, size_t numUsers)
    {
        // Default implementation: fallback to numeric input
        return selectUser();
    }

    // Optional: select from an array of names
    // Returns the index in the array, or -1 if canceled
    virtual int selectUser(const char *const *userNames, size_t numUsers)
    {
        // Default implementation: fallback to numeric input
        return selectUser();
    }

    virtual ~UserSelector() {}
};
