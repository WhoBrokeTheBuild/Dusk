#ifndef DUSK_LOG_HPP
#define DUSK_LOG_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/JSON.hpp>

#include <string>
#include <cstdio>

#if defined(DUSK_OS_WINDOWS)
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

namespace Dusk {

enum class LogLevel 
{
    Info,
    Warn,
    Error,
    Performance,
    Verbose,
    Load,
};

template <class T>
inline auto LogWrap(const T& v) {
    return v;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

template <>
inline auto LogWrap<std::string>(const std::string& v) {
    return v.c_str();
}

template <>
inline auto LogWrap<json>(const json& v) {
    static std::string str;
    str = v.get<std::string>();
    return str.c_str();
}

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

template <class ...Args>
inline void Log(LogLevel level, const char * format, Args... args)
{
    #if defined(DUSK_OS_WINDOWS)

        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        
        const int Default = 7; // White on Black

        int color = Default;

        switch (level)
        {
        case LogLevel::Info:
            color = 7; // White on Black
            break;
        case LogLevel::Warn:
            color = 6; // Yellow on Black
            break;
        case LogLevel::Error:
            color = 4; // Red on Black
            break;
        case LogLevel::Performance:
            color = 5; // Magenta on Black
            break;
        case LogLevel::Verbose:
            color = 8; // Grey on Black
            break;
        case LogLevel::Load:
            color = 2; // Green on Black
            break;
        }

        SetConsoleTextAttribute(hConsole, color);

    #else

        static bool isTTY = isatty(fileno(stdout));

        const char * Default = "0;39";

        const char * color = Default;

        switch (level)
        {
        case LogLevel::Info:
            color = "0;97"; // White
            break;
        case LogLevel::Warn:
            color = "0;33"; // Yellow
            break;
        case LogLevel::Error:
            color = "0;31"; // Red
            break;
        case LogLevel::Performance:
            color = "0;35"; // Magenta
            break;
        case LogLevel::Verbose:
            color = "1;30"; // Grey
            break;
        case LogLevel::Load:
            color = "0;32"; // Green
            break;
        }

        if (isTTY) {
            printf("\033[%sm", color);
        }

    #endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        
    printf(format, LogWrap(args)...);

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

    #if defined(DUSK_OS_WINDOWS)
        SetConsoleTextAttribute(hConsole, Default);
    #else
        if (isTTY) {
            printf("\033[%sm", Default);
        }
    #endif
}

#if !defined(DUSK_SOURCE_PATH_LENGTH)
    #define DUSK_SOURCE_PATH_LENGTH 0
#endif

#define DUSK_FILENAME (&__FILE__[DUSK_SOURCE_PATH_LENGTH])

#if defined(DUSK_ENABLE_VERBOSE_LOGGING)
    #define DuskLogVerbose(M, ...) \
        do { Dusk::Log(Dusk::LogLevel::Verbose, "[VERB](%s:%d) " M "\n", \
            DUSK_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)
#else
    #define DuskLogVerbose(M, ...) do { } while (0)
#endif

#define DuskLog(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Info, M "\n", ##__VA_ARGS__); } while (0)

#define DuskLogInfo(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Info, "[INFO](%s:%d) " M "\n", \
        DUSK_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogWarn(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Warn, "[WARN](%s:%d) " M "\n", \
        DUSK_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogError(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Error, "[ERRO](%s:%d) " M "\n", \
        DUSK_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogPerf(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Performance, "[PERF](%s:%d) " M "\n", \
        DUSK_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogLoad(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Load, "[LOAD](%s:%d) " M "\n", \
        DUSK_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

} // namespace Dusk

#endif // DUSK_LOG_HPP
