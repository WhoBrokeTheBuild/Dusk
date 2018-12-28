#include "dusk/asset/AssetLoader.hpp"

#include <dusk/asset/Mesh.hpp>

namespace dusk {

std::unordered_map<Asset *, std::unique_ptr<Asset>> AssetLoader::_Assets;
std::unordered_map<Asset *, int> AssetLoader::_AssetUseCount;
std::unordered_map<std::string, Asset *> AssetLoader::_AssetsByFilename;

} // namespace dusk
