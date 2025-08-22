#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

class NetworkClient
{
public:
    virtual ~NetworkClient() {}
    virtual int available() = 0;
    virtual int read() = 0;
    virtual void write(const char *data) = 0;
    virtual bool connected() = 0;
    // virtual bool equals(const NetworkClient &other) const = 0;
    // virtual bool operator==(const NetworkClient &other) const = 0;
    virtual bool exists() = 0; // may be redundant
};

#endif // NETWORK_CLIENT_H