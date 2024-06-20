#pragma once

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>
#include <iostream>

using namespace Pistache;
using namespace Rest;

class HelloHandler : public Http::Handler
{
public:
    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override
    {
        SPDLOG_INFO("HelloHandler::onRequest path: {}", request.resource());
        response.send(Http::Code::Ok, "Hello, World\n");
    }
};


class HttpServer
{
private:
    uint16_t port_to_listen;

    Router router;      // POST/GET/etc. route handler
    Router router_1;
    std::shared_ptr<Http::Endpoint> httpEndpoint;

    void setupRoutes();

    void handleHello(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void device_updated(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void handleHello_1(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

public:
    explicit HttpServer(uint16_t _port_to_listen = 8080);
    ~HttpServer();

    void init(size_t thr = 1);
    void start();
    void shutdown();
};
