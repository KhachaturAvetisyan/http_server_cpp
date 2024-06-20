#include "../headers/HttpServer.h"

HttpServer::HttpServer(uint16_t _port_to_listen) : port_to_listen(_port_to_listen)
{

}

HttpServer::~HttpServer()
{
    shutdown();
}

void HttpServer::init(size_t thr)
{
    auto address = Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(port_to_listen));
    auto opts = Pistache::Http::Endpoint::options()
            .threads(static_cast<int>(thr))
            .flags(Pistache::Tcp::Options::ReuseAddr);

    httpEndpoint = std::make_shared<Pistache::Http::Endpoint>(address);

    httpEndpoint->init(opts);
    setupRoutes();
}

void HttpServer::start()
{
    // httpEndpoint->setHandler(router_1.handler());
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serveThreaded();

}

void HttpServer::shutdown()
{
    httpEndpoint->shutdown();

}

void HttpServer::setupRoutes()
{
    using namespace Pistache::Rest;
    Routes::Get(router, "/hello", Routes::bind(&HttpServer::handleHello, this));
    Routes::Get(router, "/api/device_updated/:device_id/:nfc_list_update_time/:pin_list_update_time",
                Routes::bind(&HttpServer::device_updated, this));

    Routes::Get(router_1, "/hello_1", Routes::bind(&HttpServer::handleHello_1, this));
}



void HttpServer::handleHello(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{


    response.send(Pistache::Http::Code::Ok, "Hello, World!\n");
}

void HttpServer::device_updated(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{


    // Extract URL parameters
    auto device_id = request.param(":device_id").as<std::string>();
    auto nfc_list_update_time = request.param(":nfc_list_update_time").as<uint32_t>();
    auto pin_list_update_time = request.param(":pin_list_update_time").as<uint32_t>();



    response.send(Pistache::Http::Code::Ok, "GET request handled\n");
}

void HttpServer::handleHello_1(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{

    response.send(Pistache::Http::Code::Ok, "Hello, World! Router_1!!!!\n");
}
