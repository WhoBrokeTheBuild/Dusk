#include <Dusk/Util.hpp>
#include <Dusk/Log.hpp>

#include <sstream>

namespace Dusk {

DUSK_ENGINE_API
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

DUSK_ENGINE_API
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

DUSK_ENGINE_API
std::string GetExtension(std::string path)
{
    size_t pivot = path.find_last_of('.');
    return (
        pivot == std::string::npos
        ? std::string()
        : path.substr(pivot + 1)
    );
}

DUSK_ENGINE_API
std::vector<std::string> GetAssetPathList()
{
    static std::vector<std::string> paths;
    if (!paths.empty()) {
        return paths;
    }

    paths.push_back("Assets/");

    const char * path = getenv("DUSK_ASSET_PATH");
    DuskLogVerbose("DUSK_ASSET_PATH=%s", path);

    if (path) {
        std::istringstream iss(path);
        std::string p;
        while (std::getline(iss, p, DUSK_PATH_SEPARATOR)) {
            paths.push_back(p);
        }
    }
    
    return paths;
}

#if defined(DUSK_PLATFORM_WINDOWS)

DUSK_ENGINE_API
std::wstring ConvertUTF8ToWideString(std::string str)
{
    size_t maxSize = str.size() + 1;

    // Initialize to maximum potential size
    std::vector<wchar_t> wide(maxSize);

    int result = MultiByteToWideChar(
        CP_UTF8, 0, 
        str.c_str(), -1, 
        wide.data(), wide.size());
    
    if (result <= 0) {
        return std::wstring();
    }

    return std::wstring(wide.data());
}

DUSK_ENGINE_API
std::string ConvertWideStringToUTF8(std::wstring wstr)
{
    size_t maxSize = (wstr.size() * 4) + 1;

    // Initialize to maximum potential size
    std::vector<char> utf8(maxSize);

    int result = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.c_str(), -1,
        utf8.data(), utf8.size(), 
        NULL, NULL);
        
    if (result <= 0) {
        return std::string();
    }

    return std::string(utf8.data());
}

#endif // defined(DUSK_PLATFORM_WINDOWS)

} // namespace Dusk