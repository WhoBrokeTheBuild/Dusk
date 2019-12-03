#include <Dusk/Util.hpp>

namespace Dusk {

DUSK_CORE_API
std::string GetDirname(std::string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == std::string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == std::string::npos
        ? path
        : path.substr(0, pivot)
    );
}

DUSK_CORE_API
std::string GetBasename(std::string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == std::string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == std::string::npos
        ? path
        : path.substr(pivot + 1)
    );
}

DUSK_CORE_API
std::string GetExtension(std::string path)
{
    size_t pivot = path.find_last_of('.');
    return (
        pivot == std::string::npos
        ? std::string()
        : path.substr(pivot + 1)
    );
}

} // namespace Dusk