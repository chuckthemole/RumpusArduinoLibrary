#ifndef USERS_H
#define USERS_H

#include <Arduino.h>
#include "WiFi/WiFiClientWrapper.h"
#include "HttpResponse.h"
#include "RumpshiftLogger.h"
#include <ArduinoJson.h>

class Users
{
public:
    Users(WiFiClientWrapper &client, RumpshiftLogger *logger);

    HttpResponse fetch(const char *host, int port, const char *url);
    int parse(const String &json);
    void printUsers();

    struct User
    {
        String id;
        String name;

        String getName() const { return name; }
        String getId() const { return id; }
    };

    const User *getUsers() const { return _users; }
    int getUserCount() const { return _userCount; }

    String getUsersDelimit(const String &delimiter = "\n") const
    {
        String result = "";
        for (int i = 0; i < _userCount; i++)
        {
            result += _users[i].getName();
            if (i < _userCount - 1)
                result += delimiter;
        }
        return result;
    }

    void setUsers(const User *newUsers, int count);
    void clearUsers();

    bool getUserByName(String &name, User &user);

private:
    WiFiClientWrapper &_wifiClient;
    RumpshiftLogger *_logger;

    static const int MAX_USERS = 20;
    User _users[MAX_USERS];
    int _userCount = 0;
};

#endif
