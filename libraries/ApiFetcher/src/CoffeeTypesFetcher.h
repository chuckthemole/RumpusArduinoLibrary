#ifndef COFFEETYPESFETCHER_H
#define COFFEETYPESFETCHER_H

#include "ApiFetcher.h"
#include <vector>

class CoffeeTypesFetcher : public ApiClient
{
public:
    CoffeeTypesFetcher(HttpFetcher *fetcher);

    bool fetch() override;
    void process(const String &json) override;
    void display() override;

    const std::vector<String> &getCoffeeTypes();

private:
    HttpFetcher *_fetcher;
    std::vector<String> _coffeeTypes;
};

#endif
