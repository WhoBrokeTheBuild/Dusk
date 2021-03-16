#ifndef DUSK_LOG_HPP
#define DUSK_LOG_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/Version.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <fmt/os.h>
#include <fmt/color.h>

template<>
struct fmt::formatter<Dusk::Path> : public fmt::formatter<string_view>
{
    template <typename FormatContext>
    auto format(const Dusk::Path& path, FormatContext& ctx) {
        return formatter<string_view>::format(string_view(path.ToString()), ctx);
    }

};

template<>
struct fmt::formatter<Dusk::Version> : public fmt::formatter<string_view>
{
    template <typename FormatContext>
    auto format(const Dusk::Version& version, FormatContext& ctx) {
        return formatter<string_view>::format(string_view(version.ToString()), ctx);
    }

};

namespace Dusk {

enum class LogLevel
{
    Verbose = 0,
    Performance,
    Debug,
    Info,
    Warning,
    Error,
    Fatal,
    Silent,

}; // enum class LogLevel

DUSK_ENGINE_API
void SetMinimumLogLevel(LogLevel level);

DUSK_ENGINE_API
LogLevel GetMinimumLogLevel();

DUSK_ENGINE_API
bool AddLogFile(Path path);

DUSK_ENGINE_API
void CloseAllLogFiles();

DUSK_ENGINE_API
void LogMessage(LogLevel level, string_view tag, string_view message);

#define DUSK_ANCHOR (fmt::format("{}:{}", Dusk::Path(__FILE__).GetFilename().ToCString(), __LINE__))

template <class... Args>
inline void Log(LogLevel level, string_view tag, string_view format, const Args&... args) {
    LogMessage(level, tag, fmt::format(format, args...));
}

template <class... Args>
inline void LogVerbose(string_view tag, string_view format, const Args&... args) {
    LogMessage(LogLevel::Verbose, tag, fmt::format(format, args...));
}

template <class... Args>
inline void LogDebug(string_view tag, string_view format, const Args&... args) {
    LogMessage(LogLevel::Debug, tag, fmt::format(format, args...));
}

template <class... Args>
inline void LogPerformance(string_view tag, string_view format, const Args&... args) {
    LogMessage(LogLevel::Performance, tag, fmt::format(format, args...));
}

template <class... Args>
inline void LogInfo(string_view tag, string_view format, const Args&... args) {
    LogMessage(LogLevel::Info, tag, fmt::format(format, args...));
}

template <class... Args>
inline void LogWarning(string_view tag, string_view format, const Args&... args) {
    LogMessage(LogLevel::Warning, tag, fmt::format(format, args...));
}

template <class... Args>
inline void LogError(string_view tag, string_view format, const Args&... args) {
    LogMessage(LogLevel::Error, tag, fmt::format(format, args...));
}

template <class... Args>
inline void LogFatal(string_view tag, string_view format, const Args&... args) {
    LogMessage(LogLevel::Fatal, tag, fmt::format(format, args...));
    std::terminate();
}

} // namespace Dusk

#endif // DUSK_LOG_HPP
