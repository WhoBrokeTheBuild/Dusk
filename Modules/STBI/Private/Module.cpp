#include <Dusk/Module.hpp>
#include <Dusk/STBI/TextureImporter.hpp>

namespace Dusk::STBI {

void ModuleInit() {
    AddTextureImporter("STBI", std::unique_ptr<ITextureImporter>(new TextureImporter()));
}

void ModuleTerm() {
    RemoveTextureImporter("STBI");
}

DUSK_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

}
