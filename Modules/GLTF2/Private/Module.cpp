#include <Dusk/Module.hpp>
#include <Dusk/GLTF2/MeshImporter.hpp>
#include <Dusk/GLTF2/SceneImporter.hpp>

namespace Dusk::GLTF2 {

void ModuleInit() {
    AddMeshImporter("GLTF2", std::make_unique<MeshImporter>());
    AddSceneImporter("GLTF2", std::make_unique<SceneImporter>());
}

void ModuleTerm() {
    RemoveSceneImporter("GLTF2");
    RemoveMeshImporter("GLTF2");
}

DEFINE_MODULE() {
    "GLTF2",
    ModuleInit,
    ModuleTerm,
};

}