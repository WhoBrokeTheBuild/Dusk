#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Platform.hpp>

#include <string>
#include <vector>

namespace Dusk {

DUSK_ENGINE_API
std::string GetDirname(std::string path);

DUSK_ENGINE_API
std::string GetBasename(std::string path);

DUSK_ENGINE_API
std::string GetExtension(std::string path);

DUSK_ENGINE_API
std::vector<std::string> GetAssetPathList();

#if defined(DUSK_PLATFORM_WINDOWS)

DUSK_ENGINE_API
std::wstring ConvertUTF8ToWideString(std::string str);

DUSK_ENGINE_API
std::string ConvertWideStringToUTF8(std::wstring str);

class WindowsErrorMessage
{
public:

    WindowsErrorMessage(HRESULT hResult) {
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hResult,
            GetUserDefaultUILanguage(),
            (LPTSTR)&_message,
            0,
            nullptr
        );

    };

    ~WindowsErrorMessage() {
        if (_message) {
            LocalFree(_message);
            _message = nullptr;
        }
    }

    char * GetMessage() const {
        return _message;
    }

private:

    char * _message = nullptr;

};

#endif // defined(DUSK_PLATFORM_WINDOWS)

} // namespace Dusk

#endif // DUSK_UTIL_HPP