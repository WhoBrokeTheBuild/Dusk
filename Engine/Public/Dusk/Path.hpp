#ifndef DUSK_PATH_HPP
#define DUSK_PATH_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>
#include <Dusk/String.hpp>

#include <filesystem>
#include <format>

namespace dusk {

using Path = std::filesystem::path;

inline Path GetCurrentPath() {
    return std::filesystem::current_path();
}

DUSK_API
void ParseAssetPathList(StringView assetPath);

DUSK_API
List<Path> GetAssetPathList();

} // namespace dusk

#endif // DUSK_PATH_HPP