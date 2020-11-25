#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Platform.hpp>

#include <string>

namespace Dusk {

DUSK_ENGINE_API
std::string GetDirname(std::string path);

DUSK_ENGINE_API
std::string GetBasename(std::string path);

DUSK_ENGINE_API
std::string GetExtension(std::string path);

#if defined(DUSK_OS_WINDOWS)

DUSK_ENGINE_API
std::wstring ConvertUTF8ToWideString(std::string str);

DUSK_ENGINE_API
std::string ConvertWideStringToUTF8(std::wstring str);

#endif // DUSK_OS_WINDOWS

} // namespace Dusk

#endif // DUSK_UTIL_HPP