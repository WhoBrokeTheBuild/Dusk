#include <Dusk/Module.hpp>

#include <Dusk/TinyOBJ/TinyOBJMeshImporter.hpp>

namespace Dusk::TinyOBJ {

bool ModuleInit()
{
    AddMeshImporter("TinyOBJ", std::unique_ptr<MeshImporter>(New TinyOBJMeshImporter));

    return true;
}

void ModuleTerm()
{
    RemoveMeshImporter("TinyOBJ");
}

Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_DEFINE_MODULE {
    .Name       = "DuskTinyOBJ",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::TinyOBJ