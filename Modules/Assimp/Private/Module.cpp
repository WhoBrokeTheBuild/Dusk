#include <Dusk/Module.hpp>
#include <Dusk/Assimp/SceneImporter.hpp>

namespace Dusk::Assimp {

void ModuleInit() {
    AddSceneImporter("Assimp", std::make_unique<SceneImporter>());
}

void ModuleTerm() {
    RemoveSceneImporter("Assimp");
}

DUSK_MODULE() {
    "Assimp",
    ModuleInit,
    ModuleTerm,
};

}
