#include <Dusk/MeshImporter.hpp>

#include <unordered_map>

namespace Dusk {

static std::unordered_map<string, std::unique_ptr<MeshImporter>> _MeshImporterByID;

static std::vector<MeshImporter *> _MeshImporterList;

static std::unordered_map<string, std::vector<MeshImporter *>> _MeshImporterListByMediaType;

void updateMeshImporterCache()
{
    _MeshImporterList.clear();
    _MeshImporterListByMediaType.clear();

    for (const auto& [id, importer] : _MeshImporterByID) {
        _MeshImporterList.push_back(importer.get());

        for (const auto& mediaType : importer->GetSupportedMediaTypes()) {
            auto it = _MeshImporterListByMediaType.find(mediaType);
            if (it == _MeshImporterListByMediaType.end()) {
                _MeshImporterListByMediaType[mediaType] = std::vector<MeshImporter *>();
            }

            _MeshImporterListByMediaType[mediaType].push_back(importer.get());
        }
    }
}

DUSK_CORE_API
void AddMeshImporter(const string& id, std::unique_ptr<MeshImporter> importer)
{
    _MeshImporterByID[id] = std::move(importer);
    updateMeshImporterCache();
}

DUSK_CORE_API
void RemoveMeshImporter(const string& id)
{
    auto it = _MeshImporterByID.find(id);
    if (it != _MeshImporterByID.end()) {
        _MeshImporterByID.erase(it);
    }
    
    updateMeshImporterCache();
}

DUSK_CORE_API
const std::vector<MeshImporter *>& GetAllMeshImporters()
{
    return _MeshImporterList;
}

DUSK_CORE_API
const std::vector<MeshImporter *>& GetMeshImporterListForMediaType(std::string mediaType)
{
    auto it = _MeshImporterListByMediaType.find(mediaType);
    if (it != _MeshImporterListByMediaType.end()) {
        return it->second;
    }

    return _MeshImporterList;
}

} // namespace Dusk
