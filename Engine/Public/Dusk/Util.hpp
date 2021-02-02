#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Platform.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Path.hpp>

#include <vector>

namespace Dusk {

DUSK_ENGINE_API
string GetDirname(string path);

DUSK_ENGINE_API
string GetBasename(string path);

DUSK_ENGINE_API
string GetExtension(string path);

DUSK_ENGINE_API
std::vector<Path> GetAssetPathList();

#if defined(DUSK_PLATFORM_WINDOWS)

DUSK_ENGINE_API
std::wstring ConvertUTF8ToWideString(string str);

DUSK_ENGINE_API
string ConvertWideStringToUTF8(std::wstring str);

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