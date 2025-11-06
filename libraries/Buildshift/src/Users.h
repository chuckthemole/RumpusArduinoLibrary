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

    const User *getUsers() const { return users; }
    int getUserCount() const { return userCount; }

    String getUsersDelimit(const String &delimiter = "\n") const
    {
        String result = "";
        for (int i = 0; i < userCount; i++)
        {
            result += users[i].getName();
            if (i < userCount - 1)
                result += delimiter;
        }
        return result;
    }

private:
    WiFiClientWrapper &wifiClient;
    RumpshiftLogger *logger;

    static const int MAX_USERS = 20;
    User users[MAX_USERS];
    int userCount = 0;
};

#endif
