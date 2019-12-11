#include <Dusk/Module.hpp>
#include <Dusk/STBI/TextureImporter.hpp>

namespace Dusk::STBI {

void ModuleInit() {
    AddTextureImporter("stbi", std::unique_ptr<ITextureImporter>(new TextureImporter()));
}

void ModuleTerm() {
    RemoveTextureImporter("stbi");
}

DUSK_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

}
