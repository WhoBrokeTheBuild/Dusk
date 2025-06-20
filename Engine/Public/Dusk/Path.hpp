#ifndef DUSK_PATH_HPP
#define DUSK_PATH_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>
#include <Dusk/String.hpp>

#include <filesystem>
#include <format>

namespace dusk {

using Path = std::filesystem::path;

#if defined(DUSK_PLATFORM_WINDOWS)
    constexpr StringView PathListSeparator = ";";
#else
    constexpr StringView PathListSeparator = ":";
#endif

inline Path GetCurrentPath() {
    return std::filesystem::current_path();
}

DUSK_API
void ParseAssetPathList(StringView assetPath);

DUSK_API
List<Path> GetAssetPathList();

DUSK_API
Path FindShortestAssetPath(Path fullPath);

} // namespace dusk

#endif // DUSK_PATH_HPP