#ifndef ARDUINO_JSON_WRAPPER_H
#define ARDUINO_JSON_WRAPPER_H

#ifndef __cplusplus
#error "This library requires a C++11 compiler."
#elif __cplusplus < 201103L
#error "This library requires at least C++11. Please update your board package or compiler."
#endif

#include <Arduino.h>
#include <ArduinoJson.h>
#include "RumpusJsonDocument.h"
#include <memory>

/**
 * @class ArduinoJsonWrapper
 * @brief Concrete implementation of RumpusJsonDocument using ArduinoJson.
 *
 * This wrapper abstracts ArduinoJson usage behind a common interface,
 * allowing code to remain library-agnostic.
 */
class ArduinoJsonWrapper : public RumpusJsonDocument
{
public:
    /**
     * @brief Predefined document sizes for convenience.
     */
    enum Size
    {
        SMALL = 128,
        MEDIUM = 256,
        LARGE = 512
    };

    /**
     * @brief Construct a wrapper with optional document size.
     * @param size The size of the underlying StaticJsonDocument (default: MEDIUM).
     */
    explicit ArduinoJsonWrapper(Size size = MEDIUM);

    // Destructor
    ~ArduinoJsonWrapper() override;

    // Interface overrides
    void set(const char *key, const char *value) override;
    void set(const char *key, int value) override;
    RumpusJsonDocument *createObject(const char *key) override;
    RumpusJsonDocument *createArray(const char *key) override;
    String serialize() override;
    void clear() override;

private:
    std::unique_ptr<DynamicJsonDocument> docPtr; ///< Pointer to underlying StaticJsonDocument
    JsonObject nestedObject;                     ///< For nested objects/arrays

    // Internal helper to set nested JsonObject
    void setInternalObject(JsonObject &obj);
};

#endif // ARDUINO_JSON_WRAPPER_H
