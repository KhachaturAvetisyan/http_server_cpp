// logger includes
#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async_logger.h>
#include <spdlog/async.h>
#include <spdlog/sinks/syslog_sink.h>

// include standard libs
#include <iostream>
#include <vector>

// some defines
//#define LOG_FACILITY                           LOG_LOCAL0


#include "HttpServer.h"

int main()
{
    // initialize logger sinks vector
    std::vector<spdlog::sink_ptr> sinks;

#ifdef SYS_LOGGER
    // syslog logger initialization
    const auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>("intercom_listener",
                                                                             LOG_PID, LOG_FACILITY, true);
    syslog_sink->set_level(spdlog::level::trace);

    syslog_sink->set_pattern("%^[%H:%M:%S %z] [%l] [thread %t] %v%$");
    sinks.push_back(syslog_sink);
#endif

#ifdef CONSOLE_LOGGER
    // console logger initialization
    const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);

    console_sink->set_pattern("%^[%H:%M:%S %z] [%l] [thread %t] %v%$");
    sinks.push_back(console_sink);
#endif

    // initialize asynchronous multi sink logger
    const auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);

    // set default logger
    set_default_logger(logger);

    SPDLOG_INFO("Start main function!!");

    Http::listenAndServe<HelloHandler>(Pistache::Address("*:9080"));

    return 0;
}