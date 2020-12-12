#include <Dusk/Module.hpp>

#include <Dusk/GLTF2/GLTF2MeshImporter.hpp>
#include <Dusk/GLTF2/GLTF2SceneImporter.hpp>

namespace Dusk::GLTF2 {

bool ModuleInit()
{
    AddMeshImporter("GLTF2", std::unique_ptr<MeshImporter>(New GLTF2MeshImporter()));
    AddSceneImporter("GLTF2", std::unique_ptr<SceneImporter>(New GLTF2SceneImporter()));

    return true;
}

void ModuleTerm()
{
    RemoveSceneImporter("GLTF2");
    RemoveMeshImporter("GLTF2");
}

Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_DEFINE_MODULE {
    .Name       = "DuskGLTF2",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::GLTF2