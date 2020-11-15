#ifndef DUSK_SCENE_IMPORTER_HPP
#define DUSK_SCENE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Scene/Entity.hpp>

namespace Dusk {

class DUSK_ENGINE_API ISceneImporter 
{
public:

    DISALLOW_COPY_AND_ASSIGN(ISceneImporter)

    explicit ISceneImporter() = default;

    virtual ~ISceneImporter() = default;

    virtual bool LoadFromFile(Entity * root, const std::string& filename) = 0;

}; // class ISceneImporter

DUSK_ENGINE_API
void AddSceneImporter(const std::string& id, std::unique_ptr<ISceneImporter> importer);

DUSK_ENGINE_API
void RemoveSceneImporter(const std::string& id);

DUSK_ENGINE_API
const std::vector<ISceneImporter *>& GetAllSceneImporters();

} // namespace Dusk

#endif // DUSK_SCENE_IMPORTER_HPP