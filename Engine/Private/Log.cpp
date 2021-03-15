#include <Dusk/Log.hpp>

#include <fstream>

namespace Dusk {

static bool _VerboseLoggingEnabled = false;

static std::vector<FILE *> _LogFiles;

DUSK_ENGINE_API
void SetVerboseLoggingEnabled(bool enabled)
{
    _VerboseLoggingEnabled = enabled;
}

DUSK_ENGINE_API
bool IsVerboseLoggingEnabled()
{
    return _VerboseLoggingEnabled;
}

DUSK_ENGINE_API
bool AddLogFile(const string& filename)
{
    FILE * file = nullptr;
    file = fopen(filename.c_str(), "wt");
    if (file) {
        _LogFiles.push_back(file);
        DuskLogVerbose("Adding log file '%s'", filename);
        return true;
    }

    DuskLogError("Failed to add log file '%s'", filename);
    return false;
}

DUSK_ENGINE_API
std::vector<FILE *> GetAllLogFiles()
{
    return _LogFiles;
}

DUSK_ENGINE_API
void CloseAllLogFiles()
{
    for (FILE * file : _LogFiles) {
        fclose(file);
    }
    _LogFiles.clear();
}

void Log(LogLevel level, const std::string& message)
{
    
}

} // namespace Dusk