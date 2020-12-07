#include <Dusk/Module.hpp>
#include <Dusk/STBI/TextureImporter.hpp>

using namespace Dusk;

void STBIModuleInit()
{
    AddTextureImporter("STBI", std::unique_ptr<TextureImporter>(New STBITextureImporter()));
}

void STBIModuleTerm()
{
    RemoveTextureImporter("STBI");
}

DEFINE_MODULE() {
    "STBI",
    STBIModuleInit,
    STBIModuleTerm,
};
