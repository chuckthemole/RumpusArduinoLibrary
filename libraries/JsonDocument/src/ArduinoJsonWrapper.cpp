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
 * @brief Set an RumpusJsonDocument value for a key.
 */
void ArduinoJsonWrapper::set(const char *key, RumpusJsonDocument *value)
{
    if (!value)
        return;

    String nestedJson = value->serialize();
    DynamicJsonDocument temp(nestedJson.length() + 1); // TODO: I should probably be getting this size from 'value' var.
    DeserializationError err = deserializeJson(temp, nestedJson);
    if (err)
    {
        // TODO: need to bring _logger into this lib
        // if (_logger)
        //     _logger->warn("[ArduinoJsonWrapper] Failed to parse nested JSON: " + String(err.c_str()));
        return;
    }

    (*docPtr)[key] = temp.as<JsonVariant>();

    // TODO: when JsonArrayWrapper is built out, we should think about how to determine if it is array or object.
    // // Determine if value is object or array
    // JsonObject obj = value->asJsonObject();  // You would add this method to your base
    // if (obj) {
    //     (*docPtr)[key] = obj;
    // } else {
    //     JsonArray arr = value->asJsonArray();
    //     if (arr) {
    //         (*docPtr)[key] = arr;
    //     }
    // }
}

/**
 * @brief Get (or create) a nested object for a given key.
 *        If the key already exists, returns a wrapper for it.
 * @return Pointer to a new ArduinoJsonWrapper for the nested object.
 */
RumpusJsonDocument *ArduinoJsonWrapper::getObject(const char *key)
{
    JsonVariant var = (*docPtr)[key];
    JsonObject obj;

    if (var.isNull())
    {
        // Create if missing
        obj = (*docPtr).createNestedObject(key);
    }
    else
    {
        // Reuse if it already exists
        obj = var.as<JsonObject>();
    }

    auto *wrapper = new ArduinoJsonWrapper();
    wrapper->setInternalObject(obj);
    return wrapper;
}

/**
 * @brief Get (or create) a nested array for a given key.
 *        If the key already exists, returns a wrapper for it.
 * @return Pointer to a new JsonArrayWrapper for the nested array.
 */
RumpusJsonDocument *ArduinoJsonWrapper::getArray(const char *key)
{
    JsonVariant var = (*docPtr)[key];
    JsonArray arr;

    if (var.isNull())
    {
        // Create if missing
        arr = (*docPtr).createNestedArray(key);
    }
    else
    {
        // Reuse if it already exists
        arr = var.as<JsonArray>();
    }

    return new JsonArrayWrapper(arr); // ✅ safe wrapper for arrays
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
