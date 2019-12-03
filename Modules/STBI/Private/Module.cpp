#include <Dusk/Module.hpp>
#include <Dusk/Loaders/STBITextureLoader.hpp>

namespace Dusk::STBI {

void ModuleInit() {
    auto loader = std::shared_ptr<ITextureLoader>(new TextureLoader());
    SetTextureLoader("jpg", loader);
    SetTextureLoader("jpeg", loader);
    SetTextureLoader("png", loader);
    SetTextureLoader("tga", loader);
    SetTextureLoader("bmp", loader);
    SetTextureLoader("gif", loader);
}

DUSK_MODULE() {
    "STBI",
    ModuleInit,
    nullptr,
};

}
