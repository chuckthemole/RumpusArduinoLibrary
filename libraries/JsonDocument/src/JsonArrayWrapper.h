#ifndef JSON_ARRAY_WRAPPER_H
#define JSON_ARRAY_WRAPPER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "RumpusJsonDocument.h"

/**
 * @class JsonArrayWrapper
 * @brief Wrapper around ArduinoJson's JsonArray for handling JSON arrays in a unified interface.
 */
class JsonArrayWrapper : public RumpusJsonDocument
{
public:
    explicit JsonArrayWrapper(JsonArray arr) : array(arr) {}
    ~JsonArrayWrapper() override = default;

    void set(const char *key, const char *value) override {} // not used in arrays
    void set(const char *key, int value) override {}         // not used in arrays
    void set(const char *key, RumpusJsonDocument *value) override {}

    // Instead of keyed set, arrays use push/add
    void add(const char *value) { array.add(value); }
    void add(int value) { array.add(value); }
    void add(float value) { array.add(value); }
    void add(bool value) { array.add(value); }

    RumpusJsonDocument *getObject(const char *key) override { return nullptr; }
    RumpusJsonDocument *getArray(const char *key) override { return nullptr; }

    String serialize() override
    {
        String output;
        serializeJson(array, output);
        return output;
    }

    void clear() override { array.clear(); }

private:
    JsonArray array;
};

#endif // JSON_ARRAY_WRAPPER_H
