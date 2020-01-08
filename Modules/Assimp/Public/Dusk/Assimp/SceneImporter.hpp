#ifndef DUSK_ASSIMP_SCENE_IMPORTER_HPP
#define DUSK_ASSIMP_SCENE_IMPORTER_HPP

#include <Dusk/Assimp/Config.hpp>
#include <Dusk/Scene/SceneImporter.hpp>

namespace Dusk::Assimp {

class DUSK_ASSIMP_API SceneImporter : public ISceneImporter
{
public:

    bool LoadFromFile(Entity * root, const std::string& filename) override;

};

} // namespace Dusk::Assimp

#endif // DUSK_ASSIMP_SCENE_IMPORTER_HPP