#include <iostream>

// classes
#include "../headers/HttpServer.h"
#include "../headers/Logger.h"


int main()
{
    Logger logger("Http_Server", LOG_LOCAL0);

    INFO("Start main program sys logs opened.");

    HttpServer httpServer(8080);
    httpServer.init();
    httpServer.start();

    DEBUG("Start main loop.");

    while(true)
    {
    }

    httpServer.shutdown();

    INFO("Close syslog!!");

    return 0;
}