#include <Dusk/Module.hpp>
#include <Dusk/Loaders/STBITextureLoader.hpp>

namespace Dusk::STBI {

void ModuleInit() {
    Dusk::SetTextureLoader(new TextureLoader());
}

DUSK_MODULE() {
    .Name = "STBI",
    .Init = ModuleInit,
    .Term = nullptr,
};

}