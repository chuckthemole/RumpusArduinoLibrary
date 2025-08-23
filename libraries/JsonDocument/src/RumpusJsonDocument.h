#ifndef JSON_DOCUMENT_H
#define JSON_DOCUMENT_H

#include <Arduino.h>

/**
 * @class RumpusJsonDocument
 * @brief Abstract interface for a JSON document.
 *
 * This interface provides a unified way to manipulate JSON data
 * without depending on a specific underlying library (e.g., ArduinoJson).
 * Implementations should provide storage for the JSON data and support
 * nested objects and arrays.
 */
class RumpusJsonDocument
{
public:
    virtual ~RumpusJsonDocument() {}

    /**
     * @brief Set a string value for a given key in the JSON document.
     * @param key The key to set.
     * @param value The string value to assign.
     */
    virtual void set(const char* key, const char* value) = 0;

    /**
     * @brief Set an integer value for a given key in the JSON document.
     * @param key The key to set.
     * @param value The integer value to assign.
     */
    virtual void set(const char* key, int value) = 0;

    /**
     * @brief Create a nested JSON object for a given key.
     * @param key The key to assign the nested object to.
     * @return Pointer to a new RumpusJsonDocument representing the nested object.
     *
     * Allows chaining operations on nested objects:
     * @code
     * RumpusJsonDocument* nested = doc->createObject("payload");
     * nested->set("user", "Chuck");
     * nested->set("duration", 120);
     * @endcode
     */
    virtual RumpusJsonDocument* createObject(const char* key) = 0;

    /**
     * @brief Create a nested JSON array for a given key.
     * @param key The key to assign the array to.
     * @return Pointer to a new RumpusJsonDocument representing the array.
     *
     * Allows chaining operations on arrays:
     * @code
     * RumpusJsonDocument* arr = doc->createArray("values");
     * arr->set(0, "first");
     * arr->set(1, "second");
     * @endcode
     */
    virtual RumpusJsonDocument* createArray(const char* key) = 0;

    /**
     * @brief Serialize the JSON document into a string for transmission or storage.
     * @return A String containing the serialized JSON.
     */
    virtual String serialize() = 0;

    /**
     * @brief Clear all contents of the JSON document.
     */
    virtual void clear() = 0;
};

#endif // JSON_DOCUMENT_H
