#pragma once

#include <iostream>
#include <string>

# include <pistache/endpoint.h>
# include <pistache/http.h>
# include <pistache/router.h>

// include logger
#include "Logger.h"

using namespace Pistache;
using namespace Rest;

class HttpServer
{
private:
    uint16_t port_to_listen;

    Router router;      // POST/GET/etc. route handler
    std::shared_ptr<Http::Endpoint> httpEndpoint;

    void setupRoutes();

    void handleHello(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void device_updated(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

public:
    explicit HttpServer(uint16_t _port_to_listen = 8080);
    ~HttpServer();

    void init(size_t thr = 1);
    void start();
    void shutdown();
};
