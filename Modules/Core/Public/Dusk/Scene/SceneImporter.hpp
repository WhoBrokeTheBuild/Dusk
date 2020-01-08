#ifndef DUSK_SCENE_IMPORTER_HPP
#define DUSK_SCENE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Scene/Entity.hpp>

namespace Dusk {

class DUSK_CORE_API ISceneImporter 
{
public:

    virtual bool LoadFromFile(Entity * root, const std::string& filename) = 0;

}; // class ISceneImporter

DUSK_CORE_API
void AddSceneImporter(const std::string& id, std::unique_ptr<ISceneImporter> importer);

DUSK_CORE_API
void RemoveSceneImporter(const std::string& id);

DUSK_CORE_API
const std::vector<ISceneImporter *>& GetAllSceneImporters();

} // namespace Dusk

#endif // DUSK_SCENE_IMPORTER_HPP