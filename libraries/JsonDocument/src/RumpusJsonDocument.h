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
    virtual void set(const char *key, const char *value) = 0;

    /**
     * @brief Set an integer value for a given key in the JSON document.
     * @param key The key to set.
     * @param value The integer value to assign.
     */
    virtual void set(const char *key, int value) = 0;

    virtual void set(const char *key, RumpusJsonDocument *value) = 0;

    /**
     * @brief Get or create a nested JSON object for a given key.
     * @param key The key to access or assign the nested object to.
     * @return Pointer to a new RumpusJsonDocument wrapper for the nested object.
     *
     * If the key already contains an object, it will be reused.
     * If the key is missing or not an object, a new nested object will be created.
     *
     * Example usage:
     * @code
     * RumpusJsonDocument* nested = doc->getObject("payload");
     * nested->set("user", "Chuck");
     * nested->set("duration", 120);
     * @endcode
     */
    virtual RumpusJsonDocument *getObject(const char *key) = 0;

    /**
     * @brief Get or create a nested JSON array for a given key.
     * @param key The key to access or assign the array to.
     * @return Pointer to a new RumpusJsonDocument wrapper for the array.
     *
     * If the key already contains an array, it will be reused.
     * If the key is missing or not an array, a new nested array will be created.
     *
     * Example usage:
     * @code
     * RumpusJsonDocument* arr = doc->getArray("values");
     * arr->set(0, "first");
     * arr->set(1, "second");
     * @endcode
     */
    virtual RumpusJsonDocument *getArray(const char *key) = 0;

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
