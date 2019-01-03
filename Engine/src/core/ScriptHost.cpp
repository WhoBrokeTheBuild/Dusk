#include "dusk/core/ScriptHost.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/Scene.hpp>

#include <fstream>

namespace dusk {

bool ScriptHost::LoadFile(const std::string& filename)
{
    const auto& paths = GetAssetPaths();

    std::string fullPath;
    std::ifstream file;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        file.open(fullPath);

        if (file.is_open()) break;
    }

    if (!file.is_open()) {
        fprintf(stderr, "Could not open file '%s'\n", filename.c_str());
        return false;
    }
    std::string script((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();


    return true;
}

void ScriptHost::Run() 
{ }

} // namespace dusk
