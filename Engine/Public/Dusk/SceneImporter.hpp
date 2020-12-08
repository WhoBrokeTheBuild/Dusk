#ifndef DUSK_SCENE_IMPORTER_HPP
#define DUSK_SCENE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Entity.hpp>

namespace Dusk {

class DUSK_ENGINE_API SceneImporter : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(SceneImporter)

    SceneImporter() = default;

    virtual ~SceneImporter() = default;

    inline std::string GetClassID() const override {
        return "Dusk::SceneImporter";
    }

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