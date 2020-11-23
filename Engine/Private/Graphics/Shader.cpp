#include <Dusk/Graphics/Shader.hpp>
#include <Dusk/Log.hpp>

#include <array>
#include <sstream>

namespace Dusk {

DUSK_ENGINE_API
std::vector<std::string> GetShaderPaths()
{
    static std::vector<std::string> paths;
    if (!paths.empty()) {
        return paths;
    }

    paths.push_back(std::string());

    const char SEP = ':';

    const char * path = getenv("DUSK_SHADER_INCLUDE_PATH");
    DuskLogVerbose("DUSK_SHADER_INCLUDE_PATH=%s", path);

    if (path) {
        std::istringstream iss(path);
        std::string p;
        while (std::getline(iss, p, SEP)) {
            paths.push_back(p);
        }
    }
    
    return paths;
}

} // namespace Dusk