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
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serveThreaded();

    INFO("HTTP server started on port %d", port_to_listen);
}

void HttpServer::shutdown()
{
    httpEndpoint->shutdown();

    INFO("HTTP server stopped");
}

void HttpServer::setupRoutes()
{
    using namespace Pistache::Rest;
    Routes::Get(router, "/hello", Routes::bind(&HttpServer::handleHello, this));
    Routes::Get(router, "/api/device_updated/:device_id/:nfc_list_update_time/:pin_list_update_time",
                Routes::bind(&HttpServer::device_updated, this));
}



void HttpServer::handleHello(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    DEBUG("HTTP server handleHello");

    response.send(Pistache::Http::Code::Ok, "Hello, World!\n");
}

void HttpServer::device_updated(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    DEBUG("HTTP server device_updated");

    DEBUG("request.resource() = %s", request.resource().c_str());

    // Extract URL parameters
    auto device_id = request.param(":device_id").as<std::string>();
    auto nfc_list_update_time = request.param(":nfc_list_update_time").as<uint32_t>();
    auto pin_list_update_time = request.param(":pin_list_update_time").as<uint32_t>();


    DEBUG("device_id = %s", device_id.c_str());
    DEBUG("nfc_list_update_time = %d", nfc_list_update_time);
    DEBUG("pin_list_update_time = %d", pin_list_update_time);

    response.send(Pistache::Http::Code::Ok, "GET request handled\n");
}
