#ifndef DUSK_LOG_HPP
#define DUSK_LOG_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Path.hpp>

#include <format>

namespace dusk {

constexpr StringView LogGetFilename(StringView filename)
{
    size_t pivot = filename.find_last_of(Path::preferred_separator);
    return (
        pivot == StringView::npos
        ? filename
        : filename.substr(pivot + 1)
    );
}

#define DUSK_ANCHOR (std::format("{}:{:04}", dusk::LogGetFilename(__FILE__), __LINE__))

///
/// Log a message with a given tag
///
/// @param tag The prefix to the log message, wrapped in (), usually RYME_ANCHOR
/// @param message The message to log
///
DUSK_API
void LogMessage(StringView tag, StringView message);

///
/// Format and Log a message with a given tag
///
/// @param tag The prefix to the log message, wrapped in (), usually RYME_ANCHOR
/// @param format The format string for fmt::format()
/// @param args... The remaining arguments to fmt::format()
///
template <typename... Args>
inline void Log(StringView tag, StringView format, Args&&... args) {
    LogMessage(tag, std::vformat(format, std::make_format_args(args...)));
}

#if defined(DUSK_BENCHMARK)

    #define DUSK_BENCHMARK_START() \
        auto duskBenchmarkStart = std::chrono::high_resolution_clock::now()

    #define DUSK_BENCHMARK_END() \
        dusk::Log(DUSK_ANCHOR, "Function '{}' took {:.3}ms", DUSK_FUNCTION_NAME,   \
            std::chrono::duration_cast<std::chrono::duration<double, std::milli>>( \
                std::chrono::high_resolution_clock::now() - duskBenchmarkStart     \
            ).count()                                                              \
        )

#else

    #define DUSK_BENCHMARK_START() do {} while (0)

    #define DUSK_BENCHMARK_END() do {} while (0)

#endif

} // namespace dusk

#endif // DUSK_LOG_HPP