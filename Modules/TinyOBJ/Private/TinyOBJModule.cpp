#include <Dusk/Module.hpp>

#include <Dusk/TinyOBJ/TinyOBJMeshImporter.hpp>

using namespace Dusk;

void TinyOBJModuleInit()
{
    AddMeshImporter("TinyOBJ", std::unique_ptr<MeshImporter>(New TinyOBJMeshImporter));
}

void TinyOBJModuleTerm()
{
    RemoveMeshImporter("TinyOBJ");
}

DEFINE_MODULE() {
    "TinyOBJ",
    TinyOBJModuleInit,
    TinyOBJModuleTerm,
};
