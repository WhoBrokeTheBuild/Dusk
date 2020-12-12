#include <Dusk/Log.hpp>

#include <fstream>

namespace Dusk {

static std::vector<FILE *> _LogFiles;

DUSK_ENGINE_API
bool AddLogFile(const std::string& filename)
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

} // namespace Dusk