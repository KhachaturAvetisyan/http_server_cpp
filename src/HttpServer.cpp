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
    httpEndpoint->serve();

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
    Routes::Get(router, "/api/device_updated", Routes::bind(&HttpServer::device_updated, this));
}



void HttpServer::handleHello(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    DEBUG("HTTP server handleHello");

    response.send(Pistache::Http::Code::Ok, "Hello, World!\n");
}

void HttpServer::device_updated(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    DEBUG("HTTP server device_updated");

    const auto& queryParams = request.query();

    if (queryParams.has("device_id") && queryParams.has("nfc_list_update_time") && queryParams.has("pin_list_update_time"))
    {
        std::string device_id = queryParams.get("device_id").get();
        uint32_t nfc_list_update_time;
        uint32_t pin_list_update_time;

        try
        {
            nfc_list_update_time = std::stoul(queryParams.get("nfc_list_update_time").get());
        }
        catch (const std::invalid_argument& e)
        {
            ERROR( "Invalid argument: %s", e.what());
        }
        catch (const std::out_of_range& e)
        {
            ERROR( "Out of range: %s", e.what());
        }

        try
        {
            pin_list_update_time = std::stoul(queryParams.get("pin_list_update_time").get());
        }
        catch (const std::invalid_argument& e)
        {
            ERROR( "Invalid argument: %s", e.what());
        }
        catch (const std::out_of_range& e)
        {
            ERROR( "Out of range: %s", e.what());
        }


        DEBUG("device_id = %s", device_id.c_str());
        DEBUG("nfc_list_update_time = %d", nfc_list_update_time);
        DEBUG("pin_list_update_time = %d", pin_list_update_time);

        response.send(Http::Code::Ok, "OK");
    }
    else
    {
        response.send(Http::Code::Bad_Request, "Missing one or more required parameters", MIME(Text, Plain));
    }

    response.send(Pistache::Http::Code::Ok, "GET request handled\n");
}
