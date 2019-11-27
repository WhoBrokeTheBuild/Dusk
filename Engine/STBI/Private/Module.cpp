#include <Dusk/Module.hpp>
#include <Dusk/Loaders/STBITextureLoader.hpp>

namespace Dusk::STBI {

void ModuleInit() {
    SetTextureLoader(new TextureLoader());
}

void ModuleTerm() {
    SetTextureLoader(nullptr);
}

DUSK_MODULE() {
    .Name = "STBI",
    .Init = ModuleInit,
    .Term = nullptr,
};

}