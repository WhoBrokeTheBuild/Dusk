#include <Dusk/Module.hpp>
#include <Dusk/STBI/TextureImporter.hpp>

namespace Dusk::STBI {

void ModuleInit() {
    AddTextureImporter("STBI", std::unique_ptr<Dusk::TextureImporter>(new TextureImporter()));
}

void ModuleTerm() {
    RemoveTextureImporter("STBI");
}

DEFINE_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

}
