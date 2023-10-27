#pragma once

# include <syslog.h>


#define INFO(format, ...) \
    Logger::write_log(LOG_INFO, "[%s::%d] " format, __FUNCTION__, __LINE__, ## __VA_ARGS__);

#define WARNING(format, ...) \
    Logger::write_log(LOG_WARNING, "[%s::%d] " format, __FUNCTION__, __LINE__, ## __VA_ARGS__);

#define ERROR(format, ...) \
    Logger::write_log(LOG_ERR, "[%s::%d] " format, __FUNCTION__, __LINE__, ## __VA_ARGS__);

#ifdef DEBUG_VAR
#define DEBUG(format, ...) \
    Logger::write_log(LOG_DEBUG, "[%s::%d] " format, __FUNCTION__, __LINE__, ## __VA_ARGS__);
#else
#define DEBUG(format, ...) \
    do {} while (0)
#endif


class Logger
{
public:
    Logger(const char*  ident, int option, int facility);
    Logger(const char*  ident, int facility);
    ~Logger();

    template <typename... Args>
    static void write_log(int level, const char *format, Args... args)
    {
        syslog(level, format, args...);
    }
};
