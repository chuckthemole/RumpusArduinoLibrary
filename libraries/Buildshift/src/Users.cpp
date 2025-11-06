#include "Users.h"

Users::Users(WiFiClientWrapper &client, RumpshiftLogger *logger)
    : wifiClient(client), logger(logger) {}

HttpResponse Users::fetch(const char *host, int port, const char *url)
{
    logger->info("[Users] Fetching from " + String(host) + ":" + String(port) + url);

    if (!wifiClient.connected())
    {
        logger->warn("[Users] WiFiClientWrapper not connected yet");
    }

    HttpResponse resp = wifiClient.http().get(host, port, url);

    logger->info("[Users] Response status: " + String(resp.status()));
    return resp;
}

int Users::parse(const String &json)
{
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error)
    {
        logger->error(String("[Users] JSON parse failed: ") + error.c_str());
        return 0;
    }

    // Root is an object
    JsonArray arr = doc["results"].as<JsonArray>();
    userCount = 0;

    for (JsonObject obj : arr)
    {
        if (userCount >= MAX_USERS)
            break;
        users[userCount].id = obj["id"].as<String>();
        users[userCount].name = obj["name"].as<String>();
        userCount++;
    }

    logger->info("[Users] Parsed " + String(userCount) + " users.");
    return userCount;
}

void Users::printUsers()
{
    logger->info("[Users] Listing users:");
    for (int i = 0; i < userCount; i++)
    {
        logger->info("  - " + users[i].name + " (ID: " + users[i].id + ")");
    }
}
