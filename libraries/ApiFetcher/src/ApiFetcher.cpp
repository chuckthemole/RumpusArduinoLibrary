#include "ApiFetcher.h"

HttpFetcher::HttpFetcher(const String &url)
{
    setUrl(url);
}

void HttpFetcher::setUrl(const String &url)
{
    _url = url;
    String working = url;

    // Remove scheme
    if (working.startsWith("http://"))
    {
        working.remove(0, 7);
        _port = 80;
    }
    else if (working.startsWith("https://"))
    {
        working.remove(0, 8);
        _port = 443; // needs WiFiClientSecure for real https
    }
    else
    {
        _port = 80;
    }

    // Split host and path
    int slashIndex = working.indexOf('/');
    if (slashIndex != -1)
    {
        _host = working.substring(0, slashIndex);
        _path = working.substring(slashIndex);
    }
    else
    {
        _host = working;
        _path = "/";
    }

    // Extract port if present
    int colonIndex = _host.indexOf(':');
    if (colonIndex != -1)
    {
        _port = _host.substring(colonIndex + 1).toInt();
        _host = _host.substring(0, colonIndex);
    }
}

String HttpFetcher::get()
{
    WiFiClient wifi;
    HttpClient client(wifi, _host.c_str(), _port);

    client.get(_path.c_str());

    int statusCode = client.responseStatusCode();
    if (statusCode != 200)
    {
        Serial.println("HTTP GET failed, status " + String(statusCode));
        return "";
    }

    String response = client.responseBody();
    return response;
}
