#include <Dusk/Module.hpp>

#include <Dusk/STBI/STBITextureImporter.hpp>

namespace Dusk::STBI {

bool ModuleInit()
{
    AddTextureImporter("STBI", std::unique_ptr<TextureImporter>(New STBITextureImporter()));

    return true;
}

void ModuleTerm()
{
    RemoveTextureImporter("STBI");
}

Version GetVersion()
{
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_DEFINE_MODULE {
    .Name       = "DuskSTBI",
    .Initialize = ModuleInit,
    .Terminate  = ModuleTerm,
    .GetVersion = GetVersion,
};

} // namespace Dusk::STBI