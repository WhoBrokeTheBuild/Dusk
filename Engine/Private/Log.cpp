#include <Dusk/Log.hpp>

#include <fstream>

namespace Dusk {

static LogLevel _MinimumLogLevel = LogLevel::Info;

static std::vector<FILE *> _LogFiles;

DUSK_ENGINE_API
void SetMinimumLogLevel(LogLevel level)
{
    _MinimumLogLevel = level;
}

DUSK_ENGINE_API
LogLevel GetMinimumLogLevel()
{
    return _MinimumLogLevel;
}

DUSK_ENGINE_API
bool AddLogFile(Path path)
{
    FILE * file = nullptr;
    file = fopen(path.ToCString(), "wt");
    if (file) {
        _LogFiles.push_back(file);
        LogVerbose(DUSK_ANCHOR, "Adding log file '{}'", path);
        return true;
    }

    LogError(DUSK_ANCHOR, "Failed to add log file '{}'", path);
    return false;
}

DUSK_ENGINE_API
void CloseAllLogFiles()
{
    for (FILE * file : _LogFiles) {
        fclose(file);
    }
    _LogFiles.clear();
}

DUSK_ENGINE_API
void LogMessage(LogLevel level, string_view tag, string_view message)
{
    if (level < _MinimumLogLevel) {
        return;
    }

    fmt::text_style ts = bg(fmt::terminal_color::black);

    switch (level) {
    case LogLevel::Verbose:
        ts = fmt::fg(fmt::terminal_color::cyan);
        break;
    case LogLevel::Debug:
        ts = fmt::fg(fmt::terminal_color::green);
        break;
    case LogLevel::Performance:
        ts = fmt::fg(fmt::terminal_color::magenta);
        break;
    case LogLevel::Info:
        ts = fmt::fg(fmt::terminal_color::white);
        break;
    case LogLevel::Warning:
        ts = fmt::fg(fmt::terminal_color::yellow);
        break;
    case LogLevel::Error:
    case LogLevel::Fatal:
        ts = fmt::fg(fmt::terminal_color::red);
        break;
    default:
        break;
    }

    fmt::print(ts, "({}) {}\n", tag, message);

    for (FILE * file : _LogFiles) {
        fmt::print(file, "({}) {}\n", tag, message);
    }
}

} // namespace Dusk