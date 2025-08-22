#include "CoffeeTypesFetcher.h"

CoffeeTypesFetcher::CoffeeTypesFetcher(HttpFetcher *fetcher)
    : _fetcher(fetcher) {}

bool CoffeeTypesFetcher::fetch()
{
    String json = _fetcher->get();
    if (json.length() == 0)
        return false;
    process(json);
    return true;
}

void CoffeeTypesFetcher::process(const String &json)
{
    StaticJsonDocument<1024> doc;
    DeserializationError err = deserializeJson(doc, json);
    if (err)
    {
        Serial.println("JSON parse failed: " + String(err.c_str()));
        return;
    }

    _coffeeTypes.clear();
    for (JsonObject obj : doc["results"].as<JsonArray>())
    {
        if (obj.containsKey("name"))
        {
            _coffeeTypes.push_back(obj["name"].as<String>());
        }
    }
}

void CoffeeTypesFetcher::display()
{
    Serial.println("Coffee Types:");
    for (String c : _coffeeTypes)
    {
        Serial.println(" - " + c);
    }
}

const std::vector<String> &CoffeeTypesFetcher::getCoffeeTypes()
{
    return _coffeeTypes;
}
