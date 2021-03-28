#include <Dusk/Util.hpp>
#include <Dusk/Log.hpp>

#include <sstream>

namespace Dusk {

DUSK_CORE_API
string GetDirname(string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == string::npos
        ? path
        : path.substr(0, pivot)
    );
}

DUSK_CORE_API
string GetBasename(string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == string::npos
        ? path
        : path.substr(pivot + 1)
    );
}

DUSK_CORE_API
string GetExtension(string path)
{
    size_t pivot = path.find_last_of('.');
    return (
        pivot == string::npos
        ? string()
        : path.substr(pivot + 1)
    );
}

DUSK_CORE_API
std::vector<Path> GetAssetPathList()
{
    // TODO: Be better
    static std::vector<Path> paths;
    if (!paths.empty()) {
        return paths;
    }

    paths.push_back(Path("Assets"));

    const char * path = getenv("DUSK_ASSET_PATH");
    if (path) {
        LogVerbose(DUSK_ANCHOR, "DUSK_ASSET_PATH={}", path);

        std::istringstream iss(path);
        string p;
        while (std::getline(iss, p, DUSK_PATH_SEPARATOR)) {
            LogVerbose(DUSK_ANCHOR, "Adding Asset Path: '{}'", p);
            paths.push_back(Path(p));
        }
    }
    
    return paths;
}

#if defined(DUSK_PLATFORM_WINDOWS)

DUSK_CORE_API
std::wstring ConvertUTF8ToWideString(string str)
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

DUSK_CORE_API
string ConvertWideStringToUTF8(std::wstring wstr)
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
        return string();
    }

    return string(utf8.data());
}

#endif // defined(DUSK_PLATFORM_WINDOWS)

} // namespace Dusk