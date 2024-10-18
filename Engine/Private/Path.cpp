#include <Dusk/Path.hpp>

namespace dusk {

List<Path> _assetPathList;

DUSK_API
void ParseAssetPathList(StringView assetPath)
{
    for (auto& str : StringSplit(assetPath, ":")) {
        _assetPathList.emplace_back(str);
    }
}

List<Path> GetAssetPathList()
{
    return _assetPathList;
}


} // namespace dusk