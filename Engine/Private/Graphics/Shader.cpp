#include <Dusk/Graphics/Shader.hpp>

#include <array>
#include <sstream>

namespace Dusk {

DUSK_CORE_API
std::vector<std::string> GetShaderPaths()
{
    static std::vector<std::string> paths;
    if (!paths.empty()) {
        return paths;
    }

    paths.push_back("");

#if defined(DUSK_OS_WINDOWS)
    const char SEP = ';';
#else
    const char SEP = ':';
#endif

    const char * path = getenv("DUSK_SHADER_INCLUDE_PATH");
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