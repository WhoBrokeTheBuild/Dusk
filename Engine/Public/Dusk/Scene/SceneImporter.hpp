#ifndef DUSK_SCENE_IMPORTER_HPP
#define DUSK_SCENE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Scene/Entity.hpp>

namespace Dusk {

class DUSK_ENGINE_API SceneImporter 
{
public:

    DISALLOW_COPY_AND_ASSIGN(SceneImporter)

    explicit SceneImporter() = default;

    virtual ~SceneImporter() = default;

    virtual bool LoadFromFile(Entity * root, const std::string& filename) = 0;

}; // class SceneImporter

DUSK_ENGINE_API
void AddSceneImporter(const std::string& id, std::unique_ptr<SceneImporter> importer);

DUSK_ENGINE_API
void RemoveSceneImporter(const std::string& id);

DUSK_ENGINE_API
const std::vector<SceneImporter *>& GetAllSceneImporters();

} // namespace Dusk

#endif // DUSK_SCENE_IMPORTER_HPP