#include <Dusk/Module.hpp>
#include <Dusk/GLTF2/MeshImporter.hpp>
#include <Dusk/GLTF2/SceneImporter.hpp>

using namespace Dusk;

void GLTF2ModuleInit()
{
    AddMeshImporter("GLTF2", std::unique_ptr<MeshImporter>(New GLTF2MeshImporter()));
    AddSceneImporter("GLTF2", std::unique_ptr<SceneImporter>(New GLTF2SceneImporter()));
}

void GLTF2ModuleTerm()
{
    RemoveSceneImporter("GLTF2");
    RemoveMeshImporter("GLTF2");
}

DEFINE_MODULE() {
    "GLTF2",
    GLTF2ModuleInit,
    GLTF2ModuleTerm,
};
