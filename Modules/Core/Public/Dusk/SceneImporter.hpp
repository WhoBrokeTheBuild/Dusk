#ifndef DUSK_SCENE_IMPORTER_HPP
#define DUSK_SCENE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Path.hpp>

namespace Dusk {

class DUSK_CORE_API SceneImporter : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(SceneImporter)

    SceneImporter() = default;

    virtual ~SceneImporter() = default;

    virtual bool LoadFromFile(Entity * root, const Path& path) = 0;

}; // class SceneImporter

DUSK_CORE_API
void AddSceneImporter(const string& id, std::unique_ptr<SceneImporter> importer);

DUSK_CORE_API
void RemoveSceneImporter(const string& id);

DUSK_CORE_API
const std::vector<SceneImporter *>& GetAllSceneImporters();

} // namespace Dusk

#endif // DUSK_SCENE_IMPORTER_HPP