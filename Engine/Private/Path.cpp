#include <Dusk/Path.hpp>

namespace dusk {

List<Path> _assetPathList;

DUSK_API
void ParseAssetPathList(StringView assetPath)
{
    for (auto& str : StringSplit(assetPath, PathListSeparator)) {
        _assetPathList.emplace_back(str);
    }
}

DUSK_API
List<Path> GetAssetPathList()
{
    return _assetPathList;
}

DUSK_API
Path FindShortestAssetPath(Path fullPath)
{
    auto shortestPath = fullPath.string();
    for (const auto& assetPath : _assetPathList) {
        auto relativePath = std::filesystem::relative(fullPath, assetPath).string();
        if (relativePath.length() < shortestPath.length()) {
            shortestPath = relativePath;
        }
    }

    return shortestPath;
}

} // namespace dusk