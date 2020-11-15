#include <Dusk/Module.hpp>
#include <Dusk/TinyOBJ/MeshImporter.hpp>

namespace Dusk::TinyOBJ {

void ModuleInit() {
    AddMeshImporter("TinyOBJ", std::make_unique<MeshImporter>());
}

void ModuleTerm() {
    RemoveMeshImporter("TinyOBJ");
}

DEFINE_MODULE() {
    "TinyOBJ",
    ModuleInit,
    ModuleTerm,
};

}
