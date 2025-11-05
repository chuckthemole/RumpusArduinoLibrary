#pragma once
#include <Arduino.h>

class Storage
{
public:
    static constexpr int MAX_SIZE = 512;
    virtual void begin() = 0;
    virtual void clear() = 0;
    virtual void save(const String &data) = 0;
    virtual String load() = 0;
    virtual ~Storage() {}
};
