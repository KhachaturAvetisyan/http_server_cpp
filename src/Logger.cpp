#include "../headers/Logger.h"

Logger::Logger(const char*  ident, int option, int facility)
{
    // open syslog
    openlog(ident, option, facility);
}

Logger::Logger(const char *ident, int facility)
{
    // open syslog
    openlog(ident, LOG_CONS | LOG_PID | LOG_PERROR, facility);
}

Logger::~Logger()
{
    // close syslog
    closelog();
}