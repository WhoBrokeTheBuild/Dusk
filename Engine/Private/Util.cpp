#include <Dusk/Util.hpp>

#if defined(DUSK_OS_WINDOWS)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#endif // DUSK_OS_WINDOWS

#include <vector>

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

#if defined(DUSK_OS_WINDOWS)

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

#endif // DUSK_OS_WINDOWS

} // namespace Dusk