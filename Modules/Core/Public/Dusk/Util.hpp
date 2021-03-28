#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Platform.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Path.hpp>

#include <vector>

namespace Dusk {

DUSK_CORE_API
string GetDirname(string path);

DUSK_CORE_API
string GetBasename(string path);

DUSK_CORE_API
string GetExtension(string path);

DUSK_CORE_API
std::vector<Path> GetAssetPathList();

DUSK_CORE_API
std::vector<Path> GetModulePathList();

#if defined(DUSK_PLATFORM_WINDOWS)

DUSK_CORE_API
std::wstring ConvertUTF8ToWideString(string str);

DUSK_CORE_API
string ConvertWideStringToUTF8(std::wstring str);

#endif // defined(DUSK_PLATFORM_WINDOWS)

} // namespace Dusk

#endif // DUSK_UTIL_HPP