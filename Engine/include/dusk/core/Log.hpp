#ifndef DUSK_DEBUG_HPP
#define DUSK_DEBUG_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/core/JSON.hpp>

#include <cstdio> // for printf, vsnprintf

namespace dusk {

    enum LogLevel {
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_PERF,
        LOG_VERBOSE,
        LOG_LOAD,
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
    auto LogWrap<nlohmann::json>(const nlohmann::json& v) {
        return v.get<std::string>().c_str();
    }

/* 
    template <> 
    auto LogWrap<glm::vec2>(const glm::vec2& v) {
        return glm::to_string(v).c_str();
    }
    
    template <> 
    auto LogWrap<glm::vec3>(const glm::vec3& v) {
        return glm::to_string(v).c_str();
    }
    
    template <> 
    auto LogWrap<glm::vec4>(const glm::vec4& v) {
        return glm::to_string(v).c_str();
    }
    
    template <> 
    auto LogWrap<glm::mat3>(const glm::mat3& v) {
        return glm::to_string(v).c_str();
    }
    
    template <> 
    auto LogWrap<glm::mat4>(const glm::mat4& v) {
        return glm::to_string(v).c_str();
    }
    
    template <> 
    auto LogWrap<glm::quat>(const glm::quat& v) {
        return glm::to_string(v).c_str();
    }
*/
    
#pragma clang diagnostic pop

#pragma GCC diagnostic pop

    template <class ...Args>
    static inline void Log(LogLevel level, const char * format, Args... args)
    {
        #if defined(WIN32)

        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        const int DEFAULT = 7;

        int color = DEFAULT;

        switch (level)
        {
        case LOG_INFO:
            color = 7; // White
            break;
        case LOG_WARN:
            color = 6; // Yellow
            break;
        case LOG_ERROR:
            color = 4; // Red
            break;
        case LOG_PERF:
            color = 5; // Magenta
            break;
        case LOG_VERBOSE:
            color = 8; // Grey
            break;
        case LOG_LOAD:
            color = 2; // Green
            break;
        }

        SetConsoleTextAttribute(hConsole, color);

        #else 

        static const char * TERM = getenv("TERM");
        static bool hasColor = (TERM && (
            strncmp(TERM, "xterm", 5)   == 0 || 
            strncmp(TERM, "rxvt", 4)    == 0 ||
            strncmp(TERM, "konsole", 8) == 0
        ));
        
        const short FG_DEFAULT = 39;
        const short BG_DEFAULT = 49;

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

        if (hasColor) {
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

        #if defined(WIN32)
        
        SetConsoleTextAttribute(hConsole, DEFAULT);

        #else

        if (hasColor) {
            printf("\033[%dm\033[%dm", FG_DEFAULT, BG_DEFAULT);
        }

        #endif
    }

#ifndef DUSK_ENABLE_VERBOSE_LOGGING
#   define DuskLogVerbose(M, ...)  do { } while(0)
#else
#   define DuskLogVerbose(M, ...) \
        do { dusk::Log(dusk::LogLevel::LOG_VERBOSE, "[VERB](%s:%d) " M "\n", dusk::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)
#endif

#define DuskLog(M, ...) \
    do { dusk::Log(dusk::LogLevel::LOG_INFO, M "\n", ##__VA_ARGS__); } while (0)

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

}


#endif // DUSK_DEBUG_HPP
