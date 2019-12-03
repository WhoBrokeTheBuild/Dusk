#ifndef DUSK_LOG_HPP
#define DUSK_LOG_HPP

#include <Dusk/Core.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/JSON.hpp>

#include <string>
#include <cstdio>

#if !defined(DUSK_OS_WINDOWS)
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
static auto LogWrap(const T& v) {
    return v;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

template <>
auto LogWrap<std::string>(const std::string& v) {
    return v.c_str();
}

template <>
auto LogWrap<json>(const json& v) {
    return v.get<std::string>().c_str();
}

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

template <class ...Args>
static inline void Log(LogLevel level, const char * format, Args... args)
{
    #if defined(DUSK_OS_WINDOWS)

        static Handle hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        
        const int Default = 7; // White on Black

        int color = Default;

        switch (level)
        {
        case LogLevel::Info:
            fgColor = 7; // White on Black
            break;
        case LogLevel::Warn:
            fgColor = 6; // Yellow on Black
            break;
        case LogLevel::Error:
            fgColor = 4; // Red on Black
            break;
        case LogLevel::Performance:
            fgColor = 5; // Magenta on Black
            break;
        case LogLevel::Verbose:
            fgColor = 8; // Grey on Black
            break;
        case LogLevel::Load:
            fgColor = 2; // Green on Black
            break;
        }

        SetConsoleTextAttribute(hConsole, color);

    #else

        static bool isTTY = isatty(fileno(stdout));

        const short FgDefault = 39;
        const short BgDefault = 49;

        short fgColor = FgDefault;
        short bgColor = BgDefault;

        switch (level)
        {
        case LogLevel::Info:
            fgColor = 97; // White
            break;
        case LogLevel::Warn:
            fgColor = 33; // Yellow
            break;
        case LogLevel::Error:
            fgColor = 31; // Red
            break;
        case LogLevel::Performance:
            fgColor = 35; // Magenta
            break;
        case LogLevel::Verbose:
            fgColor = 37; // Grey
            break;
        case LogLevel::Load:
            fgColor = 32; // Green
            break;
        }

        if (isTTY) {
            printf("\033[%dm\033[%dm", fgColor, bgColor);
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
        SetConsoleTextAttribute(hConsole, DEFAULT);
    #else
        if (isTTY) {
            printf("\033[%dm\033[%dm", FgDefault, BgDefault);
        }
    #endif
}

#if defined(DUSK_ENABLE_VERBOSE_LOGGING)
    #define DuskLogVerbose(M, ...) \
        do { Dusk::Log(Dusk::LogLevel::Verbose, "[VERB](%s:%d) " M "\n", Dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)
#else
    #define DuskLogVerbose(M, ...) do { } while (0)
#endif

#define DuskLog(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Info, M "\n", ##__VA_ARGS__); } while (0)

#define DuskLogInfo(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Info, "[INFO](%s:%d) " M "\n", Dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogWarn(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Warn, "[WARN](%s:%d) " M "\n", Dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogError(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Error, "[ERRO](%s:%d) " M "\n", Dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogPerf(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Performance, "[PERF](%s:%d) " M "\n", Dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define DuskLogLoad(M, ...) \
    do { Dusk::Log(Dusk::LogLevel::Load, "[LOAD](%s:%d) " M "\n", Dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

} // namespace Dusk

#endif // DUSK_LOG_HPP