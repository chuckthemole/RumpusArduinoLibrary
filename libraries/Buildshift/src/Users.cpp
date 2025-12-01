#include "Users.h"

Users::Users(WiFiClientWrapper &client, RumpshiftLogger *logger)
    : _wifiClient(client), _logger(logger) {}

HttpResponse Users::fetch(const char *host, int port, const char *url)
{
    _logger->info("[Users] Fetching from " + String(host) + ":" + String(port) + url);

    if (!_wifiClient.connected())
    {
        _logger->warn("[Users] WiFiClientWrapper not connected yet");
    }

    HttpResponse resp = _wifiClient.http().get(host, port, url);

    _logger->info("[Users] Response status: " + String(resp.status()));
    return resp;
}

int Users::parse(const String &json)
{
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error)
    {
        _logger->error(String("[Users] JSON parse failed: ") + error.c_str());
        return 0;
    }

    // Root is an object
    JsonArray arr = doc["results"].as<JsonArray>();
    _userCount = 0;

    for (JsonObject obj : arr)
    {
        if (_userCount >= MAX_USERS)
            break;
        _users[_userCount].id = obj["id"].as<String>();
        _users[_userCount].name = obj["name"].as<String>();
        _userCount++;
    }

    _logger->info("[Users] Parsed " + String(_userCount) + " users.");
    return _userCount;
}

void Users::setUsers(const User *newUsers, int count)
{
    if (count < 0)
        count = 0;
    if (count > MAX_USERS)
        count = MAX_USERS;

    _userCount = count;

    for (int i = 0; i < _userCount; i++)
    {
        _users[i].id = newUsers[i].id;
        _users[i].name = newUsers[i].name;
    }

    if (_logger)
    {
        _logger->info(String("[Users] setUsers(): loaded ") + _userCount + " users");
    }
}

void Users::clearUsers()
{
    _userCount = 0;

    if (_logger)
        _logger->info("[Users] Users cleared");
}

void Users::printUsers()
{
    _logger->info("[Users] Listing users:");
    for (int i = 0; i < _userCount; i++)
    {
        _logger->info("  - " + _users[i].name + " (ID: " + _users[i].id + ")");
    }
}

bool Users::getUserByName(String &name, User &user)
{
    for (int index = 0; index < _userCount; index++)
    {
        if (_users[index].name.equals(name))
        {
            user = _users[index];
            return true;
        }
    }
    return false;
}
