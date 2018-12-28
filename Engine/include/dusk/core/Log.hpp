#ifndef DUSK_DEBUG_HPP
#define DUSK_DEBUG_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Platform.hpp>
#include <dusk/core/Util.hpp>

#include <cstdarg> // for va_list
#include <cstdio> // for printf, vsnprintf

namespace dusk {

// clang-format off

    enum LogLevel {
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_PERF,
        LOG_VERBOSE,
        LOG_LOAD,
    };

    static inline void Log(LogLevel level, const char * format, ...)
    {
        const short FG_DEFAULT = 39;
        const short BG_DEFAULT = 49;

        //static const char * TERM = getenv("TERM");

        short fgColor = FG_DEFAULT;
        short bgColor = BG_DEFAULT;

        switch (level)
        {
        case LOG_INFO:
            fgColor = 97; // White
            break;
        case LOG_WARN:
            fgColor = 33; // Yellow
            break;
        case LOG_ERROR:
            fgColor = 31; // Red
            break;
        case LOG_PERF:
            fgColor = 35; // Magenta
            break;
        case LOG_VERBOSE:
            fgColor = 37; // Grey
            break;
        case LOG_LOAD:
            fgColor = 32; // Green
            break;
        }

        #ifndef DUSK_OS_WINDOWS
            printf("\033[%dm\033[%dm", fgColor, bgColor);
        #endif

        va_list valist;
        va_start(valist, format);
        vprintf(format, valist);
        va_end(valist);

        #ifndef DUSK_OS_WINDOWS
            printf("\033[%dm\033[%dm", FG_DEFAULT, BG_DEFAULT);
        #endif
    }

#ifndef DUSK_ENABLE_VERBOSE_LOGGING
#   define DuskLogVerbose(M, ...)  do { } while(0)
#else
#   define DuskLogVerbose(M, ...) \
        do { dusk::Log(dusk::LogLevel::LOG_VERBOSE, "[VERB](%s:%d) " M "\n", dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)
#endif

#define DuskLogInfo(M, ...) \
    do { dusk::Log(dusk::LogLevel::LOG_INFO, "[INFO](%s:%d) " M "\n", dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogWarn(M, ...) \
    do { dusk::Log(dusk::LogLevel::LOG_WARN, "[WARN](%s:%d) " M "\n", dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogError(M, ...) \
    do { dusk::Log(dusk::LogLevel::LOG_ERROR, "[ERRO](%s:%d) " M "\n", dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogPerf(M, ...) \
    do { dusk::Log(dusk::LogLevel::LOG_PERF, "[PERF](%s:%d) " M "\n", dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogLoad(M, ...) \
    do { dusk::Log(dusk::LogLevel::LOG_LOAD, "[LOAD](%s:%d) " M "\n", dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

// clang-format on

}


#endif // DUSK_DEBUG_HPP
