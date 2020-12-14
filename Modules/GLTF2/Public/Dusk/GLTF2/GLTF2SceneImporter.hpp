#ifndef DUSK_GLTF2_SCENE_IMPORTER_HPP
#define DUSK_GLTF2_SCENE_IMPORTER_HPP

#include <Dusk/GLTF2/GLTF2Config.hpp>

#include <Dusk/SceneImporter.hpp>

namespace Dusk::GLTF2 {

class DUSK_GLTF2_API GLTF2SceneImporter : public SceneImporter
{
public:

    DISALLOW_COPY_AND_ASSIGN(GLTF2SceneImporter)

    GLTF2SceneImporter() = default;

    virtual bool LoadFromFile(Entity * root, const std::string& filename) override;

private:

}; // class GLTF2SceneImporter

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_SCENE_IMPORTER_HPP