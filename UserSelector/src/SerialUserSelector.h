#pragma once
#include "UserSelector.h"
#include <Arduino.h> // Required for Serial

class SerialUserSelector : public UserSelector
{
public:
    void begin() override;
    int selectUser() override;

    // New overrides for selecting from arrays
    int selectUser(const int *userIds, size_t numUsers) override;
    int selectUser(const char *const *userNames, size_t numUsers) override;
};
