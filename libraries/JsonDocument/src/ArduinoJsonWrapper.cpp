#include "ArduinoJsonWrapper.h"
#include "JsonArrayWrapper.h" // Needed for wrapping JSON arrays

/**
 * @brief Construct a new ArduinoJsonWrapper object.
 * Allocates the StaticJsonDocument on the heap using the given size enum.
 */
ArduinoJsonWrapper::ArduinoJsonWrapper(Size size)
{
    switch (size)
    {
    case SMALL:
        docPtr = std::make_unique<DynamicJsonDocument>(128);
        break;
    case MEDIUM:
        docPtr = std::make_unique<DynamicJsonDocument>(256);
        break;
    case LARGE:
        docPtr = std::make_unique<DynamicJsonDocument>(512);
        break;
    default:
        docPtr = std::make_unique<DynamicJsonDocument>(256); // fallback to medium
        break;
    }
}

/**
 * @brief Destructor cleans up allocated memory.
 */
ArduinoJsonWrapper::~ArduinoJsonWrapper()
{
}

/**
 * @brief Set a string value for a key.
 */
void ArduinoJsonWrapper::set(const char *key, const char *value)
{
    (*docPtr)[key] = value;
}

/**
 * @brief Set an integer value for a key.
 */
void ArduinoJsonWrapper::set(const char *key, int value)
{
    (*docPtr)[key] = value;
}

/**
 * @brief Create a nested object for a given key.
 * @return Pointer to a new ArduinoJsonWrapper for the nested object.
 */
RumpusJsonDocument *ArduinoJsonWrapper::createObject(const char *key)
{
    JsonObject obj = (*docPtr).createNestedObject(key);
    ArduinoJsonWrapper *wrapper = new ArduinoJsonWrapper();
    wrapper->setInternalObject(obj);
    return wrapper;
}

/**
 * @brief Create a nested array for a given key.
 * @return Pointer to a new JsonArrayWrapper for the nested array.
 */
RumpusJsonDocument *ArduinoJsonWrapper::createArray(const char *key)
{
    JsonArray arr = (*docPtr).createNestedArray(key);
    return new JsonArrayWrapper(arr); // ✅ Proper wrapper for arrays
}

/**
 * @brief Serialize the document to a string.
 */
String ArduinoJsonWrapper::serialize()
{
    String output;
    serializeJson(*docPtr, output);
    return output;
}

/**
 * @brief Clear the document.
 */
void ArduinoJsonWrapper::clear()
{
    docPtr->clear();
}

/**
 * @brief Set the internal object for a nested wrapper.
 */
void ArduinoJsonWrapper::setInternalObject(JsonObject &obj)
{
    nestedObject = obj;
}
