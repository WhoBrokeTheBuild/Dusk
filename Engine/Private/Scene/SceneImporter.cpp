#include <Dusk/Scene/SceneImporter.hpp>

#include <unordered_map>

namespace Dusk {

static std::unordered_map<std::string, std::unique_ptr<ISceneImporter>> _SceneImporters;

static std::vector<ISceneImporter *> _SceneImporterList;

void updateSceneImporterList()
{
    _SceneImporterList.clear();
    for (const auto& it : _SceneImporters) {
        _SceneImporterList.push_back(it.second.get());
    }
}

DUSK_ENGINE_API
void AddSceneImporter(const std::string& id, std::unique_ptr<ISceneImporter> importer)
{
    _SceneImporters[id] = std::move(importer);
    updateSceneImporterList();
}

DUSK_ENGINE_API
void RemoveSceneImporter(const std::string& id)
{
    auto it = _SceneImporters.find(id);
    if (it != _SceneImporters.end()) {
        _SceneImporters.erase(it);
    }
    
    updateSceneImporterList();
}

DUSK_ENGINE_API
const std::vector<ISceneImporter *>& GetAllSceneImporters()
{
    return _SceneImporterList;
}

} // namespace Dusk
